#include <SSERIAL.h>

#define INTERUPT_0  DDRD &= ~0x04   // (digital 2)PD2 - INT0 is set to input 
#define INTERUPT_0_HI  PORTD |= 0x04   // pull up input pin (digital 2)PD2 - INT0
#define INTERUPT_0_HIGHTOLOW  EICRA = 0x02  //falling edge  on INT0
#define INTERUPT_1  DDRD &= ~0x08   // (digital 3)PD3 - INT1 is set to input
#define INTERUPT_1_HI  PORTD |= 0x08   // pull up input pin (digital 3)PD3 - INT1
#define INTERUPT_1_HIGHTOLOW  EICRA |= 0x08  //falling edge  on INT1

volatile char flash = 0;  //INT0 adds to this for each lightning flash
volatile char drops = 0;  //INT1 adds to this for each rain bucket tip

unsigned int lightning = 10000;  //the lightning flashes to send-ofset flag
unsigned int rainfall = 0;  //the rainfall to send
unsigned int daylight = 0;  //the light intensity to send
unsigned int outtemp = 0;  //the temperature to send
unsigned int outhumid = 0;  //the humidity to send
unsigned int windspeed = 0;  //the wind speed to send
unsigned int winddirection = 0;  //the wind direction to send

const int outtempadc = 1;  // Analog input pin for temperature
const int daylightadc = 0;  // Analog input pin for daylight
const int baud = 100;   //the pulse width of the serial data
const int sensor_offset = -13;  //the offset to give correct RH
const int humidfreq = 6;   // digital input pin for humidity

char serialwind = 0;  // character input from computer in ASCII
char asciispeed[8] = "0";  //the computer inputs the startblock number
char asciidirection[8] = "0";  //the computer inputs the startblock number

void setup()		// run once, when the sketch starts
{
SSERIAL.begin(baud);    // set transmission baud rate default = 400
Serial.begin(9600);  // initialize serial communication with computer

INTERUPT_0;
INTERUPT_0_HI; //setup INT0
INTERUPT_0_HIGHTOLOW; //falling edge

INTERUPT_1;
INTERUPT_1_HI;  //setup INT1
INTERUPT_1_HIGHTOLOW; //falling edge

EIMSK = 0x03;  //enable interupts on INT0 and INT1
}

void loop()			   // run over and over again
{
   if (Serial.available() > 0)   //do we have serial coms
    {
    int streamnumber = Serial.available();
    for(char i = 0; i < streamnumber; i++)
    {
     serialwind = Serial.read();  //move through to find 's' or run out
     
      if (serialwind  == 's')   // s for speed sample 
      {
      byte j = 0;   // for start counter enter digits
      for (j = 0; j < 8 ; j +=1)
      asciispeed[j] = 0;  //clear the buffer before read
      j = 0; //reset counter
      while (Serial.available() != 0)   //input the sample
      {
       char temp = Serial.read();
       if(temp == 'e')
       break;  //we have the speed sample
       asciispeed[j] = temp; //update sample
       j +=1;   //next digit position
      }//end of input the speed sample
      windspeed = (windspeed + atoi(asciispeed))/2;//running average 
      }//end of the serial is wind speed 
      
      if (serialwind  == 'd')   // d for direction sample 
      {
      byte j = 0;   // for start counter enter digits
      for (j = 0; j < 8 ; j +=1)
      asciidirection[j] = 0;  //clear the buffer before read
      j = 0; //reset counter
      while (Serial.available() != 0)   //input the sample
      {
       char temp = Serial.read();
       if(temp == 'e')
       break;  //we have the direction sample
       asciidirection[j] = temp;  //update sample
       j +=1;   //next digit position
       }//end of input the direction sample
       winddirection = (winddirection + atoi(asciidirection))/2;//running average   
      }//end of the serial is wind direction 
      
     }//end of read the serial buffer
    }//end of we have a serial coms
    
  lightning += flash; //update the total flashes
  flash = 0; //reset the flash counter
  if(lightning > 11000)
  lightning = 10000;  //reset the total when getting up to 1024
  rainfall += drops;  //update the total rain
  drops = 0;  //reset the drops counter
  if(rainfall > 1000)
  rainfall = 0;  //reset the total when getting up to 1024
  
  outtemp = adc(outtempadc);   //get the analog sample for temperature
  outhumid = humidity(humidfreq); //get the humidity
  daylight = adc(daylightadc);   //get the analog sample for daylight
   
  SSERIAL.senddata(lightning);   //set lightning as the first sample
  SSERIAL.senddata(rainfall);   //sends data when requested
  SSERIAL.senddata(daylight);   //sends data when requested
  SSERIAL.senddata(outtemp);   //sends data when requested
  SSERIAL.senddata(outhumid);   //sends data when requested
  SSERIAL.senddata(windspeed);   //sends data when requested
  SSERIAL.senddata(winddirection);   //sends data when requested  
}//end of loop

inline unsigned int adc(int pin)
{
 unsigned int sample = 0;   //reset the sample value
 for(char i = 0; i <= 64; i++)
 sample += analogRead(pin);  //get 64 samples added together 1024 x 64 max
 sample = sample / 64;  //get the average of 64 samples
 return sample;
 //tempsample +=sample;  //add the 10 second sample
}//end of adc

inline unsigned int humidity(int pin)
{
    unsigned long duration = 0;
    for (char j = 0; j < 64 ; j +=1)
    {
     duration += pulseIn(pin, HIGH, 6000); //timeout if not working
    }  //pulse width has some jitter so get average to smooth
    duration /=64;
    return (duration - sensor_offset);  
}//end of humidity

ISR(INT0_vect)
{
flash +=1;  //we have a lightning flash
}// INT0 service routine for lightning detector

ISR(INT1_vect)
{
drops +=1;  //we have a rain bucket tip
}// INT1 service routine for envelope detector
