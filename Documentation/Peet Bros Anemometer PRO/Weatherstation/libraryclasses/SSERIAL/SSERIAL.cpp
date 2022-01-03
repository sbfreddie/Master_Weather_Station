/*

*/

#include "WProgram.h"
#include "SSERIAL.h"

SSERIALclass::SSERIALclass()  //constructor
{
setupD5;
setupD4;  //set tx and rx pins
baud = 2500;  //defaul rate 400 baud
}

void SSERIALclass::begin(int baud_rate)
{
baud = baud_rate;  //user supplied baud rate
}
unsigned int SSERIALclass::request()
{
 //sendpinLOW;  //request for data
 PORTD &= ~0x20; //send d5 low request for data
 delay(1);
// sendpinHIGH;  //no request for data
 PORTD |= 0x20; //send d5 high no request for data
 unsigned long start = millis();  //get the start time
 while ((PIND & 0x10) !=0)  //start pulse is 0 on pin D4
 if (millis() - start > 100)  //have we been waiting more than .1 second
 return 65535;   //we have a timeout
 //delayMicroseconds(1);  //wait until get start pulse
 delayMicroseconds((baud*3)/2);  //wait until first data bit
 unsigned int rec_data = 0;  //the integer received
 unsigned int temp = 0; //tempory for rec data
 byte j = 0;   // for start counter
 for (j = 0; j < 16 ; j +=1)  //get the 16 bits
 {
   temp = 0; //clear the tempory integer
   temp = (PIND & 0x10)<<11;  //get the received bit into b15 of temp
   rec_data |= temp>>j;  //put the bit into the integer
   delayMicroseconds(baud-5);  //wait till next bit
 }//end of got 16 bits
return rec_data; 
}

void SSERIALclass::senddata(unsigned int send_data)
{
 PORTD |= 0x20; //send d5 high
 while ((PIND & 0x10) !=0) //listen on d4 for request
 delayMicroseconds(1);  //wait until get request
 delay(10);  //just wait until the request pulse is done
 //send an integer to master, one start bit, one stop bit and the 16 bits of data
 unsigned int mask = 0x8000;  //mask for display digit bit
 PORTD &= ~0x20; //send d5 low  
 delayMicroseconds(baud);  //the pulse width
 while (mask != 0)  //send each bit to register 16 bits total
 {
 if ((send_data & mask) != 0)
 PORTD |= 0x20; //send d5 high          
 if ((send_data & mask) == 0)
 PORTD &= ~0x20; //send d5 low           
 delayMicroseconds(baud);  //the pulse width
 mask = mask>>1;  //look at next bit in digit display
 }//end of send 16 bits
 PORTD &= ~0x20; //send d5 low
 delayMicroseconds(baud);  //the pulse width
 PORTD |= 0x20; //send d5 high
}

SSERIALclass SSERIAL;
