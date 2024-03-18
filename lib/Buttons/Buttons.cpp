//
/// Buttons.cpp
/// Library C++ code
// ----------------------------------
// Developed with embedXcode 
// http://embedXcode.weebly.com
//
// Project 		Mister Controller
//
// Created by 	freddie, 8/11/18 4:12 PM
// 				Kingston Co.
//
// Copyright 	© freddie, 2018
// License     license
//
// See 			Buttons_.h and ReadMe.txt for references
//


// Library header
#include "Buttons.h"


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
*
*******************************************************************************************************************/
Colors getRandomColor()
{
    int val = (int)random (0, 13);
    switch (val)
    {
        case 0:
            return BLACK;
        case 1:
            return WHITE;
        case 2:
            return RED;
        case 3:
            return GREEN;
        case 4:
            return BLUE;
        case 5:
            return CYAN;
        case 6:
            return MAGENTA;
        case 7:
            return YELLOW;
        case 8:
            return LIGHT_GREY;
        case 9:
            return LIGHT_ORANGE;
        case 10:
            return DARK_ORANGE;
        case 11:
            return PINK;
        case 12:
            return PURPLE;
        case 13:
            return GRAYSCALE;
            
    }
    return WHITE;
}


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
*                                           Default Constructor
* This Class draws a colored printing circle on the TFT Screen
*
* Needs:
*   Nothing
* Returns:
*   A Circle Object.
*******************************************************************************************************************/

/*******************************************************************************************************************
* Default Constructor
*******************************************************************************************************************/
Circle::Circle() : _text((char*)"Circle")
{
    
    _beginningXYPositions.xPosition = 359;
    _beginningXYPositions.yPosition = 214;
    _radius = 30;
    
    _fontSize = (RA8875tsize)1;
    _numberOfTextLines = 0;  // This means no text will be drawn in the circle.
    _textcolor = WHITE;

    _fillcolor = RED;
    _bordercolor = BLACK;
    
    #if defined(DEBUG1)
        Serial.print(F("Circle::Circle Default Constructor..."));
        Serial.print(F("The is the Radius of the circle to be drawn: "));
        Serial.println(_radius);
        Serial.print(F("The is the X-Y Position of the circle to be drawn: "));
    
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the text color of the Circle to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("The is the border color of the circle to be drawn: "));
        Serial.println(_bordercolor,HEX);
    
        Serial.print(F("The is the fill color of the circle to be drawn: "));
        Serial.println(_fillcolor,HEX);
        Serial.print(F("This is the font size of the Text to be drawn in the Circle: "));
        Serial.println(_fontSize);  // This prints the font size.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Circle Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Button: "));
        Serial.println(_text);  // This prints the character array.
        Serial.println(F(""));
    #endif
}

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
Circle::Circle(char *text, RA8875tsize fontSize, Colors textColor, uint8_t numberOfTextLines, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t radius, Colors fillColor, Colors borderColor)
{
    if ( (radius < 3) || ( radius > ( (X_CONST / 2) - 1 ) ) )
        {
            #if defined(DEBUG1)
                Serial.print(F("Circle::Circle OverLoad Constructor..."));
                Serial.print(F("Circle::Circle Constructor : if ( (radius < 1) || ( radius > ( (X_CONST / 2) - 1 ) ) ): "));
                Serial.println(_radius);
                Serial.println(F("Circle::Circle Constructor : Setting radius to default 40"));
                Serial.println(F(""));
            #endif
            _radius = 40;
        }
    _radius = radius;
    _fillcolor = fillColor;
    _bordercolor = borderColor;
    _beginningXYPositions.xPosition = setBeginningXPosition;
    _beginningXYPositions.yPosition = setBeginningYPosition;
    
    _textcolor = textColor;
    
    if ( ( fontSize > 2 ) || ( fontSize < 0) )
        {
            _fontSize = (RA8875tsize)0;
        }
    else
        {
            _fontSize = fontSize;
        }
    
    _numberOfTextLines = numberOfTextLines;
    
    _text = text;
    
    #if defined(DEBUG1)
        Serial.print(F("Circle::Circle OverLoad Constructor..."));
        Serial.print(F("The is the Radius of the circle to be drawn: "));
        Serial.println(_radius);
    
        Serial.print(F("The is the X-Y Position of the circle to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the border color of the circle to be drawn: "));
        Serial.println(_bordercolor,HEX);
    
        Serial.print(F("The is the fill color of the circle to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the circle to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the circle: "));
        Serial.println(_fontSize);  // This prints the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the circle Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the circle: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
}

/*******************************************************************************************************************
* Destructor
*******************************************************************************************************************/
Circle::~Circle(){}

/*******************************************************************************************************************
*
*******************************************************************************************************************/
void Circle::setRandomProperties(int32_t min, int32_t max)
{
    _radius = (int)random (1, 50);
    Shape::setRandomProperties(min + _radius);
}

