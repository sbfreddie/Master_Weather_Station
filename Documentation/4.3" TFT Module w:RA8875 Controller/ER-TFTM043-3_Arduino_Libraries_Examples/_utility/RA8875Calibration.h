#ifndef _RA8875CAL_H_
#define _RA8875CAL_H_

/* TOUCH SCREEN AXIS INVERSION +++++++++++++++++++++++++++++++++++++++
Some chinese product have both axis inverted (EastRising),
you can uncomment INVERTETOUCH_Y or INVERTETOUCH_X or both
if your display fall in that case! */
#define INVERTETOUCH_Y
#define INVERTETOUCH_X
/* TOUCH SCREEN IN PIXELS ++++++++++++++++++++++++++++++++++++++++++++
Normally Touch Screen return ADC values (0...1024) but decomment
the following it will return values in pixels */
//#define TOUCHINPIXELS

/* Calibration Data --------------------------------*/
/* ------------- TOUCH SCREEN CALIBRATION (how to) -----------
Touch Screen are not all the same and needs a calibration, you will see
yourself by load touchPaint.ino example, try to paint all over the screen!
If you have space on one or more side you prolly need to calibrate values  
    ---- perform calibration ----
To perform the touch screen calibration, load libTouchSCalibration.ino and open serial terminal:
(you have to put data inside RA8875/_utility/RA8875Calibration.h)
1) the lowest value of x by touch the top/left angle of your tft, put value in TOUCSRCAL_XLOW
2) you can get the lowest value of y in the same time, put value in TOUCSRCAL_YLOW
3) the highest value of x by touching the lower/bottom corner of your tft, put the value in TOUCSRCAL_XHIGH
4) in the same manner you get the max value of y, put that value in TOUCSRCAL_XHIGH
*/
#define TOUCSRCAL_XLOW	0//115
#define TOUCSRCAL_YLOW	0//39
#define TOUCSRCAL_XHIGH	0//926
#define TOUCSRCAL_YHIGH	0//950


#endif