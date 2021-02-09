#include "BurstBufferDMA.h"

// see cores/teensy3/kinetis.h


BurstBufferDMA::BurstBufferDMA(uint8_t dma_channel, uint8_t ADC_num)
{

    b_size = BURST_BUFFER_SIZE;
    b_start = 0;
    b_count = 0;

    if(dma_channel<0)
        {
            dma_channel = 0;
        }
    if(ADC_num<0)
        {
            ADC_num = 0;
        }
    else if(ADC_num>1)
        {
            ADC_num = 1;
        }

    DMA_channel = dma_channel;

    ADC_number = ADC_num;

    IRQ_DMA_CH = IRQ_DMA_CH0 + DMA_channel;

    // point to the registers corresponding to the DMA_channel
    DMAMUX0_CHCFG = &DMAMUX0_CHCFG0 + DMA_channel;

    uint16_t dma_offset_16 = 0x10;
    uint32_t dma_offset_32 = 0x8;

    DMA_TCD_CSR = &DMA_TCD0_CSR + dma_offset_16 * DMA_channel;

    DMA_TCD_SADDR = &DMA_TCD0_SADDR + dma_offset_32 * DMA_channel;
    DMA_TCD_SOFF = &DMA_TCD0_SOFF + dma_offset_16 * DMA_channel;
    DMA_TCD_SLAST = &DMA_TCD0_SLAST + dma_offset_32 * DMA_channel;

    DMA_TCD_DADDR = &DMA_TCD0_DADDR + dma_offset_32 * DMA_channel;
    DMA_TCD_DOFF = &DMA_TCD0_DOFF + dma_offset_16 * DMA_channel;
    DMA_TCD_DLASTSGA = &DMA_TCD0_DLASTSGA + dma_offset_32 * DMA_channel;

    DMA_TCD_ATTR = &DMA_TCD0_ATTR + dma_offset_16 * DMA_channel;
    DMA_TCD_NBYTES_MLNO = &DMA_TCD0_NBYTES_MLNO + dma_offset_32 * DMA_channel;

    DMA_TCD_CITER_ELINKNO = &DMA_TCD0_CITER_ELINKNO + dma_offset_16 * DMA_channel;
    DMA_TCD_BITER_ELINKNO = &DMA_TCD0_BITER_ELINKNO + dma_offset_16 * DMA_channel;

    // point to the correct ADC
    ADC_RA = &ADC0_RA + (uint32_t)0x20000 * ADC_number;

    //digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));


}

void BurstBufferDMA::start()
{

    // Mark the buffer as entirely empty.
    b_count = b_start = 0;
    SIM_SCGC7 |= SIM_SCGC7_DMA;
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX;

    DMA_CR = 0; // normal mode of operation

    *DMAMUX0_CHCFG = DMAMUX_DISABLE; // disable before changing

    *DMA_TCD_ATTR = DMA_TCD_ATTR_SSIZE(DMA_TCD_ATTR_SIZE_16BIT) |
      DMA_TCD_ATTR_DSIZE(DMA_TCD_ATTR_SIZE_16BIT); // src and dst data is 16 bit (2 bytes)
    *DMA_TCD_NBYTES_MLNO = 2; // Minor Byte Transfer Count 2 bytes = 16 bits (we transfer 2 bytes each minor loop)

    *DMA_TCD_SADDR = ADC_RA; // source address
    *DMA_TCD_SOFF = 0; // don't change the address when minor loop finishes
    *DMA_TCD_SLAST = 0; // don't change src address after major loop completes

    *DMA_TCD_DADDR = elems; // destination address
    *DMA_TCD_DOFF = 2; // increment 2 bytes each minor loop
    *DMA_TCD_DLASTSGA = -BURST_BUFFER_SIZE * *DMA_TCD_DOFF;  // Reset to beginning after each major loop

    *DMA_TCD_CITER_ELINKNO = BURST_BUFFER_SIZE; // Current Major Iteration Count with channel linking disabled (how many minor loops in each major)
    *DMA_TCD_BITER_ELINKNO = BURST_BUFFER_SIZE; // Starting Major Iteration Count with channel linking disabled (how many minor loops in each major)

    *DMA_TCD_CSR = DMA_TCD_CSR_DREQ | DMA_TCD_CSR_INTMAJOR; // Control and status: disable ERQ and interrupt when major counter is complete

    DMA_CERQ = DMA_CERQ_CERQ(DMA_channel); // clear all past request
    DMA_CINT = DMA_channel; // clear interrupts

    uint8_t DMAMUX_SOURCE_ADC = DMAMUX_SOURCE_ADC0;
    if(ADC_number==1){
        DMAMUX_SOURCE_ADC = DMAMUX_SOURCE_ADC1;
    }
    *DMAMUX0_CHCFG = DMAMUX_SOURCE_ADC | DMAMUX_ENABLE; // enable mux and set channel DMA_channel to ADC0

    DMA_SERQ = DMA_SERQ_SERQ(DMA_channel); // enable DMA request
    NVIC_ENABLE_IRQ(IRQ_DMA_CH); // enable interrupts


    //digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
}


BurstBufferDMA::~BurstBufferDMA()
{

    DMA_CERQ = DMA_CERQ_CERQ(DMA_channel); // stop channel 3
    NVIC_DISABLE_IRQ(IRQ_DMA_CH);
}



bool BurstBufferDMA::isFull()
{
    return (b_start == 0 && b_count != 0);
}

bool BurstBufferDMA::isEmpty()
{
    return (b_count == 0);
}

// update internal pointers, call from dm_chX_ist.  Since we get an interrupt only
// when the entire buffer has been filled, jus
void BurstBufferDMA::done()
{
    // using DMA:
    // call this inside the dma_isr to update the b_start and/or b_end pointers
    b_start = 0;
    b_count = BURST_BUFFER_SIZE;
}

int BurstBufferDMA::read()
{
    // using DMA:
    // read last value and update b_start
    if (b_count == 0 || b_start >= BURST_BUFFER_SIZE)
        {
            return 0;
        }
    --b_count;
    return elems[b_start++];
}

int32_t BurstBufferDMA::elementCount()
{
    return BURST_BUFFER_SIZE;
}

