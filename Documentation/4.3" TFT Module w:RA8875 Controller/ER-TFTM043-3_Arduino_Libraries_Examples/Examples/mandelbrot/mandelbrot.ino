/*
  Another example adapted from ugfx http://ugfx.org
   ---------------> http://www.buydisplay.com
  This actually shows a problem of RA8875 library, writing
  one pixel it's fast but write many single pixels in the same time
  not and speed suffer.
  It shoul be a workaround that I'm actually study, this will fix 
  also loading big images from SD memory card and so on.
   Tested and worked with:
 Teensy3,Teensy3.1,Arduino UNO,Arduino YUN,Arduino Leonardo,Stellaris
 Works with Arduino 1.0.6 IDE, Arduino 1.5.8 IDE, Energia 0013 IDE
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

RA8875 tft = RA8875(RA8875_CS,RA8875_RESET);

//#endif

const int MAX = 256;
float cx, cy;
float zoom = 1.0f;


void mandelbrot(float x1, float y1, float x2, float y2) {
  unsigned int i,j;
  uint16_t iter;
  uint16_t color;

  float sy = y2 - y1;
  float sx = x2 - x1;

  for(i = 0; i < tft.width(); i++) {
    for(j = 0; j < tft.height(); j++) {
      float cy = j * sy / tft.height() + y1;
      float cx = i * sx / tft.width() + x1;
      float x = 0.0f, y = 0.0f, xx = 0.0f, yy = 0.0f;
      for(iter=0; iter <= MAX && (xx + yy) < 4.0f; iter++) {
        xx = x * x;
        yy = y * y;
        y = 2.0f * x * y + cy;
        x = xx - yy + cx;
      }
      color = ((iter<<7 & 0xF8) << 8) | ((iter<<4 & 0xFC) << 3) | (iter >> 3);
      tft.drawPixel(i, j, color);
    }
  }
}

void setup() 
{
  Serial.begin(9600);
  //while (!Serial) {;}
  Serial.println("RA8875 start");

  /* Initialise the display using 'RA8875_480x272' or 'RA8875_800x480' */
 tft.begin(RA8875_480x272);

  cx = -0.086f;
  cy = 0.85f;
}


void loop() 
{ tft.changeMode(TEXT);
   tft.setTextColor(RA8875_WHITE);
  tft.setCursor (0, 0); 
  tft.print ("www.buydisplay.com");
    tft.changeMode(GRAPHIC);
  mandelbrot(-2.0f*zoom+cx, -1.5f*zoom+cy, 2.0f*zoom+cx, 1.5f*zoom+cy);
  zoom *= 0.7f;
  if(zoom <= 0.00001f)
    zoom = 1.0f;	
}
