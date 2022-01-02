/*
 ||
 || @file       Uart2Event.cpp
 || @version 	6.6
 || @author 	Colin Duffy
 || @contact 	http://forum.pjrc.com/members/25610-duff
 || @license
 || | Copyright (c) 2015 Colin Duffy
 || | This library is free software; you can redistribute it and/or
 || | modify it under the terms of the GNU Lesser General Public
 || | License as published by the Free Software Foundation; version
 || | 2.1 of the License.
 || |
 || | This library is distributed in the hope that it will be useful,
 || | but WITHOUT ANY WARRANTY; without even the implied warranty of
 || | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 || | Lesser General Public License for more details.
 || |
 || | You should have received a copy of the GNU Lesser General Public
 || | License along with this library; if not, write to the Free Software
 || | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 || #
 ||
 */

#include "UartEvent.h"
#include "memcpy.h"

#define SCGC4_UART1_BIT     11

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
#define TX_BUFFER_SIZE TX1_BUFFER_SIZE // number of outgoing bytes to buffer
#define RX_BUFFER_SIZE RX1_BUFFER_SIZE // number of incoming bytes to buffer
#define IRQ_PRIORITY  64  // 0 = highest priority, 255 = lowest
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#ifdef SERIAL_9BIT_SUPPORT
static uint8_t use9Bits = 0;
#define BUFTYPE uint16_t
#else
#define BUFTYPE uint8_t
#define use9Bits 0
#endif

DMAMEM static volatile BUFTYPE __attribute__((aligned(TX_BUFFER_SIZE))) tx_buffer[TX_BUFFER_SIZE];
DMAMEM static volatile BUFTYPE __attribute__((aligned(RX_BUFFER_SIZE))) rx_buffer[RX_BUFFER_SIZE];

#if TX_BUFFER_SIZE > 255
static volatile uint16_t tx_buffer_head = 0;
static volatile uint16_t tx_buffer_tail = 0;
static volatile bool     tx_buffer_empty = true;
#else
static volatile uint8_t tx_buffer_head  = 0;
static volatile uint8_t tx_buffer_tail  = 0;
static volatile bool    tx_buffer_empty = true;
#endif
#if RX_BUFFER_SIZE > 255
static volatile uint16_t rx_buffer_head  = 0;
static volatile uint16_t rx_buffer_tail  = 0;
static volatile uint16_t rx_buffer_count = 0;
#else
static volatile uint8_t rx_buffer_head  = 0;
static volatile uint8_t rx_buffer_tail  = 0;
static volatile uint8_t rx_buffer_count = 0;
#endif

static volatile uint8_t transmitting  = 0;
static volatile uint8_t *transmit_pin = NULL;

#if defined(KINETISK)
static uint8_t rx_pin_num = 9;
static uint8_t tx_pin_num = 10;
#endif

DMAChannel        Uart2Event::tx;
DMAChannel        Uart2Event::rx;
Uart2Event::ISR   Uart2Event::txEventHandler;
Uart2Event::ISR   Uart2Event::rxEventHandler;

volatile int16_t  Uart2Event::priority;
volatile int      Uart2Event::rxTermCharacterTrigger;
volatile int      Uart2Event::rxBufferSizeTrigger;

uint32_t *Uart2Event::elink;

// UART0 and UART1 are clocked by F_CPU, UART2 is clocked by F_BUS
// UART0, UART1 have 8 byte fifo, and UART2 has 1 byte buffer

#ifdef HAS_KINETISK_UART1_FIFO
    /// TE: Transmitter Enable, RE: Receiver Enable, RIE: Receiver Full Interrupt or DMA Transfer Enable, ILIE: Idle Line Interrupt Enable.
    #define EVENT1_C2_TX_ENABLE        UART_C2_TE | UART_C2_ILIE
    #define EVENT1_C2_RX_ENABLE        UART_C2_RE | UART_C2_ILIE
#else
    #define EVENT1_C2_TX_ENABLE        UART_C2_TE
    #define EVENT1_C2_RX_ENABLE        UART_C2_RE
#endif

