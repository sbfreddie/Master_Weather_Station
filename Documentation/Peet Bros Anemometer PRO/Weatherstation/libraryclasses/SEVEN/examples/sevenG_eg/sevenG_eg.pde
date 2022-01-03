// simple to show uses

#include <SEVEN.h>

void setup()   {    }

void loop()      
{  
 SEVEN.displayI(789);  //displays "789_"
 delay(1000);
 SEVEN.displayF(-1.234);  //displays "-1.2_"
 delay(1000);
 SEVEN.displayP(0x1e5f4138);  //displays "FAIL"
 delay(1000);
 SEVEN.displayA(SEVEN.displayF(25.7),0x3a);  //displays "25.7C"
 delay(1000);
}
