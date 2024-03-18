//
// File			Buttons_.h
// Library header
//
// Details		<#details#>
//	
// Project	 	Mister Controller
// Developed with [embedXcode](http://embedXcode.weebly.com)
// 
// Author		freddie
// 				Kingston Co.
//
// Date			8/11/18 4:12 PM
// Version		<#version#>
// 
// Copyright	© freddie, 2018
// Licence     <#license#>
//
// See			ReadMe.txt for references
//


// Core library for code-sense - IDE-based

#include "Arduino.h"

#ifndef Buttons
#define Buttons


#include <SPI.h>        // For all SPI transactions on TFT, SD Card, and Touch Screen.
#include "RA8875.h"
#include <Stream.h>     // Needed for serial streaming stuff.
/*    Base class, abstract     */

#define RA8875_CS       15 //  This pin is for my Weather Station implementation see below...
#define RA8875_RESET    16  //  This pin is for my Weather Station implementation.  255 to disable it!
#define RA8875_INT      23  // This is the interrupt pin for the RA8875 resistive touch controller.
#define FT5206_SDA      30
#define FT5206_SCL      29
#define FT5206_WAKE     25
#define FT5206_RST      27
#define FT5206_INT      28  // This is the FT5206 interrupt pin number

extern RA8875 tft;

//#define DEBUG1    true              // Turn on/off Button functions debug messages.
//#define DEBUG2    true              // Turn on/off Button functions debugging.

// Touch screen struct.
typedef struct Point
{
    
    int16_t xPosition;
    int16_t yPosition;
    
} TouchScreenPoints_t;

typedef struct Points
{
    
    int16_t width;
    int16_t height;
    
} WidthsAndHeights_t;

typedef struct EllPoints
{
    
    int16_t longAxisLength;
    int16_t shortAxisLength;
    
} LongAndShortAxis_t;


enum Figure_types
{
    TRIANGLE,
    RECTANGLE,
    CIRCLE,
    ELLIPSE,
    RNDRECTANGLE
};

enum On_Figure
{
    ON_TRIANGLE,
    ON_RECT,
    ON_CIRCLE,
    ON_ELLIPSE,
    ON_RNDRECT,
    ERROR
};

enum Colors
{
    BLACK           = RA8875_BLACK,
    WHITE           = RA8875_WHITE,
    RED             = RA8875_RED,
    GREEN           = RA8875_GREEN,
    BLUE            = RA8875_BLUE,
    CYAN            = RA8875_CYAN,
    MAGENTA         = RA8875_MAGENTA,
    YELLOW          = RA8875_YELLOW,
    LIGHT_GREY      = RA8875_LIGHT_GREY,
    LIGHT_ORANGE    = RA8875_LIGHT_ORANGE,
    DARK_ORANGE     = RA8875_DARK_ORANGE,
    PINK            = RA8875_PINK,
    PURPLE          = RA8875_PURPLE,
    GRAYSCALE       = RA8875_GRAYSCALE
};

Colors getRandomColor();

/*******************************************************************************************************************
* These definitions are for the RA8875 TFT display.
*******************************************************************************************************************/

/*******************************************************************************************************************
* These variables are for the horizontal pixel size (Xsize), Vertical pixel size (Ysize), LineNumber (0-271), (0-479)
* or (0-1023), and current position of the cursor (Xposition) (0-799) or (0-479).
* The RA8875_FONTSCALE varible is for the multiplier of the standard (8 x 16) font size provided with the TFT lib.
* A multiplier of 0 means the font size will be 8 x 16, a multiplier of 1 means the font size will be 16 x 32 ...
*******************************************************************************************************************/

/// Uncomment one of these display definition:
//#define TFTM043
#define TFTM07
//#define TFTM08
//#define TFTM09
//#define TFTM101

#if defined(TFTM043)
    #define X_CONST 480  // These setting are for the 4.2" display (ER-TFTM043-3) 480 x 272 pixels.
    #define Y_CONST 272  // These setting are for the 4.2" display (ER-TFTM043-3) 480 x 272 pixels.
    #define RA8875_FONTSCALE  1  // Font size is 1 or 16 wide x 32 high pixels.

    #if (RA8875_FONTSCALE == 0)
        #define Xsize 8
        #define Ysize 16

    #elif (RA8875_FONTSCALE == 1)
        #define Xsize 16
        #define Ysize 32

    #elif (RA8875_FONTSCALE == 2)
        #define Xsize 32
        #define Ysize 64

    #elif (RA8875_FONTSCALE == 3)
        #define Xsize 64
        #define Ysize 128
    #endif  // #if RA8875_FONTSCALE == 0

    #define YnumberOfRows (Y_CONST / Ysize)
    #define XnumberOfColumns (X_CONST / Xsize)