#define EVENT1_C2_TX_ACTIVE             EVENT1_C2_TX_ENABLE | UART_C2_TIE  // Set Transmitter Interrupt or DMA Transfer Enable.
#define EVENT1_C2_TX_COMPLETING         EVENT1_C2_TX_ENABLE | UART_C2_TCIE  // Set Transmission Complete Interrupt Enable.
#define EVENT1_C2_TX_INACTIVE           EVENT1_C2_TX_ENABLE

#define EVENT1_C2_RX_ACTIVE             EVENT1_C2_RX_ENABLE | UART_C2_RIE  // Set Reciever Interrupt or DMA Transfer Enable.
#define EVENT1_C2_RX_INACTIVE           EVENT1_C2_RX_ENABLE

#define EVENT1_UART_DMA_ENABLE          UART_C5_TDMAS | UART_C5_RDMAS  // Set Transmitter DMA Select & Receiver DMA Select.

#define EVENT1_UART_DMA_DISABLE         UART1_C5 &= ~(UART_C5_TDMAS | UART_C5_RDMAS);  // Disables: Transmitter DMA Select , Receiver Full DMA Select, Enable bits.

// -------------------------------------------ISR------------------------------------------
void Uart2Event::user_isr_tx( void ) {
    txEventHandler( );
}

void Uart2Event::serial_dma_tx_isr( void ) {
    tx.clearInterrupt( );
    
    uint32_t head = tx_buffer_head;
    uint32_t tail = tx_buffer_tail;
    uint32_t ELINKNO = tx.TCD->CITER_ELINKNO;
    
    if ( ( tail + ELINKNO ) >= TX_BUFFER_SIZE ) tail += ELINKNO - TX_BUFFER_SIZE;
    else tail += ELINKNO;

    if (tail == head) tx_buffer_empty = true;
    
    if ( !tx_buffer_empty ) {
        transmitting = true;
        int size;
        if ( tail > head ) size = ( TX_BUFFER_SIZE - tail ) + head;
        else size = head - tail;
        
        __disable_irq( );
        tx.TCD->CITER = size;
        tx.TCD->BITER = size;
        tx.enable();
        __enable_irq( );
    } else {
        transmitting = false;
        NVIC_SET_PENDING( IRQ_UART1_ERROR );
    }
    if ( transmit_pin ) *transmit_pin = 0;
    tx_buffer_tail = tail;
}

void Uart2Event::user_isr_rx( void ) {
    rxEventHandler( );
}

