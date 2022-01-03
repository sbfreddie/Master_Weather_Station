/*    Serial data transfer of an unsigned integer from a remote arduino.
      This is the master arduino
      SSERIAL.begin(baud);  sets up the baud speed for serial transmission
      SSERIAL.request() send request to remote arduino.
      Then it waits for an unsigned integer to be sent back in serial.
      Returns 65535 if times out after 1 second.
      The request is sent (LOW) on D3.
      The data is received on D2.
*/

#include <SSERIAL.h>

int baud = 100;   //the pulse width of the serial data
unsigned int rec_data = 0;  //the integer received
void setup()   // run once, when the sketch starts
{
 Serial.begin(9600);	// initialize serial communication with computer
 SSERIAL.begin(baud);   // set transmission baud rate default = 400
}

void loop()			   // run over and over again
{ 
 rec_data = SSERIAL.request();
 Serial.println(rec_data);  //print the data from remote
 delay(100);
}//end of loop