#endif


/*******************************************************************************************************************
* These definitions are for the TFTM07, TFTM08, or TFTM09 in (X = 800, Y = 480) mode which has 50 Columns,
* and 15 rows of text when the display is in the Landscape mode.
*******************************************************************************************************************/

#ifdef TFTM07
    #define X_CONST 800  // These setting are for the 7" display (ER-TFTM07-5) 800 x 480 pixels.
    #define Y_CONST 480  // These setting are for the 7" display (ER-TFTM07-5) 800 x 480 pixels.

    #define RA8875_FONTSCALE 1  // Font size is 1 or 16 wide x 32 high pixels.

    #if (RA8875_FONTSCALE == 0)
        #define Xsize 8
        #define Ysize 16

    #elif (RA8875_FONTSCALE == 1)
        #define Xsize 16
        #define Ysize 32

    #elif (RA8875_FONTSCALE == 2)
        #define Xsize 32
        #define Ysize 64

    #elif (RA8875_FONTSCALE == 3)
        #define Xsize 64
        #define Ysize 128
    #endif  // #if RA8875_FONTSCALE == 0

    #define YnumberOfRows (Y_CONST / Ysize)
    #define XnumberOfColumns (X_CONST / Xsize)
#endif


#if defined(TFTM08)
    #define X_CONST 800  // These setting are for the 7" display (ER-TFTM08-5) 800 x 480 pixels.
    #define Y_CONST 480  // These setting are for the 7" display (ER-TFTM08-5) 800 x 480 pixels.
    #define RA8875_FONTSCALE    1  // Font size is 1 or 16 wide x 32 high pixels.

    #if (RA8875_FONTSCALE == 0)
        #define Xsize 8
        #define Ysize 16

    #elif (RA8875_FONTSCALE == 1)
        #define Xsize 16
        #define Ysize 32

    #elif (RA8875_FONTSCALE == 2)
        #define Xsize 32
        #define Ysize 64

    #elif (RA8875_FONTSCALE == 3)
        #define Xsize 64
        #define Ysize 128
    #endif  // #if RA8875_FONTSCALE == 0

    #define YnumberOfRows (Y_CONST / Ysize)
    #define XnumberOfColumns (X_CONST / Xsize)
#endif


#if defined(TFTM09)
    #define X_CONST 800  // These setting are for the 7" display (ER-TFTM09-5) 800 x 480 pixels.
    #define Y_CONST 480  // These setting are for the 7" display (ER-TFTM09-5) 800 x 480 pixels.
    #define RA8875_FONTSCALE    1  // Font size is 1 or 16 wide x 32 high pixels.

    #if (RA8875_FONTSCALE == 0)
        #define Xsize 8
        #define Ysize 16

    #elif (RA8875_FONTSCALE == 1)
        #define Xsize 16
        #define Ysize 32

    #elif (RA8875_FONTSCALE == 2)
        #define Xsize 32
        #define Ysize 64

    #elif (RA8875_FONTSCALE == 3)
        #define Xsize 64
        #define Ysize 128
        #endif  // #if RA8875_FONTSCALE == 0

    #define YnumberOfRows (Y_CONST / Ysize)
    #define XnumberOfColumns (X_CONST / Xsize)
#endif


#if defined(TFTM101)
    #define X_CONST 1024  // These setting are for the 10.1" display (ER-TFTM101-5) 1024 x 600 pixels.
    #define Y_CONST 600  // These setting are for the 10.1" display (ER-TFTM101-5) 1024 x 600 pixels.
    #define RA8875_FONTSCALE    1  // Font size is 1 or 16 wide x 32 high pixels.

    #if (RA8875_FONTSCALE == 0)
        #define Xsize 8
        #define Ysize 16

    #elif (RA8875_FONTSCALE == 1)
        #define Xsize 16
        #define Ysize 32

    #elif (RA8875_FONTSCALE == 2)
        #define Xsize 32
        #define Ysize 64

    #elif (RA8875_FONTSCALE == 3)
        #define Xsize 64
        #define Ysize 128
    #endif  // #if RA8875_FONTSCALE == 0

    #define YnumberOfRows (Y_CONST / Ysize)
    #define XnumberOfColumns (X_CONST / Xsize)
