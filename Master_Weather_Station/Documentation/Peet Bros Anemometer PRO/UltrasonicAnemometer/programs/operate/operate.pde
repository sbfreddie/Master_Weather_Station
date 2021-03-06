#define NORTHDRIVER  DDRD |= 0x20    //(digital 5)PD5 is output to north driver
#define NORTHDRIVER_ON  PORTD &= ~0x20  //(digital 5)PD5 enable north driver
#define NORTHDRIVER_OFF  PORTD |= 0x20  //(digital 5)PD5 disable north driver
#define SOUTHDRIVER  DDRD |= 0x80   //(digital 7)PD7 is output to south driver
#define SOUTHDRIVER_ON  PORTD &= ~0x80;  //(digital 7)PD7 enable south driver  
#define SOUTHDRIVER_OFF  PORTD |= 0x80  //(digital 7)PD7 disable south driver
#define EASTDRIVER  DDRC |= 0x08    //(digital 17)PC3 is output to east driver
#define EASTDRIVER_ON  PORTC &= ~0x08  //(digital 17)PC3 enable east driver
#define EASTDRIVER_OFF  PORTC |= 0x08  //(digital 17)PC3 disable east driver
#define WESTDRIVER  DDRC |= 0x02   //(digital 15)PC1 is output to west driver
#define WESTDRIVER_ON  PORTC &= ~0x02  //(digital 15)PC1 enable west driver
#define WESTDRIVER_OFF  PORTC |= 0x02  //(digital 15)PC1 disable west driver

#define NORTHLOW_IN  DDRB &= ~0x02  //(digital 9) PB1 low side of north 
#define NORTHLOW_IN_TRI  PORTB &= ~0x02 //goes to input and tristate
#define NORTHLOW_OUT  DDRB |= 0x02;  //(digital 9) PB1 low side of north
#define NORTHLOW_OUT_EARTH  PORTB &= ~0x02; //goes to output and to earth

#define SOUTHLOW_IN  DDRB &= ~0x04;  //(digital 10) PB2 low side of south  
#define SOUTHLOW_IN_TRI  PORTB &= ~0x04; //goes to input and tristate
#define SOUTHLOW_OUT  DDRB |= 0x04;  //(digital 10) PB2 low side of south 
#define SOUTHLOW_OUT_EARTH  PORTB &= ~0x04; //goes to output and to earth

#define EASTLOW_IN  DDRB &= ~0x08  //(digital 11) PB3 low side of east 
#define EASTLOW_IN_TRI  PORTB &= ~0x08 //goes to input and tristate
#define EASTLOW_OUT  DDRB |= 0x08;  //(digital 11) PB3 low side of east 
#define EASTLOW_OUT_EARTH  PORTB &= ~0x08 //goes to output and to earth

#define WESTLOW_IN  DDRB &= ~0x10  //(digital 12) PB4 low side of west 
#define WESTLOW_IN_TRI  PORTB &= ~0x10 //goes to input and tristate
#define WESTLOW_OUT  DDRB |= 0x10;  //(digital 12) PB4 low side of west 
#define WESTLOW_OUT_EARTH  PORTB &= ~0x10 //goes to output and to earth

#define MUXPIN9  DDRD |= 0x10   // 4042 pin 9 control(digital 4)PD4 is always an output
#define MUXPIN9_0  PORTB &= ~0x01  //receive south or north 
#define MUXPIN9_1  PORTB |= 0x01  //receive west or east 
#define MUXPIN10  DDRB |= 0x01   // 4042 pin 10 control(digital 8)PB0 is always an output 
#define MUXPIN10_0  PORTD &= ~0x10  //receive west or south 
#define MUXPIN10_1  PORTD |= 0x10  //receive east or north  
//9=0,10=0 receive south:9=0,10=1 receive north:9=1,10=0 receive west:9=1,10=1 receive east

