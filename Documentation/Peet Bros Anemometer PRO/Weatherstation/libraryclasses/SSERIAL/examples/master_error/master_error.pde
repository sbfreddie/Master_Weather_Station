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
//anything faster than this gives large error rate
unsigned int rec_data = 0;  //the integer received
void setup()   // run once, when the sketch starts
{
 Serial.begin(9600);	// initialize serial communication with computer
 SSERIAL.begin(baud);   // set transmission baud rate default = 400
}

void loop()			   // run over and over again
{ 
 unsigned int temp = 0;
 unsigned int errors = 0; //number of errors
 byte j = 0;   // for start counter
 for (j = 0; j < 100 ; j +=1)   //get 100 data transfers
 {
 temp = SSERIAL.request();  //get an integer transfer
 if (temp != 30555)
 {
 //Serial.println(temp);  
 errors +=1;  //we have an error
 }//end of != 30555
 delay(1);
 }//end of get 100 data transfers
 Serial.println(errors);
 delay(1);
}//end of loop


