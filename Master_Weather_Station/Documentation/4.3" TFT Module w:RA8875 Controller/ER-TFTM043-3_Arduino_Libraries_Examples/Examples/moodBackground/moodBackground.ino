/*
  Modulate background color
 Tested and worked with:
 Teensy3,Teensy3.1,Arduino UNO,Arduino YUN,Arduino Leonardo,Stellaris
 Works with Arduino 1.0.6 IDE, Arduino 1.5.8 IDE, Energia 0013 IDE
  ---------------> http://www.buydisplay.com
 */
#include <SPI.h>
#include <RA8875.h>

//Arduino DUE,Arduino mega2560
#define RA8875_INT 23 
#define RA8875_CS 22 

#define RA8875_RESET 24

//#if defined(NEEDS_SET_MODULE)//Energia, this case is for stellaris/tiva

//RA8875 tft = RA8875(3);//select SPI module 3
/*
for module 3 (stellaris)
SCLK:  PD_0
MOSI:  PD_3
MISO:  PD_2
SS:    PD_1
*/
//#else

RA8875 tft = RA8875(RA8875_CS,RA8875_RESET);//Teensy3/arduino's

//#endif


float angle;

void setup()
{
 tft.begin(RA8875_480x272);

}

// Translate a hue "angle" -120 to 120 degrees (ie -2PI/3 to 2PI/3) to
// a 6-bit R channel value
//
// This is very slow on a microcontroller, not a great example!
inline int angle_to_channel(float a) {
  if (a < -PI) a += 2*PI;
  if (a < -2*PI/3  || a > 2*PI/3) return 0;
  float f_channel = cos(a*3/4); // remap 120-degree 0-1.0 to 90 ??
  return ceil(f_channel * 255);//63
}

void loop() {
   tft.changeMode(TEXT);
   tft.setTextColor(RA8875_WHITE);
  tft.setCursor (0, 0); 
  tft.print ("www.buydisplay.com");
    tft.changeMode(GRAPHIC);
  uint16_t clr = (((angle_to_channel(angle-4*PI/3)>>1) & 0xF8) << 8) | (((angle_to_channel(angle-2*PI/3)) & 0xFC) << 3) | ((angle_to_channel(angle)>>1) >> 3);
  tft.fillScreen(clr);

  angle += 0.01;
  if(angle > PI)
    angle -= 2*PI;
}
