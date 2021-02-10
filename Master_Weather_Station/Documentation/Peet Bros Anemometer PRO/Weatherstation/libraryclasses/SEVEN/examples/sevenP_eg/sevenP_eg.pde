// simple program to demonstate picture display

#include <SEVEN.h>
#include <stdlib.h>  //access to rand function
long d = 0xffffffff; 

void setup()   {  }

void loop()                     
{ 
  int r =rand();
  d = d | r;
  d = d<<16 | rand();
  SEVEN.displayP(d);
  delay(200);
}
