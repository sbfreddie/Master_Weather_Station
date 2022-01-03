/*  On restart sends serial "ready" and current sd sector number.
    Current sd sector number held in EEPROM.
    Repeat every 10 seconds
    PC sends sd sector number to use and we start to get samples.
    Every 10 secs sample temperature humidity and airpressure.
    Send samples to PC and store in RAM.
    If 85 samples(255 bytes)in RAM save in sd card.
    If receive "u" and number from PC send requested sd sectors
    and RAM to the PC.
    If error in read/write to sd card stay in a loop and
    send "error" to PC every 10 seconds.
*/
#define sdcard   //yes we have a sd card
#define seven    //yes we have a 7 seg display
#include <EEPROM.h>
#include <SDCARD.h>    //read write 512 blocks to sd card
#include <SEVEN.h>    //seven segment display uses pins 8 , 7 and 6
#include <SSERIAL.h>
#include <stdlib.h>    // C utility functions
volatile unsigned char buffer[512] ;  //RAM store for samples
unsigned long lastmillis = 0;        // will store last time we looked
const int tempadc = 5;  // Analog input pin for temperature
const int voltadc = 1;  // Analog input pin for voltage
const int airadc = 4;  // Analog input pin for air pressure
const int pin = 9;   // digital input pin for humidity
const int sensor_offset = 1300;  //the offset to give correct RH
unsigned int tempsample = 0;  //adc temp averaged each 10 seconds
unsigned int humidsample = 0;  //humid  averaged each 10 seconds
unsigned int outtempsample = 0;  //adc temp averaged each 10 seconds
unsigned int outhumidsample = 0;  //humid  averaged each 10 seconds
unsigned int airsample =0;  //adc air averaged each 10 seconds
unsigned int daylight =0;  //adc air averaged each 10 seconds
unsigned int rainfall = 0;  //adc temp averaged each 10 seconds
unsigned int lightning = 0;  //humid  averaged each 10 seconds
unsigned int voltsample =0;  //adc air averaged each 10 seconds
unsigned int windspeed = 0;  //adc temp averaged each 10 seconds
unsigned int winddirection = 0;  //humid  averaged each 10 seconds
unsigned int ramaddress =0;  //address of next storage location in RAM
unsigned long currentblock = 0; //the current block number in the sd card
unsigned long loadblocks = 0;  //the start upload sector from computer
int inbyte = 0;  // character input from computer in ASCII
char restartblock[8];  //the computer inputs the startblock number
boolean restarted = false;   //set when the computer restarts the avr

void setup()
{
  Serial.begin(9600);  // initialize serial communication with computer 
  SSERIAL.begin(100);   // set transmission baud rate default = 400
  pinMode(pin, INPUT);  //humidity input
}

void loop()
{ 
  if ( ! restarted)  // if not restarted
  {
  while (Serial.available() == 0) //wait until computer is connected
    {
     ready();  //after restart send ready message
    }//end of wait till computer connected
   getcurrentblock();  //get the current block from computer
  }//end of not restarted
  
  if (millis() < lastmillis)
  lastmillis = 0;  //have a rollover every 52 days
  if (millis() - lastmillis > 10000)  //every 10 seconds
  {
    lastmillis += 10000;   // update the timer
    
    if (Serial.available() > 0)   //do we have a computer command
    {
     inbyte = Serial.read();  //get the command
     if (inbyte  == 'u')   //download all the stored samples 
     {   // computer sends "r4444" to upload all RAM and sd blocks from 4444
     
      byte j = 0;   // for start counter enter digits
      for (j = 0; j < 8 ; j +=1)
      restartblock[j] = '0';  //clear the buffer before read
      while (Serial.available() != 0)   //input the sd card start block
      {
       restartblock[j] = Serial.read();
       j +=1;   //next digit position 
      }//end of input sd card start block
      loadblocks = strtoul(restartblock,0,10);   //this is requested atarting block
      if (loadblocks > currentblock)
      loadblocks = currentblock;   //we must never have or runs through whole card
      
      uploadbuffer(ramaddress);  //upload the current sample that are in ram
      Serial.println("r");  //tell computer all RAM samples are sent
      uploadsdblocks();  //upload the requested sd blocks
      Serial.println("e");  //tell computer all samples are sent      
     }//end of download all stored sample samples   
    }//end of we have a computer command
    
    tempsample = adc(tempadc);   //get the analog sample for temperature
    airsample = adc(airadc);  //get analog sample for air pressure
    voltsample = adc(voltadc);  //get analog sample for voltage
    humidity();  //get the sample for the humidity
  
    unsigned int temp =  SSERIAL.request();
    if(temp != 65535)
    { //we have a connection
    if(temp > 9999)
    {  //we have the first int lightning
    lightning = temp - 10000;//lightning flashes
    rainfall =  SSERIAL.request();//200;//rain
    daylight = SSERIAL.request();//daylight
    outtempsample =  SSERIAL.request();//300;//outside temperature
    outhumidsample =  SSERIAL.request();//325;//outside humidity
    windspeed =  SSERIAL.request();//204;//wind speed = 20 kph
    winddirection =  SSERIAL.request();//768;//wind direction = 180
    if(windspeed == 0)
    winddirection = 768;//when no wind set direction = south
    }//end of have first int
    }//end of timeout
    sendsample(); //send a sample to the computer
    
    if (ramaddress == 506)  //have we have filled the ram buffer
    {         //this is 23 lots of 11 samples of 2 bytes per sample  
     lastblocksave();  //saved full ram to sd card so save block number
    }//end of have we filled the ram buffer
    
  }//end of every 10 seconds 
 
}//end of loop