#endif



/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
* This Class is the base class for all the Button objects.
*
* Needs:
*   Nothing.
* Returns:
*   Nothing.
*******************************************************************************************************************/
class Shape
{
public:
    // constructor
    Shape()
    {
        _beginningXYPositions.xPosition = 0;
        _beginningXYPositions.yPosition = 0;
        _fillcolor = WHITE;
        _bordercolor = BLACK;
        _textcolor = WHITE;
        _radius = 8;
        _text = ((char *)"");
        _fontSize = (RA8875tsize)0;  // Default font size is 0.
        _numberOfTextLines = 0;  // This is the number of lines (separated by spaces) in the character string.
        _numberOfCharacters = 0;  // Default is empty character array.
    };
    
    // Destructor
    virtual ~Shape(){};
    
    //virtual functions
    virtual void draw() = 0;  // The subclass must define this function.
    virtual void setRandomProperties(uint32_t min = 150, uint32_t max = 800)  // The Subclass may override this function.
        {
            _beginningXYPositions.xPosition = (uint16_t)random (min, max);
            _beginningXYPositions.yPosition = (uint16_t)random (0, 450);
            _fillcolor = getRandomColor();
        }
    
    ///  Getting and Setting variables
    
    /*******************************************************************************************************************
    * This function sets the Beginning X & Y position for the shape.
    *******************************************************************************************************************/
    void setPosition(int16_t setBeginningXPosition, int16_t setBeginningYPosition)
        {
            _beginningXYPositions.xPosition = setBeginningXPosition;
            _beginningXYPositions.yPosition = setBeginningYPosition;
        }
    
    /*******************************************************************************************************************
    * This function gets the current X & Y position for the shape.
    *******************************************************************************************************************/
    TouchScreenPoints_t getPosition(void)
        {
            return _beginningXYPositions;
        }
    
    /*******************************************************************************************************************
    * This function sets the Beginning X & Y width & Height position for the shape.
    *******************************************************************************************************************/
    void setWidthHeight(int16_t setBeginningWidth, int16_t setBeginningHeight)
        {
            _width_height.width = setBeginningWidth;
            _width_height.height = setBeginningHeight;
        }
    
    /*******************************************************************************************************************
    * This function gets the current X & Y width & Height position for the triangle.
    *******************************************************************************************************************/
    WidthsAndHeights_t getWidthHeight(void)
        {
            return _width_height;
        }
    
    /*******************************************************************************************************************
     * This function sets the Vertex1 X & Y position for the triangle.
     *******************************************************************************************************************/
    void setVertex1Position(int16_t setVertex1XPosition, int16_t setVertex1YPosition)
        {
            _XYPointsvertex1.xPosition =  setVertex1XPosition;
            _XYPointsvertex1.yPosition =  setVertex1YPosition;
        }
    
    /*******************************************************************************************************************
    * This function gets the Vertex1 X & Y position for the triangle.
    *******************************************************************************************************************/
    TouchScreenPoints_t getVertex1Position(void)
        {
            return _XYPointsvertex1;
        }
    
    /*******************************************************************************************************************
    * This function sets the Vertex2 X & Y position for the triangle.
    *******************************************************************************************************************/
    void setVertex2Position(int16_t setVertex2XPosition, int16_t setVertex2YPosition)
    {
        _XYPointsvertex2.xPosition =  setVertex2XPosition;
        _XYPointsvertex2.yPosition =  setVertex2YPosition;
    }
    
    /*******************************************************************************************************************
    * This function gets the Vertex2 X & Y position for the triangle.
    *******************************************************************************************************************/
    TouchScreenPoints_t getVertex2Position(void)
        {
            return _XYPointsvertex2;
        }
    
    /*******************************************************************************************************************
    * This function sets the Vertex3 X & Y position for the triangle.
    *******************************************************************************************************************/
    void setVertex3Position(int16_t setVertex3XPosition, int16_t setVertex3YPosition)
    {
        _XYPointsvertex3.xPosition =  setVertex3XPosition;
        _XYPointsvertex3.yPosition =  setVertex3YPosition;
    }
    
    /*******************************************************************************************************************
    * This function gets the Vertex3 X & Y properties for the triangle.
    *******************************************************************************************************************/
    TouchScreenPoints_t getVertex3Position(void)
        {
            return _XYPointsvertex3;
        }
    