/*******************************************************************************************************************
* This function actually draws the Circle on the TFT.
*******************************************************************************************************************/
void Circle::draw()
{
    char newString[5][20];
    int i,j,ctr;
    int chararrylen = strlen(_text);
    
    #if defined(DEBUG1)
        Serial.print(F("Circle::Circle::draw ..."));
        Serial.print(F("The is the Radius of the circle to be drawn: "));
        Serial.println(_radius);
        Serial.print(F("The is the X-Y Position of the circle to be drawn: "));

        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the border color of the circle to be drawn: "));
        Serial.println(_bordercolor,HEX);
    
        Serial.print(F("The is the fill color of the circle to be drawn: "));
        Serial.println(_fillcolor,HEX);
        Serial.print(F("The is the text color of the circle to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the circle: "));
        Serial.println(_fontSize);  // This only prints the first character of the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the circle Vertically: "));
        Serial.println(_numberOfTextLines);  // This only prints the first character of the character array.
    
        Serial.print(F("This is the size of the character array to be drawn in the circle: "));
        Serial.println(chararrylen);
    
        Serial.print(F("This is the Text to be drawn in the circle: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
    
    if (_bordercolor != BLACK)
        {
            #if defined(DEBUG1)
                Serial.println(F("Circle::Circle::draw ..."));
                Serial.println(F("Border Color is not BLACK! (0000)"));
                Serial.println(F("Drawing Circle"));
                Serial.println(F(""));
            #endif
            tft.drawCircle(_beginningXYPositions.xPosition, _beginningXYPositions.yPosition, _radius, _bordercolor);
        }
    tft.fillCircle(_beginningXYPositions.xPosition, _beginningXYPositions.yPosition, (_radius - 3), _fillcolor);
    
    if (_numberOfTextLines == 0)
        {
            return;
        }
    
    #if defined(DEBUG1)
        Serial.println(F("Circle::draw function ..."));
        Serial.println(F("Begin drawing the text in the Circle"));
        Serial.println(F(""));
    #endif
    
    tft.setFontScale(_fontSize);  // This sets the font size (0,1,2,3) passed into this class.
    
    // Need to find the Center of the figure to be drawn.
    _XCenterPosition = ( ( _beginningXYPositions.xPosition + _radius ) );
    
    _fontWidth = tft.getFontWidth();  // This is the width of each character in pixels.
    
    _YCenterPosition = ( ( _beginningXYPositions.yPosition + _radius ) );
    
    _fontHeight = tft.getFontHeight();  // This is the height of each character in pixels.
    
    /// Should calculate the Text width and height to make sure it will fit inside box.
    
    #if defined(DEBUG1)
        Serial.println(F("Circle::draw function ..."));
        Serial.print(F("_fontWidth from tft.getFontWidth: "));
        Serial.println(_fontWidth);
        Serial.print(F("_fontWidth from tft.getFontHeight: "));
        Serial.println(_fontHeight);
        Serial.print(F("_XCenterPosition: "));
        Serial.println(_XCenterPosition);
        Serial.print(F("_YCenterPosition: "));
        Serial.println(_YCenterPosition);
        Serial.println(F(""));
    #endif
    
    tft.setTextColor(_textcolor);  //set text color (backgroud will be transparent)
    
    #if defined(DEBUG1)
        Serial.println(F("Circle::draw function ..."));
        Serial.println(F("Finished setting the Text Color..."));
        Serial.print(F("Now checking for the number of lines of text: "));
        Serial.println(_numberOfTextLines);
        Serial.println(F(""));
    #endif
    
    if (_numberOfTextLines > 1 && (_numberOfTextLines < 6) )
        {
            #if defined(DEBUG1)
                Serial.println(F("Circle::draw function ..."));
                Serial.println(F("if (_numberOfTextLines > 1)..."));
                Serial.print(F("This is the text string to scan: "));
                Serial.println(_text);
                Serial.print(F("This is the size of the text string to scan: "));
                Serial.println(chararrylen);
                Serial.println(F("Parsing the _text string for a space deliminating character and making separate char strings..."));
                Serial.println(F(""));
            #endif
        
            j=0;
            ctr=0;
        
            for(i = 0; i <= chararrylen; i++)
                {
                    // If space or NULL found, put NULL into newString[ctr]
                    if( (_text[i]==' ') || (_text[i]=='\0') )
                        {
                            newString[ctr][j] = '\0';
                            ctr++;  //for next word
                            j=0;    //for next word, init index to 0
                        }
                    else
                        {
                            newString[ctr][j] = _text[i];  // Move text to the array[ctr].
                            j++;
                        }
                }
        
            _numberOfCharacters = strlen(newString[0]);  // This gives the number of characters in the 1st string without the \0 Terminating character.
        
            #if defined(DEBUG1)
                Serial.println(F("Circle::draw function ..."));
                Serial.print(F("This is the X Cursor Position: "));
                Serial.println( _XCenterPosition - ( ( _fontWidth * _numberOfCharacters ) / 2 ) );
                Serial.print(F("This is the Y Cursor Position: "));
                Serial.println( _YCenterPosition - ( ( _fontHeight / 2 ) * ( _numberOfTextLines) ) );
                Serial.println(F("Setting Cursor Position...\n"));
            #endif
        
            tft.setCursor( ( (int16_t)_XCenterPosition - ( ( ( _fontWidth * _numberOfCharacters ) / 2 ) ) ), ( (int16_t)_YCenterPosition - ( ( _fontHeight / 2 ) * ( _numberOfTextLines ) ) ) );  // Centering the Text in the X Position, Y Position 1 1/2 character heights above the center of the box.
        
            #if defined(DEBUG1)
                Serial.println(F("Circle::draw function ..."));
                Serial.print(F("This is the number of characters to print: "));
                Serial.println(_numberOfCharacters);
                Serial.print(F("This is the contents of newString: "));
                Serial.println(newString[0]);
                Serial.println(F("Now printing to the TFT...\n"));
            #endif
        
            tft.print(newString[0]);  // Prints a string pointed to by "newString[0]".
        
            for(i = 1; i < ctr; i++)  // Now print the following strings to the TFT.
                {
                    #if defined(DEBUG1)
                        Serial.println(F("Circle::draw function ..."));
                        Serial.println(F("Getting the string length from strlen..."));
                        Serial.println(F(""));
                    #endif
            
                    _numberOfCharacters = strlen(newString[i]);  // This gives the number of characters in the string without the \0 Terminating character.
            
                    #if defined(DEBUG1)
                        Serial.println(F("Circle::draw function ..."));
                        Serial.print(F("This is the number of characters to print: "));
                        Serial.println(_numberOfCharacters);
                        Serial.print(F("This is the contents of newString: "));
                        Serial.println(newString[i]);
                        Serial.println(F("Now printing to the TFT...\n"));
                    #endif
            
                    tft.setCursor( ( (int16_t)_XCenterPosition - ( ( _fontWidth * _numberOfCharacters ) / 2 ) ), (int16_t)( _YCenterPosition - ( ( _fontHeight / 2 ) * _numberOfTextLines ) ) + (_fontHeight * i) );  // Move to next line down.
            
                    tft.print(newString[i]);  // Prints a string pointed to by "newString[i]".
            
                }
        
        }
    else if (_numberOfTextLines == 1 )
        {
            _numberOfCharacters = strlen(_text);  // This gives the number of characters in the string without the \0 Terminating character.
            tft.setCursor( ( (int16_t)_XCenterPosition - ( ( ( _fontWidth * _numberOfCharacters ) / 2 ) ) ), ( (int16_t)_YCenterPosition - ( _fontHeight / 2) ) );  // Centering the Text in the box.
            tft.print(_text);
        }
}


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
* This Class draws a colored ellispe on the TFT Screen
* Parameters Needed:
*   Nothing
* Returns:
*   A Ellipse Object.
*******************************************************************************************************************/
/*******************************************************************************************************************
 * Default Constructor
 *******************************************************************************************************************/
Ellipse::Ellipse() : _text((char*)"Ellipse")
{
    
    _beginningXYPositions.xPosition = 359;
    _beginningXYPositions.yPosition = 214;
    
    _longAxis = 60;
    _shortAxis = 30;
    
    _xCenter = (_beginningXYPositions.xPosition + (_longAxis / 2));
    _yCenter = (_beginningXYPositions.yPosition + (_shortAxis / 2));
    
    _fontSize = (RA8875tsize)1;
    _numberOfTextLines = 0;  // This means no text will be drawn in the Rectangle.
    _textcolor = WHITE;

    _fillcolor = RED;
    _bordercolor = BLACK;
    
    #if defined(DEBUG2)
        Serial.print(F("Ellipse::Ellipse Default Constructor..."));
        Serial.print(F("The is the longAxis of the Ellipse to be drawn: "));
        Serial.println(_longAxis);
    
        Serial.print(F("The is the shortAxis of the Ellipse to be drawn: "));
        Serial.println(_shortAxis);

        Serial.print(F("The is the beginning X-Y Position of the Ellipse to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the xCenter of the Ellipse to be drawn: "));
        Serial.println(_xCenter);
    
        Serial.print(F("The is the yCenter of the Ellipse to be drawn: "));
        Serial.println(_yCenter);
    
        Serial.print(F("The is the border color of the Ellipse to be drawn: "));
        Serial.println(_bordercolor,HEX);
    
        Serial.print(F("The is the fill color of the Ellipse to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Ellipse to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Ellipse: "));
        Serial.println(_fontSize);  // This prints the font size.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Ellipse Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Ellipse: "));
        Serial.println(_text);  // This prints the character array.
        Serial.println(F(""));
    #endif
}

/*******************************************************************************************************************
*                                        OverLoad Constructor
* Parameters Needed:
*   text:                   A pointer to some text.
*   fontSize:               The RA8875 font size.
*   textColor:              The RA8875 text Color.
*   numberOfTextLines:      The number of lines of text to put into the Circle.
*   setBeginningXPosition:  x location of the center of the ellipse.
*   setBeginningYPosition:  y location of the center of the ellipse.
*   longAxis:               Size in pixels of the long axis.
*   shortAxis:              Size in pixels of the short axis.
*   fillColor:              RGB565 color
*   borderColor:            RGB565 color.
* Returns:
*   A Ellipse Object.
*******************************************************************************************************************/
Ellipse::Ellipse(char *text, RA8875tsize fontSize, Colors textColor, uint8_t numberOfTextLines, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t longAxis, int16_t shortAxis, Colors fillColor, Colors borderColor)
{
    if ( (longAxis < 2) || ( longAxis > ( (X_CONST / 2) - 1 ) ) )
        {
            #if defined(DEBUG2)
                Serial.print(F("Ellipse OverLoad Constructor..."));
                Serial.print(F("if ( (longAxis < 2) || ( longAxis > ( (X_CONST / 2) - 1 ) ) ): "));
                Serial.println(longAxis);
                Serial.println(F("Setting longAxis to default 40"));
                Serial.println(F(""));
            #endif
            _longAxis = 40;
        }
    else
        {
            _longAxis = longAxis;
        }
    
    if ( (shortAxis < 2) || ( shortAxis > ( (Y_CONST / 2) - 1 ) ) )
        {
            #if defined(DEBUG2)
                Serial.print(F("Ellipse OverLoad Constructor..."));
                Serial.print(F("if ( (shortAxis < 2) || ( shortAxis > ( (Y_CONST / 2) - 1 ) ) ): "));
                Serial.println(shortAxis);
                Serial.println(F("Setting shortAxis to default 20"));
                Serial.println(F(""));
            #endif
            _shortAxis = 20;
        }
    else
        {
            _shortAxis = shortAxis;
        }
    
    _fillcolor = fillColor;
    _bordercolor = borderColor;
    
    _beginningXYPositions.xPosition = setBeginningXPosition;
    _beginningXYPositions.yPosition = setBeginningYPosition;
    
    _xCenter = (_beginningXYPositions.xPosition + (_longAxis / 2));
    _yCenter = (_beginningXYPositions.yPosition + (_shortAxis / 2));
    
    _textcolor = textColor;
    
    if ( ( fontSize > 2 ) || ( fontSize < 0) )
        {
            _fontSize = (RA8875tsize)0;
        }
    else
        {
            _fontSize = fontSize;
        }
    
    _numberOfTextLines = numberOfTextLines;
    
    _text = text;
    
    #if defined(DEBUG2)
        Serial.print(F("Ellipse OverLoad Constructor..."));
        Serial.print(F("The is the longAxis of the Ellipse to be drawn: "));
        Serial.println(_longAxis);
    
        Serial.print(F("The is the shortAxis of the Ellipse to be drawn: "));
        Serial.println(_shortAxis);
    
        Serial.print(F("The is the X-Y Position of the Ellipse to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the xCenter of the Ellipse to be drawn: "));
        Serial.println(_xCenter);
    
        Serial.print(F("The is the yCenter of the Ellipse to be drawn: "));
        Serial.println(_yCenter);
    
        Serial.print(F("The is the border color of the Ellipse to be drawn: "));
        Serial.println(_bordercolor,HEX);
    
        Serial.print(F("The is the fill color of the Ellipse to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Ellipse to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Ellipse: "));
        Serial.println(_fontSize);  // This prints the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Ellipse Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Ellipse: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
}

/*******************************************************************************************************************
*                                                   Destructor
*******************************************************************************************************************/
Ellipse::~Ellipse(){}

/*******************************************************************************************************************
*                                                   FIX THIS
*******************************************************************************************************************/
void Ellipse::setRandomProperties(int32_t min, int32_t max)
{
    //_radius = (int)random (1, 50);
    //Shape::setRandomProperties(min + _radius);
}

/*******************************************************************************************************************
* This function actually draws the Ellipse on the TFT.
*******************************************************************************************************************/
void Ellipse::draw()
{
    char newString[5][20];
    int i,j,ctr;
    int chararrylen = strlen(_text);
    
    #if defined(DEBUG1)
        Serial.print(F("Ellipse::draw ..."));
        Serial.print(F("The is the longAxis of the Ellipse to be drawn: "));
        Serial.println(_longAxis);
    
        Serial.print(F("The is the shortAxis of the Ellipse to be drawn: "));
        Serial.println(_shortAxis);
    
        Serial.print(F("The is the X-Y Position of the Ellipse to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the xCenter of the Ellipse to be drawn: "));
        Serial.println(_xCenter);
    
        Serial.print(F("The is the yCenter of the Ellipse to be drawn: "));
        Serial.println(_yCenter);
    
        Serial.print(F("The is the border color of the Ellipse to be drawn: "));
        Serial.println(_bordercolor,HEX);
    
        Serial.print(F("The is the fill color of the Ellipse to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Button to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Button: "));
        Serial.println(_fontSize);  // This only prints the first character of the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Button Vertically: "));
        Serial.println(_numberOfTextLines);  // This only prints the first character of the character array.
    
        Serial.print(F("This is the size of the character array to be drawn in the Button: "));
        Serial.println(chararrylen);
    
        Serial.print(F("This is the Text to be drawn in the Button: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
    
    if (_bordercolor != BLACK)
        {
            #if defined(DEBUG1)
                Serial.println(F("Ellipse::draw ..."));
                Serial.println(F("Border Color is not BLACK! (0000)"));
                Serial.println(F("Drawing Ellipse"));
                Serial.println(F(""));
            #endif
            tft.drawEllipse(_xCenter, _yCenter, _longAxis,  _shortAxis, _bordercolor);
        }
    tft.fillEllipse(_xCenter, _yCenter, (_longAxis - 4), (_shortAxis - 4), _fillcolor);
    
    if (_numberOfTextLines == 0)
        {
            return;
        }
    
    #if defined(DEBUG1)
        Serial.println(F("Ellipse::draw function ..."));
        Serial.println(F("Begin drawing the text in the Ellipse"));
        Serial.println(F(""));
    #endif
    
    tft.setFontScale(_fontSize);  // This sets the font size (0,1,2,3) passed into this class.
    
    // Need to find the Center of the figure to be drawn.
    _XCenterPosition = _xCenter;
    
    _fontWidth = tft.getFontWidth();  // This is the width of each character in pixels.
    
    _YCenterPosition = _yCenter;
    
    _fontHeight = tft.getFontHeight();  // This is the height of each character in pixels.
    
    /// Should calculate the Text width and height to make sure it will fit inside box.
    
    #if defined(DEBUG1)
        Serial.println(F("Ellipse::draw function ..."));
        Serial.print(F("_fontWidth from tft.getFontWidth: "));
        Serial.println(_fontWidth);
        Serial.print(F("_fontWidth from tft.getFontHeight: "));
        Serial.println(_fontHeight);
        Serial.print(F("_XCenterPosition: "));
        Serial.println(_XCenterPosition);
        Serial.print(F("_YCenterPosition: "));
        Serial.println(_YCenterPosition);
        Serial.println(F(""));
    #endif
    
    tft.setTextColor(_textcolor);  //set text color (backgroud will be transparent)
    
    #if defined(DEBUG1)
        Serial.println(F("Ellipse::draw function ..."));
        Serial.println(F("Finished setting the Text Color..."));
        Serial.print(F("Now checking for the number of lines of text: "));
        Serial.println(_numberOfTextLines);
        Serial.println(F(""));
    #endif
    
    if (_numberOfTextLines > 1 && (_numberOfTextLines < 6) )
        {
            #if defined(DEBUG1)
                Serial.println(F("Ellipse::draw function ..."));
                Serial.println(F("if (_numberOfTextLines > 1)..."));
                Serial.print(F("This is the text string to scan: "));
                Serial.println(_text);
                Serial.print(F("This is the size of the text string to scan: "));
                Serial.println(chararrylen);
                Serial.println(F("Parsing the _text string for a space deliminating character and making separate char strings..."));
                Serial.println(F(""));
            #endif
        
            j=0;
            ctr=0;
        
            for(i = 0; i <= chararrylen; i++)
                {
                    // If space or NULL found, put NULL into newString[ctr]
                    if( (_text[i]==' ') || (_text[i]=='\0') )
                        {
                            newString[ctr][j] = '\0';
                            ctr++;  //for next word
                            j=0;    //for next word, init index to 0
                        }
                    else
                        {
                            newString[ctr][j] = _text[i];  // Move text to the array[ctr].
                            j++;
                        }
                }
        
            _numberOfCharacters = strlen(newString[0]);  // This gives the number of characters in the 1st string without the \0 Terminating character.
        
            #if defined(DEBUG1)
                Serial.println(F("Ellipse::draw function ..."));
                Serial.print(F("This is the X Cursor Position: "));
                Serial.println( _XCenterPosition - ( ( _fontWidth * _numberOfCharacters ) / 2 ) );
                Serial.print(F("This is the Y Cursor Position: "));
                Serial.println( _YCenterPosition - ( ( _fontHeight / 2 ) * ( _numberOfTextLines) ) );
                Serial.println(F("Setting Cursor Position...\n"));
            #endif
        
            tft.setCursor( ( (int16_t)_XCenterPosition - ( ( ( _fontWidth * _numberOfCharacters ) / 2 ) ) ), ( (int16_t)_YCenterPosition - ( ( _fontHeight / 2 ) * ( _numberOfTextLines ) ) ) );  // Centering the Text in the X Position, Y Position 1 1/2 character heights above the center of the box.
        
            #if defined(DEBUG1)
                Serial.println(F("Ellipse::draw function ..."));
                Serial.print(F("This is the number of characters to print: "));
                Serial.println(_numberOfCharacters);
                Serial.print(F("This is the contents of newString: "));
                Serial.println(newString[0]);
                Serial.println(F("Now printing to the TFT...\n"));
            #endif
        
            tft.print(newString[0]);  // Prints a string pointed to by "newString[0]".
        
            for(i = 1; i < ctr; i++)  // Now print the following strings to the TFT.
                {
                    #if defined(DEBUG1)
                        Serial.println(F("Ellipse::draw function ..."));
                        Serial.println(F("Getting the string length from strlen..."));
                        Serial.println(F(""));
                    #endif
            
                    _numberOfCharacters = strlen(newString[i]);  // This gives the number of characters in the string without the \0 Terminating character.
            
                    #if defined(DEBUG1)
                        Serial.println(F("Ellipse::draw function ..."));
                        Serial.print(F("This is the number of characters to print: "));
                        Serial.println(_numberOfCharacters);
                        Serial.print(F("This is the contents of newString: "));
                        Serial.println(newString[i]);
                        Serial.println(F("Now printing to the TFT...\n"));
                    #endif
            
                    tft.setCursor( ( (int16_t)_XCenterPosition - ( ( _fontWidth * _numberOfCharacters ) / 2 ) ), (int16_t)( _YCenterPosition - ( ( _fontHeight / 2 ) * _numberOfTextLines ) ) + (_fontHeight * i) );  // Move to next line down.
            
                    tft.print(newString[i]);  // Prints a string pointed to by "newString[i]".
            
                }
        
        }
    else if (_numberOfTextLines == 1 )
        {
            _numberOfCharacters = strlen(_text);  // This gives the number of characters in the string without the \0 Terminating character.
            tft.setCursor( ( (int16_t)_XCenterPosition - ( ( ( _fontWidth * _numberOfCharacters ) / 2 ) ) ), ( (int16_t)_YCenterPosition - ( _fontHeight / 2) ) );  // Centering the Text in the box.
            tft.print(_text);
        }
}


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
*                                           Default Constructor
* This Class draws a colored printing Rectangle on the TFT Screen
*
* Needs:
*   Nothing.
* Returns:
*   A Rectangle Object.
*******************************************************************************************************************/
Rectangle::Rectangle() : _text((char*)"Rect")
{
    _beginningXYPositions.xPosition = 359;
    _beginningXYPositions.yPosition = 214;
    _width_height.width = 80;
    _width_height.height = 50;
    
    _fontSize = (RA8875tsize)1;
    _numberOfTextLines = 0;  // This means no text will be drawn in the Rectangle.
    _textcolor = WHITE;
    
    _fillcolor = WHITE;
    _bordercolor = BLUE;
    
    #if defined(DEBUG2)
        Serial.print(F("Rectangle::Rectangle Default Constructor..."));
        Serial.print(F("The is the X-Y Position of the Rectangle to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the Width & Height of the Rectangle to be drawn: "));
        Serial.print(_width_height.width);
        Serial.print(F(" , "));
        Serial.println(_width_height.height);
    
        Serial.print(F("The is the border color of the Rectangle to be drawn: "));
        Serial.println(_bordercolor,HEX);
        Serial.print(F("The is the fill color of the Rectangle to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Rectangle to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Rectangle: "));
        Serial.println(_fontSize);  // This prints the font size.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Rectangle Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Rectangle: "));
        Serial.println(_text);  // This prints the character array.
        Serial.println(F(""));
    #endif
    
}

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
Rectangle::Rectangle(char *text, RA8875tsize fontSize, Colors textColor, uint8_t numberOfTextLines, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t Width, int16_t Height, Colors fillColor, Colors borderColor)
{
    if ( (Width == 0) || (Height == 0) )
        {
            _width_height.width = 100;
            _width_height.height = 50;
        }
    
    _beginningXYPositions.xPosition = setBeginningXPosition;
    _beginningXYPositions.yPosition = setBeginningYPosition;
    
    _width_height.width = Width;
    _width_height.height = Height;
    
    _textcolor = textColor;
    
    if ( ( fontSize > 2 ) || ( fontSize < 0) )
        {
            _fontSize = (RA8875tsize)0;
        }
    else
        {
            _fontSize = fontSize;
        }
    
    _numberOfTextLines = numberOfTextLines;
    
    _text = text;
    
    _fillcolor = fillColor;
    _bordercolor = borderColor;
    
    #if defined(DEBUG2)
        Serial.println(F("Rectangle::Rectangle OverLoad Constructor..."));
        Serial.print(F("The is the X-Y Position of the Rectangle to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the Width & Height of the Rectangle to be drawn: "));
        Serial.print(_width_height.width);
        Serial.print(F(" , "));
        Serial.println(_width_height.height);
    
        Serial.print(F("The is the border color of the Rectangle to be drawn: "));
        Serial.println(_bordercolor,HEX);
        Serial.print(F("The is the fill color of the Rectangle to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Rectangle to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Rectangle: "));
        Serial.println(_fontSize);  // This prints the character array.
    
        Serial.print(F("This is the number of rows of Text to be drawn in the Rectangle Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Rectangle: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
    
}

/*******************************************************************************************************************
*                                               Destructor
*******************************************************************************************************************/
Rectangle::~Rectangle(){}

/*******************************************************************************************************************
*
*******************************************************************************************************************/
void Rectangle::setRandomProperties(int16_t min, int16_t max)
{
    _width_height.width = (uint16_t)random (5, 150);
    _width_height.height = (uint16_t)random (5, 150);
    Shape::setRandomProperties(min, (max - (_width_height.width)) );
}

/*******************************************************************************************************************
* This function actually draws the Rectangle on the TFT.
*******************************************************************************************************************/
void Rectangle::draw()
{
    char newString[5][20];
    int i,j,ctr;
    int chararrylen = strlen(_text);
    
    #if defined(DEBUG1)
        Serial.println(F("Rectangle::Rectangle::draw function..."));
        Serial.print(F("The is the X-Y Position of the Rectangle to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the Width & Height of the Rectangle to be drawn: "));
        Serial.print(_width_height.width);
        Serial.print(F(" , "));
        Serial.println(_width_height.height);
    
        Serial.print(F("The is the border color of the Rectangle to be drawn: "));
        Serial.println(_bordercolor,HEX);
        Serial.print(F("The is the fill color of the Rectangle to be drawn: "));
        Serial.println(_fillcolor,HEX);
        Serial.print(F("The is the text color of the ectangle to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Rectangle: "));
        Serial.println(_fontSize);  // This only prints the first character of the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Rectangle Vertically: "));
        Serial.println(_numberOfTextLines);  // This only prints the first character of the character array.
    
        Serial.print(F("This is the size of the character array to be drawn in the Rectangle: "));
        Serial.println(chararrylen);
    
        Serial.print(F("This is the Text to be drawn in the Button: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
    
    if (_bordercolor != BLACK)
        {
            #if defined(DEBUG1)
                Serial.println(F("Rectangle::Rectangle::draw function ..."));
                Serial.println(F("Border Color is not BLACK! (0000)"));
                Serial.println(F("Drawing Rectangle"));
                Serial.println(F(""));
            #endif
            tft.drawRect(_beginningXYPositions.xPosition, _beginningXYPositions.yPosition, _width_height.width, _width_height.height, _bordercolor);
        }
    tft.fillRect( (_beginningXYPositions.xPosition + 2), (_beginningXYPositions.yPosition + 2), (_width_height.width - 4), (_width_height.height - 4), _fillcolor);
    
    if (_numberOfTextLines == 0)
        {
            return;
        }
    
    #if defined(DEBUG1)
        Serial.println(F("Rectangle::draw function ..."));
        Serial.println(F("Begin drawing the text in the Button"));
        Serial.println(F(""));
    #endif
    
    tft.setFontScale(_fontSize);  // This sets the font size (0,1,2,3) passed into this class.
    
    // Need to find the Center of the figure to be drawn.
    _XCenterPosition = ( ( _beginningXYPositions.xPosition + ( _width_height.width / 2 ) ) );
    
    _fontWidth = tft.getFontWidth();  // This is the width of each character in pixels.
    
    _YCenterPosition = ( ( _beginningXYPositions.yPosition + ( _width_height.height / 2 ) ) );
    
    _fontHeight = tft.getFontHeight();  // This is the height of each character in pixels.
    
    /// Should calculate the Text width and height to make sure it will fit inside box.
    
    #if defined(DEBUG1)
        Serial.println(F("Rectangle::draw function ..."));
        Serial.print(F("_fontWidth from tft.getFontWidth: "));
        Serial.println(_fontWidth);
        Serial.print(F("_fontWidth from tft.getFontHeight: "));
        Serial.println(_fontHeight);
        Serial.print(F("_XCenterPosition: "));
        Serial.println(_XCenterPosition);
        Serial.print(F("_YCenterPosition: "));
        Serial.println(_YCenterPosition);
        Serial.println(F(""));
    #endif
    
    tft.setTextColor(_textcolor);  //set text color (backgroud will be transparent)
    
    #if defined(DEBUG1)
        Serial.println(F("Rectangle::draw function ..."));
        Serial.println(F("Finished setting the Text Color..."));
        Serial.print(F("Now checking for the number of lines of text: "));
        Serial.println(_numberOfTextLines);
        Serial.println(F(""));
    #endif
    
    if ( (_numberOfTextLines > 1 ) && (_numberOfTextLines < 6) )
        {
            #if defined(DEBUG1)
                Serial.println(F("Rectangle::draw function ..."));
                Serial.println(F("if (_numberOfTextLines > 1)..."));
                Serial.print(F("This is the text string to scan: "));
                Serial.println(_text);
                Serial.print(F("This is the size of the text string to scan: "));
                Serial.println(chararrylen);
                Serial.println(F("Parsing the _text string for a space deliminating character and making separate char strings..."));
                Serial.println(F(""));
            #endif
        
            j=0;
            ctr=0;
        
            for(i = 0; i <= chararrylen; i++)
                {
                    // If space or NULL found, put NULL into newString[ctr]
                    if( (_text[i]==' ') || (_text[i]=='\0') )
                        {
                            newString[ctr][j] = '\0';
                            ctr++;  //for next word
                            j=0;    //for next word, init index to 0
                        }
                    else
                        {
                            newString[ctr][j] = _text[i];  // Move text to the array[ctr].
                            j++;
                        }
                }
        
            _numberOfCharacters = strlen(newString[0]);  // This gives the number of characters in the 1st string without the \0 Terminating character.
        
            #if defined(DEBUG1)
                Serial.println(F("Rectangle::draw function ..."));
                Serial.print(F("This is the X Cursor Position: "));
                Serial.println( _XCenterPosition - ( ( _fontWidth * _numberOfCharacters ) / 2 ) );
                Serial.print(F("This is the Y Cursor Position: "));
                Serial.println( _YCenterPosition - ( ( _fontHeight / 2 ) * ( _numberOfTextLines) ) );
                Serial.println(F("Setting Cursor Position...\n"));
            #endif
        
            tft.setCursor( ( (int16_t)_XCenterPosition - ( ( ( _fontWidth * _numberOfCharacters ) / 2 ) ) ), ( (int16_t)_YCenterPosition - ( ( _fontHeight / 2 ) * ( _numberOfTextLines ) ) ) );  // Centering the Text in the X Position, Y Position 1 1/2 character heights above the center of the box.
        
            #if defined(DEBUG1)
                Serial.println(F("Rectangle::draw function ..."));
                Serial.print(F("This is the number of characters to print: "));
                Serial.println(_numberOfCharacters);
                Serial.print(F("This is the contents of newString: "));
                Serial.println(newString[0]);
                Serial.println(F("Now printing to the TFT...\n"));
            #endif
        
            tft.print(newString[0]);  // Prints a string pointed to by "newString[0]".
        
            for(i = 1; i < ctr; i++)  // Now print the following strings to the TFT.
                {
                    #if defined(DEBUG1)
                        Serial.println(F("Rectangle::draw function ..."));
                        Serial.println(F("Getting the string length from strlen..."));
                        Serial.println(F(""));
                    #endif
            
                    _numberOfCharacters = strlen(newString[i]);  // This gives the number of characters in the string without the \0 Terminating character.
            
                    #if defined(DEBUG1)
                        Serial.println(F("Rectangle::draw function ..."));
                        Serial.print(F("This is the number of characters to print: "));
                        Serial.println(_numberOfCharacters);
                        Serial.print(F("This is the contents of newString: "));
                        Serial.println(newString[i]);
                        Serial.println(F("Now printing to the TFT...\n"));
                    #endif
            
                    tft.setCursor( ( (int16_t)_XCenterPosition - ( ( _fontWidth * _numberOfCharacters ) / 2 ) ), (int16_t)( _YCenterPosition - ( ( _fontHeight / 2 ) * _numberOfTextLines ) ) + (_fontHeight * i) );  // Move to next line down.
            
                    tft.print(newString[i]);  // Prints a string pointed to by "newString[i]".
            
                }
        
        }
    else if (_numberOfTextLines == 1 )
        {
            _numberOfCharacters = strlen(_text);  // This gives the number of characters in the string without the \0 Terminating character.
            tft.setCursor( ( (int16_t)_XCenterPosition - ( ( ( _fontWidth * _numberOfCharacters ) / 2 ) ) ), ( (int16_t)_YCenterPosition - ( _fontHeight / 2) ) );  // Centering the Text in the box.
            tft.print(_text);
        }
}


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
*                                               Default Constructor
* This Class creates a Round Rectangle.
* Needs:
*   Nothing, it uses default info.
* Returns:
*   A Round Rectangle Object.
*******************************************************************************************************************/
RndRectangle::RndRectangle() : _text((char*)"RndRect")
{
    _beginningXYPositions.xPosition = 359;
    _beginningXYPositions.yPosition = 214;
    _width_height.width = 80;
    _width_height.height = 50;
    
    _radius = 8;
    
    _fontSize = (RA8875tsize)1;
    _numberOfTextLines = 0;  // This means no text will be drawn in the Rectangle.
    _textcolor = WHITE;

    _fillcolor = WHITE;
    _bordercolor = BLACK;
    
    #if defined(DEBUG2)
        Serial.println(F("RndRectangle::RndRectangle Default Constructor..."));
        Serial.print(F("The is the X-Y Position of the RndRectangle to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the Width & Height of the RndRectangle to be drawn: "));
        Serial.print(_width_height.width);
        Serial.print(F(" , "));
        Serial.println(_width_height.height);
    
        Serial.print(F("The is the radius of the RndRectangle to be drawn: "));
        Serial.println(_radius,HEX);
    
        Serial.print(F("The is the border color of the RndRectangle to be drawn: "));
        Serial.println(_bordercolor,HEX);
        Serial.print(F("The is the fill color of the RndRectangle to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the RndRectangle to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the RndRectangle: "));
        Serial.println(_fontSize);  // This prints the font size.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the RndRectangle Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the RndRectangle: "));
        Serial.println(_text);  // This prints the character array.
        Serial.println(F(""));
    #endif
    
}

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
RndRectangle::RndRectangle(char *text, RA8875tsize fontSize, Colors textColor, uint8_t numberOfTextLines, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t Width, int16_t Height, int16_t radius, Colors fillColor, Colors borderColor)
{
    if ( (Width == 0) || (Height == 0) )
        {
            _width_height.width = 100;
            _width_height.height = 50;
        }
    
    _beginningXYPositions.xPosition = setBeginningXPosition;
    _beginningXYPositions.yPosition = setBeginningYPosition;
    
    _width_height.width = Width;
    _width_height.height = Height;
    
    _radius = radius;
    
    _textcolor = textColor;
    
    if ( ( fontSize > 2 ) || ( fontSize < 0) )
        {
            _fontSize = (RA8875tsize)0;
        }
    else
        {
            _fontSize = fontSize;
        }
    
    _numberOfTextLines = numberOfTextLines;
    
    _text = text;
    
    _fillcolor = fillColor;
    _bordercolor = borderColor;
    
    #if defined(DEBUG2)
        Serial.println(F("RndRectangle::RndRectangle OverLoad Constructor..."));
        Serial.print(F("The is the X-Y Position of the RndRectangle to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the Width & Height of the RndRectangle to be drawn: "));
        Serial.print(_width_height.width);
        Serial.print(F(" , "));
        Serial.println(_width_height.height);
    
        Serial.print(F("The is the radius of the RndRectangle corners to be drawn: "));
        Serial.println(_radius,HEX);
    
        Serial.print(F("The is the border color of the RndRectangle to be drawn: "));
        Serial.println(_bordercolor,HEX);
    
        Serial.print(F("The is the fill color of the RndRectangle to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the RndRectangle to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the RndRectangle: "));
        Serial.println(_fontSize);  // This prints the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the RndRectangle Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the RndRectangle: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
    
}

/*******************************************************************************************************************
*                                               Destructor
*******************************************************************************************************************/
RndRectangle::~RndRectangle(){}

/*******************************************************************************************************************
 *
*******************************************************************************************************************/
void RndRectangle::setRandomProperties(int32_t min, int32_t max)
{
    _width_height.width = (uint32_t)random (5, 150);
    _width_height.height = (uint32_t)random (5, 150);
    Shape::setRandomProperties(min, ( max - (_width_height.width) ) );
}

/*******************************************************************************************************************
* This function actually draws the Round Rectangle on the TFT.
*******************************************************************************************************************/
void RndRectangle::draw()
{
    char newString[5][20];
    int i,j,ctr;
    int chararrylen = strlen(_text);
    
    #if defined(DEBUG1)
        Serial.print(F("RndRectangle::draw function..."));
        Serial.print(F("The is the X-Y Position of the RndRectangle to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the Width & Height of the RndRectangle to be drawn: "));
        Serial.print(_width_height.width);
        Serial.print(F(" , "));
        Serial.println(_width_height.height);
    
        Serial.print(F("The is the radius of the RndRectangle corners to be drawn: "));
        Serial.println(_radius,HEX);
    
        Serial.print(F("The is the border color of the RndRectangle to be drawn: "));
        Serial.println(_bordercolor,HEX);
    
        Serial.print(F("The is the fill color of the RndRectangle to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the ectangle to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Rectangle: "));
        Serial.println(_fontSize);  // This only prints the first character of the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Rectangle Vertically: "));
        Serial.println(_numberOfTextLines);  // This only prints the first character of the character array.
    
        Serial.print(F("This is the size of the character array to be drawn in the Rectangle: "));
        Serial.println(chararrylen);
    
        Serial.print(F("This is the Text to be drawn in the Button: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
    
    if (_bordercolor != BLACK)
        {
            #if defined(DEBUG1)
                Serial.println(F("RndRectangle::draw function ..."));
                Serial.println(F("Border Color is not BLACK! (0000)"));
                Serial.println(F("Drawing RndRectangle"));
                Serial.println(F(""));
            #endif
            tft.drawRoundRect(_beginningXYPositions.xPosition, _beginningXYPositions.yPosition, _width_height.width, _width_height.height, _radius, _bordercolor);  //ok
        }
    tft.fillRoundRect( (_beginningXYPositions.xPosition + 2), (_beginningXYPositions.yPosition + 2), (_width_height.width - 4), (_width_height.height - 4), _radius, _fillcolor);
    
    if (_numberOfTextLines == 0)
        {
            return;
        }
    
    #if defined(DEBUG1)
        Serial.println(F("RndRectangle::draw function ..."));
        Serial.println(F("Begin drawing the text in the Button"));
        Serial.println(F(""));
    #endif
    
    tft.setFontScale(_fontSize);  // This sets the font size (0,1,2,3) passed into this class.
    
    // Need to find the Center of the figure to be drawn.
    _XCenterPosition = ( ( _beginningXYPositions.xPosition + ( _width_height.width / 2 ) ) );
    
    _fontWidth = tft.getFontWidth();  // This is the width of each character in pixels.
    
    _YCenterPosition = ( ( _beginningXYPositions.yPosition + ( _width_height.height / 2 ) ) );
    
    _fontHeight = tft.getFontHeight();  // This is the height of each character in pixels.
    
    /// Should calculate the Text width and height to make sure it will fit inside box.
    
    #if defined(DEBUG1)
        Serial.println(F("RndRectangle::draw function ..."));
        Serial.print(F("_fontWidth from tft.getFontWidth: "));
        Serial.println(_fontWidth);
        Serial.print(F("_fontWidth from tft.getFontHeight: "));
        Serial.println(_fontHeight);
        Serial.print(F("_XCenterPosition: "));
        Serial.println(_XCenterPosition);
        Serial.print(F("_YCenterPosition: "));
        Serial.println(_YCenterPosition);
        Serial.println(F(""));
    #endif
    
    tft.setTextColor(_textcolor);  //set text color (backgroud will be transparent)
    
    #if defined(DEBUG1)
        Serial.println(F("RndRectangle::draw function ..."));
        Serial.println(F("Finished setting the Text Color..."));
        Serial.print(F("Now checking for the number of lines of text: "));
        Serial.println(_numberOfTextLines);
        Serial.println(F(""));
    #endif
    
    if ( (_numberOfTextLines > 1 ) && (_numberOfTextLines < 6) )
        {
            #if defined(DEBUG1)
                Serial.println(F("RndRectangle::draw function ..."));
                Serial.println(F("if (_numberOfTextLines > 1)..."));
                Serial.print(F("This is the text string to scan: "));
                Serial.println(_text);
                Serial.print(F("This is the size of the text string to scan: "));
                Serial.println(chararrylen);
                Serial.println(F("Parsing the _text string for a space deliminating character and making separate char strings..."));
                Serial.println(F(""));
            #endif
        
            j=0;
            ctr=0;
        
            for(i = 0; i <= chararrylen; i++)
                {
                    // If space or NULL found, put NULL into newString[ctr]
                    if( (_text[i]==' ') || (_text[i]=='\0') )
                        {
                            newString[ctr][j] = '\0';
                            ctr++;  //for next word
                            j=0;    //for next word, init index to 0
                        }
                    else
                        {
                            newString[ctr][j] = _text[i];  // Move text to the array[ctr].
                            j++;
                        }
                }
        
            _numberOfCharacters = strlen(newString[0]);  // This gives the number of characters in the 1st string without the \0 Terminating character.
        
            #if defined(DEBUG1)
                Serial.println(F("RndRectangle::draw function ..."));
                Serial.print(F("This is the X Cursor Position: "));
                Serial.println( _XCenterPosition - ( ( _fontWidth * _numberOfCharacters ) / 2 ) );
                Serial.print(F("This is the Y Cursor Position: "));
                Serial.println( _YCenterPosition - ( ( _fontHeight / 2 ) * ( _numberOfTextLines) ) );
                Serial.println(F("Setting Cursor Position...\n"));
            #endif
        
            tft.setCursor( ( (int16_t)_XCenterPosition - ( ( ( _fontWidth * _numberOfCharacters ) / 2 ) ) ), ( (int16_t)_YCenterPosition - ( ( _fontHeight / 2 ) * ( _numberOfTextLines ) ) ) );  // Centering the Text in the X Position, Y Position 1 1/2 character heights above the center of the box.
        
            #if defined(DEBUG1)
                Serial.println(F("RndRectangle::draw function ..."));
                Serial.print(F("This is the number of characters to print: "));
                Serial.println(_numberOfCharacters);
                Serial.print(F("This is the contents of newString: "));
                Serial.println(newString[0]);
                Serial.println(F("Now printing to the TFT...\n"));
            #endif
        
            tft.print(newString[0]);  // Prints a string pointed to by "newString[0]".
        
            for(i = 1; i < ctr; i++)  // Now print the following strings to the TFT.
                {
                    #if defined(DEBUG1)
                        Serial.println(F("RndRectangle::draw function ..."));
                        Serial.println(F("Getting the string length from strlen..."));
                        Serial.println(F(""));
                    #endif
            
                    _numberOfCharacters = strlen(newString[i]);  // This gives the number of characters in the string without the \0 Terminating character.
            
                    #if defined(DEBUG1)
                        Serial.println(F("RndRectangle::draw function ..."));
                        Serial.print(F("This is the number of characters to print: "));
                        Serial.println(_numberOfCharacters);
                        Serial.print(F("This is the contents of newString: "));
                        Serial.println(newString[i]);
                        Serial.println(F("Now printing to the TFT...\n"));
                    #endif
            
                    tft.setCursor( ( (int16_t)_XCenterPosition - ( ( _fontWidth * _numberOfCharacters ) / 2 ) ), (int16_t)( _YCenterPosition - ( ( _fontHeight / 2 ) * _numberOfTextLines ) ) + (_fontHeight * i) );  // Move to next line down.
            
                    tft.print(newString[i]);  // Prints a string pointed to by "newString[i]".
            
                }
        
        }
    else if (_numberOfTextLines == 1 )
        {
            _numberOfCharacters = strlen(_text);  // This gives the number of characters in the string without the \0 Terminating character.
            tft.setCursor( ( (int16_t)_XCenterPosition - ( ( ( _fontWidth * _numberOfCharacters ) / 2 ) ) ), ( (int16_t)_YCenterPosition - ( _fontHeight / 2) ) );  // Centering the Text in the box.
            tft.print(_text);
        }
}


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
*                                       Default Constructor
* This Class draws a colored Triangle on the TFT Screen.  The position is set by code.
*
* Needs:
*   Nothing
* Returns:
*   A Triangle Object.
*******************************************************************************************************************/
Triangle::Triangle() : _text((char*)"Triangle")
{
    _XYPointsvertex1.xPosition = 200;
    _XYPointsvertex1.yPosition = 200;
    _XYPointsvertex2.xPosition = 400;
    _XYPointsvertex2.yPosition = 400;
    _XYPointsvertex3.xPosition = 650;
    _XYPointsvertex3.yPosition = 200;
    
    _fillcolor = YELLOW;
    
    _fontSize = (RA8875tsize)1;
    _numberOfTextLines = 0;  // This means no text will be drawn in the Rectangle.
    _textcolor = WHITE;
    
    #if defined(DEBUG2)
        Serial.print(F("Triangle::Triangle Default Constructor..."));
        Serial.print(F("The is the XYPointsvertex1.xPosition of the Triangle to be drawn: "));
        Serial.print(_XYPointsvertex1.xPosition);
        Serial.print(F(" , "));
        Serial.println(_XYPointsvertex1.yPosition);
    
        Serial.print(F("The is the XYPointsvertex2.xPosition of the Triangle to be drawn: "));
        Serial.print(_XYPointsvertex2.xPosition);
        Serial.print(F(" , "));
        Serial.println(_XYPointsvertex2.yPosition);

        Serial.print(F("The is the XYPointsvertex3.xPosition of the Triangle to be drawn: "));
        Serial.print(_XYPointsvertex3.xPosition);
        Serial.print(F(" , "));
        Serial.println(_XYPointsvertex3.yPosition);
    
        Serial.print(F("The is the fill color of the Triangle to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Triangle to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Triangle: "));
        Serial.println(_fontSize);  // This prints the font size.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Triangle Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Triangle: "));
        Serial.println(_text);  // This prints the character array.
        Serial.println(F(""));
#endif
    
}

/*******************************************************************************************************************
*                                       Over Load Constructor
* This Class draws a colored Triangle on the TFT Screen.  The 3 points of the triangle you specify.
*
* Needs:
*   text:                   A pointer to some text.
*   fontSize:               The RA8875 font size.
*   textColor:              The RA8875 text Color.
*   numberOfTextLines:      The number of lines of text to put into the Circle, Tif zero no text will be drawn in the Triangle.
*   Three vertexes:         v1 x & y, v2 x & y, v3 x & y.
*   fillColor:              A Fill Color for the Triangle.
* Returns:
*   A Triangle Object.
*******************************************************************************************************************/
Triangle::Triangle(char *text, RA8875tsize fontSize, Colors textColor, uint8_t numberOfTextLines, int16_t vertex1XPosition, int16_t vertex1YPosition, int16_t vertex2XPosition, int16_t vertex2YPosition, int16_t vertex3XPosition, int16_t vertex3YPosition , Colors fillColor)
{
        _XYPointsvertex1.xPosition = vertex1XPosition;
        _XYPointsvertex1.yPosition = vertex1YPosition;
        
        _XYPointsvertex2.xPosition = vertex2XPosition;
        _XYPointsvertex2.yPosition = vertex2YPosition;
        
        _XYPointsvertex3.xPosition = vertex3XPosition;
        _XYPointsvertex3.yPosition = vertex3YPosition;
        
        _fillcolor = fillColor;
    
        _textcolor = textColor;
    
        if ( ( fontSize > 2 ) || ( fontSize < 0) )
            {
                _fontSize = (RA8875tsize)0;
            }
        else
            {
                _fontSize = fontSize;
            }
    
        _numberOfTextLines = numberOfTextLines;
    
        _text = text;
    
    #if defined(DEBUG2)
        Serial.print(F("Triangle::Triangle Over Load Constructor..."));
        Serial.print(F("The is the XYPointsvertex1.xPosition of the Triangle to be drawn: "));
        Serial.print(_XYPointsvertex1.xPosition);
        Serial.print(F(" , "));
        Serial.println(_XYPointsvertex1.yPosition);
    
        Serial.print(F("The is the XYPointsvertex2.xPosition of the Triangle to be drawn: "));
        Serial.print(_XYPointsvertex2.xPosition);
        Serial.print(F(" , "));
        Serial.println(_XYPointsvertex2.yPosition);
    
        Serial.print(F("The is the XYPointsvertex3.xPosition of the Triangle to be drawn: "));
        Serial.print(_XYPointsvertex3.xPosition);
        Serial.print(F(" , "));
        Serial.println(_XYPointsvertex3.yPosition);
    
        Serial.print(F("The is the fill color of the Triangle to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Triangle to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Triangle: "));
        Serial.println(_fontSize);  // This prints the font size.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Triangle Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Triangle: "));
        Serial.println(_text);  // This prints the character array.
        Serial.println(F(""));
    #endif
    
}

/*******************************************************************************************************************
*                                               Destructor
*******************************************************************************************************************/
Triangle::~Triangle(){}

/*******************************************************************************************************************
* This function set Random X & Y properties for the triangle.
*******************************************************************************************************************/
void Triangle::setRandomProperties(int32_t min, int32_t max)
{
    _XYPointsvertex1.xPosition = (uint32_t) random (min, 800);
    _XYPointsvertex1.yPosition = random (0, 450);
    
    _XYPointsvertex2.xPosition = (uint32_t)random (min, 800);
    _XYPointsvertex2.yPosition = (uint32_t)random (0, 450);
    
    _XYPointsvertex3.xPosition = (uint32_t)random (min, 800);
    _XYPointsvertex3.yPosition = (uint32_t)random (0, 450);
    
    _fillcolor = getRandomColor();
}

/*******************************************************************************************************************
* This function actually draws the Triangle on the TFT.
*******************************************************************************************************************/
void Triangle::draw()
{
    char newString[5][20];
    int i,j,ctr;
    int chararrylen = strlen(_text);
    
    #if defined(DEBUG1)
        Serial.print(F("Triangle::Draw..."));
        Serial.print(F("The is the XYPointsvertex1.xPosition of the Triangle to be drawn: "));
        Serial.print(_XYPointsvertex1.xPosition);
        Serial.print(F(" , "));
        Serial.println(_XYPointsvertex1.yPosition);
    
        Serial.print(F("The is the XYPointsvertex2.xPosition of the Triangle to be drawn: "));
        Serial.print(_XYPointsvertex2.xPosition);
        Serial.print(F(" , "));
        Serial.println(_XYPointsvertex2.yPosition);
    
        Serial.print(F("The is the XYPointsvertex3.xPosition of the Triangle to be drawn: "));
        Serial.print(_XYPointsvertex3.xPosition);
        Serial.print(F(" , "));
        Serial.println(_XYPointsvertex3.yPosition);
    
        Serial.print(F("The is the fill color of the Triangle to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Triangle to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Triangle: "));
        Serial.println(_fontSize);  // This prints the font size.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Triangle Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Triangle: "));
        Serial.println(_text);  // This prints the character array.
        Serial.println(F(""));
    #endif
    
    tft.fillTriangle(_XYPointsvertex1.xPosition, _XYPointsvertex1.yPosition, _XYPointsvertex2.xPosition, _XYPointsvertex2.yPosition, _XYPointsvertex3.xPosition, _XYPointsvertex3.yPosition, _fillcolor);
    
    if (_numberOfTextLines == 0)
        {
            return;
        }
    
    #if defined(DEBUG1)
        Serial.println(F("Triangle::draw function ..."));
        Serial.println(F("Begin drawing the text in the Button"));
        Serial.println(F(""));
    #endif
    
    tft.setFontScale(_fontSize);  // This sets the font size (0,1,2,3) passed into this class.
    
    /// Need to find the Center of the figure to be drawn.
    _XCenterPosition = ( (_XYPointsvertex1.xPosition + _XYPointsvertex2.xPosition + _XYPointsvertex3.xPosition) / 3 );
    
    _fontWidth = tft.getFontWidth();  // This is the width of each character in pixels.
    
    _YCenterPosition = ( (_XYPointsvertex1.yPosition + _XYPointsvertex2.yPosition + _XYPointsvertex3.yPosition) / 3 );
    
    _fontHeight = tft.getFontHeight();  // This is the height of each character in pixels.
    
    /// Should calculate the Text width and height to make sure it will fit inside box.
    
    #if defined(DEBUG1)
        Serial.println(F("Triangle::draw function ..."));
        Serial.print(F("_fontWidth from tft.getFontWidth: "));
        Serial.println(_fontWidth);
        Serial.print(F("_fontWidth from tft.getFontHeight: "));
        Serial.println(_fontHeight);
        Serial.print(F("_XCenterPosition: "));
        Serial.println(_XCenterPosition);
        Serial.print(F("_YCenterPosition: "));
        Serial.println(_YCenterPosition);
        Serial.println(F(""));
    #endif
    
    tft.setTextColor(_textcolor);  //set text color (backgroud will be transparent)
    
    #if defined(DEBUG1)
        Serial.println(F("Triangle::draw function ..."));
        Serial.println(F("Finished setting the Text Color..."));
        Serial.print(F("Now checking for the number of lines of text: "));
        Serial.println(_numberOfTextLines);
        Serial.println(F(""));
    #endif
    
    if ( (_numberOfTextLines > 1 ) && (_numberOfTextLines < 6) )
        {
            #if defined(DEBUG1)
                Serial.println(F("Triangle::draw function ..."));
                Serial.println(F("if (_numberOfTextLines > 1)..."));
                Serial.print(F("This is the text string to scan: "));
                Serial.println(_text);
                Serial.print(F("This is the size of the text string to scan: "));
                Serial.println(chararrylen);
                Serial.println(F("Parsing the _text string for a space deliminating character and making separate char strings..."));
                Serial.println(F(""));
            #endif
        
            j=0;
            ctr=0;
        
            for(i = 0; i <= chararrylen; i++)
                {
                    // If space or NULL found, put NULL into newString[ctr]
                    if( (_text[i]==' ') || (_text[i]=='\0') )
                        {
                            newString[ctr][j] = '\0';
                            ctr++;  //for next word
                            j=0;    //for next word, init index to 0
                        }
                    else
                        {
                            newString[ctr][j] = _text[i];  // Move text to the array[ctr].
                            j++;
                        }
                }
        
            _numberOfCharacters = strlen(newString[0]);  // This gives the number of characters in the 1st string without the \0 Terminating character.
        
            #if defined(DEBUG1)
                Serial.println(F("Triangle::draw function ..."));
                Serial.print(F("This is the X Cursor Position: "));
                Serial.println( _XCenterPosition - ( ( _fontWidth * _numberOfCharacters ) / 2 ) );
                Serial.print(F("This is the Y Cursor Position: "));
                Serial.println( _YCenterPosition - ( ( _fontHeight / 2 ) * ( _numberOfTextLines) ) );
                Serial.println(F("Setting Cursor Position...\n"));
            #endif
        
            tft.setCursor( ( (int16_t)_XCenterPosition - ( ( ( _fontWidth * _numberOfCharacters ) / 2 ) ) ), ( (int16_t)_YCenterPosition - ( ( _fontHeight / 2 ) * ( _numberOfTextLines ) ) ) );  // Centering the Text in the X Position, Y Position 1 1/2 character heights above the center of the box.
        
            #if defined(DEBUG1)
                Serial.println(F("Triangle::draw function ..."));
                Serial.print(F("This is the number of characters to print: "));
                Serial.println(_numberOfCharacters);
                Serial.print(F("This is the contents of newString: "));
                Serial.println(newString[0]);
                Serial.println(F("Now printing to the TFT...\n"));
            #endif
        
            tft.print(newString[0]);  // Prints a string pointed to by "newString[0]".
        
            for(i = 1; i < ctr; i++)  // Now print the following strings to the TFT.
                {
                    #if defined(DEBUG1)
                        Serial.println(F("Triangle::draw function ..."));
                        Serial.println(F("Getting the string length from strlen..."));
                        Serial.println(F(""));
                    #endif
            
                    _numberOfCharacters = strlen(newString[i]);  // This gives the number of characters in the string without the \0 Terminating character.
            
                    #if defined(DEBUG1)
                        Serial.println(F("Triangle::draw function ..."));
                        Serial.print(F("This is the number of characters to print: "));
                        Serial.println(_numberOfCharacters);
                        Serial.print(F("This is the contents of newString: "));
                        Serial.println(newString[i]);
                        Serial.println(F("Now printing to the TFT...\n"));
                    #endif
            
                    tft.setCursor( ( (int16_t)_XCenterPosition - ( ( _fontWidth * _numberOfCharacters ) / 2 ) ), (int16_t)( _YCenterPosition - ( ( _fontHeight / 2 ) * _numberOfTextLines ) ) + (_fontHeight * i) );  // Move to next line down.
            
                    tft.print(newString[i]);  // Prints a string pointed to by "newString[i]".
            
                }
        
        }
    else if (_numberOfTextLines == 1 )
        {
            _numberOfCharacters = strlen(_text);  // This gives the number of characters in the string without the \0 Terminating character.
            tft.setCursor( ( (int16_t)_XCenterPosition - ( ( ( _fontWidth * _numberOfCharacters ) / 2 ) ) ), ( (int16_t)_YCenterPosition - ( _fontHeight / 2) ) );  // Centering the Text in the box.
            tft.print(_text);
        }
}




/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*******************************************************************************************************************
 *                                          Default Constructor
 * This Class draws a colored Button of the shape you choose on the TFT Screen.  X and Y position is set by code.
 *
 * Needs:
 *   Nothing
 * Returns:
 *   A Button Object.
 *******************************************************************************************************************/
SelectableShapeButton::SelectableShapeButton() : _text((char*)"Button")
{
    _buttonShape = RECTANGLE;
    _fillcolor = WHITE;
    _bordercolor = BLACK;
    _textcolor = WHITE;
    _beginningXYPositions.xPosition = 50;
    _beginningXYPositions.yPosition = 50;
    _width_height.width = 80;
    _width_height.height = 50;
    _radius = 8;
    _fontSize = (RA8875tsize)0;
    _numberOfTextLines = 1;
    
    #if defined(DEBUG2)
        Serial.println(F("Button::Button Default Constructor..."));
        Serial.print(F("The is the X-Y Position of the Button to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the Width & Height of the Button to be drawn: "));
        Serial.print(_width_height.width);
        Serial.print(F(" , "));
        Serial.println(_width_height.height);
    
        Serial.print(F("The is the text color of the Button to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("The is the border color of the Button to be drawn: "));
        Serial.println(_bordercolor,HEX);
    
        Serial.print(F("The is the fill color of the Button to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Button: "));
        Serial.println(_fontSize);  // This prints the font size.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Button Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Button: "));
        Serial.println(_text);  // This prints the character array.
        Serial.println(F(""));
    #endif
    
}


/*******************************************************************************************************************
*                                               Rectangle Over Ride Constructor
* This constructor creates a colored Button of the shape you choose, where you specify, the size you specify, and the colors you specify.
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
*   A Button Object.
*******************************************************************************************************************/
SelectableShapeButton::SelectableShapeButton(Figure_types buttonShape, char *text, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t Width, int16_t Height, RA8875tsize fontSize, uint8_t numberOfTextLines, Colors textColor, Colors fillColor, Colors borderColor)
{
    _buttonShape = buttonShape;
    
    if ( (Width == 0) || (Height == 0) )
        {
            _width_height.width = 80;
            _width_height.height = 50;
        }
    _width_height.width = Width;
    _width_height.height = Height;
    
    _beginningXYPositions.xPosition = setBeginningXPosition;
    _beginningXYPositions.yPosition = setBeginningYPosition;
    
    _fillcolor = fillColor;
    _bordercolor = borderColor;
    _textcolor = textColor;
    
    if ( ( fontSize > 2 ) || ( fontSize < 0) )
        {
            _fontSize = (RA8875tsize)0;
        }
    else
        {
            _fontSize = fontSize;
        }
    
    _numberOfTextLines = numberOfTextLines;
    
    _text = text;
    
    #if defined(DEBUG2)
        Serial.println(F("SelectableShapeButton::SelectableShapeButton Rectangle Button OverLoad Constructor..."));

        Serial.print(F("This is the Shape of the Button to be drawn: "));
        Serial.println(_buttonShape);
    
        Serial.print(F("This is the X-Y Position of the Rectangle Button to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("This is the Width & Height of the Rectangle Button to be drawn: "));
        Serial.print(_width_height.width);
        Serial.print(F(" , "));
        Serial.println(_width_height.height);
    
        Serial.print(F("This is the border color of the Rectangle Button to be drawn: "));
        Serial.println(_bordercolor,HEX);
        Serial.print(F("The is the fill color of the Rectangle Button to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Rectangle Button to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Rectangle Button: "));
        Serial.println(_fontSize);  // This prints the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Rectangle Button Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Rectangle Button: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
    
}


/*******************************************************************************************************************
*                                               Round Rectangle Over Ride Constructor
* This constructor creates a colored Button of the shape you choose, where you specify, the size you specify, and the colors you specify.
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
SelectableShapeButton::SelectableShapeButton(Figure_types buttonShape, char *text, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t Width, int16_t Height, int16_t radius, RA8875tsize fontSize, uint8_t numberOfTextLines, Colors textColor, Colors fillColor, Colors borderColor)
{
    _buttonShape = buttonShape;
    
    if ( (Width == 0) || (Height == 0) )
        {
            _width_height.width = 80;
            _width_height.height = 50;
        }
    _width_height.width = Width;
    _width_height.height = Height;
    
    _beginningXYPositions.xPosition = setBeginningXPosition;
    _beginningXYPositions.yPosition = setBeginningYPosition;
    
    _radius = radius;  // This is only used for the round rectangle.
    
    _fillcolor = fillColor;
    _bordercolor = borderColor;
    _textcolor = textColor;
    
    if ( ( fontSize > 2 ) || ( fontSize < 0) )
        {
            _fontSize = (RA8875tsize)0;
        }
    else
        {
            _fontSize = fontSize;
        }
    
    _numberOfTextLines = numberOfTextLines;
    
    _text = text;
    
    #if defined(DEBUG2)
        Serial.println(F("SelectableShapeButton::RndRectangle Button OverLoad Constructor..."));

        Serial.print(F("This is the Shape of the RndRectangle Button to be drawn: "));
        Serial.println(_buttonShape);
    
        Serial.print(F("This is the X-Y Position of the RndRectangle Button to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("This is the Width & Height of the RndRectangle Button to be drawn: "));
        Serial.print(_width_height.width);
        Serial.print(F(" , "));
        Serial.println(_width_height.height);
    
        Serial.print(F("The is the Radius of the RndRectangle Button to be drawn: "));
        Serial.println(_radius);
    
        Serial.print(F("This is the border color of the RndRectangle Button to be drawn: "));
        Serial.println(_bordercolor,HEX);
        Serial.print(F("The is the fill color of the RndRectangle Button to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the RndRectangle Button to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the RndRectangle Button: "));
        Serial.println(_fontSize);  // This prints the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the RndRectangle Button Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the RndRectangle Button: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
    
}


/*******************************************************************************************************************
*                                               Circle Over Ride Constructor
* This constructor creates a colored Button of the shape you choose, where you specify, the size you specify, and the colors you specify.
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
SelectableShapeButton::SelectableShapeButton(Figure_types buttonShape, char *text, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t radius, RA8875tsize fontSize, uint8_t numberOfTextLines, Colors textColor, Colors fillColor, Colors borderColor)
{
    _buttonShape = buttonShape;
    
    _beginningXYPositions.xPosition = setBeginningXPosition;
    _beginningXYPositions.yPosition = setBeginningYPosition;
    
    if ( (radius < 3) || ( radius > ( (X_CONST / 2) - 1 ) ) )
        {
            #if defined(DEBUG1)
                Serial.print(F("Circle::Circle OverLoad Constructor..."));
                Serial.print(F("Circle::Circle Constructor : if ( (radius < 1) || ( radius > ( (X_CONST / 2) - 1 ) ) ): "));
                Serial.println(_radius);
                Serial.println(F("Circle::Circle Constructor : Setting radius to default 40"));
                Serial.println(F(""));
            #endif
            _radius = 40;
        }
    
    _radius = radius;  // This is only used for the Circle and RndRectangle.
    
    _fillcolor = fillColor;
    _bordercolor = borderColor;
    _textcolor = textColor;
    
    if ( ( fontSize > 2 ) || ( fontSize < 0) )
        {
            _fontSize = (RA8875tsize)0;
        }
    else
        {
            _fontSize = fontSize;
        }
    
    _numberOfTextLines = numberOfTextLines;
    
    _text = text;
    
    #if defined(DEBUG2)
        Serial.println(F("SelectableShapeButton::Circle Button OverLoad Constructor..."));
        Serial.print(F("This is the Shape of the Circle Button to be drawn: "));
        Serial.println(_buttonShape);
    
        Serial.print(F("This is the X-Y Position of the Circle Button to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the Radius of the Circle Button to be drawn: "));
        Serial.println(_radius);
    
        Serial.print(F("This is the border color of the Circle Button to be drawn: "));
        Serial.println(_bordercolor,HEX);
        Serial.print(F("The is the fill color of the Circle Button to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Circle Button to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Circle Button: "));
        Serial.println(_fontSize);  // This prints the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Circle Button Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Circle Button: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
    
}


/*******************************************************************************************************************
*                                               Ellipse Over Ride Constructor
* This constructor creates a colored Button of the shape you choose, where you specify, the size you specify, and the colors you specify.
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
SelectableShapeButton::SelectableShapeButton(Figure_types buttonShape, char *text, int16_t setBeginningXPosition, int16_t setBeginningYPosition, RA8875tsize fontSize, int16_t longAxis, int16_t shortAxis, uint8_t numberOfTextLines, Colors textColor, Colors fillColor, Colors borderColor)
{
    _buttonShape = buttonShape;
    
    _beginningXYPositions.xPosition = setBeginningXPosition;
    _beginningXYPositions.yPosition = setBeginningYPosition;
    
    if ( (longAxis < 2) || ( longAxis > ( (X_CONST / 2) - 1 ) ) )
        {
            #if defined(DEBUG2)
                Serial.print(F("Ellipse OverLoad Constructor..."));
                Serial.print(F("if ( (longAxis < 2) || ( longAxis > ( (X_CONST / 2) - 1 ) ) ): "));
                Serial.println(longAxis);
                Serial.println(F("Setting longAxis to default 40"));
                Serial.println(F(""));
            #endif
            _LongLength_ShortLength.longAxisLength = 40;
        }
    else
        {
            _LongLength_ShortLength.longAxisLength = longAxis;
        }
    
    if ( (shortAxis < 2) || ( shortAxis > ( (Y_CONST / 2) - 1 ) ) )
        {
            #if defined(DEBUG2)
                Serial.print(F("Ellipse OverLoad Constructor..."));
                Serial.print(F("if ( (shortAxis < 2) || ( shortAxis > ( (Y_CONST / 2) - 1 ) ) ): "));
                Serial.println(shortAxis);
                Serial.println(F("Setting shortAxis to default 20"));
                Serial.println(F(""));
            #endif
            _LongLength_ShortLength.shortAxisLength = 20;
        }
    else
        {
            _LongLength_ShortLength.shortAxisLength = shortAxis;
        }

    _fillcolor = fillColor;
    _bordercolor = borderColor;
    _textcolor = textColor;
    
    if ( ( fontSize > 2 ) || ( fontSize < 0) )
        {
            _fontSize = (RA8875tsize)0;
        }
    else
        {
            _fontSize = fontSize;
        }
    
    _numberOfTextLines = numberOfTextLines;
    
    _text = text;
    
    #if defined(DEBUG2)
        Serial.println(F("Button::Button Ellipse OverLoad Constructor..."));
        Serial.print(F("This is the Shape of the Button to be drawn: "));
        Serial.println(_buttonShape);
    
        Serial.print(F("This is the X-Y Position of the Ellipse Button to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the longAxis of the Ellipse Button to be drawn: "));
        Serial.println(longAxis);
    
        Serial.print(F("The is the shortAxis of the Ellipse Button to be drawn: "));
        Serial.println(shortAxis);
    
        Serial.print(F("This is the border color of the Ellipse Button to be drawn: "));
        Serial.println(_bordercolor,HEX);
        Serial.print(F("The is the fill color of the Ellipse Button to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Ellipse Button to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Ellipse Button: "));
        Serial.println(_fontSize);  // This prints the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Ellipse Button Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Ellipse Button: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
    
}


/*******************************************************************************************************************
*                                               Triangle Over Ride Constructor
* This constructor creates a colored Button of the shape you choose, where you specify, the size you specify, and the colors you specify.
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
SelectableShapeButton::SelectableShapeButton(Figure_types buttonShape, char *text, int16_t vertex1XPosition, int16_t vertex1YPosition, int16_t vertex2XPosition, int16_t vertex2YPosition, int16_t vertex3XPosition, int16_t vertex3YPosition , RA8875tsize fontSize, uint8_t numberOfTextLines, Colors textColor, Colors fillColor)
{
    _buttonShape = buttonShape;
    
    _XYPointsvertex1.xPosition = vertex1XPosition;
    _XYPointsvertex1.yPosition = vertex1YPosition;
    
    _XYPointsvertex2.xPosition = vertex2XPosition;
    _XYPointsvertex2.yPosition = vertex2YPosition;
    
    _XYPointsvertex3.xPosition = vertex3XPosition;
    _XYPointsvertex3.yPosition = vertex3YPosition;
    
    _fillcolor = fillColor;
    _textcolor = textColor;
    
    if ( ( fontSize > 2 ) || ( fontSize < 0) )
        {
            _fontSize = (RA8875tsize)0;
        }
    else
        {
            _fontSize = fontSize;
        }
    
    _numberOfTextLines = numberOfTextLines;
    
    _text = text;
    
    #if defined(DEBUG2)
        Serial.println(F("SelectableShapeButton::SelectableShapeButton Triangle OverLoad Constructor..."));
        Serial.print(F("This is the Shape of the Button to be drawn: "));
        Serial.println(_buttonShape);
    
        Serial.print(F("The is the XYPointsvertex1.xPosition of the Triangle Button to be drawn: "));
        Serial.print(_XYPointsvertex1.xPosition);
        Serial.print(F(" , "));
        Serial.println(_XYPointsvertex1.yPosition);
    
        Serial.print(F("The is the XYPointsvertex2.xPosition of the Triangle Button to be drawn: "));
        Serial.print(_XYPointsvertex2.xPosition);
        Serial.print(F(" , "));
        Serial.println(_XYPointsvertex2.yPosition);
    
        Serial.print(F("The is the XYPointsvertex3.xPosition of the Triangle Button to be drawn: "));
        Serial.print(_XYPointsvertex3.xPosition);
        Serial.print(F(" , "));
        Serial.println(_XYPointsvertex3.yPosition);

        Serial.print(F("The is the fill color of the Triangle Button to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Triangle Button to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Triangle Button: "));
        Serial.println(_fontSize);  // This prints the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Triangle Button Vertically: "));
        Serial.println(_numberOfTextLines);  // This prints the Number of Lines of text in rows.
    
        Serial.print(F("This is the Text to be drawn in the Triangle Button: "));
        Serial.println(_text);
        Serial.println(F(""));
    #endif
    
}


/*******************************************************************************************************************
 *                                               Destructor
 *******************************************************************************************************************/
SelectableShapeButton::~SelectableShapeButton()
{
    delete newRectbutton;
    newRectbutton = NULL;
    
    delete newTributton;
    newTributton = NULL;
    
    delete newCirbutton;
    newCirbutton = NULL;
    
    delete newEllbutton;
    newEllbutton = NULL;
    
    delete newRRectbutton;
    newRRectbutton = NULL;
}


/*******************************************************************************************************************
 * This function actually draws the Button object on the TFT.
 *******************************************************************************************************************/
void SelectableShapeButton::draw()
{
    #if defined(DEBUG1)
        Serial.print(F("\nThis is the Shape of the Button to be drawn: "));
        Serial.println(_buttonShape);
    
        Serial.println(F("Button::draw function..."));
        Serial.print(F("The is the X-Y Position of the Button to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the Width & Height of the Button to be drawn: "));
        Serial.print(_width_height.width);
        Serial.print(F(" , "));
        Serial.println(_width_height.height);
    
        Serial.print(F("The is the border color of the Button to be drawn: "));
        Serial.println(_bordercolor,HEX);
        Serial.print(F("The is the fill color of the Button to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Button to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Button: "));
        Serial.println(_fontSize);  // This only prints the first character of the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Button Vertically: "));
        Serial.println(_numberOfTextLines);  // This only prints the first character of the character array.
    #endif
    
    // buttonShape: One of the enumerated shapes (TRIANGLE, RECTANGLE, CIRCLE, ELLIPSE, RNDRECTANGLE).
    switch (_buttonShape)
        {
            case RECTANGLE:
                newRectbutton = new Rectangle(_text, _fontSize, _textcolor, _numberOfTextLines, _beginningXYPositions.xPosition, _beginningXYPositions.yPosition, _width_height.width, _width_height.height, _fillcolor, _bordercolor);
                newRectbutton->draw();
                break;
                
            case TRIANGLE:
                newTributton = new Triangle(_text, _fontSize, _textcolor, _numberOfTextLines, _XYPointsvertex1.xPosition, _XYPointsvertex1.yPosition, _XYPointsvertex2.xPosition, _XYPointsvertex2.yPosition, _XYPointsvertex3.xPosition, _XYPointsvertex3.yPosition, _fillcolor);
                newTributton->draw();
                break;
                
            case CIRCLE:
                newCirbutton = new Circle(_text, _fontSize, _textcolor, _numberOfTextLines,  _beginningXYPositions.xPosition, _beginningXYPositions.yPosition, _radius, _textcolor, _bordercolor);
                newCirbutton->draw();
                break;
                
            case ELLIPSE:
                newEllbutton = new Ellipse(_text, _fontSize, _textcolor, _numberOfTextLines, _beginningXYPositions.xPosition, _beginningXYPositions.yPosition, _width_height.width, _width_height.height, _fillcolor, _bordercolor);
                newEllbutton->draw();
                break;
                
            case RNDRECTANGLE:
                newRRectbutton = new RndRectangle(_text, _fontSize, _textcolor, _numberOfTextLines, _beginningXYPositions.xPosition, _beginningXYPositions.yPosition, _width_height.width, _width_height.height, _radius, _fillcolor, _bordercolor);
                newRRectbutton->draw();
                break;
            
            default:
                break;
        }
    
}


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
On_Figure SelectableShapeButton::OnTouch(TouchScreenPoints_t pos)
{
    touch_on_figure = ERROR;
    
    #if defined(DEBUG1)
        Serial.println(F("\nThis is the SelectableShapeButton::OnTouch function... "));
        Serial.print(F("This is the Shape of the Button to be drawn: "));
        Serial.println(_buttonShape);
    
        Serial.println(F("Button::draw function..."));
        Serial.print(F("The is the X-Y Position of the Button to be drawn: "));
        Serial.print(_beginningXYPositions.xPosition);
        Serial.print(F(" , "));
        Serial.println(_beginningXYPositions.yPosition);
    
        Serial.print(F("The is the Width & Height of the Button to be drawn: "));
        Serial.print(_width_height.width);
        Serial.print(F(" , "));
        Serial.println(_width_height.height);
    
        Serial.print(F("The is the border color of the Button to be drawn: "));
        Serial.println(_bordercolor,HEX);
        Serial.print(F("The is the fill color of the Button to be drawn: "));
        Serial.println(_fillcolor,HEX);
    
        Serial.print(F("The is the text color of the Button to be drawn: "));
        Serial.println(_textcolor,HEX);
    
        Serial.print(F("This is the font size of the Text to be drawn in the Button: "));
        Serial.println(_fontSize);  // This only prints the first character of the character array.
    
        Serial.print(F("This is the number of lines of Text to be drawn in the Button Vertically: "));
        Serial.println(_numberOfTextLines);  // This only prints the first character of the character array.
    
        Serial.print(F("These are the Touch Screen Points submitted: "));
        Serial.print(pos.xPosition);
        Serial.print(F(" , "));
        Serial.println(pos.yPosition);
    #endif
    
    // Get position of  the button.
    if (_buttonShape == RECTANGLE)
        {
            rectangle_pos = newRectbutton->getPosition();
            if ( ( pos.xPosition >= rectangle_pos.xPosition ) && ( pos.xPosition <= (rectangle_pos.xPosition + _width_height.width) ) &&
                ( pos.yPosition >= rectangle_pos.yPosition ) && ( pos.yPosition <= (rectangle_pos.yPosition + _width_height.height) ) )
                {
                    touch_on_figure = ON_RECT;
                    return touch_on_figure;
                }
            else
                {
                    touch_on_figure = ERROR;
                    return touch_on_figure;
                }
        }
    else if (_buttonShape == TRIANGLE)
        {
            triangle_pos1 = newTributton->getVertex1Position();  //
            triangle_pos2 = newTributton->getVertex2Position();  //
            triangle_pos3 = newTributton->getVertex3Position();  //
            if( ( pos.xPosition >= triangle_pos1.xPosition ) && ( pos.xPosition <= (triangle_pos2.xPosition) ) &&
               ( pos.xPosition >= triangle_pos2.xPosition ) && ( pos.xPosition <= (triangle_pos3.xPosition) ) &&
               ( pos.xPosition >= triangle_pos1.xPosition ) && ( pos.xPosition <= (triangle_pos3.xPosition) ) &&
               ( pos.yPosition >= triangle_pos1.yPosition ) && ( pos.yPosition <= (triangle_pos2.yPosition) ) &&
               ( pos.yPosition >= triangle_pos2.yPosition ) && ( pos.yPosition <= (triangle_pos3.yPosition) ) &&
               ( pos.yPosition >= triangle_pos1.yPosition ) && ( pos.yPosition <= (triangle_pos3.yPosition) ) )
                {
                    touch_on_figure = ON_TRIANGLE;
                    return touch_on_figure;
                }
            else
                {
                    touch_on_figure = ERROR;
                    return touch_on_figure;
                }
        }
    else if ( _buttonShape == CIRCLE )
        {
            circle_pos = newCirbutton->getPosition();  //
            if( ( pos.xPosition >= circle_pos.xPosition ) && ( pos.xPosition <= (circle_pos.xPosition + _width_height.width) ) &&
               ( pos.yPosition >= circle_pos.yPosition ) && ( pos.yPosition <= (circle_pos.yPosition + _width_height.height) ) )
                {
                    touch_on_figure = ON_CIRCLE;
                    return touch_on_figure;
                }
            else
                {
                    touch_on_figure = ERROR;
                    return touch_on_figure;
                }
        }
    else if (_buttonShape == ELLIPSE)
        {
            ellipse_pos1 = newEllbutton->getPosition();  //
            ellipse_pos2 = newEllbutton->getEllLongShortLengths();  //
            if( ( ( pos.xPosition >= ellipse_pos1.xPosition ) && ( pos.xPosition <= (ellipse_pos1.xPosition + ellipse_pos2.longAxisLength) )
                 && ( pos.yPosition >= ellipse_pos1.yPosition ) && ( pos.yPosition <= (ellipse_pos1.yPosition + ellipse_pos2.shortAxisLength) ) )
               || ( ( pos.xPosition >= ellipse_pos1.xPosition ) && ( pos.xPosition <= (ellipse_pos1.xPosition + ellipse_pos2.shortAxisLength) )
                   && ( pos.yPosition >= ellipse_pos1.yPosition ) && ( pos.yPosition <= (ellipse_pos1.yPosition + ellipse_pos2.longAxisLength) ) ) )
                {
                    touch_on_figure = ON_ELLIPSE;
                    return touch_on_figure;
                }
            else
                {
                    touch_on_figure = ERROR;
                    return touch_on_figure;
                }
        }
    else if (_buttonShape == RNDRECTANGLE)
        {
            #if defined(DEBUG1)
                Serial.println(F("\nFound a Round Rectangle button shape...\n"));
            #endif
            rndrectangle_pos = newRRectbutton->getPosition();
            if( ( pos.xPosition >= rndrectangle_pos.xPosition ) && ( pos.xPosition <= (rndrectangle_pos.xPosition + _width_height.width) ) &&
               ( pos.yPosition >= rndrectangle_pos.yPosition ) && ( pos.yPosition <= (rndrectangle_pos.yPosition + _width_height.height) ) )
                {
                    touch_on_figure = ON_RNDRECT;
                    return touch_on_figure;
                }
            else
                {
                    touch_on_figure = ERROR;
                    return touch_on_figure;
                }
        }
    else
        {
            touch_on_figure = ERROR;
            return touch_on_figure;
        }
    return touch_on_figure;
}
