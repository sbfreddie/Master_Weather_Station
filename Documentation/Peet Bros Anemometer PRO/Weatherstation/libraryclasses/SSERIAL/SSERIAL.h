/*

*/
#ifndef SSERIAL_h
#define SSERIAL_h

#define setupD5 DDRD |=0x20   // data pin 5 is set to output send request
#define setupD4 DDRD &= ~0x10  //data pin 4 as input listen for data

#include "WProgram.h"

class SSERIALclass
{
public:
SSERIALclass();  //the constructor
void begin(int baud_rate);   //user supplies baud rate
unsigned int request();  //upload data from remote slave
void senddata(unsigned int send_data);  //send data to master

private:
int baud;  //This is user supplied baud rate in microsecs (80 min)

};//end of class SERIALclass

 extern SSERIALclass SSERIAL;

#endif
