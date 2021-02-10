//simple to count through all possible values

#include <SEVEN.h>
float d = 1010.0; //first number out of range

void setup()   {  }              

void loop()                     
{  
  SEVEN.displayF(d);
  d= d - 0.1;
  delay(200);
 if (d  < - 110)  //last number out of range
 d = 1010;
}