    /*******************************************************************************************************************
    * This function sets the LongAxis length and the ShortAxis length for the ellipse.
    *******************************************************************************************************************/
    void setEllLongShortLengths(int16_t setLongAxisLength, int16_t setShortAxisLength)
        {
            _LongLength_ShortLength.longAxisLength =  setLongAxisLength;
            _LongLength_ShortLength.shortAxisLength =  setShortAxisLength;
        }
    
    /*******************************************************************************************************************
     * This function gets the the LongAxis length and the ShortAxis length for the ellipse.
     *******************************************************************************************************************/
    LongAndShortAxis_t getEllLongShortLengths(void)
        {
            return _LongLength_ShortLength;
        }
    
    void setFillColor(Colors fillColor)
        {
            _fillcolor = fillColor;
        }

    Colors getFillColor(void)
        {
            return _fillcolor;
        }
    
    void setBorderColor(Colors borderColor)
        {
            _bordercolor = borderColor;
        }

    Colors getBorderColor()
        {
            return _bordercolor;
        }
    
    void setRadius(int16_t inputRadius)
        {
            _radius = inputRadius;
        }
    
    int16_t getRadius(void)
        {
            return _radius;
        }

    void setText(char *text)
        {
            _text = text;
            return;
        }
    
    char* getText(void)
        {
            return _text;
        }
    
    void setFontSize(RA8875tsize fontSize)
        {
            _fontSize = fontSize;
            return;
        }
    
    RA8875tsize getFontSize(void)
        {
            return _fontSize;
        }
    
    void setTextColor(Colors textColor)
        {
            _textcolor = textColor;
        }
    
    Colors getTextColor(void)
        {
            return _textcolor;
        }
    
    void setNumberOfTextLines(uint8_t numberOfTextLines)
        {
            _numberOfTextLines = numberOfTextLines;
        }
    
    uint8_t getNumberOfTextLines(void)
        {
            return _numberOfTextLines;
        }
    
protected:
    TouchScreenPoints_t _beginningXYPositions;  // This is a beginning X Position & Y Position Struct.
    WidthsAndHeights_t _width_height;  // This is a Width & Height Struct.
    LongAndShortAxis_t _LongLength_ShortLength;  // This is a Long Axis & Short Axis Struct.
    
    TouchScreenPoints_t _XYPointsvertex1;  // This is a X & Y position Struct.
    TouchScreenPoints_t _XYPointsvertex2;  // This is a X & Y position Struct.
    TouchScreenPoints_t _XYPointsvertex3;  // This is a X & Y position Struct.
    
    Colors _fillcolor;
    Colors _bordercolor;
    int16_t _radius;
    
    char * _text;  // This is a pointer to a character array.
    Colors _textcolor = BLACK;
    RA8875tsize _fontSize = (RA8875tsize)0;  // Default font size is 0.
    uint8_t _fontWidth;
    uint8_t _fontHeight;
    uint8_t _numberOfTextLines = 0;  // This is the number of lines (separated by spaces) in the character string.
    uint8_t _numberOfCharacters = 0;  // Default is empty character array.
    int16_t _XCenterPosition;
    int16_t _YCenterPosition;
    
};


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
* This Class draws a colored printing circle on the TFT Screen
*******************************************************************************************************************/
class Circle: public Shape
{
public:
    
    /*******************************************************************************************************************
    * Default Constructor
    *******************************************************************************************************************/
    Circle();
    
    /*******************************************************************************************************************
    *                                           OverLoad Constructor
    * This Class draws a colored printing circle on the TFT Screen
    *
    * Needs:
    *   text:                   A pointer to some text.
    *   fontSize:               The RA8875 font size.
    *   textColor:              The RA8875 text Color.
    *   numberOfTextLines:      The number of lines of text to put into the Circle, if this is zero no text will be drawn.
    *   setBeginningXPosition:  A beginning X Position.
    *   setBeginningYPosition:  A beginning Y Position.
    *   radius:                 A radius for the Circle.
    *   fillColor:              A Fill Color for the Circle.
    *   borderColor:            A Border Color for the Circle.
    * Returns:
    *   A Circle Object.
    *******************************************************************************************************************/
    Circle(char *text, RA8875tsize fontSize, Colors textColor, uint8_t numberOfTextLines, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t radius, Colors fillColor, Colors borderColor);
    
    /*******************************************************************************************************************
    * Destructor
    *******************************************************************************************************************/
    ~Circle();
    