inline unsigned int adc(int pin)
{
 unsigned int sample = 0;   //reset the sample value
 //tempsample = 0;  //reset the average
 int i = 0;
 for(i; i<=64; i++)
 sample += analogRead(pin);  //get 64 samples added together 1024 x 64 max
 sample = sample / 64;  //get the average of 64 samples
 return sample;
 //tempsample +=sample;  //add the 10 second sample
}//end of adc

inline void humidity()
{
    unsigned long duration = 0;
    byte j = 0;   // for start counter enter digits
    for (j = 0; j < 64 ; j +=1)
    {
     duration += pulseIn(pin, HIGH, 6000);
    }  //pulse width has some jitter so get average to smooth
    duration /=64;
    humidsample = duration - sensor_offset;  
}//end of humidity

inline void ready()
{
  unsigned long lastblock = 0;  //the last block number saved in the sd card
  unsigned long tempblock = 0;
  tempblock = EEPROM.read(0);  // remember the LSB of the last saved block
  lastblock |= tempblock;
  tempblock = EEPROM.read(1);  // remember the next LSB of the last saved block
  lastblock |= tempblock << 8;
  tempblock = EEPROM.read(2);  // remember the next LSB of the last saved block
  lastblock |= tempblock << 16;
  tempblock = EEPROM.read(3);  // remember the next MSB of the last saved block
  lastblock |= tempblock << 24;
  Serial.println("ready");  //send computer the ready to reset message
  Serial.println(lastblock);  //send computer the last saved block number
  SEVEN.displayP(0x0);
  delay(1000);
  SEVEN.displayP(0x143e5f75);  //flash ready
  delay(9000);  //every 10 seconds 
}//end of ready

inline void getcurrentblock()
{
  byte i = 0;   // for start counter enter digits
  while (Serial.available() != 0)   //input the sd card start block
    {
     restartblock[i] = Serial.read();
     i +=1;   //next digit position 
    }//end of input sd card start block
  currentblock = strtoul(restartblock,0,10);   //converts string to long base 10
  EEPROM.write(0,currentblock);  //write the LSB of saved block to EEPROM
  EEPROM.write(1,currentblock >> 8);  //write the next LSB of saved block to EEPROM
  EEPROM.write(2,currentblock >> 16);  //write the next LSB of saved block to EEPROM
  EEPROM.write(3,currentblock >> 24);  //write the MSB of saved block to EEPROM
  restarted = true;  // the computer has restarted the avr;
  lastmillis = millis();  //reset the timer  
}//end of get current block