void Uart2Event::serial_dma_rx_isr( void ) {
    rx.clearInterrupt( );
    uint32_t head, tail;
    int term_trigger, size_trigger;
    head = rx_buffer_head;
    head = ( head + 1 )&( RX_BUFFER_SIZE - 1 );
    rx_buffer_head = head;
    term_trigger = rxTermCharacterTrigger;
    size_trigger = rxBufferSizeTrigger;
    if ( size_trigger != -1 ) {
        uint16_t bufferFree;
        tail = rx_buffer_tail;
        if ( head >= tail ) bufferFree = head - tail;
        else bufferFree = RX_BUFFER_SIZE + head - tail;
        if ( bufferFree >= size_trigger ) {
            NVIC_SET_PENDING( IRQ_UART1_STATUS );
            *elink = 1;
            return;
        }
        else if ( term_trigger == -1 ) {
            *elink = 1;
            return;
        }
    }
    if ( term_trigger != -1 ) {
        char current = rx_buffer[head];
        if ( current == term_trigger ) {
            NVIC_SET_PENDING( IRQ_UART1_STATUS );
        }
        *elink = 1;
    }
    else {
        NVIC_SET_PENDING( IRQ_UART1_STATUS );
        *elink = 1;
    }
}
// -------------------------------------------CODE------------------------------------------
void Uart2Event::serial_dma_begin( uint32_t divisor )
{
    /// Enable UART1 clock
    SIM_SCGC4 |= SIM_SCGC4_UART1;    // turn on clock, TODO: use bitband.
    
    //BITBAND_REG_U32( SIM_SCGC4, SCGC4_UART1_BIT ) = 0x01;
    
    /****************************************************************
     * some code lifted from Teensyduino Core serial1.c
     ****************************************************************/
    
    #if defined(KINETISK)
        switch (rx_pin_num)
            {
                case 9: CORE_PIN9_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_PFE | PORT_PCR_MUX(3); break;
                #if defined(__MK20DX128__) || defined(__MK20DX256__)    // T3.0, T3.1, T3.2
                    case 26: CORE_PIN26_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_PFE | PORT_PCR_MUX(3); break;
                #elif defined(__MK64FX512__) || defined(__MK66FX1M0__)  // T3.5 or T3.6
                    case 59: CORE_PIN59_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_PFE | PORT_PCR_MUX(3); break;
                #endif
            }
        switch (tx_pin_num)
            {
                case 10: CORE_PIN10_CONFIG = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(3); break;
                #if defined(__MK20DX128__) || defined(__MK20DX256__)    // T3.0, T3.1, T3.2
                    case 31: CORE_PIN31_CONFIG = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(3); break;
                #elif defined(__MK64FX512__) || defined(__MK66FX1M0__)  // T3.5 or T3.6
                    case 58: CORE_PIN58_CONFIG = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(3); break;
                #endif
            }
    #elif defined(KINETISL)
        CORE_PIN9_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_PFE | PORT_PCR_MUX(3);
        CORE_PIN10_CONFIG = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(3);
    #endif
    
    #if defined(HAS_KINETISK_UART1)
        if (divisor < 32) divisor = 32;
        UART1_BDH = (divisor >> 13) & 0x1F;
        UART1_BDL = (divisor >> 5) & 0xFF;
        UART1_C4 = divisor & 0x1F;
    
        #ifdef HAS_KINETISK_UART1_FIFO
            UART1_C1 = UART_C1_ILT;  // Idle Line Type Select set to Idle character bit count starts after stop bit.
            UART1_TWFIFO = 2; // Set tx watermark to 2 bytes, causes S1_TDRE to set depending on TIE & TDMAS.
            UART1_RWFIFO = 1; // Set rx watermark to 1, causes S1_RDRF to set depending on RIE & RDMAS.
            UART1_PFIFO = UART_PFIFO_TXFE | UART_PFIFO_RXFE;  // Set Transmit FIFO Enable, Set Receive FIFO Enable.
            /// Setting UART_PFIFO_TXFE AND UART_PFIFO_RXFE assumes that the fields TXFIFOSIZE & RXFIFOSIZE are already set to 0x07.
        #else  // No FIFO
            UART1_C1 = 0;
            UART1_PFIFO = 0;
        #endif
    
    #elif defined(HAS_KINETISL_UART1)
        if (divisor < 1) divisor = 1;
        UART1_BDH = (divisor >> 8) & 0x1F;
        UART1_BDL = divisor & 0xFF;
        UART1_C1 = 0;
    #endif
    
    //UART1_C1 = 0;  //UART_C1_ILT;
    // TODO: Use UART1 fifo with dma
    UART1_TWFIFO = 2;  // tx watermark, causes C5_TDMAS DMA request
    UART1_RWFIFO = 1;  // rx watermark, causes C5_RDMAS DMA request
    
    UART1_C2 = ( EVENT1_C2_TX_ACTIVE | EVENT1_C2_RX_ACTIVE );

    UART1_C5 = EVENT1_UART_DMA_ENABLE; // setup Serial1 tx,rx to use dma
    
    if ( loopBack ) UART1_C1 |= UART_C1_LOOPS; // Set internal loop1Back
    
    /****************************************************************
     * DMA TX setup
     ****************************************************************/
    tx.destination( UART1_D );
    tx.sourceCircular( tx_buffer, TX_BUFFER_SIZE );
    tx.attachInterrupt( serial_dma_tx_isr );
    tx.interruptAtCompletion( );
    tx.disableOnCompletion( );
    tx.triggerAtHardwareEvent( DMAMUX_SOURCE_UART1_TX );
    attachInterruptVector( IRQ_UART1_ERROR, user_isr_tx );
    NVIC_SET_PRIORITY( IRQ_UART1_ERROR, 192 ); // 255 = lowest priority
    NVIC_ENABLE_IRQ( IRQ_UART1_ERROR );
    NVIC_SET_PRIORITY( IRQ_DMA_CH0 + tx.channel, IRQ_PRIORITY );
    priority = NVIC_GET_PRIORITY( IRQ_DMA_CH0 + tx.channel );
    
    /****************************************************************
     * DMA RX setup
     ****************************************************************/
    rx.source( UART1_D );
    rx.destinationCircular( rx_buffer+1, RX_BUFFER_SIZE );
    rx.attachInterrupt( serial_dma_rx_isr );
    rx.interruptAtCompletion( );
    rx.triggerContinuously( );
    rx.triggerAtHardwareEvent( DMAMUX_SOURCE_UART1_RX );
    attachInterruptVector( IRQ_UART1_STATUS, user_isr_rx );
    NVIC_SET_PRIORITY( IRQ_UART1_STATUS, 192 ); // 255 = lowest priority
    NVIC_ENABLE_IRQ( IRQ_UART1_STATUS );
    NVIC_SET_PRIORITY( IRQ_DMA_CH0 + rx.channel, IRQ_PRIORITY );
    elink = ( uint32_t * )&rx.TCD->CITER_ELINKNO;
    *elink = 1;
    rx.enable( );
}

