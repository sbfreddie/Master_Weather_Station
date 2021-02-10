/*
This is a general purpose 4 digit 7 segment LED display.

It uses two 16 bit serial input constant-current LED
drivers with four 7 segment displays.

It uses 3 digital pins from the arduino.

The code is all in a class which has 4 functions:

SEVEN.displayI(int number)
This displays numbers in the range -999 to 9999.
The number is left justified. If it is not in
the range "EEEE" will be displayed.

SEVEN.displayF(float number)
This displays numbers in the range -99.9 to 999.9.
One decimal point is displayed.
The number is left justified. If it is not in
the range "EEEE" will be displayed.

SEVEN.displayP(long picture)
Picture is 32 bits which map to each segment.

SEVEN.displayA(long origional,unsigned char addition)
This is designed to be used with displayI or displayF.
It adds the right hand character to the display.

The connections and the 7 segment display I use give
the following bit connection diagram.   

                D1
           ____________
           
          |            |
          |            |
       D3 |            | D0
          |            |
          |            |
                D2
           ____________

           
          |            |
          |            |
       D4 |            | D6
          |            |
          |            |
           ____________
                           __
                D5        |dp| D7
                           --  

 
To display a "3" the bits are:

D7 D6 D5 D4 / D3 D2 D1 D0
0  1  1  0  / 0  1  1  1  = 0x67      
*/

#include "WProgram.h"
#include "SEVEN.h"

unsigned long SEVENclass::displayI(int number)
{
unsigned long sevensegment = assignchar(number);  //run routine
displayP(sevensegment);  //output the display to the 7 segment displays
return sevensegment;
}//end of integer display

unsigned long SEVENclass::displayF(float num)
{   //number must be between -99.9 and 999.9
    unsigned long decimalpoint = decpoint;  //decimal point far left place
    unsigned long leadingzero = N[0];  //special case leading zero "0.7"
	int numInt = num;  //get the whole number
    num = (num - numInt) * 10;  //get decimal as a integer
    numInt = (numInt * 10) + num ;   //just one significant figure
	unsigned long sevensegment = assignchar(numInt);  //run routine
	if (numInt < 0)  //if negative
	{
	decimalpoint = decimalpoint>>8;  //move past minus sign
    numInt = numInt * -1;  //get positive value
	if (numInt < 10)
	{
    sevensegment = sevensegment & 0x00ff0000;  // leave single digit in place
	sevensegment = sevensegment>>8;  //make room for the 0
	sevensegment = sevensegment | minussign | leadingzero<<16; //minus and 0
	}//end of number negative and less than 1
	}// end of number negative
	else
	 if (numInt < 10)  //special case wont to display as 0.9 
	  {
       sevensegment = sevensegment>>8;  //make room for "0"
       sevensegment = sevensegment | leadingzero<<24;  //get zero into left hand side	  
	  }//end of number positive and < 1	
    numInt /=100;  //for the decimal point its *100 to big
    if (numInt >= 1)   //point must move
    decimalpoint = decimalpoint>>8;
    if (numInt >= 10)   //point must move
    decimalpoint = decimalpoint>>8;
	sevensegment = sevensegment | decimalpoint;  //include decimal point
	displayP(sevensegment);  //output the display to the 7 segment displays
    return sevensegment;
}//end of display floating point

void SEVENclass::displayA(long origional,unsigned char addition)
{
origional = origional & 0xffffff00;  //clear the last character
origional = origional | addition;  //add the character
displayP(origional);  //output the display to the 7 segment dis
}//end of display addition

unsigned long SEVENclass::assignchar(int num) // get the pattern for number
{   //number must be between -999 and 9999
	unsigned long D[5];  //working buffers for the display
	bool minus = false;  //if minus leave room for minus sign
	if ((num > 9999) | (num < -999))  
	return error;  //outside our range so display error message
	if (num < 0)
	{
	minus = true;  //offset display for minus sign
	num = -num;  //we work with positive numbers
	}//end of number is negative
	D[3] = N[num/1000];  //get ms digit into the lefthand side
	int numT = num % 1000;
	D[2] = N[numT/100];  //get next digit into next left
	numT = numT % 100;
	D[1] = N[numT/10];  //get next to next
	numT = numT % 10;
	D[0] = N[numT];   //get ls digit into right most place
	D[4] = D[0]|D[1]<<8|D[2]<<16|D[3]<<24;  //combine in to one 4 x 8 bits
	int i ;  //general counter
	for (i = 3; i>0; i--)  //if last is zero leave alone
	{
	if (D[i] == N[0])  //the "0" display pattern		
	D[4] = D[4]<<8;
	else
	break; // left justify the number
	}//end of left justify numbers
	if (minus)
	{
	D[4] = D[4]>>8;   //leave room for minus sign
	D[4] |= minussign;  //include minus sign
	}//end of include minus sign
	return D[4];
}//end of assign character

void SEVENclass::displayP (long display)  //output the pattern to the 4 digits
{
  setupD8;    //pin 8 to output
  setupD7D6;  //pin 7 and 6 to output
  unsigned long mask = 0x01;  //mask for display digit bit
  latchpinLOW;   //ready to latch data
  while (mask != 0)  //send each bit to register 32 bits total
  {  
  clockpinLOW;  //ready to clock data
  datapinHIGH;  //lets assume the data is high
  if ((display & mask) == 0)
  datapinLOW;   //wrong the data was low
  mask = mask<<1;  //look at next bit in digit display
  clockpinHIGH;  //clock the data line into the reg
  }//end of send 32 bits
  latchpinHIGH;   //latch the 8 bits to output register
}//end of send pattern to 4 digits

SEVENclass SEVEN;