inline void sendsample()
{
    Serial.println("sensors");  //send samples
    Serial.println(tempsample); //send stored sample to computer
    Serial.println(humidsample); //send stored sample to computer
    Serial.println(airsample); //send stored sample to computer
    Serial.println(voltsample); //send stored sample to computer
    Serial.println(lightning); //send stored sample to computer
    Serial.println(rainfall); //send stored sample to computer
    Serial.println(daylight); //send stored sample to computer
    Serial.println(outtempsample); //send stored sample to computer
    Serial.println(outhumidsample); //send stored sample to computer
    Serial.println(windspeed); //send stored sample to computer
    Serial.println(winddirection); //send stored sample to computer
    Serial.println(ramaddress);  //send ram address of this sample
    Serial.println(currentblock);  //send the next block number to fill
    #ifdef sdcard   //only fill up ram buffer if we have a sd card
    buffer[ramaddress] = tempsample; // store the lsb in ram
    buffer[ramaddress+1] = tempsample >> 8; //store the msb in ram
    buffer[ramaddress+2] = humidsample; // store the lsb in ram
    buffer[ramaddress+3] = humidsample >> 8; //store the msb in ram
    buffer[ramaddress+4] = airsample; // store the lsb in ram
    buffer[ramaddress+5] = airsample >> 8; //store the msb in ram
    buffer[ramaddress+6] = voltsample; // store the lsb in ram
    buffer[ramaddress+7] = voltsample >> 8; //store the msb in ram
    buffer[ramaddress+8] = lightning; // store the lsb in ram
    buffer[ramaddress+9] = lightning >> 8; //store the msb in ram
    buffer[ramaddress+10] = rainfall; // store the lsb in ram
    buffer[ramaddress+11] = rainfall >> 8; //store the msb in ram
    buffer[ramaddress+12] = daylight; // store the lsb in ram
    buffer[ramaddress+13] = daylight >> 8; //store the msb in ram
    buffer[ramaddress+14] = outtempsample; // store the lsb in ram
    buffer[ramaddress+15] = outtempsample >> 8; //store the msb in ram
    buffer[ramaddress+16] = outhumidsample; // store the lsb in ram
    buffer[ramaddress+17] = outhumidsample >> 8; //store the msb in ram
    buffer[ramaddress+18] = windspeed; // store the lsb in ram
    buffer[ramaddress+19] = windspeed >> 8; //store the msb in ram
    buffer[ramaddress+20] = winddirection; // store the lsb in ram
    buffer[ramaddress+21] = winddirection >> 8; //store the msb in ram
    ramaddress +=22 ; //increment the RAM pointer for the 11 samples
    #endif
    #ifdef seven  //only if we have a seven segment display
    if ((lastmillis/10000)%9 == 0)   //display indoor humidity
    SEVEN.displayA(SEVEN.displayI(humidsample / 10.23),0x33);   
    if ((lastmillis/10000)%9 == 1)   //display air pressure
    SEVEN.displayI( airsample / 8.5 + 950);  
    if ((lastmillis/10000)%9 == 2)   //display temperature
    SEVEN.displayA(SEVEN.displayF(tempsample / 25.6),0x3a);  
    if ((lastmillis/10000)%9 == 3)   //display voltage
    SEVEN.displayA(SEVEN.displayF(voltsample / 31.7),0x70);
    if ((lastmillis/10000)%9 == 4)   //display outdoor humidity
    SEVEN.displayA(SEVEN.displayI(outhumidsample / 10.23),0x33);
    if ((lastmillis/10000)%9 == 5)   //display outside temperature
    SEVEN.displayA(SEVEN.displayF(outtempsample /17.06 - 10) ,0x3a); 
    if ((lastmillis/10000)%9 == 6)   //display wind speed
    SEVEN.displayA(SEVEN.displayI(windspeed/10),0x72);
    if ((lastmillis/10000)%9 == 7)   //display wind direction
    SEVEN.displayA(SEVEN.displayI((winddirection-512)* 0.7),0x75);
    if ((lastmillis/10000)%9 == 8)   //display air pressure
    SEVEN.displayA(SEVEN.displayI(lightning),0x1e);      
    #endif  
}//end of send sample

inline void lastblocksave()
{
      unsigned int e = 0;  //the error code from the sd card
      e = SDCARD.writeblock(currentblock);  //save this 256 block of integer data 
      while (e != 0)   //cant continue if sd card not working
      {
       Serial.println("writesderror");  //send computer sd card error
       Serial.println(e);  //send computer the error number
       //SEVEN.displayP(0x0);
       delay(1000);
    // SEVEN.displayP(0x1e5f4138);  //flash fail
       delay(9000);  //every 10 seconds 
      }//end of sd card not working
      currentblock +=1;  //go to the next block in sd card
      EEPROM.write(0,currentblock);  //write the LSB of saved block to EEPROM
      EEPROM.write(1,currentblock >> 8);  //write the next LSB of saved block to EEPROM
      EEPROM.write(2,currentblock >> 16);  //write the next LSB of saved block to EEPROM
      EEPROM.write(3,currentblock >> 24);  //write the MSB of saved block to EEPROM      
      ramaddress = 0;   //we can now start again to save samples in RAM        
}//end of sd save

inline void uploadsdblocks()
{  
          
      int i = 0;  //counter for save/restore ram samples
      for (i=0 ; i < ramaddress  ; i += 1)  //save RAM samples in the EEPROM
      EEPROM.write(4 + i,buffer[i]);  //first 4 bytes used for sd card sect
      
      while (loadblocks != currentblock)  //upload requested blocks
      {           //load blocks must be <= to current block or run through whole sd card
      unsigned int e = 0;  //the error code from the sd card
      e = SDCARD.readblock(loadblocks);  //save this 256 block of integer data 
       while (e != 0)   //cant continue if sd card not working
       {
        Serial.println("readsderror");  //send computer sd card error
        Serial.println(e);  //send computer the error number
        SEVEN.displayP(0x0);
        delay(1000);
        SEVEN.displayP(0x1e5f4138);  //flash fail
        delay(9000);  //every 10 seconds 
       }//end of sd card not working 
     
      uploadbuffer(512);  //upload one sd block that is now in RAM
      loadblocks +=1;  //go to next block if needed
      }//end of upload blocks
      
      for (i=0 ; i < ramaddress  ; i += 1)  //restore RAM samples from the EEPROM
      buffer[i] = EEPROM.read(4 + i);  //first 4 bytes used for sd card sector
      
}//end of upload sdblocks

void uploadbuffer(int endnumber)
{
      int i = 0;  //counter for upload ram samples
      unsigned int sample = 0;  //tempory sample number
      for (i=0 ; i < endnumber  ; i += 2)  //upload RAM samples
      {
       sample =0;  //store for sample
       sample |= buffer[i + 1];   //get the MSB
       sample  = sample << 8 ;  //get it to the MSB position
       sample |= buffer[i] ;  //get the LSB
       Serial.println(sample);
      }//end upload ram samples
}//end of upload buffer