/***********************************************************************************************
* If the condition that results in the assertion of the flag, interrupt, or DMA request is not
* resolved prior to clearing the flag, the flag, and interrupt/DMA request, reasserts.
* For example, if the DMA or interrupt service routine fails to write sufficient data to the
* transmit buffer to raise it above the watermark level, the flag reasserts and generates another
* interrupt or DMA request.
* Reading an empty data register to clear one of the flags of the S1 register causes the FIFO
* pointers to become misaligned.  A receive FIFO flush reinitializes the pointers.
************************************************************************************************/
void Uart2Event::serial_clear_rx_event(void)
{
    UART1_S1;
    UART1_D;  // Clear leftover error status

     #ifdef HAS_KINETISK_UART1_FIFO
        if (!(SIM_SCGC4 & SIM_SCGC4_UART1)) return;
    
        UART1_C2 &= ~(UART_C2_RE | UART_C2_RIE | UART_C2_ILIE);  // Clears the RE: Receiver Enable, RIE: Receiver Full Interrupt or DMA Transfer Enable, ILIE: Idle Line Interrupt Enable bits.
        UART1_CFIFO = UART_CFIFO_RXFLUSH;  // Set the Receive FIFO/Buffer Flush bit to clear the Recieve Buffer FIFO.
        UART1_CFIFO &= ~(UART_CFIFO_RXFLUSH);  // Clear the Receive FIFO/Buffer Flush bit.
        UART1_C2 |= (UART_C2_RE | UART_C2_RIE | UART_C2_ILIE);  // Sets the RE: Receiver Enable, RIE: Receiver Full Interrupt or DMA Transfer Enable, ILIE: Idle Line Interrupt Enable bits.
     #endif
    
    rx.clearComplete();
    rx.clearInterrupt( );  // You need to clear the interrupt to do again later.
    //rx.disable();
}

void Uart2Event::serial_clear_tx_event(void)
{
    tx.clearComplete();
    tx.clearInterrupt( );  // You need to clear the interrupt to do again later.
}


void Uart2Event::serial_dma_format(uint32_t format) {
    /****************************************************************
     * serial1 format, from teensduino core, serial1.c
     ****************************************************************/
    uint8_t c;
    c = UART1_C1;
    c = ( c & ~0x13 ) | ( format & 0x03 );      // configure parity
    if (format & 0x04) c |= 0x10;           // 9 bits (might include parity)
    UART1_C1 = c;
    if ( ( format & 0x0F ) == 0x04) UART1_C3 |= 0x40; // 8N2 is 9 bit with 9th bit always 1
    c = UART1_S2 & ~0x10;
    if ( format & 0x10 ) c |= 0x10;           // rx invert
    UART1_S2 = c;
    c = UART1_C3 & ~0x10;
    if ( format & 0x20 ) c |= 0x10;           // tx invert
    UART1_C3 = c;
#ifdef SERIAL_9BIT_SUPPORT
    c = UART1_C4 & 0x1F;
    if ( format & 0x08 ) c |= 0x20;           // 9 bit mode with parity (requires 10 bits)
    UART1_C4 = c;
    use9Bits = format & 0x80;
#endif
}