#define INTERUPT_0  DDRD &= ~0x04   // (digital 2)PD2 - INT0 is set to input from comparitor
#define INTERUPT_0_TRI  PORTD &= ~0x04   // tri-state input pin (digital 2)PD2 - INT0
#define INTERUPT_0_HIGHTOLOW  EICRA = 0x02  //falling edge  on INT0
#define INTERUPT_1  DDRD &= ~0x08   // (digital 3)PD3 - INT1 is set to input from comparitor
#define INTERUPT_1_TRI  PORTD &= ~0x08   // tri-state input pin (digital 3)PD3 - INT1
#define INTERUPT_1_HIGHTOLOW  EICRA |= 0x08  //falling edge  on INT1

#define PULSEOUTPUT  DDRD |= 0x40;   //(digital 6)PD6 is output for OC0A 40 khz driver

volatile unsigned int zcdtof = 0;  //INT0 sets this as the tof from ZCD
volatile unsigned int envelopetof = 0;  //INT1 sets this as the tof from the envelope detector
volatile boolean pulsedone = false; //the full sequence(2.4 ms) is finished
volatile boolean pulsesent = false;  //the 40 khz pulse has been sent(0.64 ms)

unsigned long Naverage = 0; //average of north(east) envelope samples
unsigned long Saverage = 0;  //average of south(west) envelope samples
unsigned int NZCD[300]; //North tof samples from zero crossing detector
unsigned int SZCD[300]; //South tof samples from zero crossing detector
int Northerror[20]; //The tof error for north-south transmit
int Easterror[20]; //The tof error for east-west transmit
const int pulsefrequency = 196;  //the pulse frequency which gives the best response
//this gives 40.81 khz(200 = 40.0khz)
const char tempinput = 5;  //use adc5 as the ambient temperature 
const int Northadd = 100;  //add to north to compensate for N-S error
const int Eastadd = -100;  //add to east to compensate for E-W error

void setup()   
{                
MUXPIN9;
MUXPIN10;  //set 4052 mux control lines

INTERUPT_0;
INTERUPT_0_TRI; //setup INT0
INTERUPT_0_HIGHTOLOW; //falling edge

INTERUPT_1;
INTERUPT_1_TRI;  //setup INT1
INTERUPT_1_HIGHTOLOW; //falling edge

WESTDRIVER;
WESTDRIVER_OFF;
NORTHDRIVER;
NORTHDRIVER_OFF;
SOUTHDRIVER;
SOUTHDRIVER_OFF;
EASTDRIVER;
EASTDRIVER_OFF; //setup all the drivers
PULSEOUTPUT; //setup the 40khz pulse output after once through loop

//TIMER0 is used as a pulse generator

//Timer2 is the system timer.
//OCR2B and prescale are set to give 0.64 ms delay
//during this .64 ms the 40 khz generator runs
//OCR2B is reset to give a delay of .48 ms 
//INT0 and INT1 is then enabled and pulse width samples are taken.
//The smallest tof is 0.925 ms at 50C with 100kph wind.
//OCR2A and prescale are set to 2.4 ms. this stops pulse
//sequence operation via boolean pulsedone

//timer1 is set as a high speed counter 400 counts = 25 micro secs
//It runs through out. INT0 stops count at tof count
//the count when a falling pulse is detected.(-fixed delay)
TCCR1A = 0x00;  //set up timer 1 as 16 bit counter 
TCCR1B = 0x01;  // normal mode counts to 65535 and resets
TIMSK1 = 0x00;  // no interupts  1 count = 1/16 microsec

Serial.begin(9600);   // initialize serial communications at 960
}//end of set up