    /*******************************************************************************************************************
    * This function actually draws the Circle on the TFT.
    *******************************************************************************************************************/
    void draw();
    
    void setRandomProperties(int32_t min=150, int32_t max=800);
    
private:
    char * _text;  // This is a pointer to a character array.

};


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
* This Class draws a colored ellispe on the TFT Screen
*******************************************************************************************************************/
class Ellipse: public Shape
{
public:
    
    /*******************************************************************************************************************
    * Default Constructor
    *******************************************************************************************************************/
    Ellipse();
    
    /*******************************************************************************************************************
    *                                        OverLoad Constructor
    * Parameters Needed:
    *   text:                   A pointer to some text.
    *   fontSize:               The RA8875 font size.
    *   textColor:              The RA8875 text Color.
    *   numberOfTextLines:      The number of lines of text to put into the Circle.
    *   xCenter:                x location of the center of the ellipse
    *   yCenter:                y location of the center of the ellipse
    *   longAxis:               Size in pixels of the long axis
    *   shortAxis:              Size in pixels of the short axis
    *   fillColor:              RGB565 color
    *   borderColor:            RGB565 color.
    * Returns:
    *   A Ellipse Object.
    *******************************************************************************************************************/
    Ellipse(char *text, RA8875tsize fontSize, Colors textColor, uint8_t numberOfTextLines, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t longAxis, int16_t shortAxis, Colors fillColor, Colors borderColor);
    
    /*******************************************************************************************************************
    *                                                   Destructor
    *******************************************************************************************************************/
    ~Ellipse();
    
    /*******************************************************************************************************************
    *
    *******************************************************************************************************************/
    void setRandomProperties(int32_t min=150, int32_t max=800);
    
    /*******************************************************************************************************************
    * This function actually draws the Ellipse on the TFT.
    *******************************************************************************************************************/
    void draw();
    
    /* Getting and Setting variables*/
    void setlongAxis(int16_t longAxis)
        {
            _longAxis = longAxis;
        }
    
    int16_t getlongAxis(void)
        {
            return _longAxis;
        }
    
    void setshortAxis(int16_t shortAxis)
        {
            _shortAxis = shortAxis;
        }
    
    int16_t getshortAxis(void)
        {
            return _shortAxis;
        }
    
    void setxCenter(int16_t xCenter)
        {
            _xCenter = xCenter;
        }
    
    int16_t getxCenter(void)
        {
            return _xCenter;
        }
    
    void setyCenter(int16_t yCenter)
        {
            _yCenter = yCenter;
        }
    
    int16_t getyCenter(void)
        {
            return _yCenter;
        }
    
private:
    char * _text;  // This is a pointer to a character array.
    int16_t _longAxis;
    int16_t _shortAxis;
    int16_t _xCenter;
    int16_t _yCenter;
};


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
* This Class draws a colored printing Rectangle on the TFT Screen
*******************************************************************************************************************/
class Rectangle: public Shape
{
public:
    
    /*******************************************************************************************************************
    *                                           Default Constructor
    *******************************************************************************************************************/
    Rectangle();
    
    /*******************************************************************************************************************
    *                                               Over Ride Constructor
    * This constructor creates a rectangle where you specify, the size you specify, the fill color you specify, and the
    * Border you specify.
    * Then it puts the text you specify in the center of the box horizontally and vertically.
    *
    * Needs:
    *   text:                   A pointer to some text.
    *   fontSize:               The RA8875 font size.
    *   textColor:              The RA8875 text Color.
    *   numberOfTextLines:      The number of lines of text to put into the Circle, if zero no text will be drawn in the Rectangle.
    *   setBeginningXPosition:  A beginning X Position.
    *   setBeginningYPosition:  A beginning Y Position.
    *   Width:                  The Width (in pixels) of your Rectangle.
    *   Height:                 The Height (in pixels) of your Rectangle.
    *   fillColor:              A Fill Color for the Rectangle.
    *   borderColor:            A Border Color for the Rectangle.
    * Returns:
    *   A Rectangle Object.
    *******************************************************************************************************************/
    Rectangle(char *text, RA8875tsize fontSize, Colors textColor, uint8_t numberOfTextLines, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t Width, int16_t Height, Colors fillColor, Colors borderColor);
    
    /*******************************************************************************************************************
    *                                               Destructor
    *******************************************************************************************************************/
    ~Rectangle();
    
    /*******************************************************************************************************************
    * This function actually draws the Rectangle on the TFT.
    *******************************************************************************************************************/
    void draw();
    