void Uart2Event::serial_dma_end( void )
{
    if ( !( SIM_SCGC7 & SIM_SCGC7_DMA ) ) return;
    if ( !( SIM_SCGC6 & SIM_SCGC6_DMAMUX ) ) return;
    if ( !( SIM_SCGC4 & SIM_SCGC4_UART1 ) ) return;
    attachInterruptVector( IRQ_UART1_STATUS, uart1_status_isr );
    NVIC_DISABLE_IRQ(IRQ_UART1_STATUS);
    
    // flush Uart2Event tx buffer
    flush( );
    
    /****************************************************************
     * serial1 end, from teensduino core, serial1.c
     ****************************************************************/
    
    UART1_C2 = 0;  // This resets the Uart to startup conditions, all bits set to 0.
    
    #if defined(KINETISK)
        switch (rx_pin_num)
            {
                case 9: CORE_PIN9_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_MUX(1); break; // PTC3
                #if defined(__MK20DX128__) || defined(__MK20DX256__)  // T3.0, T3.1, T3.2
                    case 26: CORE_PIN26_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_MUX(1); break; // PTE1
                #elif defined(__MK64FX512__) || defined(__MK66FX1M0__) // T3.5, T3.6
                    case 59: CORE_PIN59_CONFIG = 0; break;
                #endif
            }
        switch (tx_pin_num & 127)
            {
                case 10: CORE_PIN10_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_MUX(1); break; // PTC4
                #if defined(__MK20DX128__) || defined(__MK20DX256__)  // T3.0, T3.1, T3.2
                    case 31: CORE_PIN31_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_MUX(1); break; // PTE0
                #elif defined(__MK64FX512__) || defined(__MK66FX1M0__) // T3.5, T3.6
                    case 58: CORE_PIN58_CONFIG = 0; break;
                #endif
            }
    #elif defined(KINETISL)
        CORE_PIN9_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_MUX(1);  // PTC3
        CORE_PIN10_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_MUX(1); // PTC4
    #endif
    
    UART1_S1;
    UART1_D; // clear leftover error status
    
    UART1_C5 = EVENT1_UART_DMA_DISABLE;
    tx_buffer_head = tx_buffer_tail = 0;
    tx_buffer_empty = true;
}

void Uart2Event::serial_dma_set_transmit_pin( uint8_t pin ) {
    while ( transmitting ) yield();
    pinMode( pin, OUTPUT);
    digitalWrite( pin, LOW );
    transmit_pin = portOutputRegister( pin );
}

void Uart2Event::serial_dma_set_tx_pin(uint8_t pin, uint8_t opendrain)
{
    #if defined(KINETISK)
        uint32_t cfg;
    
        if (opendrain) pin |= 128;
        if (pin == tx_pin_num) return;
        if ((SIM_SCGC4 & SIM_SCGC4_UART1))
            {
                switch (tx_pin_num & 127)
                    {
                        case 10: CORE_PIN10_CONFIG = 0; break; // PTC4
                        #if defined(__MK20DX128__) || defined(__MK20DX256__)  // T3.0, T3.1, T3.2
                            case 31: CORE_PIN31_CONFIG = 0; break; // PTE0
                        #elif defined(__MK64FX512__) || defined(__MK66FX1M0__) // T3.5, T3.6
                            case 58: CORE_PIN58_CONFIG = 0; break;
                        #endif
                    }
                if (opendrain)
                    {
                        cfg = PORT_PCR_DSE | PORT_PCR_ODE;
                    }
                else
                    {
                        cfg = PORT_PCR_DSE | PORT_PCR_SRE;
                    }
                switch (pin & 127)
                    {
                        case 10: CORE_PIN10_CONFIG = cfg | PORT_PCR_MUX(3); break;
                        #if defined(__MK20DX128__) || defined(__MK20DX256__)  // T3.0, T3.1, T3.2
                            case 31: CORE_PIN31_CONFIG = cfg | PORT_PCR_MUX(3); break;
                        #elif defined(__MK64FX512__) || defined(__MK66FX1M0__) // T3.5, T3.6
                            case 58: CORE_PIN58_CONFIG = cfg | PORT_PCR_MUX(3); break;
                        #endif
                    }
            }
        tx_pin_num = pin;
    #endif
}

