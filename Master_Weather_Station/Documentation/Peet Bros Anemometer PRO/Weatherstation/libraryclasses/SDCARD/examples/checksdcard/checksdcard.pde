/* Connect sd card as shown in circuit diagram.
   Copy SDCARD folder into arduino-0017/hardware/libraries.
   This code will write/read directly to a sector on the sd card
     sector number 0 to 1980000 for a 1.0GB card.
   This program writes 199 to every byte on the sd card.
   It will only stop when it gets read/write or memory error.
   We hope that is after the very last byte of the sd card.
*/

#include <SDCARD.h>
volatile unsigned char buffer[512] ;  // this 512 bytes read from or written to sd card
unsigned long sector = 0;  // the sector we will write or read from
int error = 0;  //the error will be 0 if the sd card works
unsigned long errormillis = 0;        // the time when error occured

void setup()			  // run once, when the sketch starts
{
 Serial.begin(9600);			   // initialize serial communication with computer
}

void loop()			   // run over and over again
{
  for (sector=0; sector<4000000 ; sector++)
  {
	int i = 0;  //general purpose counter
        for(i=0;i<512;i++)
	buffer[i]=199;      //fill the buffer with a number between 0 and 255
	
	 error = SDCARD.writeblock(sector);  //write the buffer to this sector on the sd card

	 if (error != 0)
         errormillis = millis();   //remember the time had error
	 while (error !=0)  //we stay in this loop
     {
         Serial.print("sd card write error... code =  ");
         Serial.println(error);
         Serial.print("write error at sector=  ");
         Serial.println(sector);
         sendtime();
         delay(1000);   //send every second	 
     }//end of if we have an error
    
        i = 0;  //general purpose counter
        for(i=0;i<512;i++)
	buffer[i]=0;      //clear the buffer
	 
	error = SDCARD.readblock(sector);  //read into the buffer this sector in sd card

         if (error != 0)
         errormillis = millis();   //remember the time had error
         while (error !=0)   //we stay in this loop
     {
         Serial.print("sd card read error... code =  ");
         Serial.println(error);
         Serial.print("read error at sector=  ");
         Serial.println(sector);
         sendtime();
         delay(1000);   //send every second	 
     }//end of if we have an error
     
      
       for(i=0; i<512; i++) //read 512 bytes
     {
      if (buffer[i] != 199)
      errormillis = millis();   //remember the time had error
      while (buffer[i] != 199)  //we stay in this loop
      {
         Serial.print("memory error at byte =  ");
         Serial.println(i);
         Serial.print("memory error at sector=  ");
         Serial.println(sector);
         sendtime();
         delay(1000);   //send every second	 
      }//end of we have a memory error
     }//end of read 512 bytes
     
     if (sector % 100 ==0)
     {
       errormillis = millis();   //remember the time 
       Serial.print("we have no errors up to sector =  ");
       Serial.println(sector);
       sendtime();
     }//end of ever 10 sectors
  }//end of 4000000 sectors done
}//end of loop
 
void sendtime()
{
         Serial.print("elapsed time hours= ");
         Serial.print(errormillis/3600000);
         Serial.print("  elapsed time minutes= ");
         Serial.print((errormillis/60000) % 60);
         Serial.print("  elapsed time seconds= ");
         Serial.println((errormillis/1000) % 60);
}//end of send the time

