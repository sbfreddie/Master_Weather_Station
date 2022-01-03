/*    Serial data transfer of an unsigned integer from a remote arduino.
      This is the slave arduino
      SSERIAL.begin(baud);  sets up the baud speed for serial transmission
      SSERIAL.senddata(send_data); Waits until request for data no timeout.
      when it gets request it sends send_data in serial to master.
      D2 is polled for request (LOW).
      Data is sent on D3.
*/

#include <SSERIAL.h>

unsigned int send_data = 30555;  //the integer to send
const int baud = 100;   //the pulse width of the serial data

void setup()		// run once, when the sketch starts
{
 SSERIAL.begin(baud);    // set transmission baud rate default = 400
}

void loop()			   // run over and over again
{
  SSERIAL.senddata(send_data);   //sends data when requested
}//end of loop


