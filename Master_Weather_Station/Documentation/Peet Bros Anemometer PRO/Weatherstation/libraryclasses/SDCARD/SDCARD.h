/* Card type: Ver2.00 or later Standard Capacity SD Memory Card
              1.0 and 2.0 GB cards purchased in 2009 work well.
   Usage:  Must have global variable. 
              volatile unsigned char buffer[512];
           Function calls.
              unsigned char error = SDCARD.readblock(unsigned long n);
              unsigned char error = SDCARD.writeblock(unsigned long n);
            error is 0 for correct operation
            read copies the 512 bytes from sector n to buffer.
            write copies the 512 bytes from buffer to the sector n.
   References: SD Specifications. Part 1. Physical Layer Simplified Specification
               Version 2.00 September 25, 2006 SD Group.
               http://www.sdcard.org
   Code examples:  http://www.sensor-networks.org/index.php?page=0827727742
                   http://www.avrfreaks.net   search "sd card"
   Operation:  The code reads/writes direct to the sectors on the sd card.
               It does not use a FAT. If the card has been formatted the
               FAT at the lowest sectors and files at the higher sectors
               can be written over.
               The card is not damaged but will need to be reformatted at 
               the lowest level to be used by windows/linux.
   Timing:  readblock or writeblock takes 44 msec.
   Improvement: Could initialize so that can use version 1 sd and hc sd.
                Instead of CMD1 need to use CMD8, CMD58 and CMD41.
*/
#ifndef SDCARD_h
#define SDCARD_h

#define setupSPI SPCR = 0x53; //Master mode, MSB first, SCK phase low, SCK idle low, clock/64
#define deselectSPI SPCR = 0x00;  //deselect SPI after read write block
#define clearSPI  SPSR = 0x00; // clear SPI interupt bit
#define setupDDRB DDRB |= 0x2c;  //set SS as output for cs
#define selectSDCARD PORTB &= ~0x04;  //set the SS to 0 to select the sd card 
#define deselectSDCARD PORTB |= 0x04;  //set the SS to 1 to deselect the sd card 

#include "WProgram.h"

class SDCARDclass
{
public:
unsigned char readblock(unsigned long Rstartblock);
unsigned char writeblock(unsigned long Wstartblock);

private:
unsigned char SD_reset(void);
unsigned char SD_sendCommand(unsigned char cmd, unsigned long arg);
unsigned char SPI_transmit(unsigned char data);

};//end of class SDCARDclass

 extern SDCARDclass SDCARD;

#endif