    /*******************************************************************************************************************
    * 
    *******************************************************************************************************************/
    void setRandomProperties(int16_t min = 150, int16_t max = 800);
    
private:
    char * _text;  // This is a pointer to a character array.
    
};


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
* This Class creates a Round Rectangle.
*******************************************************************************************************************/
class RndRectangle: public Shape
{
public:
    
    /*******************************************************************************************************************
    *                                               Default Constructor
    *******************************************************************************************************************/
    RndRectangle();
    
    /*******************************************************************************************************************
    *                                               Over Ride Constructor
    * This constructor creates a Round Rectangle where you specify, the size you specify, the radius you specify,
    * the fill color you specify, and the border color you specify.
    *
    * Needs:
    *   text:                   A pointer to some text.
    *   fontSize:               The RA8875 font size.
    *   textColor:              The RA8875 text Color.
    *   numberOfTextLines:      The number of lines of text to put into the Round Rectangle, if zero no text will be drawn in the Rectangle.
    *   setBeginningXPosition:  A beginning X Position.
    *   setBeginningYPosition:  A beginning Y Position.
    *   Width:                  The Width (in pixels) of your Rectangle.
    *   Height:                 The Height (in pixels) of your Rectangle.
    *   radius:                 The radius of the rounded rectangle corners.
    *   fillColor:              A Fill Color for the Circle.
    *   borderColor:            A Border Color for the Circle.
    * Returns:
    *   A printing Rectangle Object.
    *******************************************************************************************************************/
    RndRectangle(char *text, RA8875tsize fontSize, Colors textColor, uint8_t numberOfTextLines, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t Width, int16_t Height, int16_t radius, Colors fillColor, Colors borderColor);
    
    /*******************************************************************************************************************
    *                                               Destructor
    *******************************************************************************************************************/
    ~RndRectangle();
    
    /*******************************************************************************************************************
    * This function actually draws the Round Rectangle on the TFT.
    *******************************************************************************************************************/
    void draw();
    
    /*******************************************************************************************************************
    *
    *******************************************************************************************************************/
    void setRandomProperties(int32_t min=150, int32_t max=800);
    
private:
    char * _text;  // This is a pointer to a character array.
    
};


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
* This Class draws a colored Triangle on the TFT Screen.
*******************************************************************************************************************/
class Triangle : public Shape
{
public:
    
    /*******************************************************************************************************************
    *                                       Default Constructor
    *                                   The position is set by code.
    *******************************************************************************************************************/
    Triangle();
    
    /*******************************************************************************************************************
    *                                       Over Load Constructor
    * This Class draws a colored Triangle on the TFT Screen.  The 3 points of the triangle you specify.
    *
    * Needs:
    *   text:                   A pointer to some text.
    *   fontSize:               The RA8875 font size.
    *   textColor:              The RA8875 text Color.
    *   numberOfTextLines:      The number of lines of text to put into the Circle, Tif zero no text will be drawn in the Triangle.
    *   Three vertexes v1 x & y, v2 x & y, v3 x & y.
    *   fillColor:              A Fill Color for the Triangle.
    * Returns:
    *   A Triangle Object.
    *******************************************************************************************************************/
    Triangle(char *text, RA8875tsize fontSize, Colors textColor, uint8_t numberOfTextLines, int16_t vertex1XPosition, int16_t vertex1YPosition, int16_t vertex2XPosition, int16_t vertex2YPosition, int16_t vertex3XPosition, int16_t vertex3YPosition , Colors fillColor);
    
    /*******************************************************************************************************************
    *                                               Destructor
    *******************************************************************************************************************/
    ~Triangle();
    
    /*******************************************************************************************************************
    * This function actually draws the Triangle on the TFT.
    *******************************************************************************************************************/
    void draw();
    
    /*******************************************************************************************************************
    * This function sets random points for the triangle.
    *******************************************************************************************************************/
    void setRandomProperties(int32_t min=150, int32_t max=800);
    
private:
    char * _text;  // This is a pointer to a character array.
    
};


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
* This Class draws a colored Button with the shape of your choice (TRIANGLE, RECTANGLE, CIRCLE, ELLIPSE, RNDRECTANGLE)
* This constructor creates a rectangular button where you specify, the size you specify, and the colors you specify.
* Then it puts the text you specify in the center of the box horizontally and vertically on the TFT Screen.
*******************************************************************************************************************/
class SelectableShapeButton: public Shape
{
public:
    