void loop()                     
{
 int Ext = analogRead(tempinput);
 float Exts =(Ext *60.0)/1024.0 - 10.0; //the temperature in centigrade
 Exts = 20.0;  //if temp is not used
 //Serial.print("T= ");
// Serial.print(Exts);
 Exts = 331.5 + (0.6 * Exts);  //the speed of sound at the ambient temperature
 float windstep = (sqrt((207.36E6 + Exts*Exts)) - 14400.0)*3.6; 
 //the wind step in kph at the ambient temperature
 
 EASTLOW_IN;
 EASTLOW_IN_TRI;  //east low is disconnected
 EASTDRIVER_OFF;
 WESTLOW_IN;
 WESTLOW_IN_TRI;  //disconnect low side of west
 WESTDRIVER_OFF;
 
 unsigned int counter = 0; //general purpose counter
 Naverage = 0;
 NORTH();
 for(counter = 0; counter < 300; counter++)
 {
 sequence();  //transmit and receive pulse
 NZCD[counter] = zcdtof;
 if ( counter != 0)  //first one no good
 Naverage += envelopetof;
 }//end of fill the zcd samples
 
 Saverage = 0;
 SOUTH();
 for(counter = 0; counter < 300; counter++)
 {
 sequence();  //transmit and receive pulse
 SZCD[counter] = zcdtof;
 if ( counter != 0)  //first one no good
 Saverage += envelopetof;
 }//end of fill the zcd samples
 
 int diff = 0;
 long diffav = 0;
 for(counter = 3; counter < 300; counter++) //first 3 are not good
 {
 diff = NZCD[counter] - SZCD[counter];
 
 while ( diff < - 200) //a minus pulse slip so add a pulse
 diff +=400; //there may be more than one pulse slip
 
 while ( diff > 200) //a plus pulse slip so subtract a pulse
 diff -=400; //there may be more than one pulse slip
  
 diffav += diff;//sum the values

 }//end of zcd samples
// Serial.print(" z= ");
//Serial.print(diffav);
 //Serial.print(" N= ");
 Naverage = Naverage/299;
 Naverage += Northadd;  //compensate for N-S error
 //Serial.print(Naverage);
// Serial.print(" S= ");
 Saverage = Saverage/299;
// Serial.print(Saverage);
 
 long delta = (diffav *200)/55000; //The tof difference should be this
 if (delta < 0)   //diffav is exact and robust so any error must be in
 delta = - delta; //the tof difference. Calculate and update error
 for( counter = 0; counter < 19; counter++ )
 Northerror[19-counter] = Northerror[18-counter]; //shift up one
 if (Naverage > Saverage) //is difference positive
 Northerror[0] = -(((Naverage - Saverage)%400)-delta);  //put in new error term
 else  //difference is negative
 Northerror[0] = (Saverage - Naverage)%400 - delta;  //put in new error term
 int Nerror = 0; //the north-south error tof error value
 for( counter = 0; counter < 20; counter++ )
 Nerror += Northerror[counter];  
 Nerror /=20;  //the average of the tof zero error 
 Naverage += Nerror;  //compensate the tof value
// Serial.print(" e= ");
// Serial.print(Nerror);
 
 int Ediff = Naverage - Saverage;
 
 boolean function = false;
 if(diffav < 0) //we are on the negative side of wind function
 {
  function = true;
  diffav = - diffav;  //invert the wind function 
  Ediff = - Ediff;  //and  Ediff
 }//end of negative side of wind function
 float wind = (windstep * diffav)/110000.0; //must have this fine value
 if(Ediff > -100)  //we have positive wind
 {
 int pstep = -1;
 while(Ediff >-100)
 {
  pstep +=1; //count each step 
  Ediff -= 400;  //move one step
  if(pstep == 7)
  Ediff = -1;  //we have exceeded our range 100 kph max
 }//end of count number of steps positive
 wind += pstep * windstep; //this is final wind speed
 }//end of we are on the positive side
 else    //we have negative wind
{
int nstep = 0;
while(Ediff < -100)
{
 nstep += 1; //count each step
 Ediff += 400;  //move one step 
 if(nstep == 8)
 Ediff = 1;  //we have exceeded our range 100 kph
}//end of count number of steps negative
 wind -= nstep * windstep; //this is final wind speed
}//end of we are on negative side
 if (function)
 wind = - wind;//the function has been inverted
 float N_Swind = wind;
// Serial.print(" w= ");
// Serial.print(N_Swind);
 //.............................................................................................................
 NORTHLOW_IN; 
 NORTHLOW_IN_TRI; //north low is disconnected
 NORTHDRIVER_OFF;
 SOUTHLOW_IN;
 SOUTHLOW_IN_TRI;  //disconnect south low
 SOUTHDRIVER_OFF;
 
 Naverage = 0;
 EAST();
 for(counter = 0; counter < 300; counter++)
 {
 sequence();  //transmit and receive pulse
 NZCD[counter] = zcdtof;
 if ( counter != 0)  //first one no good
 Naverage += envelopetof;
 }//end of fill the zcd samples
 
 Saverage = 0;
 WEST();
 for(counter = 0; counter < 300; counter++)
 {
 sequence();  //transmit and receive pulse
 SZCD[counter] = zcdtof;
 if ( counter != 0)  //first one no good
 Saverage += envelopetof;
 }//end of fill the zcd samples
 
 diff = 0;
 diffav = 0;
 for(counter = 3; counter < 300; counter++) //first 3 are not good
 {
 diff = NZCD[counter] - SZCD[counter];
 
 while ( diff < - 200) //a minus pulse slip so add a pulse
 diff +=400; //there may be more than one pulse slip
 
 while ( diff > 200) //a plus pulse slip so subtract a pulse
 diff -=400; //there may be more than one pulse slip
  
 diffav += diff;//sum the values

 }//end of zcd samples
 //Serial.print("....z= ");
/// Serial.print(diffav);
//Serial.print(" E= ");
 Naverage = Naverage/299;
 Naverage += Eastadd;  //compensate for E-W error
// Serial.print(Naverage);
// Serial.print(" W= ");
 Saverage = Saverage/299;
// Serial.print(Saverage);
 
 delta = (diffav *200)/55000; //The tof difference should be this
 if (delta < 0)   //diffav is exact and robust so any error must be in
 delta = - delta; //the tof difference. Calculate and update error
 for( counter = 0; counter < 19; counter++ )
 Easterror[19-counter] = Easterror[18-counter]; //shift up one
 if (Naverage > Saverage) //is difference positive
 Easterror[0] = -(((Naverage - Saverage)%400)-delta);  //put in new error term
 else  //difference is negative
 Easterror[0] = (Saverage - Naverage)%400 - delta;  //put in new error term
 Nerror = 0; //the north-south error tof error value
 for( counter = 0; counter < 20; counter++ )
 Nerror += Easterror[counter];  
 Nerror /=20;  //the average of the tof zero error 
 Naverage += Nerror;  //compensate the tof value
// Serial.print(" e= ");
// Serial.print(Nerror);
 
 Ediff = Naverage - Saverage;
 
 function = false;
 if(diffav < 0) //we are on the negative side of wind function
 {
  function = true;
  diffav = - diffav;  //invert the wind function 
  Ediff = - Ediff;  //and  Ediff
 }//end of negative side of wind function
  wind = (windstep * diffav)/110000.0; //must have this fine value
 if(Ediff > -100)  //we have positive wind
 {
 int pstep = -1;
 while(Ediff >-100)
 {
  pstep +=1; //count each step 
  Ediff -= 400;  //move one step
  if(pstep == 7)
  Ediff = -1;  //we have exceeded our range 100 kph max
 }//end of count number of steps positive
 wind += pstep * windstep; //this is final wind speed
 }//end of we are on the positive side
 else    //we have negative wind
{
int nstep = 0;
while(Ediff < -100)
{
 nstep += 1; //count each step
 Ediff += 400;  //move one step 
 if(nstep == 8)
 Ediff = 1;  //we have exceeded our range 100 kph
}//end of count number of steps negative
 wind -= nstep * windstep; //this is final wind speed
}//end of we are on negative side
 if (function)
 wind = - wind;//the function has been inverted
 float W_Ewind = wind;
// Serial.print(" w= ");
// Serial.print(W_Ewind);
 
  //N_Swind is +ve if wind blowing from the North. North is 0 deg on the compass
  //W_Ewind is +ve if wind is blowing from the West. West is 270 deg on the compass.
  int Awind = sqrt(N_Swind*N_Swind + W_Ewind*W_Ewind);
 // Serial.print(".....Aw= ");
  Serial.print("s");
  Awind *=10; //scale up for station
  Serial.print(Awind);
  Serial.print("e");
  int Dwind =57.3*atan2(N_Swind,- W_Ewind) + 270.0; //270 moves result into compass
  Dwind %= 360; //we are only use 360 degree circle
 // Serial.print(" Dw= ");
  Serial.print("d");
  Dwind = 512 + (Dwind *64)/45; //scale up for station
  Serial.print(Dwind);
  Serial.print("e");
 
}//end of loop