void Uart2Event::serial_dma_set_rx_pin(uint8_t pin)
{
    #if defined(KINETISK)
        if (pin == rx_pin_num) return;
        if ((SIM_SCGC4 & SIM_SCGC4_UART1))
            {
                switch (rx_pin_num)
                    {
                        case 9: CORE_PIN9_CONFIG = 0; break; // PTC3
                        #if defined(__MK20DX128__) || defined(__MK20DX256__)  // T3.0, T3.1, T3.2
                            case 26: CORE_PIN26_CONFIG = 0; break; // PTE1
                        #elif defined(__MK64FX512__) || defined(__MK66FX1M0__) // T3.5, T3.6
                            case 59: CORE_PIN59_CONFIG = 0; break;
                        #endif
                    }
                switch (pin)
                    {
                        case 9: CORE_PIN9_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_PFE | PORT_PCR_MUX(3); break;
                        #if defined(__MK20DX128__) || defined(__MK20DX256__)  // T3.0, T3.1, T3.2
                            case 26: CORE_PIN26_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_PFE | PORT_PCR_MUX(3); break;
                        #elif defined(__MK64FX512__) || defined(__MK66FX1M0__) // T3.5, T3.6
                            case 59: CORE_PIN59_CONFIG = PORT_PCR_PE | PORT_PCR_PS | PORT_PCR_PFE | PORT_PCR_MUX(3); break;
                        #endif
                    }
            }
        rx_pin_num = pin;
    #endif
}

void Uart2Event::serial_dma_putchar( uint32_t c )
{
    serial_dma_write( &c, 1 );
}

int Uart2Event::serial_dma_write( const void *buf, unsigned int count )
{
    if (count == 0) return 0;
    uint8_t * buffer = ( uint8_t * )buf;
    uint32_t head = tx_buffer_head;
    uint32_t cnt = count;
    uint32_t free = serial_dma_write_buffer_free( );
    if ( cnt > free ) cnt = free;
    uint32_t next = head + cnt;
    bool wrap = next >= TX_BUFFER_SIZE ? true : false;
    if ( wrap )
        {
            uint32_t over = next - TX_BUFFER_SIZE;
            uint32_t under = TX_BUFFER_SIZE - head;
            memcpy_fast( tx_buffer+head, buffer, under );
            memcpy_fast( tx_buffer, buffer+under, over );
            head = over;
        }
    else
        {
            memcpy_fast( tx_buffer+head, buffer, cnt );
            head += cnt;
        }
    tx_buffer_head = head;
    tx_buffer_empty = false;
    if ( !transmitting )
        {
            transmitting = true;
            __disable_irq( );
            tx.TCD->CITER = cnt;
            tx.TCD->BITER = cnt;
            tx.enable( );
            __enable_irq( );
        }
    return cnt;
}

void Uart2Event::serial_dma_flush( void ) {
    // wait for any remainding dma transfers to complete
    //raise_priority( );
    while ( serial_dma_write_buffer_free() != TX_BUFFER_SIZE ) yield();
    while ( transmitting ) yield( );
    //lower_priority( );
}

int Uart2Event::serial_dma_write_buffer_free( void ) {
    uint32_t head, tail;
    head = tx_buffer_head;
    tail = tx_buffer_tail;
    if ( (head == tail) && !tx_buffer_empty ) return 0;
    if ( head >= tail ) return TX_BUFFER_SIZE - head + tail;
    return tail - head;
}

int Uart2Event::serial_dma_available( void ) {
    uint32_t head, tail;
    head = rx_buffer_head;
    tail = rx_buffer_tail;
    if ( head >= tail ) return head - tail;
    return RX_BUFFER_SIZE + head - tail;
}

int Uart2Event::serial_dma_getchar( void ) {
    uint32_t head, tail;
    int c;
    head = rx_buffer_head;
    tail = rx_buffer_tail;
    if ( head == tail ) return -1;
    tail = ( tail + 1 )&( RX_BUFFER_SIZE - 1 );
    c = rx_buffer[tail];
    rx_buffer_tail = tail;
    return c;
}

int Uart2Event::serial_dma_peek( void ) {
    uint32_t head, tail;
    head = rx_buffer_head;
    tail = rx_buffer_tail;
    if ( head == tail ) return -1;
    tail = ( tail + 1 )&( RX_BUFFER_SIZE - 1 );
    return rx_buffer[tail];
}

void Uart2Event::serial_dma_clear( void ) {
    rx_buffer_tail = rx_buffer_head;
}
