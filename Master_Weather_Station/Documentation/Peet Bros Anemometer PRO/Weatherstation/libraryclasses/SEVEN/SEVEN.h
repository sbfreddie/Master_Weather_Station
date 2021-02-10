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
#ifndef SEVEN_h
#define SEVEN_h

#define setupD8 DDRB |=0x01;   // data pin 8 is set to output 
#define setupD7D6 DDRD |= 0xC0;   // data pin 7 and 6 is set to output
#define latchpinLOW PORTB &= ~0x01;  //set latch pin 8 to low
#define latchpinHIGH PORTB |= 0x01;  //set latch pin 8 to high
#define clockpinLOW PORTD &= ~0x80;  //set clock pin 7 to low
#define clockpinHIGH PORTD |= 0x80;  //set clock pin 7 to high
#define datapinLOW PORTD &= ~0x40;  //set data pin 6 to low
#define datapinHIGH PORTD |= 0x40;  //set data pin 6 to high
const unsigned char N[] = {0x7b,0x41,0x37,0x67,0x4d,0x6e,0x7e,0x43,0x7f,0x4f};
//the 7 segment display codes for each digit "0" ......."9"
const unsigned long error = 0x3e3e3e3e;  //the error display message EEEE
const unsigned long minussign = 0x04000000;   //the display minus sign
const unsigned long decpoint = 0x80000000;  //decimal point far left place

#include "WProgram.h"

class SEVENclass
{
public:
unsigned long displayI(int number);  //display number from -999 to 9999
unsigned long displayF(float number);  //display number from - 99.9 to 999.9
void displayP(long picture);  //display the picture passed
void displayA(long origional,unsigned char addition); //add last character onto the display 

private:
unsigned long assignchar(int number); // get the pattern for number

};//end of class SEVENclass

 extern SEVENclass SEVEN;

#endif