ISR(INT0_vect)
{
 zcdtof = TCNT1; //save the counted tof value
 EIMSK &= ~0x01; //disable INT0 we have counted this tof
}// INT0 service routine for zero crossing detector

ISR(INT1_vect)
{
 envelopetof = TCNT1; //save the counted tof value
 EIMSK = 0x00; //disable INT1 we have counted this tof
}// INT1 service routine for envelope detector

ISR(TIMER2_COMPB_vect)
{
  if( ! pulsesent)
  {
  OCR2B = 30;//wait 0.48 ms to start the tof counter
  TCCR0B = 0x00; //stop the pulse driver
  pulsesent = true; //the pulse has been sent
  TCNT2 = 0x00;
  }//end of sentpulse wait another .48 ms till start counter
  else
  {
  OCR2A = 80; //80 gives a total interupt in 2.4 ms 
  TIMSK2 = 0x02;  //enable interupts when count to OCR2A
  TCNT2 = 0x00; //after 2.4 ms drop out of delay loop after pulse received
  EIFR = 0x03;  //clear INT0 and INT1 flag they may have been previously set
  EIMSK = 0x03;  //enable interupts on INT0 and INT1
  TCNT1 = 0x00;  //start the tof count
  }//end of delay after sent pulse
}//end of interupt when OCR2B count reached -pulse sent

