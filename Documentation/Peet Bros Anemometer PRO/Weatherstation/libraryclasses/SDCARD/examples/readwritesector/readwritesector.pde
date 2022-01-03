/* Connect sd card as shown in circuit diagram.
   Copy SDCARD folder into arduino-0017/hardware/libraries.
   This code will write/read directly to a sector on the sd card
     sector number 0 to 1980000 for a 1.0GB card.
*/

#include <SDCARD.h>
volatile unsigned char buffer[512] ;  // this 512 bytes read from or written to sd card
unsigned long sector = 99999;  // the sector we will write or read from
int error = 0;  //the error will be 0 if the sd card works

void setup()			  // run once, when the sketch starts
{
 Serial.begin(9600);			   // initialize serial communication with computer
}

void loop()			   // run over and over again
{
   if (Serial.available() > 0)   // do nothing if have not received a byte by serial
   {
    int inByte = Serial.read();
    if (inByte == 'r')  // send a "r" to start the read / write sector process
    {
	int i = 0;  //general purpose counter
        for(i=0;i<512;i++)
	buffer[i]=199;      //fill the buffer with a number between 0 and 255
	
	 error = SDCARD.writeblock(sector);  //write the buffer to this sector on the sd card
	
	 if (error !=0)
     {
         Serial.print("sd card write error... code =  ");
         Serial.println(error);	 
     }//end of if we have an error
	 
	error = SDCARD.readblock(sector);  //read into the buffer this sector in sd card

         if (error !=0)
     {
         Serial.print("sd card read error... code =  ");
         Serial.println(error);	 
     }//end of if we have an error
	else
     {
       int number =0;
       for(i=0; i<512; i++) //read 512 bytes
     {
       number = buffer[i];    //convert to integer       
       if(((i % 8) == 0) & ( i != 0))  
       Serial.println("");   //write 8 then start a new line
       Serial.print(number);
       Serial.print("  ");   //tab to next number
     }//end of read 512 bytes
     }//end of else we have no error
    } //end of received "r"
   }//end of serial available
}//end of loop
 