    /*******************************************************************************************************************
    *                                          Default Constructor
    *                                   X and Y position is set by code.
    *******************************************************************************************************************/
    SelectableShapeButton();
    
    /*******************************************************************************************************************
    *                                             Rectangle Over Ride Constructor
    * This constructor creates a selectable shape button where you specify, the size you specify, and the colors you specify.
    * Then it puts the text you specify in the center of the box horizontally and vertically.
    *
    * Needs:
    *   buttonShape:            One of the enumerated shapes (TRIANGLE, RECTANGLE, CIRCLE, ELLIPSE, RNDRECTANGLE).
    *   text:                   A pointer to some text.
    *   setBeginningXPosition:  A beginning X Position.
    *   setBeginningYPosition:  A beginning Y Position.
    *   Width:                  The Width (in pixels) of your Rectangle.
    *   Height:                 The Height (in pixels) of your Rectangle.
    *   fontSize:               The RA8875 font size.
    *   numberOfTextLines:      The number of lines of text to put into the Circle, if zero no text will be drawn in the Rectangle.
    *   textColor:              The RA8875 text Color.
    *   fillColor:              A Fill Color for the Rectangle.
    *   borderColor:            A Border Color for the Rectangle.
    * Returns:
    *   A SelectableShapeButton Object.
    *******************************************************************************************************************/
    SelectableShapeButton(Figure_types buttonShape, char *text, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t Width, int16_t Height, RA8875tsize fontSize, uint8_t numberOfTextLines, Colors textColor, Colors fillColor, Colors borderColor);
    
    
    /*******************************************************************************************************************
    *                                               Round Rectangle Over Ride Constructor
    * This constructor creates a rectangular button where you specify, the size you specify, and the colors you specify.
    * Then it puts the text you specify in the center of the box horizontally and vertically.
    *
    * Needs:
    *   buttonShape:            One of the enumerated shapes (TRIANGLE, RECTANGLE, CIRCLE, ELLIPSE, RNDRECTANGLE).
    *   text:                   A pointer to some text.
    *   setBeginningXPosition:  A beginning X Position.
    *   setBeginningYPosition:  A beginning Y Position.
    *   Width:                  The Width (in pixels) of your Rectangle.
    *   Height:                 The Height (in pixels) of your Rectangle.
    *   radius:                 The Radius of the Round Rectangle Corners.
    *   fontSize:               The RA8875 font size.
    *   numberOfTextLines:      The number of lines of text to put into the Circle, if zero no text will be drawn in the Rectangle.
    *   textColor:              The RA8875 text Color.
    *   fillColor:              A Fill Color for the Rectangle.
    *   borderColor:            A Border Color for the Rectangle.
    * Returns:
    *   A Button Object.
    *******************************************************************************************************************/
    SelectableShapeButton(Figure_types buttonShape, char *text, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t Width, int16_t Height, int16_t radius, RA8875tsize fontSize, uint8_t numberOfTextLines, Colors textColor, Colors fillColor, Colors borderColor);
    
    
    /*******************************************************************************************************************
    *                                               Circle Over Ride Constructor
    * This constructor creates a rectangular button where you specify, the size you specify, and the colors you specify.
    * Then it puts the text you specify in the center of the box horizontally and vertically.
    *
    * Needs:
    *   buttonShape:            One of the enumerated shapes (TRIANGLE, RECTANGLE, CIRCLE, ELLIPSE, RNDRECTANGLE).
    *   text:                   A pointer to some text.
    *   setBeginningXPosition:  A beginning X Position.
    *   setBeginningYPosition:  A beginning Y Position.
    *   radius:                 A radius for the Circle.
    *   fontSize:               The RA8875 font size.
    *   numberOfTextLines:      The number of lines of text to put into the Circle, if zero no text will be drawn in the Rectangle.
    *   textColor:              The RA8875 text Color.
    *   fillColor:              A Fill Color for the Rectangle.
    *   borderColor:            A Border Color for the Rectangle.
    * Returns:
    *   A Button Object.
    *******************************************************************************************************************/
    SelectableShapeButton(Figure_types buttonShape, char *text, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t radius, RA8875tsize fontSize, uint8_t numberOfTextLines, Colors textColor, Colors fillColor, Colors borderColor);
    
    
    /*******************************************************************************************************************
    *                                               Ellipse Over Ride Constructor
    * This constructor creates a rectangular button where you specify, the size you specify, and the colors you specify.
    * Then it puts the text you specify in the center of the box horizontally and vertically.
    *
    * Needs:
    *   buttonShape:            One of the enumerated shapes (TRIANGLE, RECTANGLE, CIRCLE, ELLIPSE, RNDRECTANGLE).
    *   text:                   A pointer to some text.
    *   setBeginningXPosition:  x location of the center of the ellipse.
    *   setBeginningYPosition:  y location of the center of the ellipse.
    *   longAxis:               Size in pixels of the long axis.
    *   shortAxis:              Size in pixels of the short axis.
    *   fontSize:               The RA8875 font size.
    *   numberOfTextLines:      The number of lines of text to put into the Ellipse, if zero no text will be drawn in the Ellipse.
    *   textColor:              The RA8875 text Color.
    *   fillColor:              A Fill Color for the Rectangle.
    *   borderColor:            A Border Color for the Rectangle.
    * Returns:
    *   A Button Object.
    *******************************************************************************************************************/
    SelectableShapeButton(Figure_types buttonShape, char *text, int16_t setBeginningXPosition, int16_t setBeginningYPosition,RA8875tsize fontSize, int16_t longAxis, int16_t shortAxis, uint8_t numberOfTextLines, Colors textColor, Colors fillColor, Colors borderColor);
    
    
    /*******************************************************************************************************************
    *                                               Triangle Over Ride Constructor
    * This constructor creates a rectangular button where you specify, the size you specify, and the colors you specify.
    * Then it puts the text you specify in the center of the box horizontally and vertically.
    *
    * Needs:
    *   buttonShape:            One of the enumerated shapes (TRIANGLE, RECTANGLE, CIRCLE, ELLIPSE, RNDRECTANGLE).
    *   text:                   A pointer to some text.
    *   Three vertexes:         v1 x & y, v2 x & y, v3 x & y.
    *   fontSize:               The RA8875 font size.
    *   numberOfTextLines:      The number of lines of text to put into the Ellipse, if zero no text will be drawn in the Ellipse.
    *   textColor:              The RA8875 text Color.
    *   fillColor:              A Fill Color for the Rectangle.
    * Returns:
    *   A SelectableShapeButton Object.
    *******************************************************************************************************************/
    SelectableShapeButton(Figure_types buttonShape, char *text, int16_t vertex1XPosition, int16_t vertex1YPosition, int16_t vertex2XPosition, int16_t vertex2YPosition, int16_t vertex3XPosition, int16_t vertex3YPosition , RA8875tsize fontSize, uint8_t numberOfTextLines, Colors textColor, Colors fillColor);
    
    
    /*******************************************************************************************************************
     *                                               Destructor
     *******************************************************************************************************************/
    ~SelectableShapeButton();
    
