// simple to show all integers
#include <SEVEN.h>
int d = 10100;  //out of range

void setup()   {    }

void loop()                     
{  
 SEVEN.displayI(d);
 d= d - 1;
 delay(200);
 if (d  < - 1100)
 d = 10100;
}