ISR(TIMER2_COMPA_vect)
{
 TCCR2B = 0x00; //stop the system timer
 TIMSK2 = 0x00;  //disable all interupts on system timer
 pulsedone = true; //we have finished sequence
}//end of interupt when OCR2A count reached -pulse received

inline void NORTH(void)
{
 NORTHLOW_IN; 
 NORTHLOW_IN_TRI; //north low is disconnected
 SOUTHDRIVER_OFF;  //disable south drive
 SOUTHLOW_OUT;
 SOUTHLOW_OUT_EARTH;
 MUXPIN9_0;
 MUXPIN10_0;  //9=0,10=0 receive from north
 NORTHDRIVER_ON;
}//end of select north

inline void EAST(void)
{
 EASTLOW_IN;
 EASTLOW_IN_TRI;  //east low is disconnected
 WESTDRIVER_OFF;
 WESTLOW_OUT; 
 WESTLOW_OUT_EARTH; //earth low side of west
 MUXPIN9_1;
 MUXPIN10_0;  //9=1,10=0 receive from west
 EASTDRIVER_ON;
}//end of select east

inline void SOUTH(void)
{
 SOUTHLOW_IN;
 SOUTHLOW_IN_TRI;  //disconnect south low
 NORTHDRIVER_OFF;
 NORTHLOW_OUT;
 NORTHLOW_OUT_EARTH;
 MUXPIN9_0;
 MUXPIN10_1; //9=0,10=1 receive from the south
 SOUTHDRIVER_ON;
}//end of select south

inline void WEST(void)
{
 WESTLOW_IN;
 WESTLOW_IN_TRI;  //disconnect low side of west
 EASTDRIVER_OFF;
 EASTLOW_OUT;
 EASTLOW_OUT_EARTH;  //earth the east low side
 MUXPIN9_1;
 MUXPIN10_1;  //9=1,10=1 receive from east
 WESTDRIVER_ON;
}//end of select west

inline void sequence(void)
{
 OCR0A = pulsefrequency; //set this constant to give best response 
 TCCR0A = 0x42;  //toggle OC0A CTC mode count to OCR0A
 TCCR0B = 0x01;  //use prescale = /1 start counting pulse generator
 TCNT0 = 0x00;  //set start value of counter
 
 TIFR2 = 0x07; // clear all flags
 TIMSK2 = 0x04;  //enable interupts when count to OCR2B
 OCR2B = 40;  //40 gives interupt in 0.64 ms 
 TCCR2A = 0x00;  //normal port operation counts to max 255
 TCCR2B = 0x06;  //use prescale  /256 start count
 TCNT2 = 0x00; //after .64 ms stop pulse generator and start tof count
 
 pulsedone = false; //wait for system timer
 pulsesent = false; //wait for system timer
 while( ! pulsedone)   //TIMER2 will set after 2.4 ms
 asm volatile("nop"); 
}//end of timing sequence