    /*******************************************************************************************************************
     * This function actually draws the Button object on the TFT.
     *******************************************************************************************************************/
    void draw();
    
    /*******************************************************************************************************************
     * This function checks for a touch inside the boundaries of the Button.
     *   Needs:
     *       TouchScreenPoints Struct with xposition and yposition.
     *   Returns:
     *       On_Figure variable, which could be any of the following:
     *           ON_TRIANGLE,
     *           ON_RECT,
     *           ON_CIRCLE,
     *           ON_ELLIPSE,
     *           ON_RNDRECT,
     *           ERROR
     *******************************************************************************************************************/
    On_Figure OnTouch(TouchScreenPoints_t pos);
    
private:
    Figure_types _buttonShape;
    char * _text;  // This is a pointer to a character array.
    
    // These are for the triangle button.
    TouchScreenPoints_t XYPointsvertex1;
    TouchScreenPoints_t XYPointsvertex2;
    TouchScreenPoints_t XYPointsvertex3;
    
    TouchScreenPoints_t rectangle_pos;
    TouchScreenPoints_t triangle_pos1;
    TouchScreenPoints_t triangle_pos2;
    TouchScreenPoints_t triangle_pos3;
    TouchScreenPoints_t circle_pos;
    TouchScreenPoints_t ellipse_pos1;
    LongAndShortAxis_t ellipse_pos2;
    TouchScreenPoints_t rndrectangle_pos;
    
    On_Figure touch_on_figure;  // This variable tells you whether or not the buttons was pushed, On_Figure is an enumerated type.
    
    // Button Types.
    Rectangle *newRectbutton;
    RndRectangle *newRRectbutton;
    Circle *newCirbutton;
    Triangle *newTributton;
    Ellipse *newEllbutton;
};


#endif
