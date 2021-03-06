///
/// @mainpage	Master_Weather_Station
///
/// @details	This is the Master Weather Station Code
/// @n
/// @n
/// @n @a		Developed with [embedXcode+](https://embedXcode.weebly.com)
///
/// @author		freddie
/// @author		Kingston Co.
/// @date		2/9/19 3:31 PM
/// @version	<#version#>
///
/// @copyright	(c) freddie, 2019
/// @copyright	All rights reserved
///
/// @see		ReadMe.txt for references
///


///
/// @file		Master_Weather_Station.ino
/// @brief		Main sketch
///
/// @details	<#details#>
/// @n @a		Developed with [embedXcode+](https://embedXcode.weebly.com)
///
/// @author		freddie
/// @author		Kingston Co.
/// @date		2/9/19 3:31 PM
/// @version	<#version#>
///
/// @copyright	(c) freddie, 2019
/// @copyright	All rights reserved
///
/// @see		ReadMe.txt for references
/// @n
///


// Core library for code-sense - IDE-based
// !!! Help: http://bit.ly/2AdU7cu
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(ROBOTIS) // Robotis specific
#include "libpandora_types.h"
#include "pandora.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(LITTLEROBOTFRIENDS) // LittleRobotFriends specific
#include "LRF.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(REDBEARLAB) // RedBearLab specific
#include "Arduino.h"
#elif defined(RFDUINO) // RFduino specific
#include "Arduino.h"
#elif defined(SPARK) || defined(PARTICLE) // Particle / Spark specific
#include "application.h"
#elif defined(ESP8266) // ESP8266 specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/// Include application, user and local libraries
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/

#include "Master_Weather_Station.h"

/*******************************************************************************************************************
 *                                       Teensy (ARM) and Arduino (AVR) Data Types:
 ********************************************************************************************************************
 *
 * On 32-bit ARM systems like Teensy 3.x/LC:
 *
 * char, signed char, and unsigned char are 8 bits;
 * The plain char type is unsigned by default;
 * short and unsigned short are 16 bits;
 * int and unsigned int are 32 bits;
 * long and unsigned long are 32 bits;
 * pointers are 32 bits;
 * long long and unsigned long long are 64 bits;
 * float is 32 bits;
 * double is 64 bits;
 * long double is 64 bits.
 *
 *
 * On 8-bit AVR systems like Teensy 2.0, Arduino Uno, etc:
 *
 * char, signed char, and unsigned char are 8 bits;
 * The plain char type is signed by default;
 * short and unsigned short are 16 bits;
 * int and unsigned int are 16 bits;
 * pointers are 16 bits;
 * long and unsigned long are 32 bits;
 * long long and unsigned long long are 64 bits;
 * float is 32 bits;
 * double is 32 bits (this violates ISO C/C++);
 * long double is 32 bits (this violates ISO C/C++).
 *
 *******************************************************************************************************************/


/*******************************************************************************************************************
 *                                      Peet Bros wind instruments ANOMOMETER
 ********************************************************************************************************************
 *
 * Using interrupts for wind.
 *
 * Provides:
 * Wind speed and direction (true and apparent)
 *
 * Connections:
 * Wind speed on pin 2 - INT0 - yellow wire from the Anomometer.
 * Wind dir on pin 3 - INT1 - green wire from the Anomometer.
 * Reset on pin 4 (low -> reset, High -> run)
 *
 * The wind rotor turns anti-clockwise.
 *
 * In ULTIMETER Weather Stations, speed is determined by measuring the time interval between
 * two successive closures of the speed reed. Calibration is done as follows (RPS = revolutions
 * per second):
 * 0.010 < RPS < 3.229 (approximately 0.2 < MPH < 8.2):
 * windSpeedDur < 309693
 * MPH = -0.1095(RPS*RPS) + 2.9318(RPS) – 0.1412
 * KNOTS = -0.09515(RPS*RPS) + 2.5476(RPS) – 0.1226
 *
 * 3.230 < RPS < 54.362 (approximately 8.2 < MPH < 136.0):
 * windSpeedDur < 18395
 * MPH = 0.0052(RPS*RPS) + 2.1980(RPS) + 1.1091
 * KNTS = 0.0045(RPS*RPS) + 1.9099(RPS) + 0.9638
 *
 * 54.363 < RPS < 66.332 (approximately 136.0 < MPH < 181.5):
 *
 * MPH = 0.1104(RPS*RPS) – 9.5685(RPS) + 329.87
 * KNTS = 0.09593(RPS*RPS) – 8.3147(RPS) + 286.65
 *
 * Conversions used are:
 *   mph * 0.86897 = knots,
 *   mph * 1.6094 = kmph.
 *   mph * 0.48037 = m/s
 *
 *******************************************************************************************************************/

/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/// Prototypes
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/

// NONE

/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
///Define structures and classes
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/

/*******************************************************************************************************************
 * These variables are for the TimeZone functions.
 *******************************************************************************************************************/
bool updateGPSFlag = false;

// Australia Eastern Time Zone (Sydney, Melbourne)
TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, 660};    // UTC + 11 hours
TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, 600};    // UTC + 10 hours
Timezone ausET(aEDT, aEST);

// Moscow Standard Time (MSK, does not observe DST)
TimeChangeRule msk = {"MSK", Last, Sun, Mar, 1, 180};       // Russia Standard Time
Timezone tzMSK(msk);

// Eastern European Time (Athens, Kiev)
TimeChangeRule EEST = {"EEST", Last, Sun, Mar, 2, 180};     // Eastern European Summer Time
TimeChangeRule EET = {"EET ", Last, Sun, Oct, 3, 120};       // Eastern European Standard Time
Timezone EE(EEST, EET);

// Central European Time (Frankfurt, Paris, Rome)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
Timezone CE(CEST, CET);

// United Kingdom (London, Belfast)
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
Timezone UK(BST, GMT);

// UTC
TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};     // UTC
Timezone UTC(utcRule);

// US Eastern Time Zone (New York, Detroit)
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  // Eastern Daylight Time = UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   // Eastern Standard Time = UTC - 5 hours
Timezone usET(usEDT, usEST);

// US Central Time Zone (Chicago, Houston)
TimeChangeRule usCDT = {"CDT", Second, Sun, Mar, 2, -300};  // UTC - 5
TimeChangeRule usCST = {"CST", First, Sun, Nov, 2, -360};  // UTC - 6
Timezone usCT(usCDT, usCST);

// US Mountain Time Zone (Denver, Salt Lake City)
TimeChangeRule usMDT = {"MDT", Second, Sun, Mar, 2, -360};  // UTC - 6
TimeChangeRule usMST = {"MST", First, Sun, Nov, 2, -420};  // UTC - 7
Timezone usMT(usMDT, usMST);

// Arizona is US Mountain Time Zone but does not use DST
Timezone usAZ(usMST);  // UTC - 7

// US Pacific Time Zone (Las Vegas, Los Angeles)
TimeChangeRule usPDT = {"PDT", Second, Sun, Mar, 2, -420};  // UTC - 7
TimeChangeRule usPST = {"PST", First, Sun, Nov, 2, -480};  // UTC - 8
Timezone usPT(usPDT, usPST);

// Hawaii is Hawaiian Time Zone but does not use DST
TimeChangeRule usHST = {"HST", Last, Sun, Mar, 1,  -600};  // UTC - 10 Hawaiian Standard Time (no daylight saving time)
Timezone usHT(usHST);

Timezone* timezones[] = { &ausET, &tzMSK, &EE, &CE, &UK, &UTC, &usET, &usCT, &usMT, &usAZ, &usPT, &usHT };

Timezone* tz;      //pointer to the time zone, start with Pacific Time zone.
TimeChangeRule* tcr;            //pointer to the time change rule struct, use to get TZ abbrev
uint8_t tzIndex = 10;            //indexes the timezones[] array, start with PST.

bool UTCTimeCorrected = false;

/*******************************************************************************************************************
 * These variables are for the GPS Sensor Module functions.
 *******************************************************************************************************************/
#if defined(USINGGPS)
    /*******************************************************************************************************************
    *   This Class is the uBlox GPS receiver Class.
    *   Needs:
    *       Hardware serial port the uBlox GPS Receiver is connected to (1->3) on Teensy 3.2.
    * fix Type: 0: no fix, 1: dead reckoning only, 2: 2D-fix, 3: 3D-fix, 4: GNSS + dead reckoning combined, 5: time only fix
    *******************************************************************************************************************/
    UBLOX gps(GPShwSERIAL);

    gpsCompletionCodes gpsCodes;  // These are the Return codes from the GPS device to determine errors or good data.

    /*******************************************************************************************************************
    * This struct is for the uBlox data structure.
    *******************************************************************************************************************/
    gpsData uBloxData;

#endif

dotColorSelection statusDotColor = DOT_NONE;  // This varible holds the color of the RTC corrected by GPS Status Dot.

/*******************************************************************************************************************
 * These variables are for the DS18B20 Temp Sensors on the One Wire Bus.
 *******************************************************************************************************************/
#if defined(USINGTEMPSENSORS)
    uint8_t deviceCount = 0;
    uint8_t oneWireLocationOnTeensyPin = 21;
    uint8_t teensySearchForOneWireBusPin = 0;
    volatile bool freshTempsAvailableFlag = false;

    bool waterRelayON = false;  // The water relay is OFF initially.
    bool coolerONFlag = false;  // The Cooler Function is OFF initially.
    bool firstRun = false;      // This flag is to indicate the first time the Cooler function is used.

    /*******************************************************************************************************************
    *                           These Classes are for the One Wire Temp Sensors.
    *******************************************************************************************************************/
    OneWire oneWire(oneWireLocationOnTeensyPin);
    DallasTemperature tempSensors(&oneWire);

    /*******************************************************************************************************************
    * These struct(s) are for the DS18B20 Temp Sensors on the One Wire Bus.
    *******************************************************************************************************************/
    // Add room for 8 temp sensors on the bus.
    ds18b20DataStruct oneWireTemps[8];

#endif

/******************************************************************************************************************
 * These variables are for counting longer times than the 1, 2, 10, and 30 second intervals.
 *******************************************************************************************************************/
uint8_t twelveMinuteCount = 0;
uint16_t sixtyMinuteCount = 0;


/******************************************************************************************************************
 * These variables are for the days' highs and lows calculations which is then printed on the TFT.
 *******************************************************************************************************************/

float insideDailyHighTemp = 0;
float insideDailyLowTemp = 0;

float outsideDailyHighTemp = 0;
float outsideDailyLowTemp = 0;
bool firstOutsideReading = true;

/******************************************************************************************************************
 * These variables are for the buffers used to graph data on the TFT.
 *******************************************************************************************************************/
/*
 const uint16_t bufferDataSize1 = 4096;
 const uint16_t bufferDataSize2 = 4096;
 const uint16_t bufferDataSize3 = 4096;
 const uint16_t bufferDataSize4 = 4096;
 const uint16_t bufferSizes = 768;     // This is the buffer sizes in floats.
 
 Circular_Buffer<float, bufferSizes> circularBuffer1;
 Circular_Buffer<float, bufferSizes> circularBuffer2;
 Circular_Buffer<float, bufferSizes> circularBuffer3;
 Circular_Buffer<float, bufferSizes> circularBuffer4;
 
 bool layer2Enabled = false;
 bool ANDFunctionEnabled = false;
 bool ORFunctionEnabled = false;
 */

/*******************************************************************************************************************
 * These variables are for the Interrupt Driven Timers that makes everything happen.
 *******************************************************************************************************************/
volatile bool oneSecondFlag     = false;        // One Second Timer Interrupt Flag.
volatile bool twoSecondFlag     = false;        // Two Second Timer Interrupt Flag.
volatile bool tenSecondFlag     = false;        // Ten Second Timer Interrupt Flag.
volatile bool thirtySecondFlag  = false;        // Thirty Second Timer Interrupt Flag.


/*******************************************************************************************************************
*                           This struct has all the weather system data in it.
*******************************************************************************************************************/
weatherSensorsData  WeatherDataStruct;
bool slaveDataReceived = false;

/*******************************************************************************************************************
*                           This struct has the ACK or NAK in it.
*******************************************************************************************************************/
masterReturnCommand  dataReturnedToSlave;


/*******************************************************************************************************************
*                           These variables & Classes are to transfer data between Teensy's.
*******************************************************************************************************************/
char prefixChar = 0x02;  // ASCII Start of Text Character (STX).
char suffixChar = 0x03;  // ASCII End of Text Character (ETX).

uint8_t sizeOfWeatherDataStruct = (sizeof(WeatherDataStruct));

volatile bool incomingDMABufferFullFlag = false;  // This flag is set when the DMA system has a full buffer.

Uart2Event Event2;

volatile bool print_flag = false;  // Flag to indicate rx buffer size has been met

//  I set the serial2 rx & tx buffer sizes to 128 bytes.
volatile uint8_t numberOfBytesFromDMAReceive;  // This is the number of bytes recieved fromt the dma Rx ISR.

const uint8_t RxBUFSIZE = RX1_BUFFER_SIZE;  // Size of internal buffer (defaults to 64 bytes)
uint8_t receiveBuffer[RxBUFSIZE];  // User buffer array to hold incoming data.

const uint8_t maxRxBufferSize = (RxBUFSIZE - 1);
const uint8_t maxRxDataBufferSize = (maxRxBufferSize - 6);

uint8_t commandMessageDataFromSlave[maxRxDataBufferSize];  // The maximum received data packet is 121 bytes + (6 bytes of overhead).

const uint8_t TxBUFSIZE = TX1_BUFFER_SIZE;  // Size of internal buffer (defaults to 64 bytes)
uint8_t transmitBuffer[TxBUFSIZE];  // This buffer is temporary storage for the weather data struct converted to bytes.
uint8_t const maxTxBufferSize = (TxBUFSIZE - 1);
uint8_t const maxTxDataBufferSize = (maxTxBufferSize - 6);

commandTypes receiveCommand = ENDOFCOMMANDS;  // This is the received command from the master to perform some function.
uint8_t receivedDataSize = 0;  // This is the size of the command data packet.

/// These are the Command Codes extracted from the received packet handler:

uint8_t maxCommandNumber = ( ENDOFCOMMANDS - 1 );

FastCRC16 CRC16;


/*******************************************************************************************************************
 * These definitions are for the TFT, and Touch Screen located on the ER-TFTM043-3 4.3" or ER-TFTM070-5 7" TFT Board.
 *******************************************************************************************************************/

//#define RA8875_CS       10 //see below...
#define RA8875_CS       15 //  This pin is for my Weather Station implementation see below...

/*******************************************************************************************************************
 * Teensy 3.x can use any of these pins for SPI CS (Chip Select): 2,6,9,10,15,20,21,22,23.
 *******************************************************************************************************************/

//#define RA8875_RESET    9  //any pin or 255 to disable it!

#define RA8875_RESET    16  //  This pin is for my Weather Station implementation.  255 to disable it!
#define RA8875_INT      23  // This is the interrupt pin for the RA8875 resistive touch controller.


/************************************************************************************************************
 * Teensy3.x has small difference since it can use alternative SPI pins,
 * essential if you want to use this library with Audio Shield.
 *
 * for Teensy 3.1 or 3.2:
 *
 * RA8875 tft = RA8875(RA8875_CS,RA8875_RST,RA8875_INT);
 * or
 * RA8875 tft = RA8875(RA8875_CS,RA8875_RESET,RA8875_INT,alt_mosi,alt_sclk,alt_miso);
 * You can set alternative SPI pins:
 * RA8875 tft = CSpin,RSTpin=255,MOSI=11,SCLK=13,MISO=12;//Using the default configuration
 * RA8875 tft = RA8875(RA8875_CS, RA8875_RESET, 7, 14, 8); // Using the alternative SPI bus.
 * ------------------------------------------------------------------------------------------------------------
 *
 * RA8875 tft = RA8875(RA8875_CS,RA8875_RST,true);//this used ONLY with Teensy LC will use second SPI bus.
 *
 * ------------------------------------------------------------------------------------------------------------
 *
 * RA8875 tft = RA8875(RA8875_CS,RA8875_RESET);  // Teensy
 * or
 * RA8875 tft = RA8875(RA8875_CS);  // Teensy (not using rst pin)
 *************************************************************************************************************/

//RA8875 tft = RA8875(RA8875_CS, RA8875_RESET, 7, 14, 8); // Teensy using alt SPI bus.


bool tftPresentFlag = true; // This flag is set false during Setup, if there is no TFT display present.

#define SCREEN_ROTATION 0  // This number defines the screen rotation, 1 & 2 = Portrait, 0 & 3 = Landscape

/*******************************************************************************************************************
 *                                        The Capacitive Touch Screen (FT5206) or
 *                                  the 4-wire Resistive Touch Screen Controller Embedded.
 ********************************************************************************************************************
 * If you are using Capacitive Touch Screen FT5206 these are the data:
 *******************************************************************************************************************/
#if defined(USE_FT5206_TOUCH)  // This the the FT5206 flag.

#define FT5206_SDA      30
#define FT5206_SCL      29
#define FT5206_WAKE     25
#define FT5206_RST      27
#define FT5206_INT      28  // This is the FT5206 interrupt pin number

RA8875 tft = RA8875(RA8875_CS, RA8875_RESET, 11, 14, 12, FT5206_SDA, FT5206_SCL); // Teensy using std SPI bus with alternate clock pin & SDA, SCL.

uint16_t    touchScreenCoordinatesArray[MAXTOUCHLIMIT][2];  // This array is to hold coordinates recieved from the touch controller.

touchedPoints localPostionsJustTouched;  // Instantiate the touch screen points struct, This is a struct that hold the X & Y position of the point touched on the TFT touchscreen..

uint16_t    xPositionOfTouchedSpot = 0;      // This varible is for the x positon of the spot touched on the TFT touchscreen.
uint16_t    yPositionOfTouchedSpot = 0;      // This varible is for the y positon of the spot touched on the TFT touchscreen.
bool        foundTouchFingerDown = false;   // This flag is for dectecting the finger down touch on the touch screen.
bool        foundTouchFingerUp = false;     // This flag is for detecting the finger up touch on the touch screen.

#elif defined(USE_RA8875_TOUCH)  // This the the onboard resistive touch controller.


RA8875 tft = RA8875(RA8875_CS, RA8875_RESET, 11, 14, 12); // Teensy using std SPI bus with alternate clock pin.

uint16_t    xPositionOfTouchedSpot = 0;      // This varible is for the x positon of the spot touched on the TFT touchscreen.
uint16_t    yPositionOfTouchedSpot = 0;      // This varible is for the y positon of the spot touched on the TFT touchscreen.

#else

RA8875 tft = RA8875(RA8875_CS, RA8875_RESET, 11, 14, 12); // Teensy using std SPI bus with alternate clock pin.

#endif

/*******************************************************************************************************************
* These structs are for the Check for Preferences function, Set Day of the Week function, Set Master Preferences function,
* Temp Sensors Preferences function, set Time Zone function, set Rain Preferences function, set Cooler Preferences function,
* set Time Preferences function, and the Numeric Keypad function.
*
* Round Rectangle definitions:
* char *text, RA8875tsize fontSize, Colors textColor, uint8_t numberOfTextLines, int16_t setBeginningXPosition,
* int16_t setBeginningYPosition, int16_t Width, int16_t Height, int16_t radius, Colors fillColor, Colors borderColor.
*
* Round Rectangle Button definitions:
* Figure_types buttonShape, char *text, int16_t setBeginningXPosition, int16_t setBeginningYPosition, int16_t Width,
* int16_t Height, int16_t radius, RA8875tsize fontSize, uint8_t numberOfTextLines, Colors textColor, Colors fillColor,
* Colors borderColor.
*******************************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 7 Buttons (2 rows and 4 columns) definitions are for the Set Day of the Week function.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create a Round Rectangle Button for the Sunday.
SelectableShapeButton Sunday( RNDRECTANGLE, (char*)"Sunday", EightButtonPanel1st_XPosition, EightButtonPanel1st_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Monday.
SelectableShapeButton Monday( RNDRECTANGLE, (char*)"Monday", EightButtonPanel2nd_XPosition, EightButtonPanel2nd_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Tuesday.
SelectableShapeButton Tuesday( RNDRECTANGLE, (char*)"Tuesday", EightButtonPanel3rd_XPosition, EightButtonPanel3rd_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Wednesday.
SelectableShapeButton Wednesday( RNDRECTANGLE, (char*)"Wednesday", EightButtonPanel4th_XPosition, EightButtonPanel4th_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Thursday.
SelectableShapeButton Thursday( RNDRECTANGLE, (char*)"Thursday", EightButtonPanel5th_XPosition, EightButtonPanel5th_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Friday.
SelectableShapeButton Friday( RNDRECTANGLE, (char*)"Friday", EightButtonPanel6th_XPosition, EightButtonPanel6th_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Saturday.
SelectableShapeButton Saturday( RNDRECTANGLE, (char*)"Saturday", EightButtonPanel7th_XPosition, EightButtonPanel7th_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the NotUsed.
//SelectableShapeButton NotUsed2( RNDRECTANGLE, (char*)"Not Used", EightButtonPanel8th_XPosition, EightButtonPanel8th_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)2, MAGENTA, WHITE, BLUE );


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 6 Buttons (2 rows and 3 columns) definitions are for the Set Master Preferences function.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create a Round Rectangle Button for the SetTimePreferences.
SelectableShapeButton SetTimePreferences( RNDRECTANGLE, (char*)"Set Time Preferences", SixButtonPanel1st_XPosition, SixButtonPanel1st_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the SetRainPreferences.
SelectableShapeButton SetRainPreferences( RNDRECTANGLE, (char*)"Set Rain Preferences", SixButtonPanel2nd_XPosition, SixButtonPanel2nd_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the SetTempSensorsPreferences.
SelectableShapeButton SetTempSensorsPreferences( RNDRECTANGLE, (char*)"Set Temp Sensors Preferences", SixButtonPanel3rd_XPosition, SixButtonPanel3rd_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)4, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the SetCoolerPreferences.
SelectableShapeButton NotUsed1( RNDRECTANGLE, (char*)"Not Used", SixButtonPanel4th_XPosition, SixButtonPanel4th_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)2, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the NotUsed.
SelectableShapeButton ResetMaster( RNDRECTANGLE, (char*)"Reset Master", SixButtonPanel5th_XPosition, SixButtonPanel5th_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)2, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the NotUsed.
SelectableShapeButton ResetSlave( RNDRECTANGLE, (char*)"Reset Slave", SixButtonPanel6th_XPosition, SixButtonPanel6th_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)2, MAGENTA, WHITE, BLUE );


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 6 Buttons definitions are for the set Temp Sensors Preferences function.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create a Round Rectangle Button for the RenumberTempIDs.
SelectableShapeButton RenumberTempIDs( RNDRECTANGLE, (char*)"Renumber Temp IDs", SixButtonPanel1st_XPosition, SixButtonPanel1st_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the ChkForSensors.
SelectableShapeButton ChkForSensors( RNDRECTANGLE, (char*)"Chk For Sensors", SixButtonPanel2nd_XPosition, SixButtonPanel2nd_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Fnd1WirePins.
SelectableShapeButton Fnd1WirePins( RNDRECTANGLE, (char*)"Fnd 1Wire Pins", SixButtonPanel3rd_XPosition, SixButtonPanel3rd_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the IdentifyTmpDevs.
SelectableShapeButton IdentifyTmpDevs( RNDRECTANGLE, (char*)"Identify Pins Tmp Devs", SixButtonPanel4th_XPosition, SixButtonPanel4th_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)4, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the PlotTempDataonTFT.
SelectableShapeButton PlotTempDataonTFT( RNDRECTANGLE, (char*)"Plot Temp DataonTFT", SixButtonPanel5th_XPosition, SixButtonPanel5th_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the NotUsed.
SelectableShapeButton NotUsed3( RNDRECTANGLE, (char*)"Not Used", SixButtonPanel6th_XPosition, SixButtonPanel6th_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)2, MAGENTA, WHITE, BLUE );

/*
////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 6 Buttons definitions are for the set Time Zone function.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create a Round Rectangle Button for the HawaiiTimeZone.
SelectableShapeButton HawaiiTimeZone( RNDRECTANGLE, (char*)"Hawaii Time Zone", SixButtonPanel1st_XPosition, SixButtonPanel1st_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the PacificTimeZone.
SelectableShapeButton PacificTimeZone( RNDRECTANGLE, (char*)"Pacific Time Zone", SixButtonPanel2nd_XPosition, SixButtonPanel2nd_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the MountainTimeZone.
SelectableShapeButton MountainTimeZone( RNDRECTANGLE, (char*)"Mountain Time Zone", SixButtonPanel3rd_XPosition, SixButtonPanel3rd_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the CentralTimeZone.
SelectableShapeButton CentralTimeZone( RNDRECTANGLE, (char*)"Central Time Zone", SixButtonPanel4th_XPosition, SixButtonPanel4th_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the EasternTimeZone.
SelectableShapeButton EasternTimeZone( RNDRECTANGLE, (char*)"Eastern Time Zone", SixButtonPanel5th_XPosition, SixButtonPanel5th_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the LondonTimeZone.
SelectableShapeButton LondonTimeZone( RNDRECTANGLE, (char*)"London Time Zone", SixButtonPanel6th_XPosition, SixButtonPanel6th_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );
*/


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 8 Buttons definitions are for the set Time Zone function.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create a Round Rectangle Button for the HawaiiTimeZone.
SelectableShapeButton HawaiiTimeZone( RNDRECTANGLE, (char*)"Hawaii Time Zone", EightButtonPanel1st_XPosition, EightButtonPanel1st_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the PacificTimeZone.
SelectableShapeButton PacificTimeZone( RNDRECTANGLE, (char*)"Pacific Time Zone", EightButtonPanel2nd_XPosition, EightButtonPanel2nd_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the MountainTimeZone.
SelectableShapeButton MountainTimeZone( RNDRECTANGLE, (char*)"Mountain Time Zone", EightButtonPanel3rd_XPosition, EightButtonPanel3rd_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the CentralTimeZone.
SelectableShapeButton CentralTimeZone( RNDRECTANGLE, (char*)"Central Time Zone", EightButtonPanel4th_XPosition, EightButtonPanel4th_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the EasternTimeZone.
SelectableShapeButton EasternTimeZone( RNDRECTANGLE, (char*)"Eastern Time Zone", EightButtonPanel5th_XPosition, EightButtonPanel5th_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the LondonTimeZone.
SelectableShapeButton LondonTimeZone( RNDRECTANGLE, (char*)"London Time Zone", EightButtonPanel6th_XPosition, EightButtonPanel6th_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Central European Time Zone.
SelectableShapeButton CentralEuTimeZone( RNDRECTANGLE, (char*)"Central EU TimeZone", EightButtonPanel7th_XPosition, EightButtonPanel7th_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Eastern European Time Zone.
SelectableShapeButton EasternEuTimeZone( RNDRECTANGLE, (char*)"Eastern EU TimeZone", EightButtonPanel8th_XPosition, EightButtonPanel8th_YPosition, EightButtonPanel_Width, EightButtonPanel_Height, EightButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );



////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 6 Buttons definitions are for the set Rain Preferences function.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create a Round Rectangle Button for the NotUsed.
SelectableShapeButton SetMasterRainOffset( RNDRECTANGLE, (char*)"Set Master Rain Offset", SixButtonPanel1st_XPosition, SixButtonPanel1st_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)4, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the NotUsed.
SelectableShapeButton ClearMasterRainOffset( RNDRECTANGLE, (char*)"Clear Master Rain Offset", SixButtonPanel2nd_XPosition, SixButtonPanel2nd_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)4, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the NotUsed.
SelectableShapeButton SetHumidityThreshold( RNDRECTANGLE, (char*)"Set Humidity Threshold", SixButtonPanel3rd_XPosition, SixButtonPanel3rd_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the NotUsed.
SelectableShapeButton SetSlaveYearlyValue( RNDRECTANGLE, (char*)"Set Slave Yearly Value", SixButtonPanel4th_XPosition, SixButtonPanel4th_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)4, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the EasternTimeZone.
SelectableShapeButton ClearSlaveRain( RNDRECTANGLE, (char*)"Clear Slave Rain", SixButtonPanel5th_XPosition, SixButtonPanel5th_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the LondonTimeZone.
SelectableShapeButton ResetSlaveRain( RNDRECTANGLE, (char*)"Reset Slave Rain", SixButtonPanel6th_XPosition, SixButtonPanel6th_YPosition, SixButtonPanel_Width, SixButtonPanel_Height, SixButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 4 Buttons definitions are for the set Temperature Calibration Preferences function.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create a Round Rectangle Button for the CoolerOn.
SelectableShapeButton SetMasterTempCalibration( RNDRECTANGLE, (char*)"SetMaster Temp Calibration", FourButtonPanel1st_XPosition, FourButtonPanel1st_YPosition, FourButtonPanel_Width, FourButtonPanel_Height, FourButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the CoolerOff.
SelectableShapeButton SetSlaveTempCalibration( RNDRECTANGLE, (char*)"SetSlave Temp Calibration", FourButtonPanel2nd_XPosition, FourButtonPanel2nd_YPosition, FourButtonPanel_Width, FourButtonPanel_Height, FourButtonPanel_radius, (RA8875tsize)1, (uint8_t)2, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the NotUsed.
SelectableShapeButton NotUsed7( RNDRECTANGLE, (char*)"Not Used", FourButtonPanel3rd_XPosition, FourButtonPanel3rd_YPosition, FourButtonPanel_Width, FourButtonPanel_Height, FourButtonPanel_radius, (RA8875tsize)1, (uint8_t)2, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the NotUsed.
SelectableShapeButton NotUsed8( RNDRECTANGLE, (char*)"Not Used", FourButtonPanel4th_XPosition, FourButtonPanel4th_YPosition, FourButtonPanel_Width, FourButtonPanel_Height, FourButtonPanel_radius, (RA8875tsize)1, (uint8_t)2, MAGENTA, WHITE, BLUE );


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 4 Buttons definitions are for the set Time Preferences function.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create a Round Rectangle Button for the SetTimeZone.
SelectableShapeButton SetTimeZone( RNDRECTANGLE, (char*)"Set Time Zone", FourButtonPanel1st_XPosition, FourButtonPanel1st_YPosition, FourButtonPanel_Width, FourButtonPanel_Height, FourButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the NotUsed.
SelectableShapeButton SetWeekday( RNDRECTANGLE, (char*)"Set Weekday", FourButtonPanel2nd_XPosition, FourButtonPanel2nd_YPosition, FourButtonPanel_Width, FourButtonPanel_Height, FourButtonPanel_radius, (RA8875tsize)1, (uint8_t)2, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the NotUsed.
SelectableShapeButton SetTimeFromGPS( RNDRECTANGLE, (char*)"Set Time from GPS", FourButtonPanel3rd_XPosition, FourButtonPanel3rd_YPosition, FourButtonPanel_Width, FourButtonPanel_Height, FourButtonPanel_radius, (RA8875tsize)1, (uint8_t)4, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the NotUsed.
SelectableShapeButton SetSlaveTime( RNDRECTANGLE, (char*)"Set Slave Time", FourButtonPanel4th_XPosition, FourButtonPanel4th_YPosition, FourButtonPanel_Width, FourButtonPanel_Height, FourButtonPanel_radius, (RA8875tsize)1, (uint8_t)3, MAGENTA, WHITE, BLUE );


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 14 Buttons (4 rows and 4 columns) definitions are for the Numeric Keypad function.
/// These next 14 defines are for the 8 button Numeric Keypad Panel.
///  __________________
///  | 8 | 9 | - | C |
///   ------------------
///  | 4 | 5 | 6 | 7 |
///   ------------------
///  | 0 | 1 | 2 | 3 |
///   ------------------
///  | Enter  |  .   |
///   ------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create a Round Rectangle Button for the Sunday.
SelectableShapeButton Button8( RNDRECTANGLE, (char*)"8", NumericKeypad1st_XPosition, NumericKeypad1st_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Monday.
SelectableShapeButton Button9( RNDRECTANGLE, (char*)"9", NumericKeypad2nd_XPosition, NumericKeypad2nd_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Minus Sign.
SelectableShapeButton ButtonMinusSign( RNDRECTANGLE, (char*)"Chng Sign", NumericKeypad3rd_XPosition, NumericKeypad3rd_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)2, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Tuesday.
SelectableShapeButton ButtonC( RNDRECTANGLE, (char*)"Clear", NumericKeypad4th_XPosition, NumericKeypad4th_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Wednesday.
SelectableShapeButton Button4( RNDRECTANGLE, (char*)"4", NumericKeypad5th_XPosition, NumericKeypad5th_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Thursday.
SelectableShapeButton Button5( RNDRECTANGLE, (char*)"5", NumericKeypad6th_XPosition, NumericKeypad6th_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Friday.
SelectableShapeButton Button6( RNDRECTANGLE, (char*)"6", NumericKeypad7th_XPosition, NumericKeypad7th_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Saturday.
SelectableShapeButton Button7( RNDRECTANGLE, (char*)"7", NumericKeypad8th_XPosition, NumericKeypad8th_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Wednesday.
SelectableShapeButton Button0( RNDRECTANGLE, (char*)"0", NumericKeypad9th_XPosition, NumericKeypad9th_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Thursday.
SelectableShapeButton Button1( RNDRECTANGLE, (char*)"1", NumericKeypad10th_XPosition, NumericKeypad10th_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Friday.
SelectableShapeButton Button2( RNDRECTANGLE, (char*)"2", NumericKeypad11th_XPosition, NumericKeypad11th_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Saturday.
SelectableShapeButton Button3( RNDRECTANGLE, (char*)"3", NumericKeypad12th_XPosition, NumericKeypad12th_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the Enter Key.
SelectableShapeButton ButtonEnter( RNDRECTANGLE, (char*)"Enter", NumericKeypad13th_XPosition, NumericKeypad13th_YPosition, NumericKeypad13th_Width, NumericKeypad13th_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Button for the (.) decimal point Key.
SelectableShapeButton ButtonDecimalPoint( RNDRECTANGLE, (char*)".", NumericKeypad14th_XPosition, NumericKeypad14th_YPosition, NumericKeypad_Width, NumericKeypad_Height, NumericKeypad_radius, (RA8875tsize)1, (uint8_t)1, MAGENTA, WHITE, BLUE );

// Create a Round Rectangle Shape with no button for the Answer.
RndRectangle ButtonAnswer( (char*)"          ", (RA8875tsize)1, MAGENTA, (uint8_t)0, NumericKeypadAnswer_XPosition, NumericKeypadAnswer_YPosition, NumericKeypadAnswer_Width, NumericKeypadAnswer_Height, NumericKeypadAnswer_radius, WHITE, BLUE );

float numberEnteredFromKeypad = 0;  // This is the number returned from the numeric keypad functions.


/*******************************************************************************************************************
 * These struct is for the Vertical Bar Gauge Code which is for displaying data in a FFT type window.
 *******************************************************************************************************************/
//verticalBarGauge1 VertBarGauge;    // Instantiate V bar Gauge Parameters.


/*******************************************************************************************************************
 * This struct is for the Oscilloscope Code which is for displaying data in a Oscilloscope (Amplitude vs. Time)
 * type window.
 *******************************************************************************************************************/
//oscilloscopeStruct OscopeParams;    // Instantiate Oscilloscope Parameters.


/*******************************************************************************************************************
 *                                            The Rain Bucket Sensor
 ********************************************************************************************************************
 * This is the Rain Sensor pin:  The rain sensor is a tipping bucket style rain sensor with a rectangular inlet on top
 * of 1.945" [49.17mm] x 4.307" [109.62 mm].  Each tip generates a pulse from a magnetic sensor and contains 2 cc of water.
 *
 * Rain bucket Sensor Details:   0.06102374 in³ (Cubic Inch) = 1 (cc) or (ml) ; 1 sq in = 645.16 sq mm (or cm)
 * The volume of each bucket tip is 2 cc (or 2ml) (or 0.12204748 cubic inches).
 * Therefore the volume of rain that fell is: (Bucket Volume * Number of tips).
 *
 * The area of the rectangular input funnel is 1.945" [49.17mm] x 4.307" [109.62 mm] = 8.377115 sqin or 5404.5795134 sq mm
 * The "Depth of rain" = volume of rain (in cc) or (cuin) / area of funnel (in sq mm) or (sq in)
 * or "Depth of Rain" = 2 cc or (0.12204748 cu in) * Number of bucket tips / 5404.5795134 sq mm or (8.377115 sqin)
 *
 * The rainfall intensity is usually given in units of mm/hour or (inches/hour).  This can be calculated by logging
 * the time of each tip of the bucket along with the number of tips that have occured.
 * The total rainfall for each day (24 hours beginning at Midnight) is also usually calculated.
 * The yearly rainfall data is also usually calculated beginning at Jan 1.
 *******************************************************************************************************************/


/*******************************************************************************************************************
 * This instantiates the class for reading and writing to the RTC EEPROM (AT24C32) 0 -> 0xFFF.
 *******************************************************************************************************************/
AT24C32 eepromMemory(7);

const uint16_t rainMemoryLocation = 0x100;
const uint16_t TempCalOffsetLocation = 0x110;
const uint16_t TimeZoneSettingLocation = 0x130;
float rainYearlyOffset = 0;

/*******************************************************************************************************************
 * Usage with 32-Bit "time_t"
 * Time uses a special time_t variable type, which is the number of seconds elapsed since 1970.
 * Using time_t lets you store or compare times as a single number, rather that dealing with 6 numbers and details
 * like the number of days in each month and leap years.
 *
 *******************************************************************************************************************/
time_t lastTimeSetByClock = 0;      // This time is set by the clock reading routine.
time_t PreviousTimeForWind = 0;     // This time is for the timeout of the wind data.

/*******************************************************************************************************************
 * This Variable is for determining whether or not the Teensy Clock has been set (true) or not (false).
 *******************************************************************************************************************/
bool pctime = false;


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/// Define variables and constants:
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/

volatile bool SetTimeFlag = false;
volatile int SetTimeIterationCount = 0;
volatile bool NormalCommandLineModeFlag = true;
volatile bool preferencesInProgressFlag = false;


/*******************************************************************************************************************
 * This is the DS3231's Address on the I2C bus.
 *******************************************************************************************************************/
#define DS3231_CTRL_ID 0x68

/*******************************************************************************************************************
 *
 * These definitions are for the BME280 Barometric Pressure, Temp, and Humidity Sensor.
 *
 * Designed specifically to work with the GY-BME280-3.3 BME280 Breakout Board.
 * ----> http://www.ebay.com/itm/High-Precision-Atmospheric-GY-BMP280-3-3-Pressure-Sensor-Module-for-Arduino/151854537522?rt=nc&_soffid=5013107907&_soffType=OrderSubTotalOffer&_trksid=p5731.m3795
 *
 * These sensors use I2C, or SPI to communicate, 2 pins are required for the I2C interface, 4 pins are required for
 * the SPI interface.
 *
 * I2C0 on Teensy 3.x Interface:
 * Connect Pin 1 VCC of the BME280 sensor card to 5.0V.
 * Connect Pin 2 GND to Ground
 * Connect Pin 3 SCL to i2c clock - on Teensy 3.1 that is: Pin 19 (SCL0).
 * Connect Pin 4 SDI to i2c data - on Teensy 3.1 that is: Pin 18 (SDA0).
 *
 * I2C1 on Teensy 3.x Interface:
 * Connect Pin 1 VCC of the BME280 sensor card to 5.0V.
 * Connect Pin 2 GND to Ground
 * Connect Pin 3 SCL to i2c clock - on Teensy 3.1 that is: Pin 16 (SCL1).
 * Connect Pin 4 SDI to i2c data - on Teensy 3.1 that is: Pin 17 (SDA1).
 *
 * SPI0 Interface:
 * Connect Pin 1 to the power supply, 3V.
 * Connect Pin 2 to common power/data ground.
 * Connect Pin 3 (SCL) pin to SPI clock - on Teensy 3.1 that is: Pin 13 (SCLK)..
 * Connect Pin 4 (SDI) pin to SPI data in - on Teensy 3.1 that is: Pin 11 (DOUT or MOSI)..
 * Connect Pin 5 (CSB) pin to SPI Chip Select - on Teensy 3.1 that is: Pin 2,6,9,10,15,20,21,22,23 (CS)..
 * Connect Pin 6 (SDO) pin to SPI data out - on Teensy 3.1 that is: Pin 12 (DIN or MISO)..
 *
 * Pressure is returned in the SI units of Pascals. 100 Pascals = 1 hPa = 1 millibar. Often times barometric pressure
 * is reported in millibar or inches-mercury. For future reference 1 pascal = 0.000295333727 inches of mercury, or
 * 1 inch Hg = 3386.39 Pascals. So if you take the pascal value of say 100734 and divide by 3389.39 you'll get 29.72
 * inches-Hg.
 *
 *****************************************************************************************************************/

BME280 SystemSensor;  // Instantiate the BME280 sensor object

/*******************************************************************************************************************
 * This struct is for the System Sensor function.
 *******************************************************************************************************************/
struct SensorState
{ //12 bytes
    
    float Temperature; //32 bits (4 bytes).
    float Baro_Pressure; //32 bits (4 bytes).
    float Relative_Humidity; //32 bits (4 bytes).
    
} sensorState;

/*******************************************************************************************************************
 * RTC Date and time variables.
 * These are the Data for the setTime Function in Time.h
 *
 * void setTime(int hr,int min,int sec,int dy, int mnth, int yr)
 * year can be given as full four digit year or two digts (2010 or 10 for 2010);
 * it is converted to years since 1970
 *******************************************************************************************************************/
const char * monthName[12] =
{
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};


/*******************************************************************************************************************
 * These definitions are for the Wind Direction Flower.
 *******************************************************************************************************************/
// Define array for Wind Direction Flower:
int16_t WindDirFlowerPos[3] = {0, 0, 50}; //x position, y position, r Radius (Size of Wind Direction Flower)

// Define colors for background, face, Wind Direction Pointer.
const uint16_t WindDirFlowerColors[3] = {RA8875_BLACK, RA8875_BLACK, RA8875_RED};

// Define array with positions for Wind Direction Pointer X, Wind Direction Pointer Y
uint16_t oldWindDirFlowerPos[2] = {0, 0};

volatile boolean execute = false;
volatile int interval = 0;
char input;
bool windFlowerBlanked = false;

/*******************************************************************************************************************
 * IntervalTimer uses interrupts to call a function at a precise timing interval.
 * IntervalTimer will call your function from interrupt context. Because it can interrupt your program at
 * any moment, special design is necessary to share data with the rest of your program.
 * Up to 4 IntervalTimer objects may be active simultaneuously.  IntervalTimer is supported only on Teensy 3.x.
 *
 *******************************************************************************************************************/
// Create and Instantiation three IntervalTimer objects.
IntervalTimer oneSecondTimer;       // One Second Timer Interrupt.
IntervalTimer twoSecondTimer;       // Two Second Timer Interrupt.
IntervalTimer tenSecondTimer;       // Ten Second Timer Interrupt.
IntervalTimer thirtySecondTimer;  // Thirty Second Timer Interrupt.


/*******************************************************************************************************************
 * These definitions are for the Clock functions.
 *******************************************************************************************************************/
// Define array for Clockface:
int16_t clockPos[3] = {0, 0, 50}; //x size, y size, r Radius

// Define array for Time in hours, minutes, and seconds.
uint8_t currentTime[3] = {0, 0, 0}; //hh, mm, ss

// Define a variable for the Day of the Week.
uint8_t currentDayOfWeek = 0;

// Define array for Day, Month, and Year.
int currentDate[3] = {0, 0, 0}; // Day, Month, Year

// Define colors for background, face, hh hand, mm hand, and ss hand.
const uint16_t clockColors[5] = {RA8875_BLACK, RA8875_BLACK, RA8875_CYAN, RA8875_GREEN, RA8875_RED};

// Define array with positions for hour hand X, hour hand Y, minute hand X, minute hand Y, second hand X, second hand Y
uint16_t oldPos[6] = {0, 0, 0, 0, 0, 0};

// Need variable for the current time.
//unsigned long targetTime = 0;


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
///                                     Functions:
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/


/*******************************************************************************************************************
 * This Function prints the local char(8 bit) buffer provided contents on the monitor.
 *******************************************************************************************************************/
void printBuffer(uint8_t *BufferAddress, int16_t LocalBufferSize)
{
    uint16_t i = 0;
    uint8_t j;
    
    Serial.print(F("Local Buffer Size: "));
    Serial.println(LocalBufferSize, DEC);
    
    Serial.println(F("Local Buffer value:"));
    Serial.println(F("Address , Value"));
    
    j = 1;
    Serial.print(uint32_t(&BufferAddress[i]), HEX);  // Print the address of the Array.
    for (i = 0; i < LocalBufferSize; i++)
    {
        
        //tft.print( uint32_t (&BufferAddress[i]), HEX);
        Serial.print(", ");
        //tft.print(F(", "));
        Serial.print(BufferAddress[i],HEX);  // Increase the index into the array.
        //tft.print(BufferAddress[i]);
        
        if ( j == 8 )
        {
            Serial.println("");
            j = 1;
            Serial.print(uint32_t(&BufferAddress[i]), HEX);  // Get the address of the Array.
        }
        else
        {
            Serial.print(" : ");
            j++;
        }
    }
    
    Serial.println("");
}


/*******************************************************************************************************************
 * This Function prints the local char(8 bit) buffer provided contents on the monitor.
 *******************************************************************************************************************/
void printLocalBuffers(char *LocalBufferAddress, int16_t LocalBufferSize)
{
    uint16_t i = 0;
    uint8_t j;
    
    Serial.print(F("Local Buffer Size: "));
    Serial.println(LocalBufferSize, DEC);
    
    Serial.println(F("Local Buffer value:"));
    Serial.println(F("Address , Value"));
    
    j = 1;
    Serial.print(uint32_t(&LocalBufferAddress[i]), HEX);  // Print the address of the Array.
    for (i = 0; i < LocalBufferSize; i++)
    {
        
        //tft.print( uint32_t (&buffer1[i]), HEX);
        Serial.print(", ");
        //tft.print(F(", "));
        Serial.print(LocalBufferAddress[i],HEX);  // Increase the index into the array.
        //tft.print(buffer1[i]);
        
        if ( j == 8 )
        {
            Serial.println("");
            j = 1;
            Serial.print(uint32_t(&LocalBufferAddress[i]), HEX);  // Get the address of the Array.
        }
        else
        {
            Serial.print(" : ");
            j++;
        }
    }
    
    Serial.println("");
}


#if defined(USINGTEMPSENSORS)
/***************************************************************************************************************
 * This function gets the temperature from a Temp Sensor(s) on the One Wire Bus using the ID number of the sensor.
 * Needs:
 *   The ID number of the Temp Sensor on the One Wire Bus.
 * Returns:
 *   A Floating Point Value of the Temperature in Degrees Farenheit of the Temp Sensor ID Passed into the Function.
 ***************************************************************************************************************/
float getTempByID(int id)
{
    for (uint8_t index = 0; index < deviceCount; index++)
    {
        if (oneWireTemps[index].id == id)
        {
            return tempSensors.getTempF(oneWireTemps[index].addr);
        }
    }
    return -999;  // Error no device by that ID.
}


/***************************************************************************************************************
 * This function prints the address(es) of the Temp Sensor(s) on the One Wire Bus.
 * Needs:
 *   The Device address(es) of the Temp Sensor on the One Wire Bus.
 * Returns:
 *   Nothing.
 ***************************************************************************************************************/
void printAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < deviceCount; i++)
    {
        // zero pad the address if necessary
        if (deviceAddress[i] < 16) Serial.print(F("0"));
        Serial.print(deviceAddress[i], HEX);
    }
}


/***************************************************************************************************************
 * This function prints the address(es) of the Temp Sensor(s) on the One Wire Bus.
 * Needs:
 *   The Device address(es) of the Temp Sensor on the One Wire Bus.
 * Returns:
 *   Nothing.
 ***************************************************************************************************************/
void tftPrintAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < deviceCount; i++)
    {
        // zero pad the address if necessary
        if (deviceAddress[i] < 16) tft.print(F("0"));
        tft.print(deviceAddress[i], HEX);
    }
}


/***************************************************************************************************************
 * This function prints the ID's and the address(es) of all the Temp Sensor(s) on the One Wire Bus.
 * Needs:
 *   Nothing.
 * Returns:
 *   Nothing.
 ***************************************************************************************************************/
void checkForSensors(void)
{
    // Check all sensors are set
    Serial.println();
    for (uint8_t index = 0; index < deviceCount; index++)
    {
        Serial.print(oneWireTemps[index].id);
        Serial.print(F("\t"));
        printAddress(oneWireTemps[index].addr);
        Serial.println();
    }
    Serial.println();
}


/***************************************************************************************************************
 * This function prints the ID's and the address(es) of all the Temp Sensor(s) on the One Wire Bus.
 * Needs:
 *   Nothing.
 * Returns:
 *   Nothing.
 ***************************************************************************************************************/
void tftCheckForSensors(void)
{
    // Check all sensors are set
    Serial.println();
    tft.println();
    for (uint8_t index = 0; index < deviceCount; index++)
    {
#if defined(DEBUG33)
        Serial.print(oneWireTemps[index].id);
        Serial.print(F("\t"));
#endif
        tft.print(oneWireTemps[index].id);
        tft.print(F("\t"));
        tftPrintAddress(oneWireTemps[index].addr);
#if defined(DEBUG33)
        Serial.println();
#endif
        tft.println();
    }
#if defined(DEBUG33)
    Serial.println();
#endif
    tft.println();
}


/***************************************************************************************************************
 * This function finds all the Temp Sensor(s) and lists the addresses of the temp sensor(s) on the One Wire Bus
 * Pin Number Passed in to the Function.  This function also updates the deviceCount Global Variable.
 * Needs:
 *   Teensy Pin Number to search for an Active One Wire Bus Device.
 * Returns:
 *   True if a live One Wire Bus Device is found.
 *   False if no active One Wire Bus Device is found.
 ***************************************************************************************************************/
bool findDevices(int teensySearchForOneWireBusPin)
{
    uint8_t count = 0;
    
#if defined(DEBUG33)
    Serial.println(F("Beginning the search with a new pin number..."));
#endif
    
    oneWire.begin(teensySearchForOneWireBusPin);
    
    if (oneWire.search(oneWireTemps[count].addr)) // Find any OneWire device on this Teensy Bus Pin.
    {
        // Found a OneWire Device on the Teensy Pin.
#if defined(DEBUG33)
        Serial.print(F("\nTeensy Pin Number with a OneWire Bus: "));
        Serial.println(teensySearchForOneWireBusPin, DEC);
        Serial.println(F("OneWire Bus Address(s) of Temp Sensor(s) ="));
        Serial.println(F("{"));
#endif
        
        do  // Now find all the other OneWire devices on this Bus.
        {
            count++;
#if defined(DEBUG33)
            Serial.print(F("\t{ "));
            for (uint8_t i = 0; i < 8; i++)
            {
                Serial.print(F("0x"));
                if (oneWireTemps[count].addr[i] < 0x10) Serial.print(F("0"));
                Serial.print(oneWireTemps[count].addr[i], HEX);
                if (i < 7) Serial.print(F(", "));
            }
            Serial.println(F(" },"));
#endif
        }
        while (oneWire.search(oneWireTemps[count].addr));  // Returns false if no devices, or no more devices; True if device found
        
        deviceCount = count;  // Copy the number of devices into the global variable deviceCount.
        
#if defined(DEBUG33)
        Serial.println(F("};"));
        Serial.print(F("Number of active Temp Sensor devices found: "));
        Serial.println(count);
#endif
        
#if defined(DEBUG33)
        Serial.println(F("Resetting the oneWire Bus..."));
#endif
        
        oneWire.reset();
        return true;
    }
    else
    {
#if defined(DEBUG33)
        Serial.println(F("Resetting the oneWire Bus..."));
#endif
        oneWire.reset();
        return false;
    }
}


/***************************************************************************************************************
 * This function finds all the Temp Sensor(s) and lists the addresses of the temp sensor(s) on the One Wire Bus
 * Pin Number Passed in to the Function.  This function also updates the deviceCount Global Variable.
 * Needs:
 *   Teensy Pin Number to search for an Active One Wire Bus Device.
 * Returns:
 *   True if a live One Wire Bus Device is found.
 *   False if no active One Wire Bus Device is found.
 ***************************************************************************************************************/
bool tftfindDevices(int teensySearchForOneWireBusPin)
{
    uint8_t count = 0;
    
#if defined(DEBUG33)
    Serial.println(F("Beginning the search with a new pin number..."));
#endif
    tft.println(F("Beginning the search with a new pin number..."));
    
    oneWire.begin(teensySearchForOneWireBusPin);
    
    if (oneWire.search(oneWireTemps[count].addr)) // Find any OneWire device on this Teensy Bus Pin.
    {
        // Found a OneWire Device on the Teensy Pin.
#if defined(DEBUG33)
        Serial.print(F("\nTeensy Pin Number with a OneWire Bus: "));
        Serial.println(teensySearchForOneWireBusPin, DEC);
        Serial.println(F("OneWire Bus Address(s) of Temp Sensor(s) ="));
        Serial.println(F("{"));
#endif
        
        tft.print(F("\nTeensy Pin Number with a OneWire Bus: "));
        tft.println(teensySearchForOneWireBusPin, DEC);
        tft.println(F("OneWire Bus Address(s) of Temp Sensor(s) ="));
        tft.println(F("{"));
        
        do  // Now find all the other OneWire devices on this Bus.
        {
            count++;
#if defined(DEBUG33)
            Serial.print(F("\t{ "));
            for (uint8_t i = 0; i < 8; i++)
            {
                Serial.print(F("0x"));
                if (oneWireTemps[count].addr[i] < 0x10) Serial.print(F("0"));
                Serial.print(oneWireTemps[count].addr[i], HEX);
                if (i < 7) Serial.print(F(", "));
            }
            Serial.println(F(" },"));
#endif
            tft.print(F("\t{ "));
            for (uint8_t i = 0; i < 8; i++)
            {
                tft.print(F("0x"));
                if (oneWireTemps[count].addr[i] < 0x10) tft.print(F("0"));
                tft.print(oneWireTemps[count].addr[i], HEX);
                if (i < 7) tft.print(F(", "));
            }
            tft.println(F(" },"));
        }
        while (oneWire.search(oneWireTemps[count].addr));  // Returns false if no devices, or no more devices; True if device found
        
        deviceCount = count;  // Copy the number of devices into the global variable deviceCount.
        
#if defined(DEBUG33)
        Serial.println(F("};"));
        Serial.print(F("Number of active Temp Sensor devices found: "));
        Serial.println(count);
#endif
        
        tft.println(F("};"));
        tft.print(F("Number of active Temp Sensor devices found: "));
        tft.println(count);
        
#if defined(DEBUG33)
        Serial.println(F("Resetting the oneWire Bus..."));
#endif
        tft.println(F("Resetting the oneWire Bus..."));
        
        oneWire.reset();
        return true;
    }
    else
    {
#if defined(DEBUG33)
        Serial.println(F("Resetting the oneWire Bus..."));
#endif
        tft.println(F("Resetting the oneWire Bus..."));
        oneWire.reset();
        return false;
    }
}


/***************************************************************************************************************
 * This function finds the Temp Sensor(s) on the One Wire Bus Pin Number Passed in to the Function.
 * Needs:
 *   Nothing
 * Returns:
 *   True if a live One Wire Bus Device is found.
 *   False if no active One Wire Bus Device is found.
 ***************************************************************************************************************/
bool findActiveOneWirePins(void)
{
    bool oneWireBusFound = false;
    
#if defined(DEBUG33)
    Serial.println(F("Start the search for Devices(s) on the OneWire bus... \n"));
#endif
    
    for (teensySearchForOneWireBusPin = 0; teensySearchForOneWireBusPin < 33; teensySearchForOneWireBusPin++)
    {
        if (findDevices(teensySearchForOneWireBusPin))
        {
#if defined(DEBUG33)
            Serial.print(F("Found the Temp Sensor(s) on the oneWire bus on Teensy's pin number: "));
            Serial.println(teensySearchForOneWireBusPin, DEC);
#endif
            oneWireLocationOnTeensyPin = teensySearchForOneWireBusPin;
            oneWireBusFound = true;
            break;
        }
        else
        {
#if defined(DEBUG33)
            Serial.print(F("Did Not find any Temp Sensor(s) on the oneWire bus on Teensy's pin number: "));
            Serial.println(teensySearchForOneWireBusPin, DEC);
#endif
            oneWireBusFound = false;
        }
    }
    return oneWireBusFound;
}


/***************************************************************************************************************
 * This function finds the Temp Sensor(s) on the One Wire Bus Pin Number Passed in to the Function.
 * Needs:
 *   Nothing
 * Returns:
 *   True if a live One Wire Bus Device is found.
 *   False if no active One Wire Bus Device is found.
 ***************************************************************************************************************/
bool tftFindActiveOneWirePins(void)
{
    bool oneWireBusFound = false;
    
#if defined(DEBUG33)
    Serial.println(F("Start the search for Devices(s) on the OneWire bus... \n"));
#endif
    tft.println(F("Start the search for Devices(s) on the OneWire bus... \n"));
    
    for (teensySearchForOneWireBusPin = 0; teensySearchForOneWireBusPin < 33; teensySearchForOneWireBusPin++)
    {
        if (findDevices(teensySearchForOneWireBusPin))
        {
#if defined(DEBUG33)
            Serial.print(F("Found the Temp Sensor(s) on the oneWire bus on Teensy's pin number: "));
            Serial.println(teensySearchForOneWireBusPin, DEC);
#endif
            tft.print(F("Found the Temp Sensor(s) on the oneWire bus on Teensy's pin number: "));
            tft.println(teensySearchForOneWireBusPin, DEC);
            oneWireLocationOnTeensyPin = teensySearchForOneWireBusPin;
            oneWireBusFound = true;
            break;
        }
        else
        {
#if defined(DEBUG33)
            Serial.print(F("Did Not find any Temp Sensor(s) on the oneWire bus on Teensy's pin number: "));
            Serial.println(teensySearchForOneWireBusPin, DEC);
#endif
            tft.print(F("Did Not find any Temp Sensor(s) on the oneWire bus on Teensy's pin number: "));
            tft.println(teensySearchForOneWireBusPin, DEC);
            oneWireBusFound = false;
        }
    }
    return oneWireBusFound;
}


/***************************************************************************************************************
 * This function gets the count of the number of Temp Sensor Devices on the OneWire bus.
 * Needs:
 *   Nothing
 * Returns:
 *   Number of Devices found on the One Wire Bus.
 *   0 if no devices found.
 ***************************************************************************************************************/
uint8_t identifyCurrentTempDevicesOnBus(void)
{
    deviceCount = 0;
    
    // Get the count of the number of Temp Sensor Devices on the OneWire bus.
#if defined(DEBUG33)
    Serial.println(F("Get the count of the number of Temp Sensor Devices on the OneWire bus..."));
#endif
    deviceCount = tempSensors.getDeviceCount();
    
#if defined(DEBUG33)
    Serial.print(F("Number of Temp devices: "));
    Serial.println(deviceCount);
#endif
    
#if defined(DEBUG33)
    Serial.println();
    Serial.println(F("These are the current ID's of the Temp Sensors:"));
    for (uint8_t index = 0; index < deviceCount; index++)
    {
        DeviceAddress t;
        tempSensors.getAddress(t, index);
        printAddress(t);
        Serial.print(F("\t\tID: "));
        int id = tempSensors.getUserData(t);
        Serial.println(id);
    }
#endif
    
    return deviceCount;
}


/***************************************************************************************************************
 * This function gets the count of the number of Temp Sensor Devices on the OneWire bus.
 * Needs:
 *   Nothing
 * Returns:
 *   Number of Devices found on the One Wire Bus.
 *   0 if no devices found.
 ***************************************************************************************************************/
uint8_t tftIdentifyCurrentTempDevicesOnBus(void)
{
    deviceCount = 0;
    
    // Get the count of the number of Temp Sensor Devices on the OneWire bus.
#if defined(DEBUG33)
    Serial.println(F("Get the count of the number of Temp Sensor Devices on the OneWire bus..."));
#endif
    tft.println(F("Get the count of the number of Temp Sensor Devices on the OneWire bus..."));
    deviceCount = tempSensors.getDeviceCount();
    
#if defined(DEBUG33)
    Serial.print(F("Number of Temp devices: "));
    Serial.println(deviceCount);
#endif
    tft.print(F("Number of Temp devices: "));
    tft.println(deviceCount);
    
#if defined(DEBUG33)
    Serial.println();
    Serial.println(F("These are the current ID's of the Temp Sensors:"));
    for (uint8_t index = 0; index < deviceCount; index++)
    {
        DeviceAddress t;
        tempSensors.getAddress(t, index);
        printAddress(t);
        Serial.print(F("\t\tID: "));
        int id = tempSensors.getUserData(t);
        Serial.println(id);
    }
#endif
    
    tft.println();
    tft.println(F("These are the current ID's of the Temp Sensors:"));
    for (uint8_t index = 0; index < deviceCount; index++)
    {
        DeviceAddress t;
        tempSensors.getAddress(t, index);
        tftPrintAddress(t);
        tft.print(F("\t\tID: "));
        int id = tempSensors.getUserData(t);
        tft.println(id);
    }
    
    return deviceCount;
}


/***************************************************************************************************************
 * This function relabels the UserData word (int16_t) associated with each Temp Sensor Device on the OneWire bus.
 * It changes the UserData word (int16_t) of first Temp Sensor it finds, and relabels it with the number you enter,
 * then it proceeds to change all the other sensors UserData word to the next sequential number.  (0,1,2,3,4,5) or
 * (100, 101, 102, 103, 104, 105
 *
 * Needs:
 *   Nothing
 * Returns:
 *   Number of Devices found on the One Wire Bus.
 *   0 if no devices found.
 ***************************************************************************************************************/
uint8_t relabelTempSensorIDs(void)
{
    Serial.println();
    Serial.print(F("Enter the beginning temp sensor ID number for this group on the One Wire Bus: "));
    
    char c = 0;
    int16_t id = 0;  // The new device ID can be from 0 -> 32,767.
    
    while (!Serial.available());  // Wait for data from usb keyboard.
    
    while (c != '\n' && c != '\r')
    {
        c = Serial.read();
        Serial.print(c);  // Display it on monitor
        switch(c)
        {
            case '0'...'9':
                id *= 10;
                id += (c - '0');
                break;
            default:
                break;
        }
        while (!Serial.available());  // Wait for data from usb keyboard.
    }
    
    Serial.println();
    Serial.println(id);
    Serial.println();
    
    Serial.println(F("Start labeling the Temp sensors ..."));
    for(uint8_t index = 0; index < deviceCount; index++)
    {
        Serial.print(F("."));
        DeviceAddress t;
        if(!tempSensors.getAddress(t, index))
        {
            Serial.println(F("Error, Temp Sensor address was not found ..."));
        }
        tempSensors.setUserData(t, id);
        id++;
    }
    
    Serial.println();
    Serial.println(F("Show the results ..."));
    for(uint8_t index = 0; index < deviceCount; index++)
    {
        DeviceAddress t;
        tempSensors.getAddress(t, index);
        printAddress(t);
        Serial.print(F("\t\tID: "));
        int id = tempSensors.getUserData(t);
        Serial.println(id);
    }
    
    // Read ID's per sensor and put them in oneWireTemps array.
    for(uint8_t index = 0; index < deviceCount; index++)
    {
        // go through sensors
        tempSensors.getAddress(oneWireTemps[index].addr, index);
        oneWireTemps[index].id = tempSensors.getUserData(oneWireTemps[index].addr);
    }
    
    Serial.println(F("Done ..."));
    
    return deviceCount;
}



/***************************************************************************************************************
 * This function relabels the UserData word (int16_t) associated with each Temp Sensor Device on the OneWire bus.
 * It changes the UserData word (int16_t) of first Temp Sensor it finds, and relabels it with the number you enter,
 * then it proceeds to change all the other sensors UserData word to the next sequentia number.  (0,1,2,3,4,5) or
 * (100, 101, 102, 103, 104, 105
 *
 * Needs:
 *   Nothing
 * Returns:
 *   Number of Devices found on the One Wire Bus.
 *   0 if no devices found.
 ***************************************************************************************************************/
uint8_t tftRelabelTempSensorIDs(void)
{
#if defined(DEBUG33)
    Serial.println();
    Serial.print(F("Enter the beginning temp sensor ID number for this group on the One Wire Bus: "));
#endif
    
    tft.println();
    tft.print(F("Enter the beginning temp sensor ID number for this group on the One Wire Bus: "));
    
    char c = 0;
    int16_t id = 0;  // The new device ID can be from 0 -> 32,767.
    
    while (!Serial.available());  // Wait for data from usb keyboard.
    
    while (c != '\n' && c != '\r')
    {
        c = Serial.read();
        Serial.print(c);  // Display it on monitor
        switch(c)
        {
            case '0'...'9':
                id *= 10;
                id += (c - '0');
                break;
            default:
                break;
        }
        while (!Serial.available());  // Wait for data from usb keyboard.
    }
    
    Serial.println();
    Serial.println(id);
    Serial.println();
    
#if defined(DEBUG33)
    Serial.println(F("Start labeling the Temp sensors ..."));
#endif
    tft.println(F("Start labeling the Temp sensors ..."));
    for (uint8_t index = 0; index < deviceCount; index++)
    {
#if defined(DEBUG33)
        Serial.print(F("."));
#endif
        tft.print(F("."));
        DeviceAddress t;
        if (!tempSensors.getAddress(t, index))
        {
#if defined(DEBUG33)
            Serial.println(F("Error, Temp Sensor address was not found ..."));
#endif
            tft.println(F("Error, Temp Sensor address was not found ..."));
        }
        tempSensors.setUserData(t, id);
        id++;
    }
    
#if defined(DEBUG33)
    Serial.println();
    Serial.println(F("Show the results ..."));
    for (uint8_t index = 0; index < deviceCount; index++)
    {
        DeviceAddress t;
        tempSensors.getAddress(t, index);
        printAddress(t);
        Serial.print(F("\t\tID: "));
        int id = tempSensors.getUserData(t);
        Serial.println(id);
    }
    Serial.println(F("Done ..."));
#endif
    
    tft.println();
    tft.println(F("Show the results ..."));
    for (uint8_t index = 0; index < deviceCount; index++)
    {
        DeviceAddress t;
        tempSensors.getAddress(t, index);
        tftPrintAddress(t);
        tft.print(F("\t\tID: "));
        int id = tempSensors.getUserData(t);
        tft.println(id);
    }
    tft.println(F("Done ..."));
    
    return deviceCount;
}


/***************************************************************************************************************
 * This function prints on the TFT the Temperature associated with each Temp Sensor Device on the OneWire bus.
 *
 * Needs:
 *   Nothing.
 * Returns:
 *   Nothing.
 ***************************************************************************************************************/
void getDS18B20TempsIntoStruct(void)
{
    //  Now print the actual Temps from the Sensors.
    for (uint8_t id = 0; id < deviceCount; id++)
    {
        oneWireTemps[id].tempData = (getTempByID(id));  // Fill the Struct with Temp Data from Sensors.
    }
    
    freshTempsAvailableFlag = true;  // This Flag means there is new temp Data from the DS18B20 Sensors.
}


/***************************************************************************************************************
 * This function prints on the TFT the Temperature associated with each Temp Sensor Device on the OneWire bus.
 *
 * Needs:
 *   X Position to start printing
 *   Y Position to start printing
 * Returns:
 *   Nothing.
 ***************************************************************************************************************/
void printDS18B20TempsOnTFT(uint16_t x, uint16_t y)
{
    // Move Cursor intial coordinates
    tft.setCursor (x, y);
    
    tft.setFontScale(0);  // Set Font to normal size (8W x 16T pixels).
    
    tft.print(F("DS18B20 Sensors:"));  // Print the Heading.
    
    tft.setCursor ( ( x + ( Xsize / 2 ) ), ( y + ( Ysize / 2 ) ) );  // Step down one line (Y) and over one char(X) to print the Labels
    tft.print(F("ID's   Temps:"));  // Print the Heading.
    
    tft.setCursor ( ( x + ( ( Xsize / 2 )  * 2) ), ( y + ( ( Ysize / 2 ) * 2) ) );  // Step down two lines (Y) and over two chars(X) to clear the data.
    // First Clear the data printed previously
    for (uint8_t index = 0; index < deviceCount; index++)
    {
        tft.print(F("             "));  // Clear 13 Characters from TFT.
        tft.setCursor ( x + ( ( Xsize / 2 ) * 2 ), ( y + ( ( Ysize / 2 ) * (index + 2) ) ) );
    }
    
    tft.setCursor( ( x + ( ( Xsize / 2 ) * 2) ), ( y + ( ( Ysize / 2 ) * 2) ) );  // Step down two lines (Y) and over one char(X) to print the ID's and Temperatures.
    
    //  Now print the actual Temps from the Sensors.
    for (uint8_t id = 0; id < deviceCount; id++)
    {
        if (oneWireTemps[id].tempData == -999)  // -999 if no device
        {
            tft.print(F("Error, Temp Sensor returned Bad Value or No Device"));
        }
        else
        {
            tft.print(oneWireTemps[id].id);
            tft.print(F("     "));  // Add 5 spaces to align.
            tft.print(oneWireTemps[id].tempData);
        }
        tft.setCursor ( (x + ( ( Xsize / 2 ) * 2) ), ( y + ( ( Ysize / 2 ) * (id + 3) ) ) );
    }
    tft.setFontScale(1);  // Set Font to 2x normal size (16W x 32T pixels).
}


/***************************************************************************************************************
 * This function prints on the Monitor the Temperature associated with each Temp Sensor Device on the OneWire bus.
 *
 * Needs:
 *   Nothing
 * Returns:
 *   Nothing.
 ***************************************************************************************************************/
void printDS18B20TempsOnMonitor(void)
{
    Serial.println();
    Serial.println(F("\t\tGet temp by ID"));  // assume ID = 0 ... 16
    Serial.println(F("Sensor ID's\tTemp Value"));
    for (int id = 0; id < deviceCount; id++)
    {
        Serial.print(F("\t"));  // One Tab (4 spaces).
        Serial.print(oneWireTemps[id].id);
        Serial.print(F("\t\t  "));  // Two Tabs (8 spaces) and two spaces.
        
        if (oneWireTemps[id].tempData == -999)  // -999 if no device
        {
            Serial.print(F("Error, Temp Sensor returned Bad Value or No Device"));
        }
        else
        {
            Serial.println(oneWireTemps[id].tempData);
        }
    }
    Serial.println();
}
#endif


/***************************************************************************************************************
*           This function puts the rainfall data info on the screen where ever you specify.
*    TODO: Convert Imperial (in) to Metric (mm) or multiply by 25.4.
* Needs:
*   xPos - X Position on TFT screen (0 -> 49 Columns) or (0 -> 799 pixels).
*   yPos - Y Position on TFT screen (0 -> 14 Rows) or (0 -> 479 pixels).
*
* Returns;
*   Nothing.
***************************************************************************************************************/
void drawRainSensorInfo(uint16_t xPos, uint16_t yPos)
{
    if (WeatherDataStruct.hourlyRainTotal > 0)
        {
            tft.setCursor (xPos, yPos);
            tft.print(F("Rain/Hr: "));  // 9 characters.
            tft.print("      ");  // 6 spaces.
            tft.setCursor (xPos + (Xsize * 10), yPos);
            tft.println(WeatherDataStruct.hourlyRainTotal,2);  // Hourly Rain Total is a float.
        }
    else  // Blank out the hourly rain fall number from screen.
        {
            tft.setCursor (xPos, yPos);
            tft.print("               ");  // 15 spaces.
        }
    
    if (WeatherDataStruct.dailyRainTotal > 0)
        {
            tft.setCursor ( xPos, (yPos + ( Ysize )) );
            tft.print(F("Rain/Day: "));  // 10 characters.
            tft.print("      ");  // 6 spaces.
            tft.setCursor (xPos + (Xsize * 10), (yPos + ( Ysize )));
            tft.println(WeatherDataStruct.dailyRainTotal,2);  // Daily Rain Total is a float.
        }
    else  // Blank out the Daily rain fall number from screen.
        {
            tft.setCursor (xPos, (yPos + ( Ysize )));
            tft.print("                ");  // 16 spaces.
        }
    
    if ( ( WeatherDataStruct.yearlyRainTotal + rainYearlyOffset ) > 0)
        {
            tft.setCursor (xPos, (yPos + ( Ysize * 2 )));
            tft.print(F("Rain/Year: "));  // 11 characters.
            tft.print("      ");  // 6 spaces.
            tft.setCursor (xPos + (Xsize * 10), (yPos + ( Ysize * 2 )));
            tft.println( ( WeatherDataStruct.yearlyRainTotal + rainYearlyOffset ),2);  // Yearly Rain Total is a float.
        }
    else  // Blank out the Yearly rain fall number from screen.
        {
            tft.setCursor (xPos, (yPos + ( Ysize * 2 )));
            tft.print("                 ");  // 17 spaces.
        }
    return;
}


/***************************************************************************************************************
* This function blanks out the Wind Direction Flower and the Wind Data from the screen, leaving "No Wind" at the
* top left corner.
* Input:
*       Pos[] array.
*       xPos
*       yPos
* pos[0] is center x of the Wind Direction Flower circle.
* pos[1] is center y of the Wind Direction Flower circle.
* pos[2] is the radius of the Wind Direction Flower circle.
* xPos is the x Position on the screen to start printing the text.
* yPos is the y Position of the screen to start printing the text.
***************************************************************************************************************/
void drawBlankWind(int16_t pos[], uint16_t xPos, uint16_t yPos)
{
    int16_t xBlankStart, xWidth, yBlankStart, yHeight;
    
    tft.setCursor (xPos, yPos);
    tft.print("                   ");  // Clear the previous text from screen
    tft.setCursor (xPos, yPos);
    tft.println(F("No Wind!"));
    tft.setCursor (xPos, (yPos + Ysize));  // Need to blank the next four readings off the screen while there is no wind.
    tft.println("                    ");  // 20 spaces to remove all the text.
    tft.println("                ");  // 16 spaces to remove all the text.
    
    xBlankStart = 0;  // Start at left of screen.
    
    xWidth = (pos[2] * 2) + 34;  // Two small characters extra plus 26 pixels safety zone.
    
    yBlankStart = (pos[1] - pos[2]) - 20;
    
    yHeight = (pos[2] * 2) + 56;  // Two small characters extra plus 2 pixels safety zone.
    
    tft.fillRect(xBlankStart, yBlankStart, xWidth, yHeight, RA8875_BLACK);  // Remove the Wind Direction Flower Gauge from the screen.
}

/***************************************************************************************************************
* This function puts the wind speed and directon info on the screen where ever you specify by xPos & yPos.
*   TODO: Convert from Imperial (mph) to Metric (m/s).
***************************************************************************************************************/
void drawPrintWind(uint16_t xPos, uint16_t yPos, float WindSpd, float MaxWindSpeed, float WindDir)
{
    tft.setCursor (xPos, yPos);
    tft.print(F("Ave Wind Spd: "));
    tft.print("      ");
    tft.setCursor (xPos + (Xsize * 14), yPos);
    tft.println(WindSpd, 2);  // Wind Speed is a float, only need 2 decimal places.
    
    tft.setCursor (xPos, (yPos + Ysize));
    tft.print(F("Max Wind Spd: "));
    tft.print("      ");
    tft.setCursor (xPos + (Xsize * 14), (yPos + Ysize));
    tft.println(MaxWindSpeed, 2);  // Max wind Speed is a float, only need 2 decimal places.
    
    tft.print(F("Wind Dir: "));
    tft.print("      ");
    tft.setCursor (xPos + (Xsize * 10), yPos + (Ysize * 2));
    tft.println(WindDir, 1);  // Wind Direction is a float.
    
    
}


/***************************************************************************************************************
*       This function prints a Red or Green Dot just below the 12 of the Clock Face on the TFT Screen.
*       This indicates whether or not the RTC clock has been set to UTC from the Gps
*   Needs:
*       Xpos - The X position on the TFT which is the center of the Dot.
*       Ypos - The Y position on the TFT which is the center of the Dot.
*       dotColor - The color of the Dot
*
*   Returns:
*       Nothing
***************************************************************************************************************/
void printUTCStatusDotOnTFT(uint16_t xPos, uint16_t yPos, uint16_t dotColor )
{
    tft.fillCircle(xPos, yPos, 6, dotColor);
}


#if defined(USINGGPS)



    /*******************************************************************************************************************
    * This Function blanks out the Lat, Long, etc on the TFT monitor.
    *
    * Needs:
    *   X Position to start printing
    *   Y Position to start printing
    * Returns:
    *   Nothing.
    *******************************************************************************************************************/
    void blankOutGPSPositionOnTFT(uint16_t x, uint16_t y)
    {
        // Move Cursor to initial coordinates
        tft.setFontScale(0);  // Set Font to normal size (8W x 16T pixels).
    
        tft.setCursor (x, y );
        tft.print(F("                 "));  // Blank out the first line with 17 spaces.
    
        tft.setCursor ( x, ( y + ( Ysize / 2 ) ) );
        tft.print(F("                "));  // Blank out the second line with 16 spaces.
    
        tft.setCursor ( x, ( y + ( 2 * ( Ysize / 2 ) ) ) );
        tft.print(F("                "));  // Blank out the third line with 16 spaces.
    
        tft.setCursor ( x, ( y + ( 3 * ( Ysize / 2 ) ) ) );
        tft.print(F("                "));  // Blank out the fourth line with 16 spaces.
    
        tft.setCursor ( x, ( y + ( 4 * ( Ysize / 2 ) ) ) );
        tft.print(F("                "));  // Blank out the fifth line with 16 spaces.
    
        tft.setCursor ( x, ( y + ( 5 * ( Ysize / 2 ) ) ) );
        tft.print(F("                "));  // Blank out the sixth line with 16 spaces.
    
        tft.setFontScale(1);  // Set Font to 2x normal size (16W x 32T pixels).
    
    }




    /*******************************************************************************************************************
     * This Function prints the Lat, Long, etc on the TFT monitor.
     *
     * Needs:
     *   X Position to start printing
     *   Y Position to start printing
     * Returns:
     *   Nothing.
     *******************************************************************************************************************/
    void printGPSPositionOnTFT(uint16_t x, uint16_t y, uint8_t validityCode)
    {
        blankOutGPSPositionOnTFT(x, y);  // Blank out the entire GPS Data from the TFT.
    
        // Move Cursor to initial coordinates
        tft.setFontScale(0);  // Set Font to normal size (8W x 16T pixels).
    
        tft.setCursor (x, y);
    
        tft.print(F("FixType: "));  // Print the Heading.
        if ( validityCode == 2 )
            {
                tft.print(F("2D-Fix"));  // Print the Fix Type.
            }
        else if ( validityCode == 3 )
            {
                tft.print(F("3D-Fix"));  // Print the Fix Type.
            }
    
        tft.setCursor ( x, ( y + ( Ysize / 2 ) ) );  // Step down one line (Y) to print the Longitude.
        tft.print(F("Sats: "));  // Print the Heading.
        tft.print(uBloxData.numSV,DEC);  // Print the Number of Satellites Used.
    
        tft.setCursor ( x, ( y + ( 2 * ( Ysize / 2 ) ) ) );  // Step down one line (Y) to print the Longitude.
        tft.print(F("Lat: "));  // Print the Heading.
        tft.print(uBloxData.lat,3);  // Print the Latitude.
    
        tft.setCursor ( x, (y + (3 * ( Ysize / 2 )) ) );  // Step down two line (Y) to print the Longitude.
        tft.print(F("Lon: "));  // Print the Heading.
        tft.print(uBloxData.lon,3);  // Print the Longitude.
    
        tft.setCursor ( x, (y + (4 * ( Ysize / 2 )) ) );  // Step down three lines (Y) to print the Altitude.
        tft.print(F("Alt: "));  // Print the Heading.
        tft.print( ( 3.281 * (uBloxData.hMSL) ),1 );  // Print the Altitude, for an approximate result, multiply the length value by 3.281.
    
        tft.setCursor ( x, (y + (5 * ( Ysize / 2 )) ) );  // Step down four lines (Y) to print the Altitude.
        tft.print(F("Spd: "));  // Print the Speed in MPH.
        tft.print( ( 2.237 * (uBloxData.gSpeed) ),1 );  // Print the speed, multiply the speed value by 2.237 to get miles per hour.
    
        tft.setFontScale(1);  // Set Font to 2x normal size (16W x 32T pixels).
    
    }


/*******************************************************************************************************************
* This Function clears the local char(8 bit) buffer provided, of data (Set to zero).
*******************************************************************************************************************/
void clearBuffer(uint8_t *LocalBufferAddress, int16_t LocalBufferSize)
{
    #if defined (DEBUG2)
        Serial.print(F("\nClearing Buffer, its Size is: "));
        Serial.print(LocalBufferSize, DEC);
    #endif
    
    memset(LocalBufferAddress, 0, LocalBufferSize);
    
    //for (uint16_t i = 0; i < LocalBufferSize; i++ )
        //{
            //LocalBufferAddress[i] = 0;  // Clear the data in the array.
        //}
}


/***************************************************************************************************************
* This function updates the Location Data on the TFT from the GPS.
*
*******************************************************************************************************
*               High level Commands, for the user ~ UBLOX lib. v1.0.2 2018-03-20 *
*******************************************************************************************************
*     NOTE: gps.command(Boolean) == (true)Print Message Acknowledged on USB Serial Monitor
* end();                            // Disables Teensy serial communication, to re-enable, call begin(Baud)
* Poll_CFG_Port1(bool);             // Polls the configuration for one I/O Port, I/O Target 0x01=UART1
* Poll_NAV_PVT();                   // Polls UBX-NAV-PVT    (0x01 0x07) Navigation Position Velocity Time Solution
* Poll_NAV_POSLLH();                // Polls UBX-NAV-POSLLH (0x01 0x02) Geodetic Position Solution
* Poll_NAV_ATT();                   // Polls UBX-NAV-ATT    (0x01 0x05) Attitude Solution
*
* ### Periodic Auto Update ON,OFF Command ###
* Ena_NAV_PVT(bool);                // Enable periodic auto update NAV_PVT
* Dis_NAV_PVT(bool);                // Disable periodic auto update NAV_PVT
* Ena_NAV_ATT(bool);                // Enable periodic auto update NAV_ATT
* Dis_NAV_ATT(bool);                // Disable periodic auto update NAV_ATT
* Ena_NAV_POSLLH(bool);             // Enable periodic auto update NAV_POSLLH
* Dis_NAV_POSLLH(bool);             // Disable periodic auto update NAV_POSLLH
*
* #### u-blox Switch off all NMEA MSGs ####
* Dis_all_NMEA_Child_MSGs(bool);    // Disable All NMEA Child Messages Command
*
* ### High level Command Generator ###
* SetGPSbaud(uint32_t baud, bool)   // Set UBLOX GPS Port Configuration Baud rate
* SetNAV5(uint8_t dynModel, bool)   // Set Dynamic platform model Navigation Engine Settings (0:portable, 3:pedestrian, Etc)
* SetRATE(uint16_t measRate, bool)  // Set Navigation/Measurement Rate Settings (100ms=10.00Hz, 200ms=5.00Hz, 1000ms=1.00Hz, Etc)
*
*   UBX-NAV-PVT --  Navigation Position Velocity Time Solution
*   ### UBX Protocol, Class NAV 0x01, ID 0x07 ###
*   UBX-NAV-PVT (0x01 0x07)    (Payload U-blox-M8=92, M7&M6=84)
*   iTOW                       ///< [ms], GPS time of the navigation epoch
*   utcYear                    ///< [year], Year (UTC)
*   utcMonth                   ///< [month], Month, range 1..12 (UTC)
*   utcDay                     ///< [day], Day of month, range 1..31 (UTC)
*   utcHour                    ///< [hour], Hour of day, range 0..23 (UTC)
*   utcMin                     ///< [min], Minute of hour, range 0..59 (UTC)
*   utcSec                     ///< [s], Seconds of minute, range 0..60 (UTC)
*   valid                      ///< [ND], Validity flags
*   tAcc                       ///< [ns], Time accuracy estimate (UTC)
*   utcNano                    ///< [ns], Fraction of second, range -1e9 .. 1e9 (UTC)
*   fixType                    ///< [ND], GNSSfix Type: 0: no fix, 1: dead reckoning only, 2: 2D-fix, 3: 3D-fix, 4: GNSS + dead reckoning combined, 5: time only fix
*   flags                      ///< [ND], Fix status flags
*   flags2                     ///< [ND], Additional flags
*   numSV                      ///< [ND], Number of satellites used in Nav Solution
*   lon                        ///< [deg], Longitude
*   lat                        ///< [deg], Latitude
*   height                     ///< [m], Height above ellipsoid
*   hMSL                       ///< [m], Height above mean sea level
*   hAcc                       ///< [m], Horizontal accuracy estimate
*   vAcc                       ///< [m], Vertical accuracy estimate
*   velN                       ///< [m/s], NED north velocity
*   velE                       ///< [m/s], NED east velocity
*   velD                       ///< [m/s], NED down velocity
*   gSpeed                     ///< [m/s], Ground Speed (2-D)
*   heading                    ///< [deg], Heading of motion (2-D)
*   sAcc                       ///< [m/s], Speed accuracy estimate
*   headingAcc                 ///< [deg], Heading accuracy estimate (both motion and vehicle)
*   pDOP                       ///< [ND], Position DOP
*   headVeh                    ///< [deg], Heading of vehicle (2-D)             #### NOTE: u-blox8 only ####
*   --- magDec, magAcc --- TODO TEST
*   magDec                     ///< [deg], Magnetic declination                 #### NOTE: u-blox8 only ####
*   magAcc                     ///< [deg], Magnetic declination accuracy        #### NOTE: u-blox8 only ####
*
* Needs:
*   Nothing
* Returns:
*       GNSSfix Type:
*       0: No fix
*       1: Dead reckoning only
*       2: 2D-fix
*       3: 3D-fix
*       4: GNSS + dead reckoning combined
*       5: Time only fix
***************************************************************************************************************/
uint8_t updatePositionFromGPS(void)
{
    uint8_t fixType;
    
    #if defined(DEBUG34)
        int rd;
        byte buffer[512];  // Serial1 Recieve Buffer is set to 512 Bytes.
    
        Serial.print(F("This is the Size of the uBloxData Struct: "));
        Serial.println(sizeof(uBloxData));  // Print the size of the uBloxData Struct.
    
        // check if any data has arrived on the hardware serial port
        rd = Serial1.available();
        Serial.print(F("Amount of Serial Data Available in Serial1: "));
        Serial.println(rd);
    
        if (rd > 0)
            {
                printLocalBuffers(buffer, sizeof(buffer));
                Serial1.readBytes((char *)buffer, rd);
                clearBuffer(buffer, sizeof(buffer));
                Serial.println();
            }
        return 0;
    #endif
    
    
    // GNSSfix Type:
    //  0: no fix
    //  1: dead reckoning only
    //  2: 2D-fix
    //  3: 3D-fix
    //  4: GNSS + dead reckoning combined
    //  5: time only fix
    //  6: Bad Ublox Packet
    
    if ( gps.read( &uBloxData ) )  // Returns false if a full packet is not received.
        {
            fixType = ( uBloxData.fixType );  // Strip off unneeded bits.
        
            #if defined(DEBUG34)
                Serial.print(F("uBlox Fix Type: "));
                Serial.println(fixType, HEX);  /// Print the fixType code to the terminal.
            #endif

            switch (fixType)
                {
                    case NO_FIX:
                        blankOutGPSPositionOnTFT(( XRIGHT - ( Xsize * 10) ), ( YMIDDLE ) );  // Blank out the entire GPS Data from the TFT.
                        tft.setCursor(( XRIGHT - ( Xsize * 10) ), ( YMIDDLE + Ysize ) );
                        tft.print(F("NO FIX"));  // Print the Heading.
                        return NO_FIX;
                        
                    case DEAD_RECKONING_ONLY:
                        blankOutGPSPositionOnTFT(( XRIGHT - ( Xsize * 10) ), ( YMIDDLE ) );  // Blank out the entire GPS Data from the TFT.
                        tft.setCursor(( XRIGHT - ( Xsize * 10) ), ( YMIDDLE + Ysize ) );
                        tft.print(F("DR ONLY"));  // Print the Heading.
                        return DEAD_RECKONING_ONLY;
                        
                    case TWO_D_FIX:
                        blankOutGPSPositionOnTFT(( XRIGHT - ( Xsize * 10) ), ( YMIDDLE ));  // Blank out the entire GPS Data from the TFT.
                        printGPSPositionOnTFT( ( XRIGHT - ( Xsize * 9 ) ), ( YMIDDLE ), fixType );  // Print the Lat, Long, etc on the TFT.
                        return TWO_D_FIX;
                        
                    case THREE_D_FIX:
                        blankOutGPSPositionOnTFT(( XRIGHT - ( Xsize * 10) ), ( YMIDDLE ));  // Blank out the entire GPS Data from the TFT.
                        printGPSPositionOnTFT( ( XRIGHT - ( Xsize * 9 ) ), ( YMIDDLE ), fixType  );  // Print the Lat, Long, etc on the TFT.
                        return THREE_D_FIX;
                        
                    case GNSS_DR_COMBINED:
                        blankOutGPSPositionOnTFT(( XRIGHT - ( Xsize * 10) ), ( YMIDDLE ) );  // Blank out the entire GPS Data from the TFT.
                        tft.setCursor(( XRIGHT - ( Xsize * 10) ), ( YMIDDLE + Ysize ) );
                        tft.print(F("GNS/DR COM"));  // Print the Heading.
                        return GNSS_DR_COMBINED;
                        
                    case TIME_FIX_ONLY:
                        blankOutGPSPositionOnTFT(( XRIGHT - ( Xsize * 10) ), ( YMIDDLE ) );  // Blank out the entire GPS Data from the TFT.
                        tft.setCursor(( XRIGHT - ( Xsize * 10) ), ( YMIDDLE + Ysize ) );
                        tft.print(F("T FIX ONLY"));  // Print the Heading.
                        return TIME_FIX_ONLY;
                        
                    default:
                        return NO_FIX;
                }
        }
    else
        {
            #if defined(DEBUG34)
                Serial.println(F("uBlox did not recieve a Full Packet!"));
            #endif
            blankOutGPSPositionOnTFT(( XRIGHT - ( Xsize * 10) ), ( YMIDDLE ) );  // Blank out the entire GPS Data from the TFT.
            tft.setCursor( ( XRIGHT - ( Xsize * 10) ), ( YMIDDLE + Ysize ) );
            tft.print(F("BAD PKT"));  // Print the Heading.
            return BAD_UBLOX_PACKET;
        }
    
}


/***************************************************************************************************************
* This function updates the internal clock of the DS3231 Clock from the GPS Time.
*
*******************************************************************************************************
*               High level Commands, for the user ~ UBLOX lib. v1.0.2 2018-03-20 *
*******************************************************************************************************
*     NOTE: gps.command(Boolean) == (true)Print Message Acknowledged on USB Serial Monitor
* end();                            // Disables Teensy serial communication, to re-enable, call begin(Baud)
* Poll_CFG_Port1(bool);             // Polls the configuration for one I/O Port, I/O Target 0x01=UART1
* Poll_NAV_PVT();                   // Polls UBX-NAV-PVT    (0x01 0x07) Navigation Position Velocity Time Solution
* Poll_NAV_POSLLH();                // Polls UBX-NAV-POSLLH (0x01 0x02) Geodetic Position Solution
* Poll_NAV_ATT();                   // Polls UBX-NAV-ATT    (0x01 0x05) Attitude Solution
*
* ### Periodic Auto Update ON,OFF Command ###
* Ena_NAV_PVT(bool);                // Enable periodic auto update NAV_PVT
* Dis_NAV_PVT(bool);                // Disable periodic auto update NAV_PVT
* Ena_NAV_ATT(bool);                // Enable periodic auto update NAV_ATT
* Dis_NAV_ATT(bool);                // Disable periodic auto update NAV_ATT
* Ena_NAV_POSLLH(bool);             // Enable periodic auto update NAV_POSLLH
* Dis_NAV_POSLLH(bool);             // Disable periodic auto update NAV_POSLLH
*
* #### u-blox Switch off all NMEA MSGs ####
* Dis_all_NMEA_Child_MSGs(bool);    // Disable All NMEA Child Messages Command
*
* ### High level Command Generator ###
* SetGPSbaud(uint32_t baud, bool)   // Set UBLOX GPS Port Configuration Baud rate
* SetNAV5(uint8_t dynModel, bool)   // Set Dynamic platform model Navigation Engine Settings (0:portable, 3:pedestrian, Etc)
* SetRATE(uint16_t measRate, bool)  // Set Navigation/Measurement Rate Settings (100ms=10.00Hz, 200ms=5.00Hz, 1000ms=1.00Hz, Etc)
*
*   UBX-NAV-PVT --  Navigation Position Velocity Time Solution
*   ### UBX Protocol, Class NAV 0x01, ID 0x07 ###
*   UBX-NAV-PVT (0x01 0x07)    (Payload U-blox-M8=92, M7&M6=84)
*   iTOW                       ///< [ms], GPS time of the navigation epoch
*   utcYear                    ///< [year], Year (UTC)
*   utcMonth                   ///< [month], Month, range 1..12 (UTC)
*   utcDay                     ///< [day], Day of month, range 1..31 (UTC)
*   utcHour                    ///< [hour], Hour of day, range 0..23 (UTC)
*   utcMin                     ///< [min], Minute of hour, range 0..59 (UTC)
*   utcSec                     ///< [s], Seconds of minute, range 0..60 (UTC)
*   valid                      ///< [ND], Validity flags
*   tAcc                       ///< [ns], Time accuracy estimate (UTC)
*   utcNano                    ///< [ns], Fraction of second, range -1e9 .. 1e9 (UTC)
*   fixType                    ///< [ND], GNSSfix Type: 0: no fix, 1: dead reckoning only, 2: 2D-fix, 3: 3D-fix, 4: GNSS + dead reckoning combined, 5: time only fix
*   flags                      ///< [ND], Fix status flags
*   flags2                     ///< [ND], Additional flags
*   numSV                      ///< [ND], Number of satellites used in Nav Solution
*   lon                        ///< [deg], Longitude
*   lat                        ///< [deg], Latitude
*   height                     ///< [m], Height above ellipsoid
*   hMSL                       ///< [m], Height above mean sea level
*   hAcc                       ///< [m], Horizontal accuracy estimate
*   vAcc                       ///< [m], Vertical accuracy estimate
*   velN                       ///< [m/s], NED north velocity
*   velE                       ///< [m/s], NED east velocity
*   velD                       ///< [m/s], NED down velocity
*   gSpeed                     ///< [m/s], Ground Speed (2-D)
*   heading                    ///< [deg], Heading of motion (2-D)
*   sAcc                       ///< [m/s], Speed accuracy estimate
*   headingAcc                 ///< [deg], Heading accuracy estimate (both motion and vehicle)
*   pDOP                       ///< [ND], Position DOP
*   headVeh                    ///< [deg], Heading of vehicle (2-D)             #### NOTE: u-blox8 only ####
*   --- magDec, magAcc --- TODO TEST
*   magDec                     ///< [deg], Magnetic declination                 #### NOTE: u-blox8 only ####
*   magAcc                     ///< [deg], Magnetic declination accuracy        #### NOTE: u-blox8 only ####
*
* Needs:
*   Nothing
* Returns:
*   boolean:
*       false if success.
*       true if error.
***************************************************************************************************************/
bool updateTimeFromGPS(void)
{
    int WeekDay;
    uint8_t validityCode;
    tmElements_t tmElementsStruct;
    time_t tempTimeTStorage;
    
    #if defined(DEBUG28)
        Serial.println("\nThis is the updateTimeFromGPS function...");
        Serial.print(F("This is the Size of the uBloxData Struct: "));
        Serial.println(sizeof(uBloxData));  // Print the size of the uBloxData Struct.
    #endif
    
    #if defined(DEBUG29)
        int rd;
        byte buffer[256];
    
    
        // check if any data has arrived on the hardware serial port
        rd = Serial1.available();
        Serial.print(F("\nThis is the updateTimeFromGPS function: Amount of Serial Data Available in Serial1: "));
        Serial.println(rd);
    
        Serial.print(F("This is the Size of the uBloxData Struct: "));
        Serial.println(sizeof(uBloxData));  // Print the size of the uBloxData Struct.
    
        if (rd > 0)
            {
                Serial1.readBytes((char *)buffer, rd);
                printLocalBuffers(buffer, rd);
                clearBuffer(buffer, sizeof(buffer));
                Serial.println();
                
            }
        return false;
    #endif
    
    // Read the DS1307 RTC to get the Day of the Week
    if (!RTC.read(tmElementsStruct))
        {
            #if defined(DEBUG28)
                Serial.println(F("\nThis is the updateTimeFromGPS function: No DS1307 Present on the Bus!"));
            #endif
            return true;
        }
    WeekDay = tmElementsStruct.Wday;  // Save the Week Day in this temp location.
    
    #if defined(DEBUG28)
        Serial.println(F("\nThis is the updateTimeFromGPS function: gps.read(&uBloxData)"));
    #endif
    if ( gps.read(&uBloxData) )  // Returns false if a full packet is not received.
        {
            validityCode = ( (uBloxData.valid) & (0x0F) );  // Strip off unneeded bits.
            if( (validityCode == 0x07) || (validityCode == 0x03) )  // These codes mean the time & date are accurate.
                {
                    #if defined(DEBUG28)
                        Serial.print(F("\nThis is the updateTimeFromGPS function: Found Good Validity Code: "));
                        Serial.println(validityCode, HEX);
                    #endif
                    
                    tmElementsStruct.Hour = uBloxData.utcHour;
                    tmElementsStruct.Minute = uBloxData.utcMin;
                    tmElementsStruct.Second = (uBloxData.utcSec + 2);  // This compensates for the lost 2 seconds between the functions two second.
                    
                    tmElementsStruct.Day = uBloxData.utcDay;
                    tmElementsStruct.Month = uBloxData.utcMonth;
                    tmElementsStruct.Year = CalendarYrToTm(uBloxData.utcYear);
            
                    /*----------------------------------------------------------------------*
                     * Convert the given UTC time to local time, standard or                *
                     * daylight time, as appropriate.                                       *
                     *----------------------------------------------------------------------*/
                    tempTimeTStorage = makeTime(tmElementsStruct);  // Converts tmElements struct into a Time_T (UNIX Time), LastTimeSetByClock is a Time_T (UNIX Time) variable.
                    tempTimeTStorage = (*tz).toLocal(tempTimeTStorage, &tcr);  // This will convert the clock Time to local time.
                    breakTime(tempTimeTStorage, tmElementsStruct);  // This Converts the Time_t Unix Time to a tmElements struct.
            
            
                    tmElementsStruct.Wday = WeekDay;
            
                    #if defined(DEBUG28)
                        Serial.print(uBloxData.utcDay);   ///< [day], Day of month, range 1..31 (UTC)
                        Serial.print(F("/"));
                        Serial.print(uBloxData.utcMonth); ///< [month], Month, range 1..12 (UTC)
                        Serial.print(F("/"));
                        Serial.print(uBloxData.utcYear);  ///< [year], Year (UTC)
                    
                        Serial.print(F("\t"));
                    
                        Serial.print(uBloxData.utcHour);  ///< [hour], Hour of day, range 0..23 (UTC)
                        Serial.print(F(":"));
                        Serial.print(uBloxData.utcMin);   ///< [min], Minute of hour, range 0..59 (UTC)
                        Serial.print(F(":"));
                        Serial.print(uBloxData.utcSec);   ///< [s], Seconds of minute, range 0..60 (UTC)
                    
                        Serial.print(F("\t"));
                        Serial.print(F("Validity Data: "));
                        Serial.print(uBloxData.valid,HEX);   ///< [s], Seconds of minute, range 0..60 (UTC)
                        Serial.println(F(" HEX"));
                    #endif
            
                    // and configure the DS1307 RTC with this info
                    if (!RTC.write(tmElementsStruct))
                        {
                            #if defined(DEBUG28)
                                Serial.println(F("No DS1307 Present on the Bus!\n"));
                            #endif
                            return true;
                        }
            
                    statusDotColor = DOT_GREEN;
                    UTCTimeCorrected = true;
                    #if defined(DEBUG28)
                        Serial.println(F("Setting DS1307 Time from GPS was Successful!"));
                        Serial.print(F("statusDotColor is: "));
                        Serial.println(statusDotColor);
                        Serial.print(F("UTCTimeCorrected flag: "));
                        Serial.println(UTCTimeCorrected);
                        Serial.println(F(""));
                    #endif
                    return false;  // Success in setting time from GPS!
                }
            else
                {
                    statusDotColor = DOT_YELLOW;
                    UTCTimeCorrected = false;
                    #if defined(DEBUG28)
                        Serial.println(F("uBlox Data is NOT Valid!"));
                        Serial.print(F("statusDotColor is: "));
                        Serial.println(statusDotColor);
                        Serial.print(F("UTCTimeCorrected flag: "));
                        Serial.println(UTCTimeCorrected);
                        Serial.println(F(""));
                    #endif
                    return true;
                }
        }
    else
        {
            statusDotColor = DOT_YELLOW;
            UTCTimeCorrected = false;
            #if defined(DEBUG28)
                Serial.println(F("\nThis is the updateTimeFromGPS function: uBlox did not recieve a Full Packet!"));
                Serial.print(F("statusDotColor is: "));
                Serial.println(statusDotColor);
                Serial.print(F("UTCTimeCorrected flag: "));
                Serial.println(UTCTimeCorrected);
                Serial.println(F(""));
            #endif
            return true;
        }
}
#endif

/***************************************************************************************************************
* This function puts the time, date, and Day of the Week on the screen where ever you specify on the TFT.
* We are not using 12 Hr time (Boolean am/pm is false), we are using military time.
***************************************************************************************************************/
void drawPrintTime(uint16_t x, uint16_t y, uint8_t h, uint8_t m, uint8_t s, uint8_t day, int dd, int mm, int yr, bool ampm)
{
    // First lets print the date:
    
    tft.setCursor (x, y);
    tft.print("          ");  // Clear the line (10 spaces) first to remove the remnants from previous print.
    tft.setCursor (x, y);
    tft.print(mm);
    tft.write('/');
    tft.print(dd);
    tft.write('/');
    tft.print(yr);
    
    // Now lets print the time in 24 hr format.
    tft.setCursor (x + Xsize, (y - (Ysize * 4)));  // Step up four lines to print the time.
    tft.print("        ");  // Clear the line (8 spaces) first to remove the remnants from previous print.
    tft.setCursor (x + Xsize, (y - (Ysize * 4)));
    if (ampm == true)
        {
            if (h > 12)
                {
                    if (h < 22) tft.print('0');
                    tft.print (h - 12);
                }
            else
                {
                    if (h < 10) tft.print('0');
                    tft.print (h);
                }
        }
    else
        {
            if (h < 10) tft.print('0');
            tft.print (h);
        }
    
    tft.print (':');
    if (m < 10) tft.print('0');
    tft.print (m);
    tft.print (':');
    if (s < 10) tft.print('0');
    tft.print (s);
    
    if (ampm == true)
        {
            if (h > 12)
                {
                    tft.print(" pm");
                }
            else
                {
                    tft.print (" am");
                }
        }
    
    // Now lets print the time zone.
    tft.setCursor ( ( x + (Xsize * 4) ), ( y - ( Ysize * 3 ) ) );  // Step up four lines to print the time zoe.
    tft.print("   ");  // Clear the line (3 spaces) first to remove the remnants from previous print.
    tft.setCursor ( ( x + (Xsize * 4) ), ( y - ( Ysize * 3 ) ) );
    tft.print(tcr -> abbrev);
    
    switch (day)  // Day can be 1 -> 7 inclusive.
    {
        case 0:  // This means the day of the week was not initialized.
            tft.setCursor( x + (Xsize / 2) , ( y + ( Ysize * 5 ) ) );  // Step down to the bottom to clear the Day.
            tft.print(F("         "));  // Clear the line (9 spaces) first to remove the remnants from previous print.
            tft.setCursor( x + (Xsize * 2) , ( y + ( Ysize * 5 ) ) );  // Step down to the bottom to print the Day.
            tft.print(F("Bad Day"));  // Clear the line (9 spaces) first to remove the remnants from previous print.
            break;
            
        case 1:  // This is Sunday (6 Characters)
            tft.setCursor( x + (Xsize / 2) , ( y + ( Ysize * 5 ) ) );  // Step down to the bottom to clear the Day.
            tft.print(F("         "));  // Clear the line (9 spaces) first to remove the remnants from previous print.
            tft.setCursor( x + (Xsize * 2) , ( y + ( Ysize * 5 ) ) );  // Step down to the bottom to print the Day.
            tft.print(F("Sunday"));
            break;
            
        case 2:  // This is Monday (6 Characters)
            tft.setCursor( x + (Xsize / 2) , ( y + ( Ysize * 5 ) ) );  // Step down to the bottom to clear the Day.
            tft.print(F("         "));  // Clear the line (9 spaces) first to remove the remnants from previous print.
            tft.setCursor( x + (Xsize * 2) , (y + (Ysize * 5)));
            tft.print(F("Monday"));
            break;
            
        case 3:  // This is Tuesday (7 Characters)
            tft.setCursor( x + (Xsize / 2) , ( y + ( Ysize * 5 ) ) );  // Step down to the bottom to clear the Day.
            tft.print(F("         "));  // Clear the line (9 spaces) first to remove the remnants from previous print.
            tft.setCursor( x + ( (Xsize / 2) * 3) , (y + (Ysize * 5)));
            tft.print(F("Tuesday"));
            break;
            
        case 4:  // This is Wednesday (9 Characters)
            tft.setCursor( x + (Xsize / 2) , ( y + ( Ysize * 5 ) ) );  // Step down to the bottom to clear the Day.
            tft.print(F("         "));  // Clear the line (9 spaces) first to remove the remnants from previous print.
            tft.setCursor( x + (Xsize / 2)  , (y + (Ysize * 5)));
            tft.print(F("Wednesday"));
            break;
            
        case 5:  // This is Thursday (8 Characters)
            tft.setCursor( x + (Xsize / 2) , ( y + ( Ysize * 5 ) ) );  // Step down to the bottom to clear the Day.
            tft.print(F("         "));  // Clear the line (9 spaces) first to remove the remnants from previous print.
            tft.setCursor( x + Xsize , (y + (Ysize * 5)));
            tft.print(F("Thursday"));
            break;
            
        case 6:  // This is Friday (6 Characters)
            tft.setCursor( x + (Xsize / 2) , ( y + ( Ysize * 5 ) ) );  // Step down to the bottom to clear the Day.
            tft.print(F("         "));  // Clear the line (9 spaces) first to remove the remnants from previous print.
            tft.setCursor( x + (Xsize * 2) , (y + (Ysize * 5)));
            tft.print(F("Friday"));
            break;
            
        case 7:  // This is Saturday (8 Characters)
            tft.setCursor( x + (Xsize / 2) , ( y + ( Ysize * 5 ) ) );  // Step down to the bottom to clear the Day.
            tft.print(F("         "));  // Clear the line (9 spaces) first to remove the remnants from previous print.
            tft.setCursor( x + Xsize , (y + (Ysize * 5)));
            tft.print(F("Saturday"));
            break;
            
        default:
            tft.setCursor( x + (Xsize / 2) , ( y + ( Ysize * 5 ) ) );  // Step down to the bottom to clear the Day.
            tft.print(F("         "));  // Clear the line (9 spaces) first to remove the remnants from previous print.
            tft.setCursor( x + (Xsize * 2) , ( y + ( Ysize * 5 ) ) );  // Step down to the bottom to print the Day.
            tft.print(F("Bad Day"));  // Clear the line (9 spaces) first to remove the remnants from previous print.
            break;
    }
    
}

/***************************************************************************************************************
*        This function prints Humidity, Pressure, and Temperature for the inside (Master) weather station
*        at the top right of the TFT Screen.
*       TODO: Convert from Imperial (F) to Metric (C).
***************************************************************************************************************/
void drawPrintInsideTempPresHum(uint16_t xPos, uint16_t yPos, float Temperature, float Baro_Pressure, float Relative_Humidity)
{
    tft.setCursor (xPos, yPos);  // Set the cursor to the first printing position on TFT.
    
    tft.print(F("Temp: "));  // Print Temperature on TFT with trailing space.
    tft.print(F("            "));  // Clear the previous (12 possible spaces) write data with spaces.
    tft.setCursor (xPos + (Xsize * 6), yPos);  // Move to the new print place (7 characters from starting point xPos).
    tft.print(Temperature, 2);  // Print the temperature with two decimal places.
    tft.print(F(" deg F"));  // Tack on degrees F to the line.
    
    tft.setCursor (xPos + (Xsize * 2), yPos + Ysize);
    tft.print(F("Pres: "));
    tft.print(F("     "));  // Clear the previous (5 possible spaces) write data with spaces.
    tft.setCursor (xPos + (Xsize * 8), yPos + Ysize);
    tft.print(Baro_Pressure, 2);
    tft.print(F(" InHg"));
    
    tft.setCursor (xPos + (Xsize * 5), yPos + (Ysize * 2));
    tft.print(F("Hum: "));
    tft.print(F("        "));  // Clear the previous (8 possible spaces) write data with spaces.
    tft.setCursor (xPos + (Xsize * 10), yPos + (Ysize * 2));
    tft.print(Relative_Humidity, 2);
    tft.print(F(" %"));
    
}


/***************************************************************************************************************
*        This function the Days high and Low for the inside (Master) weather station
*        at the top-right of the TFT Screen.
*       TODO: Convert from Imperial (F) to Metric (C).
***************************************************************************************************************/
void drawPrintInsideHighLowTempsTFT(uint16_t xPos, uint16_t yPos)
{
    if (insideDailyHighTemp < sensorState.Temperature)
    {
        insideDailyHighTemp = sensorState.Temperature;
    }
    
    if (insideDailyLowTemp > sensorState.Temperature)
    {
        insideDailyLowTemp = sensorState.Temperature;
    }
    
    tft.setCursor(xPos, yPos);  // This starts at 11 over from the right, and the Top + 3 lines
    tft.setFontScale(0);  // Set Font to normal size (8W x 16T pixels).
    tft.print(F("High Temp | Low  Temp"));
    
    tft.setCursor( ( xPos + Xsize ), ( yPos + ( Ysize / 2 ) ) );
    tft.print(F("     "));  // Clear the previous (5 possible spaces) write data with spaces.
    tft.setCursor( ( xPos + Xsize ), ( yPos + ( Ysize / 2 ) ) );
    tft.print(insideDailyHighTemp, 1);  // Print the High temp for the day with one decimal point.
    
    tft.setCursor (xPos + (Xsize * 7), ( yPos + ( Ysize / 2 )  ) );
    tft.print(F("     "));  // Clear the previous (5 possible spaces) write data with spaces.
    tft.setCursor (xPos + (Xsize * 7), ( yPos + ( Ysize / 2 )  ) );
    tft.print(insideDailyLowTemp, 1);  // Print the Low temp for the day with one decimal point.
    
    tft.setFontScale(1);  // Set Font to 2x normal size (16W x 32T pixels).
}


/***************************************************************************************************************
*       This function prints Humidity, Pressure, and Temperature for the outside (Slave) weather station
*       at the bottom right of the TFT Screen.
*       TODO: Convert from Imperial (F) to Metric (C).
***************************************************************************************************************/
void drawPrintOutsideTempPresHum(uint16_t xPos, uint16_t yPos, float Temperature, float Baro_Pressure, float Relative_Humidity)
{
    tft.setCursor (xPos, yPos);  // Set the cursor to the first printing position on TFT.
    
    tft.print(F("Hum: "));
    tft.print(F("     "));  // Clear the previous (5 possible spaces) write data with spaces.
    tft.setCursor (xPos + (Xsize * 5), yPos);
    tft.print(Relative_Humidity, 2);
    tft.print(F("%"));
    
    tft.setCursor (xPos - (Xsize * 3), yPos + Ysize);
    tft.print(F("Pres: "));
    tft.print(F("     "));  // Clear the previous (5 possible spaces) write data with spaces.
    tft.setCursor (xPos + (Xsize * 3), yPos + Ysize);
    tft.print(Baro_Pressure, 2);
    tft.print(F("InHg"));
    
    tft.setCursor (xPos - (Xsize * 5), yPos + (Ysize * 2));
    tft.print(F("Temp: "));  // Print Temperature on TFT with trailing space.
    tft.print("            ");  // Clear the previous (13 possible spaces) write data with spaces.
    tft.setCursor (xPos + (Xsize * 1), yPos + (Ysize * 2));  // Move to the new print place (1 character from starting point xPos).
    tft.print(Temperature, 2);  // Print the temperature with two decimal places.
    tft.print(F(" deg F"));  // Tack on degrees F to the line.
    
}


/***************************************************************************************************************
*        This function prints the Days high and Low for the outside (Master) weather station
*        at the bottom-right of the TFT Screen.
*       TODO: Convert from Imperial (F) to Metric (C).
***************************************************************************************************************/
void drawPrintOutsideHighLowTempsTFT(uint16_t xPos, uint16_t yPos)
{
    if (outsideDailyHighTemp < WeatherDataStruct.Temperature)
        {
            outsideDailyHighTemp = WeatherDataStruct.Temperature;
        }
    
    if (outsideDailyLowTemp > WeatherDataStruct.Temperature)
        {
            outsideDailyLowTemp = WeatherDataStruct.Temperature;
        }
    
    tft.setCursor (xPos, yPos);  // This starts at 11 over from the right, and the Top + 3 lines
    tft.setFontScale(0);  // Set Font to normal size (8W x 16T pixels).
    tft.print(F("High Temp | Low  Temp"));
    
    tft.setCursor( ( xPos + Xsize ), ( yPos + ( Ysize / 2 ) ) );
    tft.print(F("     "));  // Clear the previous (5 possible spaces) write data with spaces.
    tft.setCursor( ( xPos + Xsize ), ( yPos + ( Ysize / 2 ) ) );
    tft.print(outsideDailyHighTemp, 1);  // Print the High temp for the day.
    
    tft.setCursor (xPos + (Xsize * 7), ( yPos + ( Ysize / 2 )  ) );
    tft.print(F("     "));  // Clear the previous (5 possible spaces) write data with spaces.
    tft.setCursor (xPos + (Xsize * 7), ( yPos + ( Ysize / 2 )  ) );
    tft.print(outsideDailyLowTemp ,1);  // Print the Low temp for the day.
    
    tft.setFontScale(1);  // Set Font to 2x normal size (16W x 32T pixels).
}


/***************************************************************************************************************
*       This function prints a Red, Yellow, or Green Dot, to indicate the status of the data coming from Slave
*       at the bottom right of the TFT Screen, just to the left of the Pres info.
*       Red indicates there is no data.
*       Yellow indicates the data from the slave is bad.
*       Green indicates there is good data from the slave.
*   Needs:
*       Xpos - The X position on the TFT which is the center of the Dot.
*       Ypos - The Y position on the TFT which is the center of the Dot.
*       dotColor - The color of the Dot
*
*   Returns:
*       Nothing
***************************************************************************************************************/
void printStatusDotOnTFT(uint16_t xPos, uint16_t yPos, uint16_t dotColor )
{
    tft.fillCircle(xPos, yPos, 6, dotColor);
}


/***************************************************************************************************************
* This function clears the Weather Data from the outside Weather Station from the TFT.
***************************************************************************************************************/
void drawBlankOutsideTempPresHum(uint16_t xPos, uint16_t yPos)
{
    tft.setCursor (xPos, yPos);  // Set the cursor to the first printing position on TFT.
    tft.print("             ");  // Clear the 13 possible spaces, write data with spaces.
    
    tft.setCursor (xPos - (Xsize * 2), yPos + Ysize);
    tft.print("                ");  // Clear the previous 16 possible spaces, write data with spaces.
    
    tft.setCursor (xPos - (Xsize * 5), yPos + (Ysize * 2));
    tft.print("                  ");  // Clear the previous 18 possible spaces, write data with spaces.
    
    tft.setCursor (xPos - (Xsize * 7), yPos + (Ysize * 3));
    tft.print("                  ");  // Clear the previous 18 possible spaces, write data with spaces.
    
    // Blank out the Yearly rain fall number from screen.
    tft.setCursor (XLEFT, (YLINE15));
    tft.print("                 ");  // 17 spaces.
}



/***************************************************************************************************************
* This function draws the Major and Minor tick marks in the circle.
***************************************************************************************************************/
void roundGaugeTickMarks(uint16_t x, uint16_t y, uint16_t r, int from, int to, float dev, int degrees , uint16_t color)
{
    float dsec;
    int i;
    for (i = from; i <= to; i += degrees)
    {
        dsec = i * (PI / 180);
        tft.drawLine(
                     x + (cos(dsec) * (r / dev)) + 1,
                     y + (sin(dsec) * (r / dev)) + 1,
                     x + (cos(dsec) * r) + 1,
                     y + (sin(dsec) * r) + 1,
                     color);
    }
}

/***************************************************************************************************************
* This function draws the Wind Direction Flower, White circle on black backround with major and minor tick marks.
* Needs:
*       X Position array (16 bit integer), from (unsigned 16 bit integer), to (unsigned 16 bit integer).
* Where:
*       X Position array has 3 numbers, starting x position, starting y position, and the radius of the circle.
*       from is the first tick mark in degrees from the top center (12 o'clock position).
*       to is the last tick mark in degrees from the top center (12 o'clock position).
*
***************************************************************************************************************/
void drawWindGauge(int16_t pos[], uint16_t from, uint16_t to, uint16_t Circle_color , uint16_t Major_color , uint16_t Minor_color)
{
    tft.drawCircle(pos[0], pos[1], pos[2], Circle_color); //draw instrument container
    tft.drawCircle(pos[0], pos[1], pos[2] + 1, Circle_color); //draw instrument container
    roundGaugeTickMarks(pos[0], pos[1], pos[2], from, to, 1.3, 30, Major_color);  // Draw Major Ticks.
    if (pos[2] > 15) roundGaugeTickMarks(pos[0], pos[1], pos[2], from + 5, to - 5, 1.1, 5, Minor_color); // Draw minor ticks.
    tft.setFontScale(0);  // 0 is normal size (8 x 16 pixels)
    tft.setCursor ((WindDirFlowerPos[0] - 2), YTOP + ((Ysize / 2) * 8) + 3);  // Size is set for 8 x 16 pixels.
    tft.print(F("N"));
    tft.setCursor (XLEFT + 8, (YMIDDLE - 7) );
    tft.print(F("W"));
    tft.setCursor ((XLEFT + ((Xsize / 2) * 26) - 3), (YMIDDLE - 6) );
    tft.print(F("E"));
    tft.setCursor ((WindDirFlowerPos[0] - 3), YBOTTOM - ((Ysize / 2) * 9) - 3);
    tft.print(F("S"));
    tft.setFontScale(1);  // Set Font to 2x normal size (16 x 32 pixels).
}

/***************************************************************************************************************
* This function draws the Clock Face, White circle on black backround with major and minor tick marks, and the
* 12, 3, 6, and 9 around the inside of the clock lines.
* Needs:
*       pos[]:  Position array (16 bit integer),
*       from:  (unsigned 16 bit integer),
*       to:  (unsigned 16 bit integer),
*       Circle_color:  The color of the inside of the circle,
*       Major_color:  The Major tick marks color,
*       Minor_color:  The Minor tick marks color.
* Where:
*       X Position array has 3 numbers, starting x position, starting y position, and the radius of the circle.
*       from:  is the first tick mark in degrees from the top center (12 o'clock position).
*       to:  is the last tick mark in degrees from the top center (12 o'clock position).
* Returns:  Nothing
*
***************************************************************************************************************/
void drawGauge(int16_t pos[], uint16_t from, uint16_t to, uint16_t Circle_color , uint16_t Major_color , uint16_t Minor_color)
{
    tft.drawCircle(pos[0], pos[1], pos[2], Circle_color); //draw instrument container
    tft.drawCircle(pos[0], pos[1], pos[2] + 1, Circle_color); //draw instrument container
    roundGaugeTickMarks(pos[0], pos[1], pos[2], from, to, 1.3, 30, Major_color);  // Draw Major Ticks every 30 degrees.
    if (pos[2] > 15) roundGaugeTickMarks(pos[0], pos[1], pos[2], from + 6, to - 6, 1.1, 6, Minor_color); // Draw minor ticks every 6 degrees.
    
    tft.setCursor ((pos[0] - 15 ), (pos[1] - pos[2]) + 40 );  // Size is set for 16 x 32 pixels.
    tft.print(F("12"));
    tft.setCursor (((pos[0] - pos[2]) + 50 ), (pos[1] - 18));
    tft.print(F("9"));
    tft.setCursor (((pos[0] + pos[2]) - 64 ), (pos[1] - 16));
    tft.print(F("3"));
    tft.setCursor ((pos[0] - 6 ) , (pos[1] + pos[2]) - 64 );
    tft.print(F("6"));
    
    
}


/***************************************************************************************************************
 * This function Converts a character from ASCII to decimal.
 ***************************************************************************************************************/
//static uint8_t conv2d(const char* p)
//{
//    uint8_t v = 0;
//    if ('0' <= *p && *p <= '9')
//        {
//            v = *p - '0';
//        }
//    return 10 * v + *++p - '0';
//}

/*******************************************************************************************************************
* Define array for Wind Direction Flower:
* int16_t WindDirFlowerPos[3] = {100, 240, 90}; //x position, y position, r Radius (Size of Wind Direction Flower)
*
* Define colors for background, face, Wind Direction Pointer.
* const uint16_t WindDirFlowerColors[3] = {RA8875_BLACK, RA8875_BLACK, RA8875_RED};
*
* Define array with positions for Previous Wind Direction Pointer X, and Previous Wind Direction Pointer Y
* uint16_t oldWindDirFlowerPos[2] = {0, 0};
********************************************************************************************************************/
void drawWindDirPointer(int16_t windPositionArray[], int windDegreeValue, const uint16_t colors[])
{
    int degreeslength = windPositionArray[2] / 2 - windPositionArray[2] / 4;  // = 22.5
    
    float sx = cos(((windDegreeValue) - 90) * 0.0174532925);  // For wind direction 0 degrees = .999624217
    float sy = sin(((windDegreeValue) - 90) * 0.0174532925);  // For wind direction 0 degrees = -.027412134
    
    // Erase old hand position
    tft.drawLine(oldWindDirFlowerPos[0], oldWindDirFlowerPos[1], windPositionArray[0] + 1, windPositionArray[1] + 1, colors[1]);
    
    // Draw new hand positions
    tft.drawLine(sx * (windPositionArray[2] - degreeslength) + windPositionArray[0] + 1, sy * (windPositionArray[2] - degreeslength) + windPositionArray[1] + 1, windPositionArray[0] + 1, windPositionArray[1] + 1, colors[2]);
    tft.fillCircle(windPositionArray[0] + 1, windPositionArray[1] + 1, 3, colors[2]);
    
    // Update old x&y coords
    
    oldWindDirFlowerPos[0] = sx * (windPositionArray[2] - degreeslength) + windPositionArray[0] + 1;  // For wind direction 0 degrees = 168.4366806
    oldWindDirFlowerPos[1] = sy * (windPositionArray[2] - degreeslength) + windPositionArray[1] + 1;  // For wind direction 0 degrees = -8.456643339
    
}

/********************************************************************************************************************
* This function draws the clock hands where ever you specify.
* These definitions are for the Clock functions.
* // Define array for Clockface:
* int16_t clockPos[3] = {0, 0, 50}; //x size, y size, r Radius
*
* //Define array for Time in hours, minutes, and seconds.
* uint8_t currentTime[3] = {0, 0, 0}; //hh, mm, ss
* int currentDate[3] = {0, 0, 0}; // Day, Month, Year
* // Define colors for background, face, hh hand, mm hand, and ss hand.
* const uint16_t clockColors[5] = {RA8875_BLACK, RA8875_BLACK, RA8875_CYAN, RA8875_BLUE, RA8875_RED};
*
* // Define array with positions for hour hand X, hour hand Y, minute hand X, minute hand Y, second hand X, second hand Y
* uint16_t oldPos[6] = {0, 0, 0, 0, 0, 0};
*
* // Need variable for the current time.
* unsigned long targetTime = 0;
*
*********************************************************************************************************************/
void drawClockHands(int16_t pos[], uint8_t curTime[], const uint16_t colors[])
{
    
    uint8_t hlen = (pos[2] / 2 - pos[2] / 12);
    uint8_t mlen = pos[2] / 2 - pos[2] / 4;
    uint8_t slen = pos[2] / 2 - pos[2] / 4;
    float hx = cos(((curTime[0] * 30 + (curTime[1] * 6 + (curTime[2] * 6) * 0.01666667) * 0.0833333) - 90) * 0.0174532925);
    float hy = sin(((curTime[0] * 30 + (curTime[1] * 6 + (curTime[2] * 6) * 0.01666667) * 0.0833333) - 90) * 0.0174532925);
    float mx = cos(((curTime[1] * 6 + (curTime[2] * 6) * 0.01666667) - 90) * 0.0174532925);
    float my = sin(((curTime[1] * 6 + (curTime[2] * 6) * 0.01666667) - 90) * 0.0174532925);
    float sx = cos(((curTime[2] * 6) - 90) * 0.0174532925);
    float sy = sin(((curTime[2] * 6) - 90) * 0.0174532925);
    
    // Erase just old hand positions
    tft.drawLine(oldPos[0], oldPos[1], pos[0] + 1, pos[1] + 1, colors[1]);
    tft.drawLine(oldPos[2], oldPos[3], pos[0] + 1, pos[1] + 1, colors[1]);
    tft.drawLine(oldPos[4], oldPos[5], pos[0] + 1, pos[1] + 1, colors[1]);
    // Draw new hand positions
    tft.drawLine(hx * (pos[2] - hlen) + pos[0] + 1, hy * (pos[2] - hlen) + pos[1] + 1, pos[0] + 1, pos[1] + 1, colors[2]);
    tft.drawLine(mx * (pos[2] - mlen) + pos[0] + 1, my * (pos[2] - mlen) + pos[1] + 1, pos[0] + 1, pos[1] + 1, colors[3]);
    tft.drawLine(sx * (pos[2] - slen) + pos[0] + 1, sy * (pos[2] - slen) + pos[1] + 1, pos[0] + 1, pos[1] + 1, colors[4]);
    tft.fillCircle(pos[0] + 1, pos[1] + 1, 3, colors[4]);
    
    // Update old x&y coords
    oldPos[4] = sx * (pos[2] - slen) + pos[0] + 1;
    oldPos[5] = sy * (pos[2] - slen) + pos[1] + 1;
    oldPos[2] = mx * (pos[2] - mlen) + pos[0] + 1;
    oldPos[3] = my * (pos[2] - mlen) + pos[1] + 1;
    oldPos[0] = hx * (pos[2] - hlen) + pos[0] + 1;
    oldPos[1] = hy * (pos[2] - hlen) + pos[1] + 1;
}


/*******************************************************************************************************************
* Utility function for digital clock display: prints preceding colon and leading 0
*******************************************************************************************************************/
void tftPrintDigits(int digits)
{
    tft.print(":");
    if(digits < 10)
    {
        tft.print('0');
    }
    tft.print(digits);
}

/*******************************************************************************************************************
* Digital clock display of the time on TFT
*******************************************************************************************************************/
void tftDigitalClockDisplay(bool dateTimeFlag)
{
    time_t t = now(); // Get current time from Real Time Clock
    tft.print(F("Time: "));
    tft.print(hour(t));
    tftPrintDigits(minute(t));
    tftPrintDigits(second(t));
    if (dateTimeFlag == true)
    {
        tft.print(" ");
        tft.print(month(t));
        tft.print("/");
        tft.print(day(t));
        tft.print("/");
        tft.print(year(t));
    }
}

/*******************************************************************************************************************
* This subroutine gets the time from the Teensy 3.1's internal crystal clock.
*******************************************************************************************************************/

time_t getTeensy3Time(void)
{
    return Teensy3Clock.get();
}

/*******************************************************************************************************************
* Utility function for digital clock display: prints preceding colon and leading 0
*******************************************************************************************************************/
void printDigits(int digits)
{
    Serial.print(":");
    if(digits < 10)
    {
        Serial.print('0');
    }
    Serial.print(digits);
}

/*******************************************************************************************************************
* Digital clock display of the time on monitor
*******************************************************************************************************************/
void digitalClockDisplay(void)
{
    time_t t = now(); //get current time from Real Time Clock
    Serial.print(F("This is the current Time: "));
    Serial.print(hour(t));
    printDigits(minute(t));
    printDigits(second(t));
    Serial.print(" ");
    Serial.print(month(t));
    Serial.print("/");
    Serial.print(day(t));
    Serial.print("/");
    Serial.print(year(t));
    Serial.println("");
}


/*******************************************************************************************************************
* Code to process time sync messages from the serial port
*******************************************************************************************************************/
#define TIME_HEADER  "T"   // Header tag for serial time sync message

unsigned long processSyncMessage(void)
{
    unsigned long pctime = 0L;
    const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
    
    if(Serial.find(TIME_HEADER))
    {
        pctime = Serial.parseInt();
        return pctime;
        if( pctime < DEFAULT_TIME)
        { // check the value is a valid time (greater than Jan 1 2013)
            pctime = 0L; // return 0 to indicate that the time is not valid
        }
    }
    return pctime;
}


/*******************************************************************************************************************
*  This funtion allows input of data directly from a USB connected computer, running a special program.
*******************************************************************************************************************/
void setPCTime (void)
{
    if (pctime == false)
    {
        while (!Serial.available())
        {
            delay(10);
        }
        
        time_t t = processSyncMessage();
        if (t != 0)
        {
            Teensy3Clock.set(t); // set the RTC
            setTime(t);
            pctime = true;  // Done setting RTC clock exit the while loop
        }
        
        digitalClockDisplay();  // Display the new clock value.
    }
}


/*******************************************************************************************************************
* ----------------------------------------------FIX THIS------------------------------------------------------------
* This funtion extracts the tm.month, tm.day, and tm.year from the passed in char string.
*
* int sscanf ( const char * s, const char * format, ...);
* Read formatted data from string:
* Reads data from s and stores them according to parameter format into the locations given by the additional arguments,
* as if scanf was used, but reading from s instead of the standard input (stdin).
*
* The additional arguments should point to already allocated objects of the type specified by their corresponding
* format specifier within the format string.
*
* Return Value:
* On success, the function returns the number of items in the argument list successfully filled. This count can match
* the expected number of items or be less (even zero) in the case of a matching failure.
* In the case of an input failure before any data could be successfully interpreted, EOF is returned.
*******************************************************************************************************************/
bool getDate(const char *str)
{
    char Month[12];
    int Day, Year;
    uint8_t monthIndex;
    
    if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
    
    for (monthIndex = 0; monthIndex < 12; monthIndex++)
        {
            if (strcmp(Month, monthName[monthIndex]) == 0) break;
        }
    
    if (monthIndex >= 12) return false;  // Error if more than 12 months.
    
    return true;
}


/*******************************************************************************************************************
* ----------------------------------------------FIX THIS------------------------------------------------------------
*  This funtion extracts the tm.hour, tm.min, and tm.second from the passed in char string.
*
* int sscanf ( const char * s, const char * format, ...);
* Read formatted data from string:
* Reads data from s and stores them according to parameter format into the locations given by the additional arguments,
* as if scanf was used, but reading from s instead of the standard input (stdin).
*
* The additional arguments should point to already allocated objects of the type specified by their corresponding
* format specifier within the format string.
*
* Return Value:
* On success, the function returns the number of items in the argument list successfully filled. This count can match
* the expected number of items or be less (even zero) in the case of a matching failure.
* In the case of an input failure before any data could be successfully interpreted, EOF is returned.
*******************************************************************************************************************/
bool getTime(const char *str)
{
    int Hour, Min, Sec;
    
    if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
    
    return true;
}


/*******************************************************************************************************************
* ----------------------------------------------FIX THIS------------------------------------------------------------
*  This funtion allows input of time, date, and day of the week directly from the keyboard.
* TBD
*******************************************************************************************************************/
void setDSTime (void)
{
    tmElements_t tmElementsStruct;
    int Wday;
    bool parse=false;
    bool config=false;
    
    Serial.println(F("Enter the day of the week (Sunday is day 1, Saturday is day 7)"));
    tft.println(F("Enter the day of the week (Sunday is day 1, Saturday is day 7)"));
    
    while (!Serial.available())
        {
            delay(10);
        }
    
    //  Wday = Serial.parseInt();
    Wday = Serial.read();
    if (Wday > '0' && Wday <= '7')
        {
            Wday = Wday - '0';
            tmElementsStruct.Wday = Wday;
        }
    else
        {
            Serial.println(F("Error: Day of Week must be from 1 to 7!"));
            tft.println(F("Error: Day of Week must be from 1 to 7!"));
        }
    
    
    // get the date and time the compiler was run
    if (getDate(__DATE__) && getTime(__TIME__))
        {
            parse = true;
            // and configure the RTC with this info
            if (RTC.write(tmElementsStruct))
                {
                    config = true;
                }
        }
    
    while (!Serial) ; // wait for Arduino Serial Monitor
    delay(200);
    if (parse && config)
        {
            Serial.print(F("DS1307 configured Time="));
            Serial.print(__TIME__);
            Serial.print(F(", Date="));
            Serial.println(__DATE__);
        }
    else if (parse)
        {
            Serial.println(F("DS3231 Communication Error :-{"));
            Serial.println(F("Please check your circuitry"));
        }
    else
        {
            Serial.print(F("Could not parse info from the compiler, Time=\""));
            Serial.print(__TIME__);
            Serial.print(F("\", Date=\""));
            Serial.print(__DATE__);
            Serial.println(F("\""));
        }
    
    digitalClockDisplay();  // Display the new clock value.
    
}


#if defined(USINGTEMPSENSORS)
/***************************************************************************************************************
* This function Prints on the TFT Screen the Temperatures associated with each Temp Sensor Device
* on the OneWire bus.
*
* Needs:
*   Nothing
* Returns:
*   Nothing.
***************************************************************************************************************/
void tftPrintTempData(void)
{
    for (int id = 0; id < deviceCount; id++)
    {
        tft.print(F("\t"));  // One Tab (4 spaces).
        tft.print(oneWireTemps[id].id);
        tft.print(F("\t\t  "));  // Two Tabs (8 spaces) and two spaces.
        
        if (oneWireTemps[id].tempData == -999)  // -999 if no device
        {
            tft.print(F("Error, Temp Sensor returned Bad Value or No Device"));
        }
        else
        {
            tft.println(oneWireTemps[id].tempData);
        }
    }
}


/***************************************************************************************************************
 * This function Plots on the TFT Screen the Temperatures associated with each Temp Sensor Device
 * on the OneWire bus, using an Oscilloscope type display.  This display plots 12 minutes worth of data.
 *
 * Needs:
 *   Nothing
 * Returns:
 *   Nothing.
 ***************************************************************************************************************/
void tftPlotTempData(void)
{
    for (int id = 0; id < deviceCount; id++)
    {
        tft.print(F("\t"));  // One Tab (4 spaces).
        tft.print(oneWireTemps[id].id);
        tft.print(F("\t\t  "));  // Two Tabs (8 spaces) and two spaces.
        
        if (oneWireTemps[id].tempData == -999)  // -999 if no device
        {
            tft.print(F("Error, Temp Sensor returned Bad Value or No Device"));
        }
        else
        {
            tft.println(oneWireTemps[id].tempData);
        }
    }
}


/***************************************************************************************************************
 * This function adds the Temp Data of the DS18B20 Temp Sensors to the four Large Temp Sensor Circular Buffers.
 *
 * Needs:
 *   Nothing
 * Returns:
 *   boolean:
 *       false if success.
 *       true if error.
 ***************************************************************************************************************/
/*
 bool addDS18B20TempsToArray(void)
 {
 uint8_t index = 0;
 bool error = false;
 
 switch (deviceCount)
 {
 case 4:
 circularBuffer1.push_back(oneWireTemps[index].tempData);
 index++;
 circularBuffer2.push_back(oneWireTemps[index].tempData);
 index++;
 circularBuffer3.push_back(oneWireTemps[index].tempData);
 index++;
 circularBuffer4.push_back(oneWireTemps[index].tempData);
 error = false;
 break;
 
 case 3:
 circularBuffer1.push_back(oneWireTemps[index].tempData);
 index++;
 circularBuffer2.push_back(oneWireTemps[index].tempData);
 index++;
 circularBuffer3.push_back(oneWireTemps[index].tempData);
 error = false;
 break;
 
 case 2:
 circularBuffer1.push_back(oneWireTemps[index].tempData);
 index++;
 circularBuffer2.push_back(oneWireTemps[index].tempData);
 error = false;
 break;
 
 case 1:
 circularBuffer1.push_back(oneWireTemps[index].tempData);
 error = false;
 break;
 
 default:
 error = true;
 break;
 }
 return error;
 
 }
 */

/***************************************************************************************************************
* This function controls the Solenoid that feeds the water to the overhead misters which keeps us Cool.
*
* Needs:
*   Nothing
* Returns:
*   Nothing
***************************************************************************************************************/
void controlTheWaterRelay(void)
{
    float presentControlTemp;
    float lastControlTemp;
    uint8_t presentTempControlSensor = 3;
    
    
    if (coolerONFlag == true)
        {
            if (firstRun == true)
                {
                    waterRelayON = true;
                    presentControlTemp = oneWireTemps[presentTempControlSensor].tempData;
                    lastControlTemp = presentControlTemp;
                }
        
            if (waterRelayON == true)  // Check the control temp sensor to see if it should be on or off.
                {
                    // Check for the water control temp sensor change in temp
                }
            else  // Check to see if it should be turned off.
                {
                    // Check the controlling sensor to see if it should be on or off.
                    waterRelayON = true;
                }
        }
    
}
#endif


/*******************************************************************************************************************
* ----------------------------------------------FIX THIS------------------------------------------------------------
*  This funtion Sets the scroll function on the tft window.
* ------------------------------------------ SCROLL FUNCTIONS-------------------------------------------------------
* void      setScrollMode(enum RA8875scrollMode mode "{ SIMULTANEOUS, LAYER1ONLY, LAYER2ONLY, BUFFERED }")
* void         setScrollWindow(int16_t XLeft,int16_t XRight ,int16_t YTop ,int16_t YBottom);
* void         scroll(int16_t x,int16_t y);
*
*******************************************************************************************************************/

uint8_t setScrollWindow(int16_t xDirStartLeft, int16_t xDirFinishRight, int16_t yDirStartTop, int16_t yDirStartBottom, RA8875scrollMode scrollMode)
{
    tft.setScrollMode(scrollMode);
    tft.setScrollWindow(xDirStartLeft, xDirFinishRight, yDirStartTop, yDirStartBottom);  //Specifies scrolling activity area
    return(tft.errorCode());
    
}

/*******************************************************************************************************************
* ----------------------------------------------FIX THIS------------------------------------------------------------
*  This funtion scrolls the window Down.
* ------------------------------------------ SCROLL FUNCTIONS-------------------------------------------------------
* void         scroll(int16_t x,int16_t y);
*
*******************************************************************************************************************/
uint8_t scrollWindowDown(int16_t xDirStartLeft, int16_t numberOfRows)
{
    tft.scroll(xDirStartLeft, numberOfRows);  //  First varible is x direction, second varible is y direction.
    return(tft.errorCode());
}


/*******************************************************************************************************************
* ----------------------------------------------FIX THIS------------------------------------------------------------
*  This funtion scrolls the window Up.
* ------------------------------------------ SCROLL FUNCTIONS-------------------------------------------------------
* void         scroll(int16_t x,int16_t y);
*
*******************************************************************************************************************/
uint8_t scrollWindowUp(int16_t xDirFinishRight, int16_t numberOfRows)
{
    tft.scroll(xDirFinishRight, numberOfRows);  //  First varible is x direction, second varible is y direction.
    return(tft.errorCode());
}


/*******************************************************************************************************************
* ----------------------------------------------FIX THIS------------------------------------------------------------
*  This function scrolls the window Left.
* ------------------------------------------ SCROLL FUNCTIONS-------------------------------------------------------
*
* void         scroll(int16_t x,int16_t y);
*
*******************************************************************************************************************/
uint8_t scrollWindowLeft(int16_t numberOfRows, int16_t xDirStartLeft)
{
    tft.scroll(numberOfRows, xDirStartLeft);  //  First varible is x direction, second varible is y direction.
    return(tft.errorCode());
}


/*******************************************************************************************************************
* ----------------------------------------------FIX THIS------------------------------------------------------------
*  This function scrolls the window Right.
* ------------------------------------------ SCROLL FUNCTIONS-------------------------------------------------------
* uint8_t         scroll(int16_t x,int16_t y);
*******************************************************************************************************************/
uint8_t scrollWindowRight(int16_t numberOfRows, int16_t xDirStartRight)
{
    tft.scroll(numberOfRows, xDirStartRight);  //  First varible is x direction, second varible is y direction.
    return(tft.errorCode());
}


/*******************************************************************************************************************
*  This function creates a 16bit XMODEM CRC.
* Needs:
*   The input buffer pointer to create the XMODEM CRC code from.
*       inputBuffer,
*   The input buffer size:
*       bufferSize
* Returns:
*   16 BitXMODEM CRC:
*
*******************************************************************************************************************/
uint16_t crcCreate(uint8_t * inputBuffer, uint16_t bufferSize)
{
    uint16_t crc;
    
    #if defined (DEBUG24)
        Serial.print(F("This is the crcCreate Sub. This is the size of the input buffer: "));
        Serial.println(bufferSize, DEC);
    
        Serial.println(F("This is the crcCreate Sub. This is the data in the input Buffer: "));
        printBuffer(inputBuffer, bufferSize);
    #endif
    
    crc = CRC16.xmodem(inputBuffer, bufferSize);
    
    #if defined (DEBUG24)
        Serial.print(F("This is the crcCreate Sub. This is the XMODEM CRC Created: "));
        Serial.println(crc, HEX);
        Serial.println(F(""));  // Move to next line.
    #endif
    
    return crc;
}


/*******************************************************************************************************************
*  This function checks for a good 16bit XMODEM CRC from the inputBuffer recieved.
* Needs:
*   The input buffer to check for the correct CRC code.
*       inputBuffer,
*   Two bytes of CRC Data:
*       CRC1,
*       CRC2
* Returns:
*   Boolean:
*       True if CRC check is good,
*       False if CRC check is Bad.
*
*******************************************************************************************************************/
bool crcCheck(uint8_t * inputBuffer, uint16_t bufferSize , uint16_t inputCRC)
{
    uint16_t crc;
    
    #if defined (DEBUG23)
        Serial.print(F("This is the crcCheck Sub. This is the size of the buffer: "));
        Serial.println(bufferSize);
    
        Serial.print(F("This is the crcCheck Sub. This is the input CRC: "));
        Serial.println(inputCRC, HEX);
    
        Serial.println(F("This is the crcCheck Sub. This is the data in the inputBuffer: "));
        printBuffer(inputBuffer, bufferSize);
    #endif
    
    crc = CRC16.xmodem(inputBuffer, bufferSize);
    
    if (inputCRC == crc)
        {
            #if defined (DEBUG23)
                Serial.println(F("This is the crcCheck Sub. CRC check was good!\n"));
                Serial.print(F("This is the crcCheck Sub. This is the CRC generated from CRC16.xmodem(inputBuffer, bufferSize): "));
                Serial.println(crc, HEX);
            #endif
        return true;
        }
    else
        {
            #if defined (DEBUG23)
                Serial.print(F("This is the crcCheck Sub. CRC check was BAD! This is the CRC generated from CRC16.xmodem function:"));
                Serial.println(crc, HEX);
            #endif
        return false;
        }
}


/*******************************************************************************************************************
*                   This function Sends the Data Packet sent over the serial port to the slave weather station.
* The size of the packet is 127 bytes maximum set by a global variable.
* This is the form of the Packet sent from the master:
*   [Stx(byte)], [Command(byte)], [Size of Data Packet(byte)], [Data...(121 bytes)], [CRC1(byte)], [CRC2(byte)], [Etx(byte)]
*
* Needs:
*   command:
*       This is the Command Code sent to the Slave. (Refer to the .h file for Command Codes).
*
*   inputDataPointer:
*       This pointer is the address of the place to get the data for transmission.
*
*   sizeOfInputData:
*       This is the size of the data packet being sent to the Slave.
*
* Returns:
*   Nothing
*******************************************************************************************************************/
void sendDataToSlave(commandTypes command, void *inputDataPointer, uint8_t sizeOfInputData)
{
    uint8_t CRC1, CRC2;
    uint16_t outputCRC;
    uint8_t i, j;
    digitalWrite(13, HIGH);  // This turns on the LED.
    
    #if defined (DEBUG21)
        Serial.print(F("This is the sendDataToSlave Sub. This is the size of the data packet going to the slave: "));
        Serial.println(sizeOfInputData);
        Serial.print(F("This is the command byte going to the slave: "));
        Serial.println(command);
        Serial.println(F("Printing the data in the inputDataPointer Buffer..."));
        printBuffer((uint8_t *)inputDataPointer, sizeOfInputData);
    #endif
    
    // Send the data to the Slave.
    
    #if defined (DEBUG21)
        Serial.println(F("\nThis is the sendDataToSlave Sub. Begin filling the transmit buffer..."));
    #endif
    
    if ( sizeOfInputData <= ( maxTxBufferSize - 6 ) )  //make sure the object isn't too large, the buffer is 128 minus 6 bytes of overhead.
        {
            #if defined (DEBUG21)
                Serial.println(F("This is the sendDataToSlave Sub. Filling a temp pointer array of the bytes to send..."));
            #endif
            
            void * ptr = inputDataPointer;
            byte * b = (byte *) ptr;  // Create a pointer array of the bytes to send.
        
            j = 0;  // Start with the pointer to the transmit buffer array at zero.
            i = 0;  // Start with the Pointer to the input data array at zero.
            
            #if defined (DEBUG21)
                Serial.println(F("This is the sendDataToSlave Sub. Now writing the prefix character to the Transmit buffer..."));
                Serial.print(F("This is the value of the prefix character: "));
                Serial.println(prefixChar, DEC);
                Serial.print(F("This is the value of j: "));
                Serial.println(j, DEC);
            #endif
            
            transmitBuffer[j] = ((byte)prefixChar);  // Write the preffix character to signify the start of a packet.
        
            j++; // increment the pointer to the transmit buffer array.
        
            #if defined (DEBUG21)
                Serial.println(F("This is the sendDataToSlave Sub. Wrote the prefix character to the Transmit buffer..."));
                Serial.print(F("This is the value of j: "));
                Serial.println(j, DEC);
            #endif
        
            transmitBuffer[j] = ((byte)command);  // Write the Command character to identify the command to the Slave.
        
            j++; // increment the pointer to the transmit buffer array.
        
            #if defined (DEBUG21)
                Serial.println(F("This is the sendDataToSlave Sub. Wrote the command byte to the Transmit buffer..."));
                Serial.print(F("This is the value of j: "));
                Serial.println(j, DEC);
            #endif
            transmitBuffer[j] = ((byte)sizeOfInputData);  // Write the Size of the data packet.
        
            j++; // increment the pointer to the transmit buffer array.
        
            #if defined (DEBUG21)
                Serial.println(F("This is the sendDataToSlave Sub. Wrote the size of the data packet to the Transmit buffer..."));
                Serial.print(F("This is the value of j: "));
                Serial.println(j, DEC);
            #endif
        
        
            #if defined (DEBUG21)
                Serial.println(F("This is the sendDataToSlave Sub. Writing the data packet to the Transmit buffer..."));
            #endif
            // Loop through all the bytes being sent and write them to the transmit buffer.
            for( i = 0; i < sizeOfInputData ; i++)
                {
                    transmitBuffer[j] = b[i];  // Write each byte to the Transmit buffer.
                    j++; // increment the pointer to the transmit buffer array.
                }
            
            #if defined (DEBUG21)
                Serial.println(F("This is the sendDataToSlave Sub. Now packing the Transmit buffer so its full..."));
                Serial.print(F("This is the value of j: "));
                Serial.println(j, DEC);
            #endif
            
            /// NEED TO PACK THE TRANSMIT BUFFER SO IT HAS THE FULL 127 BYTE PACKET
            
            for( i = 0; i < ( maxTxDataBufferSize - sizeOfInputData ); i++)
                {
                    transmitBuffer[j] = 0;  // Write the extra bytes to the Transmit buffer.
                    j++; // increment the pointer to the transmit buffer array.
                }
            
            #if defined (DEBUG21)
                Serial.print(F("This is the sendDataToSlave Sub. This is the value of j: "));
                Serial.println(j, DEC);
            #endif
        
            outputCRC = (crcCreate(b, sizeOfInputData ));  // This function creates the 16bit XMODEM-CRC from the 122 bytes of data.
        
            #if defined (DEBUG21)
                Serial.print(F("This is the sendDataToSlave Sub. This is the CRC word created by crcCreate: "));
                Serial.println(outputCRC, HEX);
                Serial.print(F("This is the sendDataToSlave Sub. This is the value of j: "));
                Serial.println(j, DEC);
            #endif
        
            CRC1 = 0;
            CRC1 = (outputCRC & 0x00ff);  // This is the low byte.
        
            #if defined (DEBUG21)
                Serial.print(F("This is the sendDataToSlave Sub. This is the data in CRC1: "));
                Serial.println(CRC1, HEX);
            #endif
        
            transmitBuffer[j] = CRC1;  // Send the low XMODEM-CRC byte to the transmitbuffer.
        
            j++; // increment the pointer to the transmit buffer array.
        
            CRC2 = 0;
            CRC2 = ( ( ( outputCRC & 0xff00 ) >> 8 ) );  // This is the high byte.
        
            #if defined (DEBUG21)
                Serial.print(F("This is the sendDataToSlave Sub. This is the data in CRC2: "));
                Serial.println(CRC2, HEX);
                Serial.print(F("This is the value of j: "));
                Serial.println(j, DEC);
            #endif
        
            transmitBuffer[j] = CRC2;  // Send the high XMODEM-CRC byte to the transmitbuffer.
        
            j++;
        
            #if defined (DEBUG21)
                Serial.println(F("This is the sendDataToSlave Sub. Writing the suffix character to signify the end of the packet..."));
                Serial.print(F("This is the value of j: "));
                Serial.println(j, DEC);
            #endif
        
            transmitBuffer[j] = (byte)suffixChar;  // Write the suffix character to signify the end of a stream.
        
            #if defined (DEBUG21)
                Serial.println(F("This is the sendDataToSlave Sub. Printing the data in the transmit Buffer..."));
                printBuffer(transmitBuffer, TxBUFSIZE);
                Serial.println(F("\nNow sending the transmit buffer to Event2 for transmitting...\n"));
            #endif
        
            Event2.write( transmitBuffer, maxTxBufferSize );
        
            digitalWrite(13, LOW);  // This turns off the LED.
        }
    else
        {
            #if defined (DEBUG21)
                Serial.println(F("This is the sendDataToSlave Sub. The outgoing Struct is to large for the TX1_BUFFER_SIZE...\n"));
            #endif
            digitalWrite(13, LOW);  // This turns off the LED.
            return;
    }
    
}


/*******************************************************************************************************************
* ------------------------------------------------------------------------------------------------------------------
*  This function Sends the ACK or NAK to the slave weather station.
* Needs:
*   command:
*       This is the command type to send to the Slave (ACKNAK).
*
*   dataGood:
*       This is the indication of whether the received data packet was good(ACK) or bad(NAK).
*
* Returns:
*   Nothing.
*******************************************************************************************************************/
void sendAckNakToSlave(commandTypes command, bool dataGood)
{
    #if defined (DEBUG21)
        Serial.print(F("This is the sendAckNakToSlave Sub. This is the size of the dataReturnedToSlave Struct: "));
        Serial.println(sizeof(dataReturnedToSlave));
    #endif
    
    
    if (dataGood)
        {
            dataReturnedToSlave.returnText[0] = 'A';  // Return the Ack indicating all is OK.
            dataReturnedToSlave.returnText[1] = 'C';  // Return the Ack indicating all is OK.
            dataReturnedToSlave.returnText[2] = 'K';  // Return the Ack indicating all is OK.
            #if defined (DEBUG21)
                Serial.println(F("This is the sendAckNakToSlave Sub. Sending the ACK\n"));
            #endif
        }
    else
        {
            dataReturnedToSlave.returnText[0] = 'N';  // Return the Nak indicating all is Not OK.
            dataReturnedToSlave.returnText[1] = 'A';  // Return the Nak indicating all is Not OK.
            dataReturnedToSlave.returnText[2] = 'K';  // Return the Nak indicating all is Not OK.
            #if defined (DEBUG21)
                Serial.println(F("This is the sendAckNakToSlave Sub. Sending the NAK\n"));
            #endif
        }
    
    sendDataToSlave(command, &dataReturnedToSlave, sizeof(dataReturnedToSlave));
    
}


/*******************************************************************************************************************
*               This function Sends the Time commands to the slave weather station.
* Needs:
*      Nothing.
*
* Returns:
*   boolean:
*       true if no error,
*       false if errror.
*******************************************************************************************************************/
bool setSlaveRTC(void)
{
    tmElements_t timeNow;
    time_t UnixTimeNow;
    commandTypes command = SETTIME;
    
    #if defined (DEBUG21)
        Serial.print(F("This is the sendTimeCommandsToSlave Sub. This is the size of the unixTime Struct: "));
        Serial.println(sizeof(unixTime));
        Serial.println(F(""));
    #endif
    
    /*******************************************************************************************************************
     * Get current time from DS3231 and put into variables curentTime[0] (hh), [1] (mm), & [2] (ss).
     *******************************************************************************************************************/
    if (RTC.read(timeNow))
        {
            UnixTimeNow = makeTime(timeNow);  // Converts tmElements array into a Time_T (UNIX Time), LastTimeSetByClock is a Time_T (UNIX Time) variable.
        }
    else
        {
            if (RTC.chipPresent())
                {
                    Serial.println(F("The DS3231 is stopped.  Please run the SetTime"));
                    Serial.println(F("example to initialize the time and begin running."));
                    Serial.println();
                    return false;
                }
            else
                {
                    Serial.println(F("DS3231 read error!  Please check the circuitry."));
                    Serial.println();
                    return false;
                }
        }
    sendDataToSlave(command, &UnixTimeNow, sizeof(UnixTimeNow));
    return true;
}


/*******************************************************************************************************************
*  This function gets the entire Weather Struct from the slave weather station.
* Needs:
*   The buffer pointer to put the incoming data in (WeatherDataStruct).
*       void* ptr
*
* Returns:
*   A code which tells whether of not the data packet was recieved correctly.
*       Return Code:
*           GOOD_PACKET         = 0
*           BAD_PACKET          = 1
*           PACKET_NOT_FOUND    = 2
*           BAD_CRC             = 3
*           BAD_ACK             = 4
*           BAD_COMMAND         = 5
*           BAD_PREFIX_CHAR     = 6
*           BAD_SUFFIX_CHAR     = 7
*           BAD_DATA_SIZE       = 8
 *******************************************************************************************************************/
packetStatus getDataFromSlave( void *inputDataPointer)
{
    uint8_t CRC1, CRC2;
    uint16_t inputCRC;
    uint8_t i, j;
    
    digitalWrite(13, HIGH);  // This turns on the LED.
    
    #if defined (DEBUG17)
        Serial.print(F("\nThis is the getDataFromSlave Sub. This is the size of the data Returned From Master: "));
        Serial.println(numberOfBytesFromDMAReceive);
        Serial.println(F("Printing the data in the receiveBuffer Buffer..."));
        printBuffer(receiveBuffer, RxBUFSIZE);
    #endif
    
    
    i = 0;  // Start with the Pointer to the input data array at zero.
    j = 0; // Start with the pointer to the receiveBuffer array at zero.
    
    #if defined (DEBUG17)
        Serial.println(F("Begin transferring bytes from transmit buffer to local buffer..."));
        Serial.print(F("This is the value of j: "));
        Serial.println(j, DEC);
        Serial.println(F("Read the prefix character from the transmit Buffer OK..."));
        Serial.print(F("This is the value of the prefix character: "));
        Serial.println( ( receiveBuffer[j] ), DEC);
    #endif
    
    // Receive the data from the slave.
    // The recieved packet is in the receiveBuffer, (Stx), (Command), (Size of Data Packet), (Data...), (CRC1),(CRC2), (Etx).
    // It needs to be parsed for a command byte and good data.
    if( ( receiveBuffer[j] ) != (byte)prefixChar )  // Check for the correct Prefix Character.
        {
            // Prefix character is not found
            #if defined (DEBUG17)
                Serial.println(F("Prefix character is not found!\n"));
            #endif
            digitalWrite(13, LOW);  // This turns off the LED.
            return BAD_PACKET;
        }
    else
        {
            j++; // increment the pointer to the receiveBuffer array.
        
            #if defined (DEBUG17)
                Serial.println(F("Reading the receive Command from the transmit Buffer..."));
                Serial.print(F("This is the value of the Command Code: "));
                Serial.println( ( receiveBuffer[j] ), DEC);
                Serial.print(F("This is the value of j: "));
                Serial.println(j, DEC);
            #endif
        
            receiveCommand = (commandTypes)receiveBuffer[j];  // This is the number of the command recieved from the master to execute.
            if ( !(receiveCommand >= 0 ) && ( receiveCommand < maxCommandNumber ) )
                {
                    #if defined (DEBUG17)
                        Serial.println(F("Receive Command is bad!\n"));
                    #endif
                    digitalWrite(13, LOW);  // This turns off the LED.
                    return BAD_COMMAND;
                }
        
            j++; // increment the pointer to the receiveBuffer array.
        
            #if defined (DEBUG17)
                Serial.println(F("Read the receive Command from the transmit Buffer OK..."));
                Serial.print(F("This is the value of j: "));
                Serial.println(j, DEC);
                Serial.print(F("This is the size of the Command Data Packet: "));
                Serial.println( ( receiveBuffer[j] ), DEC);
            #endif
        
            receivedDataSize = receiveBuffer[j];  // This is the size of the command data packet (<= 127 bytes).
            if ( receivedDataSize > maxRxDataBufferSize )
                {
                    #if defined (DEBUG17)
                        Serial.println(F("Receive Data Size is more than the buffer size!\n"));
                    #endif
                    digitalWrite(13, LOW);  // This turns off the LED.
                    return BAD_DATA_SIZE;
                }
        
            j++; // increment the pointer to the receiveBuffer array.
        
            uint8_t data[receivedDataSize];  // Create a tmp char array of the data from the Uart.
            #if defined (DEBUG17)
                Serial.println(F("Read the receive Data Size from the transmit Buffer OK..."));
                Serial.print(F("This is the value of receivedDataSize: "));
                Serial.println(receivedDataSize, DEC);
                Serial.print(F("This is the value of j: "));
                Serial.println(j, DEC);
            #endif
        
            for( i = 0; i < ( receivedDataSize ); i++)
                {
                    data[i] = (uint8_t)receiveBuffer[j];  // Copy the data from the receive Buffer to the temp byte array.
                    j++; // increment the pointer to the receiveBuffer array.
                }
        
            // Need to get the 2 byte CRC from the next two bytes in the receiveBuffer (DMA data from Uart2).
            /// Need to do the CRC check on these two characters.
            #if defined (DEBUG17)
                Serial.println(F("Transfered the data from the transmit buffer to the temp array..."));
                Serial.print(F("This is the data in the buffer: "));
                printBuffer(data, receivedDataSize);
                Serial.print(F("\nThis is the value of j: "));
                Serial.println(j, DEC);
            #endif
        
            /// Need to move the pointer to the first CRC byte all the way past the 121 byte data packet.
        
            j = j + ( maxRxDataBufferSize - receivedDataSize );
        
            #if defined (DEBUG17)
                Serial.println(F("Moved the j pointer to the end of the data packet..."));
                Serial.print(F("This is the value of j: "));
                Serial.println(j, DEC);
            #endif
        
        
            CRC1 = (uint8_t)receiveBuffer[j];
        
            j++; // increment the pointer to the receiveBuffer array.
        
            #if defined (DEBUG17)
                Serial.print(F("This is the CRC1 byte from the receiveBuffer: "));
                Serial.println(CRC1, HEX);
                Serial.print(F("This is the value of j: "));
                Serial.println(j, DEC);
            #endif
        
            CRC2 = (uint8_t)receiveBuffer[j];
        
            #if defined (DEBUG22)
                Serial.print(F("This is the CRC2 byte from the receiveBuffer: "));
                Serial.println(CRC2, HEX);
            #endif
        
            /// Convert two 8 bit bytes to a 16 bit word.
            inputCRC = 0;  // Start out with zero
            inputCRC = ( ( uint16_t)CRC2 << 8);  // This is the high byte, shift left 8 places and put into the 16 bit inputCRC variable.
            inputCRC = (inputCRC | ( uint16_t)CRC1);
        
            #if defined (DEBUG17)
                Serial.print(F("This is the returned CRC word: "));
                Serial.println(inputCRC, HEX);
            #endif
        
            // bool crcCheck(void * inputBuffer, uint16_t bufferSize , uint16_t inputCRC)
            if ( crcCheck(data, receivedDataSize, inputCRC) == false )
                {
                    // The CRC recieved was bad.
                    #if defined (DEBUG17)
                        Serial.println(F("The CRC recieved was bad!\n"));
                    #endif
                    digitalWrite(13, LOW);  // This turns off the LED.
                    return BAD_CRC;
                }
            else
                {
                    j++; // increment the pointer to the receiveBuffer array.
            
                    #if defined (DEBUG17)
                        Serial.print(F("This is the value of j: "));
                        Serial.println(j, DEC);
                    #endif
            
                    if( receiveBuffer[j] !=  suffixChar)
                        {
                            // Suffix character is not found
                            #if defined (DEBUG17)
                                Serial.println(F("Suffix character is Bad!\n"));
                            #endif
                            digitalWrite(13, LOW);  // This turns off the LED.
                            return BAD_SUFFIX_CHAR;
                        }
                    else
                        {
                            #if defined (DEBUG17)
                                Serial.println(F("Coping the bytes into the struct passed in!"));
                            #endif
                
                            memcpy(inputDataPointer, data, receivedDataSize);  // Copy the bytes from the receiveBuffer (DMA data from Uart2) into the dataReturnedFromMaster Struct.
                
                            #if defined (DEBUG17)
                                Serial.println(F("Good Packet found!\n"));
                            #endif
                            digitalWrite(13, LOW);  // This turns off the LED.
                            return GOOD_PACKET;
                        }
                }
        }
}


/*******************************************************************************************************************
*  This function checks for the proper ACK from the slave weather station.
*
* Needs:
*   void* inputBufferPointer:
*       The buffer pointer to get the incoming data from (commandMessageDataFromMaster).
*
* Returns:
*   Boolean:
*       false if ACK is good.
*       true if ACK is bad.
*******************************************************************************************************************/
bool checkForACK(uint8_t *inputBufferPointer)
{
    if (inputBufferPointer[0] == 'A')
        {
            if (inputBufferPointer[1] == 'C')
                {
                    if (inputBufferPointer[2] == 'K')
                        {
                            #if defined (DEBUG22)
                                Serial.println(F("\nThis is the checkForACK Sub. Return packet received from the master has an ACK!\n"));
                            #endif
                            return false;
                        }
                    else
                        {
                            #if defined (DEBUG22)
                                Serial.println(F("Return packet received from the master is missing K!\n"));
                            #endif
                            return true;
                        }
                }
            else
                {
                    #if defined (DEBUG22)
                        Serial.println(F("Return packet received from the master is missing C!\n"));
                    #endif
                    return true;
                }
        }
    else
        {
            #if defined (DEBUG22)
                Serial.println(F("Return packet received from the master is missing A!\n"));
            #endif
            return true;
        }
    return true;
}


/*******************************************************************************************************************
*                   This function processes the input data packet received from the Slave.
* Needs:
*   Nothing.
*
* Returns:
*   Boolean:
*       false if bad packet is found.
*       true if good packet is found and processed.
*******************************************************************************************************************/
bool processPacketFromSlave(void)
{
    packetStatus temp = PACKET_NOT_FOUND;
    
    #if defined(DEBUG43)
        Serial.println(F("This is the processPacketFromSlave Sub. Received data, now printing the buffer:"));
        printBuffer(receiveBuffer, sizeof(receiveBuffer));
    #endif
    
    temp = getDataFromSlave(&commandMessageDataFromSlave);
    if ( temp == GOOD_PACKET )  // getDataFromSlave returns GOOD_PACKET if there is good data available.
        {
            switch (receiveCommand)  // This is the Received command from the Slave.
                {
                    case ACKNAK:
                        if( !checkForACK(commandMessageDataFromSlave) )  // Check to see if the ACK was received correctly.
                            {
                                #if defined(DEBUG43)
                                    Serial.println(F("This is the processPacketFromSlave Sub. Received the ACK from the Slave OK!"));
                                #endif
                            }
                        else
                            {
                                #if defined(DEBUG43)
                                    Serial.println(F("This is the processPacketFromSlave Sub. Received ACK from the Slave was BAD!\n"));
                                #endif
                            }
                        memset(commandMessageDataFromSlave, 0, sizeof(commandMessageDataFromSlave));  // Clear the Master Message Data buffer.
                        return true;
                        
                    case XFERWDSTRCT:
                        // Move the data from the command message data buffer from the Slave to the weather data struct.
                        memcpy(&WeatherDataStruct, commandMessageDataFromSlave, sizeOfWeatherDataStruct);  // Copy the bytes from the commandMessageDataFromSlave buffer into the WeatherDataStruct.
                        
                        slaveDataReceived = true;  // Received Weather data struct from slave (outside) weather station.
                        if (firstOutsideReading)
                            {
                                outsideDailyHighTemp = WeatherDataStruct.Temperature;
                                outsideDailyLowTemp = WeatherDataStruct.Temperature;
                                firstOutsideReading = false;
                            }

                        #if defined(DEBUG43)
                            Serial.println(F("This is the processPacketFromSlave Sub. Received the remote data struct from the Slave OK!\n"));
                        #endif
                        memset(commandMessageDataFromSlave, 0, sizeof(commandMessageDataFromSlave));  // Clear the Master Message Data buffer.
                        printStatusDotOnTFT(XRIGHT - (Xsize * 17), (YLINE14 + 16), RA8875_GREEN);  // Print green dot to indicate good data.
                        return true;

                    default:
                        #if defined(DEBUG43)
                            Serial.println(F("This is the processPacketFromSlave Sub. Undefined command received..\n"));
                        #endif
                        sendAckNakToSlave(ACKNAK, false);  // Send the ACK to the Slave with good data set false.
                        memset(commandMessageDataFromSlave, 0, sizeof(commandMessageDataFromSlave));  // Clear the Master Message Data buffer.
                        printStatusDotOnTFT(XRIGHT - (Xsize * 17), (YLINE14 + 16), RA8875_YELLOW);  // Print red dot to indicate bad data.
                        return false;
                }
        
        }
    else
        {
            sendAckNakToSlave(ACKNAK, false);  // Send the NAK to the Slave with good data set false.
            
            #if defined(DEBUG43)
                Serial.println(F("This is the processPacketFromSlave Sub. Data recieved from the Slave was bad!, Here is the Code: "));
                switch (temp)
                    {
                        case BAD_PACKET:
                            Serial.println(F("Bad Packet\n"));
                            return false;
                
                        case PACKET_NOT_FOUND:
                            Serial.println(F("Packet Not Found\n"));
                            return false;
                
                        case BAD_CRC:
                            Serial.println(F("Bad CRC\n"));
                            return false;
                
                        case BAD_COMMAND:
                            Serial.println(F("Bad ACK\n"));
                            return false;
                
                        case BAD_PREFIX_CHAR:
                            Serial.println(F("BAD Prefix\n"));
                            return false;
                
                        case BAD_SUFFIX_CHAR:
                            Serial.println(F("BAD Suffix\n"));
                            return false;
                
                        case BAD_DATA_SIZE:
                            Serial.println(F("BAD Data Size\n"));
                            return false;
                
                        default:
                            Serial.println(F("Unrecognized Error\n"));
                            return false;
                    }
                #endif  // End of #if defined(DEBUG38)
            return false;
        }
}


/*******************************************************************************************************************
*  This function clears the offset float number in the RTC EEPROM to be used to add to the value submitted by the
*  outside Weather Station rain/year, for display on the inside Weather Station Rain/Year Value.
*
* Needs:
*   Nothing.
* Returns:
*   Nothing.
*******************************************************************************************************************/
void clearMasterRainOffset(void)
{
        eepromMemory.writeFloat(rainMemoryLocation, 0);  // First 4 bytes is the rain per year in float.
}


/*******************************************************************************************************************
* ----------------------------------------------FIX THIS------------------------------------------------------------
*  This function checks for a touch from the TFT touchscreen, either resistive or capacitive.
*
* Needs:
*   Nothing.
* Returns:
*   boolean:
*       Found touch(true).
*       No touch found(false).
*******************************************************************************************************************/
bool checkForTouch(void)
{
    Watchdog.reset();  // Kick the dog, its set to reset the teensy in 12 seconds.
    
    #if defined(USE_FT5206_TOUCH)  // We are using the capacitive touch screen controller.
    
        uint8_t touch_state, nr_of_touches;  // , gesture;
    
        if (tft.touched(true))  // If touched is "true" detach isr.
            {
                // At this point we need to fill the FT5206 registers...
                tft.updateTS();  // Now we have the data inside the library.
                nr_of_touches = tft.getTouches();
                touch_state = tft.getTouchState();
                //gesture = tft.getGesture();
        
                if (touch_state > 0)
                    {
                        switch (touch_state)
                            {
                                case 0:
                                    break;
                    
                                case 1:  // Finger up is state 1
                                    foundTouchFingerUp = true;  // Finger up found.
                                    break;
                    
                                case 2:  // Finger down is state 2
                                    // Get the coordinates, We need a two dimensional array:
                                    // uint16_t coordinates[MAXTOUCHLIMIT][2];  // to hold coordinates
                                    tft.getTScoordinates(touchScreenCoordinatesArray);  // done
                    
                                    // Now the coordinates array has the x,y coordinates of all touches
                                    for (uint8_t i = 0; i < nr_of_touches; i++)
                                        {
                                            xPositionOfTouchedSpot = touchScreenCoordinatesArray[ i ] [ 0 ];
                                            yPositionOfTouchedSpot = touchScreenCoordinatesArray[ i ] [ 1 ];
                                        }
                    
                                    foundTouchFingerDown = true;
                                    break;
                    
                                default:
                                    break;
                            }
            
                    }
        
                nr_of_touches = 0;
                touch_state = 0;
                //gesture = 0;
        
                if ( (foundTouchFingerDown == true) && (foundTouchFingerUp == true) )
                    {
                        foundTouchFingerUp = false;
                        foundTouchFingerDown = false;
                        tft.enableCapISR();  // capacitive touch screen interrupt it's armed.
                        return true;
                    }
        
                tft.enableCapISR();  // capacitive touch screen interrupt it's armed.
                return false;
            }
        else
            {
                tft.enableCapISR();  // capacitive touch screen interrupt it's armed.
                return false;
            }
    
    #endif
    
    
    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
    
        if (tft.touched(true))
            {
                tft.enableISR(false);  // Stop any futher interrupts.
                tft.touchEnable(false);  // Set the touch enable false so no futher touches will occur.
                tft.touchReadPixel(&xPositionOfTouchedSpot, &yPositionOfTouchedSpot);  // Read directly in pixels!
                return true;
            }
        else
            {
                return false;
            }
    #endif
    
}


/*******************************************************************************************************************
*  This function waits up to 8 seconds for another touch from the TFT touch screen.
*
* Needs:
*   Nothing
* Returns:
*   Boolean:
*       true if another touch was found.
*       false if no touch after 8 seconds ellapsed.
*******************************************************************************************************************/
bool waitForAnotherTouch(void)
{
    bool anotherTouchFound;
    unsigned long currentMillis = 0;
    
    #if defined(DEBUG35)
        Serial.println(F("\nThis is the waitForAnotherTouch Sub"));
    #endif
    
    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
        delay(150);  // Wait 150 milliseconds.
        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
        tft.enableISR(true);  // Start up interrupts again.
    #endif
    
    anotherTouchFound = false;
    
    currentMillis = millis();

    while ( ( (millis() - currentMillis) <= 8000) )  // Wait for 8 seconds for another touch, otherwise return with error.
        {
            if (checkForTouch() == true)
                {
                    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                        delay(300);  // Wait 300 milliseconds.
                    #endif
            
                    #if defined(DEBUG35)
                        Serial.println(F("This is the waitForAnotherTouch Sub: Caught another touch..\n"));
                    #endif
            
                    anotherTouchFound = true;
                    break;
                }
            delay(100);  // Wait 50 milliseconds.
        }
    
    if (anotherTouchFound == true)
        {
            return true;
        }
    else
        {
            #if defined(DEBUG35)
                Serial.println(F("This is the waitForAnotherTouch Sub: Waited 8 seconds and found no touch.\n"));
            #endif
            return false;
        }
}


/*******************************************************************************************************************
*   This function creates a 14 button Numeric Keypad on the TFT in order to input the number from the TFT screen.
*   __________________
*  | 8 | 9 | - | C |
*   ------------------
*  | 4 | 5 | 6 | 7 |
*   ------------------
*  | 0 | 1 | 2 | 3 |
*   ------------------
*  | Enter  |  .   |
*   ------------------
* There are 4 rows of 4 columns of Buttons.
*
* Needs:
*   Nothing
* Returns:
*   uint8_t:
*       The name of the button touched.
*******************************************************************************************************************/
uint8_t numericKeypad(void)
{
    TouchScreenPoints_t postionsJustTouched;
    postionsJustTouched.xPosition = 0;
    postionsJustTouched.yPosition = 0;
    
    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
        tft.enableISR(true);  // Start up interrupts again.
    #endif
    
    if (waitForAnotherTouch() == true)  //  Wait for Another touch returns true if touch was found otherwise false.
        {
            postionsJustTouched.xPosition = xPositionOfTouchedSpot;
            postionsJustTouched.yPosition = yPositionOfTouchedSpot;
            // This is the first box from the left on top row.
            if ( Button8.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. - We have a hit inside the 8 box...\n"));
                    #endif
            
                    return EIGHT_BUTTON;
                }
        
            // This is the second box from the left on the top row.
            else if ( Button9.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. We have a hit inside the 9 box...\n"));
                    #endif
            
                    return NINE_BUTTON;
                }
        
            // This is the third box from the left on the top row.
            else if ( ButtonMinusSign.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. We have a hit inside the - box...\n"));
                    #endif
                
                    return MINUS_SIGN_BUTTON;
                }
            
            // This is the forth box from the left on the top row.
            else if ( ButtonC.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. We have a hit inside the C box...\n"));
                    #endif
            
                    return CLEAR_BUTTON;
                }
        
            // This is the first box from the left on the second row.
            else if ( Button4.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. We have a hit inside the 4 box...\n"));
                    #endif
            
                    return FOUR_BUTTON;
                }
            // This is the second box from the left on the second row.
            else if ( Button5.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. We have a hit inside the 5 box...\n"));
                    #endif
            
                    return FIVE_BUTTON;
                }
        
            // This is the third box from the left on the second row.
            else if ( Button6.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. We have a hit inside the 6 box...\n"));
                    #endif
            
                    return SIX_BUTTON;
                }
        
            // This is the forth box from the left on the second row.
            else if ( Button7.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. - We have a hit inside the 7 box...\n"));
                    #endif
            
                    return SEVEN_BUTTON;
                }
        
            // This is the first box from the left on the third row.
            else if ( Button0.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. We have a hit inside the 0 box...\n"));
                    #endif
            
                    return ZERO_BUTTON;
                }
        
            // This is the second box from the left on the third row.
            else if ( Button1.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. We have a hit inside the 1 box...\n"));
                    #endif
            
                    return ONE_BUTTON;
                }
        
            // This is the third box from the left on the third row.
            else if ( Button2.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. We have a hit inside the 2 box...\n"));
                    #endif
            
                    return TWO_BUTTON;
                }
            // This is the forth box from the left on the third row.
            else if ( Button3.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. We have a hit inside the 3 box...\n"));
                    #endif
            
                    return THREE_BUTTON;
                }
            
            // This is the first box from the left on the forth row.
            else if ( ButtonDecimalPoint.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. We have a hit inside the decimal point box...\n"));
                    #endif
                
                    return DECIMALPNT_BUTTON;
                }
            
            // This is the second box from the left on the forth row.
            else if ( ButtonEnter.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG36)
                        Serial.println(F("This is the numericKeypad Sub. We have a hit inside the Enter box...\n"));
                    #endif
            
                    return ENTER_BUTTON;
                }
            else
                {
                    return NO_BUTTON;
                }
        }
    else
        {
            #if defined(DEBUG36)
                Serial.println(F("This is the numericKeypad Sub: Waited 8 seconds and found no touch.\n"));
            #endif
            return NO_BUTTON;
        }
}


/*******************************************************************************************************************
*   This function prints the number on the TFT screen from the keypad inputs.
* Needs:
*   float:
*       Answer = The number typed into the TFT numeric keypad.
* Returns:
*   Nothing
*******************************************************************************************************************/
void tftPrintKeypadAnswer(float Answer)
{
    TouchScreenPoints_t answerRndRectLocation;
    
    #if defined(DEBUG37)
        Serial.println(F("\nThis is the tftPrintKeypadAnswer Sub"));
        #endif
    
    answerRndRectLocation = ButtonAnswer.getPosition();  // Get the location of the keypad answer box on the tft.
    
    ButtonAnswer.draw();  // This Clears the Answer Button of any data located inside.
    
    tft.setTextColor(RA8875_RED, RA8875_WHITE);  //set text color & backgroud color.
    tft.setFontSize((RA8875tsize)RA8875_FONTSCALE);
    
    tft.setCursor( ( answerRndRectLocation.xPosition + Xsize ), ( answerRndRectLocation.yPosition + ( Ysize / 2 ) ) );
    tft.print(Answer, 3);  // Print the number typed into the TFT numeric keypad.
    
    
    tft.setTextColor(RA8875_WHITE);  // Return the text color & backgroud color (Transparent) to there default.
    tft.setFontSize((RA8875tsize)RA8875_FONTSCALE);  // Return the text size to the default.
    
    #if defined(DEBUG37)
        Serial.print(F("This is the Answer:"));
        Serial.println(Answer, 3);
        Serial.println(F(""));
    #endif
}

/*******************************************************************************************************************
*  This function creates a 14 button Numeric Keypad on the TFT in order to input the number from the TFT screen.
*  And then returns the number entered thru the on screen Numeric Keypad to the caller.
*   __________________
*  | 8 | 9 | - | C |
*   ------------------
*  | 4 | 5 | 6 | 7 |
*   ------------------
*  | 0 | 1 | 2 | 3 |
*   ------------------
*  | Enter  |  .   |
*   ------------------
* There are 4 rows of 4 columns of Buttons.
* Needs:
*   Nothing
*
*   boolean:
*       pass(true).
*       fail(false).
*******************************************************************************************************************/
bool getNumberFromNumericKeypad(void)
{
    uint8_t keyPressed = NO_BUTTON;
    float tempNumber1 = 0, tempNumber2 = 0;
    uint8_t numberOfDecimalPoints = 0;
    bool NegativeNumber = false;
    
    #if defined(DEBUG38)
        Serial.println(F("\nThis is the getNumberFromNumericKeypad Sub. It draws a 14 button Numeric Keypad with Answer Box on the TFT screen."));
    #endif
    tft.clearScreen();  // Clear the tft screen.
    
    /// Put up the 12 button Numeric Keypad on the TFT Screen.
    Button8.draw();
    Button9.draw();
    ButtonMinusSign.draw();
    ButtonC.draw();
    Button4.draw();
    Button5.draw();
    Button6.draw();
    Button7.draw();
    Button0.draw();
    Button1.draw();
    Button2.draw();
    Button3.draw();
    ButtonEnter.draw();
    ButtonDecimalPoint.draw();
    ButtonAnswer.draw();  // This Rounded Rectangle has no text, this is where the answer will be printed.
    
    do
        {
            keyPressed = numericKeypad();  // This function returns the key pressed.
            switch (keyPressed)
                {
                    case ZERO_BUTTON ... NINE_BUTTON:
                        #if defined(DEBUG38)
                            Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. This is the BUTTON pressed: "));
                            Serial.println(keyPressed);
                        #endif
                        
                        tempNumber1 = ( ( tempNumber1 * 10 ) + keyPressed );  // Shift previous number left (multiply by 10) and add new number.
                        tftPrintKeypadAnswer(tempNumber1);
                        
                        #if defined(DEBUG38)
                            Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. Found a number between 0 & 9, This is the number entered: "));
                            Serial.println(tempNumber1, 3);
                        #endif
                        
                        break;
                        
                    case MINUS_SIGN_BUTTON:
                        #if defined(DEBUG38)
                            Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. The Minus Sign was pressed, Now setting the Negative Number Flag."));
                        #endif
                        if (NegativeNumber == true)
                            {
                                NegativeNumber = false;
                            }
                        else
                            {
                                NegativeNumber = true;
                            }
                        
                        break;
                        
                    case DECIMALPNT_BUTTON:
                        #if defined(DEBUG38)
                            Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. The DECIMAL POINT BUTTON was pressed, getting the numbers less than 1...\n"));
                        #endif
                        numberOfDecimalPoints = 1;
                        do
                            {
                                keyPressed = numericKeypad();  // This function returns the key pressed.
                                switch (keyPressed)
                                    {
                                        case ZERO_BUTTON:
                                            #if defined(DEBUG38)
                                                Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. This is the BUTTON pressed: "));
                                                Serial.println(keyPressed);
                                            #endif
                                            tempNumber2 = ( tempNumber2 + (keyPressed * ( powf(10 , -numberOfDecimalPoints) ) ) );  // Shift previous number right (divide by 10).
                                            tftPrintKeypadAnswer((float)(tempNumber2 + tempNumber1));
                                            
                                            #if defined(DEBUG38)
                                                Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. Found a number 0, This is the number entered: "));
                                                Serial.println(tempNumber2, 3);
                                            #endif
                                            break;
                                            
                                        case ONE_BUTTON ... NINE_BUTTON:
                                            tempNumber2 = ( tempNumber2 + (keyPressed * ( powf(10 , -numberOfDecimalPoints) ) ) );  // Shift previous number right (divide by 10) and add new number.
                                            tftPrintKeypadAnswer((float)( tempNumber2 + tempNumber1 ) );
                                        
                                            #if defined(DEBUG38)
                                                Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. This is the BUTTON pressed: "));
                                                Serial.println(keyPressed);
                                                Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. This is the numberOfDecimalPoints number: "));
                                                Serial.println(numberOfDecimalPoints);
                                                Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. This is the results of the powf function: "));
                                                Serial.println( ( keyPressed * ( powf(10 , -numberOfDecimalPoints) ) ),3);
                                                Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. This is the tempNumber2: "));
                                                Serial.println(tempNumber2, 3);
                                            #endif
                                            break;
                                        
                                        default:
                                            break;
                                    }
                                numberOfDecimalPoints++;
                            }
                        while ( ( keyPressed != ENTER_BUTTON ) && ( keyPressed != NO_BUTTON ) && ( keyPressed != CLEAR_BUTTON ) && ( keyPressed != DECIMALPNT_BUTTON ) && ( keyPressed != MINUS_SIGN_BUTTON )  );
                        
                        if (keyPressed == ENTER_BUTTON)
                            {
                                if (NegativeNumber == true)
                                    {
                                        numberEnteredFromKeypad = ( (tempNumber1 + tempNumber2) * (-1) );
                                        tftPrintKeypadAnswer(numberEnteredFromKeypad);
                                        #if defined(DEBUG38)
                                            Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. Return the number entered into the keypad: "));
                                            Serial.println( (numberEnteredFromKeypad), 3 );
                                            Serial.println(F(""));
                                        #endif
                                    }
                                else
                                    {
                                        numberEnteredFromKeypad = (tempNumber1 + tempNumber2);
                                        tftPrintKeypadAnswer(numberEnteredFromKeypad);
                                        #if defined(DEBUG38)
                                            Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. Return the number entered into the keypad: "));
                                            Serial.println( (numberEnteredFromKeypad), 3 );
                                            Serial.println(F(""));
                                        #endif
                                    }

                                return true;
                            }
                        else if ( keyPressed == NO_BUTTON )
                            {
                                #if defined(DEBUG38)
                                    Serial.println(F("\nThis is the getNumberFromNumericKeypad Sub. Found NO_BUTTON, returning false or ERROR!\n"));
                                #endif
                                return false;
                            }
                        else if ( keyPressed == CLEAR_BUTTON )
                            {
                                #if defined(DEBUG38)
                                    Serial.println(F("\nThis is the getNumberFromNumericKeypad Sub. Found CLEAR_BUTTON inside the decimal routine..."));
                                #endif
                                tempNumber2 = 0;
                                tftPrintKeypadAnswer((float)(tempNumber2 + tempNumber1));
                            }
                        else if ( keyPressed == DECIMALPNT_BUTTON )
                            {
                                #if defined(DEBUG38)
                                    Serial.println(F("\nThis is the getNumberFromNumericKeypad Sub. Found DECIMALPNT_BUTTON, returning false or ERROR!\n"));
                                #endif
                                return false;
                            }
                        else if ( keyPressed == MINUS_SIGN_BUTTON )
                            {
                                #if defined(DEBUG38)
                                    Serial.println(F("\nThis is the getNumberFromNumericKeypad Sub. Found MINUS_SIGN_BUTTON!\n"));
                                #endif
                                NegativeNumber = true;
                                if (NegativeNumber == true)
                                    {
                                        tftPrintKeypadAnswer( ((float)(tempNumber2 + tempNumber1)) * (-1) );
                                    }
                                else
                                    {
                                        tftPrintKeypadAnswer((float)(tempNumber2 + tempNumber1));
                                    }
                            }
                        else
                            {
                                #if defined(DEBUG38)
                                    Serial.println(F("\nThis is the getNumberFromNumericKeypad Sub. Found Underfined BUTTON or ERROR!\n"));
                                #endif
                                return false;
                            }
                        break;
                
                    case CLEAR_BUTTON:
                        tempNumber1 = 0;
                        tempNumber2 = 0;
                        tftPrintKeypadAnswer((float)(tempNumber2 + tempNumber1));
                        #if defined(DEBUG38)
                            Serial.println(F("\nThis is the getNumberFromNumericKeypad Sub. Set Answer to Zero..."));
                        #endif
                        break;
                
                    default:
                        break;
                        
                }
        }
    while ( ( keyPressed != ENTER_BUTTON ) && ( keyPressed != NO_BUTTON ) );
    
    if (keyPressed == ENTER_BUTTON)
        {
            if (NegativeNumber == true)
                {
                    numberEnteredFromKeypad = ( (tempNumber1 + tempNumber2) * (-1) );
                    tftPrintKeypadAnswer(numberEnteredFromKeypad);
                    #if defined(DEBUG38)
                        Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. Return the number entered into the keypad: "));
                        Serial.println( (numberEnteredFromKeypad), 3 );
                        Serial.println(F(""));
                    #endif
                }
            else
                {
                    numberEnteredFromKeypad = (tempNumber1 + tempNumber2);
                    tftPrintKeypadAnswer(numberEnteredFromKeypad);
                    #if defined(DEBUG38)
                        Serial.print(F("\nThis is the getNumberFromNumericKeypad Sub. Return the number entered into the keypad: "));
                        Serial.println( (numberEnteredFromKeypad), 3 );
                        Serial.println(F(""));
                    #endif
                }
            
            return true;
        }
    else if ( keyPressed == NO_BUTTON )
        {
            #if defined(DEBUG38)
                Serial.println(F("\nThis is the getNumberFromNumericKeypad Sub. Found NO_BUTTON, returning false or ERROR!\n"));
            #endif
            return false;
        }
    else
        {
            #if defined(DEBUG38)
                Serial.println(F("\nThis is the getNumberFromNumericKeypad Sub. Found Undefined BUTTON or ERROR!\n"));
            #endif
            return false;
        }
}


/*******************************************************************************************************************
*  This function puts an offset float number in the RTC EEPROM to be used to add to the value submitted by the
*  outside Weather Station rain/year, for display on the inside Weather Station Rain/Year Value.
*
* Needs:
*   Nothing
*
*   boolean:
*       pass(true).
*       fail(false).
*******************************************************************************************************************/
bool setMasterYearlyRainOffset(void)
{
    #if defined(DEBUG39)
        float yearlyRainOffsetFromEEPROM = 0;
    #endif
    
    #if defined(DEBUG39)
        Serial.print(F("\nThis is the setMasterYearlyRainOffset function..."));
    #endif
    
    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
        tft.enableISR(true);  // Start up interrupts again.
    #endif
    
    
    if (getNumberFromNumericKeypad() == true)
        {
            #if defined(DEBUG39)
                Serial.print(F("This is the number from the numeric keypad to be entered into the EEPROM: "));
                Serial.println(numberEnteredFromKeypad, 3);
                Serial.print(F("This is the Rain Memory address location to write to in the EEPROM: "));
                Serial.println(rainMemoryLocation, HEX);
                Serial.println(F(""));
            #endif
            
            eepromMemory.writeFloat(rainMemoryLocation, numberEnteredFromKeypad);  // First 4 bytes is the rain per year in float.
            
            #if defined(DEBUG39)  // This the the TFT debug flag.
                Serial.print(F("This is the Rain Memory address location to read from in the EEPROM: "));
                Serial.println(rainMemoryLocation, HEX);
                yearlyRainOffsetFromEEPROM = eepromMemory.readFloat(rainMemoryLocation);  // First 4 bytes is the rain per year in float.
                Serial.print(F("Read EEPROM Memory yearly rain offset, Rain Offset: "));
                Serial.println(yearlyRainOffsetFromEEPROM, 3);
            #endif

            return true;
        }
    else
        {
            #if defined(DEBUG39)
                Serial.print(F("The numeric keypad function produced an ERROR!\n"));
                Serial.println(numberEnteredFromKeypad, 3);
                Serial.println(F(""));
            #endif
            
            return false;
        }
}


/*******************************************************************************************************************
*               This function Sends the rain commands to the slave weather station.
* Needs:
*   command:
*       This is the command type to send to the Slave.
*           Rain Commands: CLEARRAIN, RESETRAIN, SETRAINTOTAL.
 *
 *   boolean:
 *       pass(true).
 *       fail(false).
*******************************************************************************************************************/
bool sendRainCommandsToSlave(commandTypes command)
{
    #if defined (DEBUG21)
        Serial.print(F("This is the sendClearRainToSlave Sub. This is the size of the dataReturnedToSlave Struct: "));
        Serial.println(sizeof(dataReturnedToSlave));
    #endif
    
    switch (command)
        {
            case CLEARRAIN:
                dataReturnedToSlave.returnText[0] = 'C';  // .
                dataReturnedToSlave.returnText[1] = 'L';  // .
                dataReturnedToSlave.returnText[2] = 'R';  // .
                #if defined (DEBUG21)
                    Serial.println(F("This is the sendClearRainToSlave Sub. Sending the CLEARRAIN Command\n"));
                #endif
            
                sendDataToSlave(command, &dataReturnedToSlave, sizeof(dataReturnedToSlave));
                return true;
            
            case RESETRAIN:
                dataReturnedToSlave.returnText[0] = 'R';  // .
                dataReturnedToSlave.returnText[1] = 'S';  // .
                dataReturnedToSlave.returnText[2] = 'T';  // .
                #if defined (DEBUG21)
                    Serial.println(F("This is the sendResetRainToSlave Sub. Sending the RESETRAIN Command\n"));
                #endif
            
                sendDataToSlave(command, &dataReturnedToSlave, sizeof(dataReturnedToSlave));
                return true;
            
            case SETRAINTOTAL:
                #if defined (DEBUG21)
                    Serial.println(F("This is the Set Rain Total To Slave Sub. Sending the SETRAINTOTAL Command\n"));
                #endif
                
                #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                    tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                    tft.enableISR(true);  // Start up interrupts again.
                #endif
                
                if (getNumberFromNumericKeypad() == true)
                    {
                        #if defined(DEBUG21)
                            Serial.print(F("This is the number from the numeric keypad to be entered into the Slave EEPROM: "));
                            Serial.println(numberEnteredFromKeypad, 3);
                            Serial.println(F(""));
                        #endif
                    
                        sendDataToSlave(command, &numberEnteredFromKeypad, sizeof(numberEnteredFromKeypad));
                        return true;
                    }
                else
                    {
                        #if defined(DEBUG21)
                            Serial.print(F("The numeric keypad function produced an ERROR!\n"));
                            Serial.println(numberEnteredFromKeypad, 3);
                            Serial.println(F(""));
                        #endif
                    
                        return false;
                    }
                
            case SETHUMIDITYTHRESHOLD:
                #if defined (DEBUG21)
                    Serial.println(F("This is the Set Humidity Threshold to Slave Sub. Sending the SETHUMIDITYTHRESHOLD Command\n"));
                #endif
                
                #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                    tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                    tft.enableISR(true);  // Start up interrupts again.
                #endif
                
                if (getNumberFromNumericKeypad() == true)
                    {
                        #if defined(DEBUG21)
                            Serial.print(F("This is the number from the numeric keypad to be entered into the Slave EEPROM: "));
                            Serial.println(numberEnteredFromKeypad, 3);
                            Serial.println(F(""));
                        #endif
                    
                        sendDataToSlave(command, &numberEnteredFromKeypad, sizeof(numberEnteredFromKeypad));
                        return true;
                    }
                else
                    {
                        #if defined(DEBUG21)
                            Serial.print(F("The numeric keypad function produced an ERROR!\n"));
                            Serial.println(numberEnteredFromKeypad, 3);
                            Serial.println(F(""));
                        #endif
                    
                        return false;
                    }
                
                default:
                    return false;
            }
}




/*******************************************************************************************************************
*  This funtion gives lets you choose your time zone from several choices.
*
* Needs:
*   Nothing.
*
* Returns:
*   boolean:
*       pass(true).
*       fail(false).
*******************************************************************************************************************/
bool setTimeZone(void)
{
    TouchScreenPoints_t postionsJustTouched;
    postionsJustTouched.xPosition = 0;
    postionsJustTouched.yPosition = 0;
    tmElements_t tmElementsStruct;  // Temp struct for calculating time.
    time_t tempTimeTStorage;  // Temp storage for Time_t.
    
    #if defined(DEBUG30)
        Serial.println(F("This is the setTimeZone Sub. It draws 8 boxes on the screen with text in them."));
    #endif
    
    tft.clearScreen();  // Clear the tft screen.
    
    HawaiiTimeZone.draw();
    PacificTimeZone.draw();
    MountainTimeZone.draw();
    CentralTimeZone.draw();
    EasternTimeZone.draw();
    LondonTimeZone.draw();
    CentralEuTimeZone.draw();
    EasternEuTimeZone.draw();
    
    #if defined(DEBUG30)
        Serial.println(F("This is the setTimeZone Sub. Their should be 8 boxes on the screen with the 8 Time zones in them."));
    #endif
    
    if (waitForAnotherTouch() == true)  //  Wait for Another touch returns true if touch was found otherwise false.
        {
            postionsJustTouched.xPosition = xPositionOfTouchedSpot;
            postionsJustTouched.yPosition = yPositionOfTouchedSpot;
            
            // This is the first box from the left on top row.
            if ( HawaiiTimeZone.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG30)
                        Serial.println(F("\n This is the setTimeZone Sub. - We have a hit inside the Hawaii Time Zone box, Setting the Time Zone to Hawaii Time!"));
                    #endif
                    
                    tzIndex = 11;  // Indexes the timezones[] array, 11 is HST.
                    tz = timezones[tzIndex];
                    eepromMemory.write(TimeZoneSettingLocation, tzIndex);

                    #if defined(USINGGPS)
                        if ( updateTimeFromGPS() )  // Returns false if successful, otherwise true.
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: tried to update time from GPS Failed."));
                                #endif
                                return false;
                            }
                        else
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: Successfully updated time from GPS."));
                                #endif
                                return true;
                            }
                    #endif
                    
                    return true;
                }
        
            // This is the second box from the left on top row.
            else if ( PacificTimeZone.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG30)
                        Serial.println(F("\n This is the setTimeZone Sub. We have a hit inside the Pacifix Time Zone box, Setting the Time zone to Pacific Time."));
                    #endif
                    
                    tzIndex = 10;  // Indexes the timezones[] array, 10 is PST.
                    tz = timezones[tzIndex];
                    eepromMemory.write(TimeZoneSettingLocation, tzIndex);
                    
                    #if defined(USINGGPS)
                        if ( updateTimeFromGPS() )  // Returns false if successful, otherwise true.
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: tried to update time from GPS Failed."));
                                #endif
                                return false;
                            }
                        else
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: Successfully updated time from GPS."));
                                #endif
                                return true;
                            }
                    #endif
                    
                    return true;
                }
        
            // This is the third box from the left on top row.
            else if ( MountainTimeZone.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG30)
                        Serial.println(F("\n This is the setTimeZone Sub. We have a hit inside the Mountain Time Zone box, Setting the Time zone to Mountain Time."));
                    #endif
                    
                    tzIndex = 8;  // Indexes the timezones[] array, 8 is MST.
                    tz = timezones[tzIndex];
                    eepromMemory.write(TimeZoneSettingLocation, tzIndex);
                    
                    #if defined(USINGGPS)
                    if ( updateTimeFromGPS() )  // Returns false if successful, otherwise true.
                        {
                            #if defined(DEBUG30)
                                Serial.println(F("This is the setTimeZone Sub: tried to update time from GPS Failed."));
                            #endif
                            return false;
                        }
                    else
                        {
                            #if defined(DEBUG30)
                                Serial.println(F("This is the setTimeZone Sub: Successfully updated time from GPS."));
                            #endif
                            return true;
                        }
                    #endif
                    
                    return true;
                }
        
            // This is the fourth box from the left on top row.
            else if ( CentralTimeZone.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG30)
                        Serial.println(F("\n This is the setTimeZone Sub. We have a hit inside the Central time Zone box, Setting the Time Zone to Central Time."));
                    #endif
                    
                    tzIndex = 7;  // Indexes the timezones[] array, 7 is CST.
                    tz = timezones[tzIndex];
                    eepromMemory.write(TimeZoneSettingLocation, tzIndex);
                    
                    #if defined(USINGGPS)
                        if ( updateTimeFromGPS() )  // Returns false if successful, otherwise true.
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: tried to update time from GPS Failed."));
                                #endif
                                return false;
                            }
                        else
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: Successfully updated time from GPS."));
                                #endif
                                return true;
                            }
                    #endif
                    
                    return true;
                }
            
            // This is the first box from the left on bottom row.
            else if ( EasternTimeZone.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG30)
                        Serial.println(F("\n This is the setTimeZone Sub. We have a hit inside the Eastern time Zone box, Setting the Time Zone to Eastern Time."));
                    #endif
                    
                    tzIndex = 6;  // Indexes the timezones[] array, 6 is EST.
                    tz = timezones[tzIndex];
                    eepromMemory.write(TimeZoneSettingLocation, tzIndex);
                    
                    #if defined(USINGGPS)
                        if ( updateTimeFromGPS() )  // Returns false if successful, otherwise true.
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: tried to update time from GPS Failed."));
                                #endif
                                return false;
                            }
                        else
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: Successfully updated time from GPS."));
                                #endif
                                return true;
                            }
                    #endif
                    
                    return true;
                }
            
            // This is the second box from the left on bottom row.
            else if ( LondonTimeZone.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG30)
                        Serial.println(F("\n This is the setTimeZone Sub. We have a hit inside the London time Zone box, Setting the Time Zone to London Time."));
                    #endif
                    
                    tzIndex = 4;  // Indexes the timezones[] array, 4 is LST.
                    tz = timezones[tzIndex];
                    eepromMemory.write(TimeZoneSettingLocation, tzIndex);
                    
                    #if defined(USINGGPS)
                        if ( updateTimeFromGPS() )  // Returns false if successful, otherwise true.
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: tried to update time from GPS Failed."));
                                #endif
                                return false;
                            }
                        else
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: Successfully updated time from GPS."));
                                #endif
                                return true;
                            }
                    #endif
                    
                    return true;
                }
            
            // This is the third box from the left on bottom row.
            else if ( CentralEuTimeZone.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG30)
                        Serial.println(F("\n This is the setTimeZone Sub. We have a hit inside the Central Eu Time Zone box, Setting the Time Zone to Central Eu Time Zone."));
                    #endif
                    
                    tzIndex = 3;  // Indexes the timezones[] array, 3 is CE.
                    tz = timezones[tzIndex];
                    eepromMemory.write(TimeZoneSettingLocation, tzIndex);
                    
                    #if defined(USINGGPS)
                        if ( updateTimeFromGPS() )  // Returns false if successful, otherwise true.
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: tried to update time from GPS Failed."));
                                #endif
                                return false;
                            }
                        else
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: Successfully updated time from GPS."));
                                #endif
                                return true;
                            }
                    #endif
                    
                    return true;
                }
            
            // This is the fourth box from the left on bottom row.
            else if ( EasternEuTimeZone.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG30)
                        Serial.println(F("\n This is the setTimeZone Sub. We have a hit inside the Eastern Eu Time Zone box, Setting the Time Zone to Eastern Eu Time Zone."));
                    #endif
                    
                    tzIndex = 2;  // Indexes the timezones[] array, 2 is EE.
                    tz = timezones[tzIndex];
                    eepromMemory.write(TimeZoneSettingLocation, tzIndex);
                    
                    #if defined(USINGGPS)
                        if ( updateTimeFromGPS() )  // Returns false if successful, otherwise true.
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: tried to update time from GPS Failed."));
                                #endif
                                return false;
                            }
                        else
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("This is the setTimeZone Sub: Successfully updated time from GPS."));
                                #endif
                                return true;
                            }
                    #endif
                    
                    return true;
                }

        }
    
    else
        {
            #if defined(DEBUG30)
                Serial.println(F("This is the setTimeZone Sub: Waited 8 seconds and found no touch."));
            #endif
        
            tft.clearScreen();  // Clear the tft screen.
        
            return false;
        }
    return false;
    
}


/*******************************************************************************************************************
*  This funtion sets the day of the week in the RTC (DS3231).
*
* Needs:
*   dayOfWeek:
*       uint_8t number from 1 to 7 for the days of the week starting with Sunday as day 1.
*
* Returns:
*   boolean:
*       pass(true).
*       fail(false).
*******************************************************************************************************************/
bool setRTCDayOfWeek(uint8_t dayOfWeek)
{
    tmElements_t tmElementsStruct;
    
    if (!RTC.read(tmElementsStruct))  // Read the DS1307 RTC to get the Day of the Week
        {
            if (RTC.chipPresent())
                {
                    #if defined(DEBUG44)
                        Serial.println(F("The DS3231 is stopped.  Please run the SetTime"));
                        Serial.println(F("example to initialize the time and begin running."));
                        Serial.println();
                    #endif
                    return false;
                }
            else
                {
                    #if defined(DEBUG44)
                        Serial.println(F("DS3231 read error!  Please check the circuitry."));
                        Serial.println();
                    #endif
                    return false;
                }
        }
    // Check to see if the day of the week passed in is within range.
    if (dayOfWeek < 1 || dayOfWeek > 7)
        {
            return false;
        }
    
    tmElementsStruct.Wday = dayOfWeek;  // Set the Day of the Week.
    
    #if defined(DEBUG44)
        Serial.print(F("This is the Day of the Week: "));
        Serial.println(tmElementsStruct.Wday, DEC);
    #endif
    
    // and configure the DS1307 RTC with this info
    if (!RTC.write(tmElementsStruct))
        {
            if (RTC.chipPresent())
                {
                    #if defined(DEBUG44)
                        Serial.println(F("The DS3231 is stopped.  Please run the SetTime"));
                        Serial.println(F("example to initialize the time and begin running."));
                        Serial.println();
                    #endif
                    return false;
                }
            else
                {
                    #if defined(DEBUG44)
                        Serial.println(F("DS3231 read error!  Please check the circuitry."));
                        Serial.println();
                    #endif
                    return false;
                }
        }
    else
        {
            currentDayOfWeek = (tmElementsStruct.Wday);
            return true;  // Passed tests day of week is set!
        }
}


/*******************************************************************************************************************
*  This funtion lets you choose which day of the week today is.
*
* Needs:
*   Nothing.
*
* Returns:
*   pass(true).
*   fail(false).
*******************************************************************************************************************/
bool setDayOfWeek(void)
{
    TouchScreenPoints_t postionsJustTouched;
    postionsJustTouched.xPosition = 0;
    postionsJustTouched.yPosition = 0;

    
    xPositionOfTouchedSpot = 0;
    xPositionOfTouchedSpot = 0;
    
    #if defined(DEBUG44)
        Serial.println(F("This is the setDayOfWeek Sub. It draws 7 boxes on the screen with text in them."));
    #endif
    
    tft.clearScreen();  // Clear the tft screen.
    
    Sunday.draw();
    Monday.draw();
    Tuesday.draw();
    Wednesday.draw();
    Thursday.draw();
    Friday.draw();
    Saturday.draw();

    #if defined(DEBUG44)
        Serial.println(F("This is the setDayOfWeek Sub. Their should be 7 boxes on the screen with the days of the week in them."));
    #endif
    
    if (waitForAnotherTouch() == true)  //  Wait for Another touch returns true if touch was found otherwise false.
        {
            postionsJustTouched.xPosition = xPositionOfTouchedSpot;
            postionsJustTouched.yPosition = yPositionOfTouchedSpot;
            // This is the first box on top from the left.
            if ( Sunday.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG44)
                        Serial.println(F("\n This is the setDayOfWeek Sub. setDayOfWeek Section - We have a hit inside the Sunday box, Setting the day of the week to Sunday!"));
                    #endif
            
                    if( !setRTCDayOfWeek((uint8_t) 1) )
                        {
                            #if defined(DEBUG44)
                                Serial.println(F("Setting the DS1307 day of the week to Sunday was Unsuccessful!"));
                            #endif
                            return false;  // Setting day of week failed.
                        }
                    else
                        {
                            #if defined(DEBUG44)
                                Serial.println(F("Setting the DS1307 day of the week to Sunday was Successful!"));
                            #endif
                            return true;  // We are done setting the Day of the Week
                        }
                }
        
            // This is the second box on the top from the left.
            else if ( Monday.OnTouch(postionsJustTouched) == ON_RNDRECT )
                    {
                        #if defined(DEBUG44)
                            Serial.println(F("\n This is the setDayOfWeek Sub. We have a hit inside the set Monday box, Setting the Day of the Week to Monday."));
                        #endif
                        if( !setRTCDayOfWeek((uint8_t) 2) )
                            {
                                #if defined(DEBUG44)
                                    Serial.println(F("Setting the DS1307 day of the week to Monday was Unsuccessful!"));
                                #endif
                                return false;  // Setting day of week failed.
                            }
                        else
                            {
                                #if defined(DEBUG44)
                                    Serial.println(F("Setting the DS1307 day of the week to Monday was Successful!"));
                                #endif
                                return true;  // We are done setting the Day of the Week
                            }
                    }
        
            // This is the third box on the top from the left.
            else if ( Tuesday.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG44)
                        Serial.println(F("\n This is the setDayOfWeek Sub. We have a hit inside the set Tuesday box, Setting the Day of the Week to Tuesday."));
                    #endif
                    if( !setRTCDayOfWeek((uint8_t) 3) )
                        {
                            #if defined(DEBUG44)
                                Serial.println(F("Setting the DS1307 day of the week to Monday was Unsuccessful!"));
                            #endif
                            return false;  // Setting day of week failed.
                        }
                    else
                        {
                            #if defined(DEBUG44)
                                Serial.println(F("Setting the DS1307 day of the week to Monday was Successful!"));
                            #endif
                            return true;  // We are done setting the Day of the Week
                        }
                }

            // This is the forth box on the top from the left.
            else if ( Wednesday.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG44)
                        Serial.println(F("\n This is the setDayOfWeek Sub. We have a hit inside the set Tuesday box, Setting the Day of the Week to Tuesday."));
                    #endif
                    if( !setRTCDayOfWeek((uint8_t) 4) )
                        {
                            #if defined(DEBUG44)
                                Serial.println(F("Setting the DS1307 day of the week to Monday was Unsuccessful!"));
                            #endif
                            return false;  // Setting day of week failed.
                        }
                    else
                        {
                            #if defined(DEBUG44)
                                Serial.println(F("Setting the DS1307 day of the week to Monday was Successful!"));
                            #endif
                            return true;  // We are done setting the Day of the Week
                        }
                }

        
            // This is the first box on bottom from the left.
            else if ( Thursday.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG44)
                        Serial.println(F("\n This is the setDayOfWeek Sub. We have a hit inside the set Thursday box, Setting the Day of the Week to Thursday."));
                    #endif
                    if( !setRTCDayOfWeek((uint8_t) 5) )
                        {
                            #if defined(DEBUG44)
                                Serial.println(F("Setting the DS1307 day of the week to Monday was Unsuccessful!"));
                            #endif
                            return false;  // Setting day of week failed.
                        }
                    else
                        {
                            #if defined(DEBUG44)
                                Serial.println(F("Setting the DS1307 day of the week to Monday was Successful!"));
                            #endif
                            return true;  // We are done setting the Day of the Week
                        }
                }
        
            // This is the second box on the bottom from the left.
            else if ( Friday.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG44)
                        Serial.println(F("\n This is the setDayOfWeek Sub. We have a hit inside the set Friday box, Setting the Day of the Week to Friday."));
                    #endif
                    if( !setRTCDayOfWeek((uint8_t) 6) )
                        {
                            #if defined(DEBUG44)
                                Serial.println(F("Setting the DS1307 day of the week to Monday was Unsuccessful!"));
                            #endif
                            return false;  // Setting day of week failed.
                        }
                    else
                        {
                            #if defined(DEBUG44)
                                Serial.println(F("Setting the DS1307 day of the week to Monday was Successful!"));
                            #endif
                            return true;  // We are done setting the Day of the Week
                        }
                }
        
            // This is the third box on the bottom from the left.
            else if ( Saturday.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG44)
                        Serial.println(F("\n This is the setDayOfWeek Sub. We have a hit inside the set Saturday box, Setting the Day of the Week to Saturday."));
                    #endif
                    if( !setRTCDayOfWeek((uint8_t) 7) )
                        {
                            #if defined(DEBUG44)
                                Serial.println(F("Setting the DS1307 day of the week to Monday was Unsuccessful!"));
                            #endif
                            return false;  // Setting day of week failed.
                        }
                    else
                        {
                            #if defined(DEBUG44)
                                Serial.println(F("Setting the DS1307 day of the week to Monday was Successful!"));
                            #endif
                            return true;  // We are done setting the Day of the Week
                        }
                }
        }
    else
        {
            #if defined(DEBUG44)
                Serial.println(F("This is the setDayOfWeek Sub: Waited 8 seconds and found no touch."));
            #endif
        
            tft.clearScreen();  // Clear the tft screen.
        
            return false;
        }
    return false;
    
}


/*******************************************************************************************************************
*  This preference panel allows you to select the different rain settings.
*
* Needs:
*   Nothing
* Returns:
*   pass(true).
*   fail(false).
*******************************************************************************************************************/
bool setRainPreferences(void)
{
    TouchScreenPoints_t postionsJustTouched;
    postionsJustTouched.xPosition = 0;
    postionsJustTouched.yPosition = 0;
    
    xPositionOfTouchedSpot = 0;
    xPositionOfTouchedSpot = 0;
    
    #if defined(DEBUG45)
        Serial.println(F("\nThis is the setRainPreferences Sub. It draws 6 boxes on the screen with text in them."));
    #endif
    
    tft.clearScreen();  // Clear the tft screen.
    
    SetMasterRainOffset.draw();
    ClearMasterRainOffset.draw();
    SetHumidityThreshold.draw();
    SetSlaveYearlyValue.draw();
    ClearSlaveRain.draw();
    ResetSlaveRain.draw();
    
    #if defined(DEBUG45)
        Serial.println(F("This is the setRainPreferences Sub. Their should be 6 boxes on the screen with text in them."));
    #endif
    
    if (waitForAnotherTouch() == true)  //  Wait for Another touch returns true if touch was found otherwise false.
        {
            postionsJustTouched.xPosition = xPositionOfTouchedSpot;
            postionsJustTouched.yPosition = yPositionOfTouchedSpot;
            // This is the first box on top from the left.
            if ( SetMasterRainOffset.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG45)
                        Serial.println(F("This is the setRainPreferences Sub.  - We have a hit inside the set Set Master Rain Offset box, Setting the Master Rain Offset to your choice!\n"));
                    #endif
                    
                    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                        tft.enableISR(true);  // Start up interrupts again.
                    #endif
                    
                    setMasterYearlyRainOffset();
                    return true;
                }
        
            // This is the second box on the top from the left.
            else if ( ClearMasterRainOffset.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG45)
                        Serial.println(F("This is the setRainPreferences Sub. We have a hit inside the set Clear Master Rain box, Clearing the Master Rain.\n"));
                    #endif
            
                    clearMasterRainOffset();  // Clear the Master Yearly Total Rain offset in the EEPROM.
            
                    return true;
                }
        
            // This is the third box on the top from the left.
            else if ( SetHumidityThreshold.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG45)
                        Serial.println(F("This is the setRainPreferences Sub. We have a hit inside the SetHumidityThreshold box.\n"));
                    #endif
                    sendRainCommandsToSlave(SETHUMIDITYTHRESHOLD);
                    return true;
                }
        
            // This is the first box on the bottom from the left.
            else if ( SetSlaveYearlyValue.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG45)
                        Serial.println(F("This is the setRainPreferences Sub.  Setting the Slave Yearly Rain Value.\n"));
                    #endif
                    sendRainCommandsToSlave(SETRAINTOTAL);
                    return true;
                }
            // This is the second box on the bottom from the left.
            else if ( ClearSlaveRain.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG45)
                        Serial.println(F("This is the setRainPreferences Sub.  Clearing the Slave Rain values.\n"));
                    #endif
                    sendRainCommandsToSlave(CLEARRAIN);
                    return true;
                }
            
            // This is the third box on the bottom from the left.
            else if ( ResetSlaveRain.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG45)
                        Serial.println(F("This is the setRainPreferences Sub.  Reseting the Slave Rain Values.\n"));
                    #endif
                    sendRainCommandsToSlave(RESETRAIN);
                    return true;
                }
        }
    else
        {
            #if defined(DEBUG45)
                Serial.println(F("This is the setRainPreferences Sub: Waited 8 seconds and found no touch.\n"));
            #endif
        
            tft.clearScreen();  // Clear the tft screen.
            return false;
        }
    return false;
    
}

/*******************************************************************************************************************
*                        This preference panel Sets Various Time Preferences to your liking.
*   It puts up 10 button boxes on the screen with the following titles:
*       Set Your Time zone:        Set the Time Zone for where you live (ET, CT, MT, PT).
*       Set The Day of the Week:   Set the current day of the week (Sun, Mon, Tues, Wed, Thurs, Fri, Sat).
*       Set Time from GPS:         Sets the DS3231 Real Time Clock to the Time from the GPS Module (GMT).
*       Identify Tmps Devs:        Identify's the DS18B20 Temp devices on the 1 Wire Bus.
*       Plot Temp Data on TFT:     This puts up the DS18B20 Temp Data on the TFT in Horizontal charts.
*       Not Used:                  TBD
*       Not Used:                  TBD
*       Not Used:                  TBD
* ------------------------------------- Set Time Preferences -----------------------------------------
*
* Needs:
*   Nothing
* Returns:
*   pass(true).
*   fail(false).
*******************************************************************************************************************/
bool setTimePreferences(void)
{
    TouchScreenPoints_t postionsJustTouched;
    postionsJustTouched.xPosition = 0;
    postionsJustTouched.yPosition = 0;

    
    xPositionOfTouchedSpot = 0;
    xPositionOfTouchedSpot = 0;
    
    #if defined(DEBUG30)
        Serial.println(F("This is the setTimePreferences Sub. It draws 4 boxes on the screen with text in them."));
    #endif
    
    tft.clearScreen();  // Clear the tft screen.
    
    SetTimeZone.draw();
    SetWeekday.draw();
    SetTimeFromGPS.draw();
    SetSlaveTime.draw();
    
    #if defined(DEBUG30)
        Serial.println(F("This is the setTimePreferences Sub. Their should be 4 boxes on the screen with text in them."));
    #endif
    
    if (waitForAnotherTouch() == true)  //  Wait for Another touch returns true if touch was found otherwise false.
        {
            postionsJustTouched.xPosition = xPositionOfTouchedSpot;
            postionsJustTouched.yPosition = yPositionOfTouchedSpot;
            // This is the first box on top from the left.
            if ( SetTimeZone.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG30)
                        Serial.println(F("\nThis is the setTimePreferences Sub.  - We have a hit inside the set Time Zone box, Setting the timezone to your choice!"));
                    #endif
                    
                    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                        tft.enableISR(true);  // Start up interrupts again.
                    #endif
                    
                    setTimeZone();  // This function sets the local time zone accordian to your choice.
            
                    return true;
                }
        
            // This is the second box on the top from the left.
            else if ( SetWeekday.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG30)
                        Serial.println(F("\nThis is the setTimePreferences Sub. We have a hit inside the set setDayOfWeek box, Setting the Day of the Week."));
                    #endif
                    
                    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                        tft.enableISR(true);  // Start up interrupts again.
                    #endif
                    
                    setDayOfWeek();  // Set today's day of the week (Sun -> Sat).
            
                    return true;
                }
        
            // This is the third box on the top from the left.
            else if ( SetTimeFromGPS.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG30)
                        Serial.println(F("\nThis is the setTimePreferences Sub. We have a hit inside the set setTimeFromGPS box,  Setting the RTC from the GPS Module."));
                    #endif
                    #if defined(USINGGPS)
                        Serial1.clear();  // Clear the Serial1 buffer of all data
                        gps.Poll_NAV_PVT();  // Polls UBX-NAV-PVT, this requests the data from the GPS Module.
                        delay(600);
                    
                        if (updateTimeFromGPS())  // This gets the GMT Time from the GPS attached to Serial port 1, returns false if good or true if bad.
                            {
                                #if defined(DEBUG30)
                                    Serial.println(F("Error: Did not update DS3231 Clock with GPS time!"));
                                    Serial.print(F("statusDotColor is: "));
                                    Serial.println(statusDotColor);
                                    Serial.print(F("UTCTimeCorrected flag: "));
                                    Serial.println(UTCTimeCorrected);
                                #endif
                            }
                    #endif
            
                    return true;
                }
        
            // This is the third box on the top from the left.
            else if ( SetSlaveTime.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG30)
                        Serial.println(F("\nThis is the setTimePreferences Sub."));
                    #endif
            
                    if (!setSlaveRTC() )  // This function sends the time from its RTC to the Slave RTC.
                        {
                            #if defined(DEBUG30)
                                Serial.println(F("\nThis is the setTimePreferences Sub."));
                            #endif
                            return false;
                        }
            
                    return true;
                }
        }
    else
        {
            #if defined(DEBUG30)
                Serial.println(F("This is the setTimePreferences Sub: Waited 8 seconds and found no touch."));
            #endif
        
            tft.clearScreen();  // Clear the tft screen.
            return false;
        }
    return false;
}


/*******************************************************************************************************************
 *                   This function Sets the Temperature Offset Calibration setting to your liking.
 *   It puts up 4 button boxes on the screen with the following titles:
 *       Set Master Temp Calibration:       Sets the Temperature Offset Calibration of the System Sensor.
 *       Set Slave Temp Calibration:        Sets the Slave Temperature Offset Calibration of the System Sensor.
 *       Unused:                            Not Used (Future expansion).
 *       Unused:                            Not Used (Future expansion).
 * ------------------------------- Sets the Temperature Offset Calibration -----------------------------------------
 * bool setTemperatureCalibrationOffset(void);
 *
 * Needs:
 *   Nothing
 * Returns:
 *   pass(true).
 *   fail(false).
 *******************************************************************************************************************/
bool setTemperatureCalibrationOffset(void)
{
    TouchScreenPoints_t postionsJustTouched;
    postionsJustTouched.xPosition = 0;
    postionsJustTouched.yPosition = 0;
    
    xPositionOfTouchedSpot = 0;
    xPositionOfTouchedSpot = 0;
    
    #if defined(DEBUG34)
        Serial.println(F("\nThis is the setTemperatureCalibrationOffset Function. It draws 4 boxes on the screen with text in them."));
    #endif
    
    tft.clearScreen();  // Clear the tft screen.
    
    SetMasterTempCalibration.draw();
    SetSlaveTempCalibration.draw();
    NotUsed7.draw();
    NotUsed8.draw();
    
    #if defined(DEBUG34)
        Serial.println(F("This is the setTemperatureCalibrationOffset function. Their should be 4 boxes on the screen with text in them."));
    #endif
    
    if (waitForAnotherTouch() == true)  //  Wait for Another touch returns true if touch was found otherwise false.
        {
            postionsJustTouched.xPosition = xPositionOfTouchedSpot;
            postionsJustTouched.yPosition = yPositionOfTouchedSpot;
            // This is the first box on top from the left.
            if ( SetMasterTempCalibration.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG34)
                        Serial.println(F("setTemperatureCalibrationOffset Section - We have a hit inside the Set Master Temp Calibration box, setting the Master Temp Offset!"));
                    #endif
                    
                    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                        tft.enableISR(true);  // Start up interrupts again.
                    #endif
                    
                    if (getNumberFromNumericKeypad() == true)
                        {
                            #if defined(DEBUG34)
                                Serial.print(F("setTemperatureCalibrationOffset function. This is the number from the numeric keypad to be entered into the EEPROM: "));
                                Serial.println(numberEnteredFromKeypad, 3);
                                Serial.print(F("setTemperatureCalibrationOffset function. This is the Temp Cal Offset Memory address location to write to in the EEPROM: "));
                                Serial.println(TempCalOffsetLocation, HEX);
                                Serial.println(F(""));
                            #endif
                        
                            eepromMemory.writeFloat(TempCalOffsetLocation, numberEnteredFromKeypad);  // First 4 bytes is the rain per year in float.
                            SystemSensor.settings.tempCorrection = numberEnteredFromKeypad;
                            
                            #if defined(DEBUG34)  // This the the TFT debug flag.
                                Serial.print(F("setTemperatureCalibrationOffset function. This is the Temp Cal Offset Location to read from in the EEPROM: "));
                                Serial.println(TempCalOffsetLocation, HEX);
                                //TempCalOffsetLocation = eepromMemory.readFloat(TempCalOffsetLocation);  // First 4 bytes is the rain per year in float.
                                Serial.print(F("setTemperatureCalibrationOffset function. Read EEPROM Memory Temp Cal Offset Location, Master Temp Calibration Offset: "));
                                Serial.println(eepromMemory.readFloat(TempCalOffsetLocation), 3);
                            #endif

                            return true;
                        }
                    else
                        {
                            #if defined(DEBUG34)
                                Serial.print(F("setTemperatureCalibrationOffset function. The numeric keypad function produced an ERROR!\n"));
                                Serial.println(numberEnteredFromKeypad, 3);
                                Serial.println(F(""));
                            #endif
                        
                            return false;
                        }
                }
        
            // This is the second box on the top from the left.
            else if ( SetSlaveTempCalibration.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG34)
                        Serial.println(F("TempCalOffsetLocation Section - We have a hit inside the Set Slave Temp Calibration box, setting the Slave Temp Offset!"));
                    #endif
                    
                    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                        tft.enableISR(true);  // Start up interrupts again.
                    #endif
                    
                    if (getNumberFromNumericKeypad() == true)
                        {
                            #if defined(DEBUG34)
                                Serial.print(F("setTemperatureCalibrationOffset function. This is the number from the numeric keypad to be sent to the Slave: "));
                                Serial.println(numberEnteredFromKeypad, 3);
                                Serial.println(F(""));
                            #endif
                        
                            sendDataToSlave(SETTEMPOFFSET, &numberEnteredFromKeypad, sizeof(numberEnteredFromKeypad));
                        
                            return true;
                        }
                    else
                        {
                            #if defined(DEBUG34)
                                Serial.print(F("setTemperatureCalibrationOffset function. The numeric keypad function produced an ERROR!\n"));
                                Serial.println(numberEnteredFromKeypad, 3);
                                Serial.println(F(""));
                            #endif
                        
                            return false;
                        }

                }
        
            // This is the third box on the top from the left.
            else if ( NotUsed7.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG34)
                        Serial.println(F("setTemperatureCalibrationOffset Section - We have a hit inside the not used7 box!"));
                    #endif
                    return true;
                }
        
            // This is the third box on the top from the left.
            else if ( NotUsed8.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG34)
                        Serial.println(F("setTemperatureCalibrationOffset Section - We have a hit inside the not used8 box!"));
                    #endif
                    return true;
                }
        }
    else
        {
            #if defined(DEBUG34)
                Serial.println(F("setTemperatureCalibrationOffset function. Waited 8 seconds and found no touch.\n"));
            #endif
        
            tft.clearScreen();  // Clear the tft screen.
            return false;
        }
    return false;
}


/*******************************************************************************************************************
*                               This function adjusts the preferences to your liking.
*   It puts up 6 button boxes on the screen with the following titles:
*   It puts up 6 button boxes on the screen with the following titles:
*       SetTimePreferences:         Set Time Preferences.
*       SetRainPreferences:         Set Rain Preferences.
*       SetTempSensorsPreferences:  Set Temp Sensors Preferences.
*       SetCoolerPreferences:       Set Cooler Preferences.
*       Not Used:                   TBD
*       Not Used:                   TBD
* ------------------------------------------ SET PREFERENCES-------------------------------------------------------
* Needs:
*   Nothing
* Returns:
*   pass(true).
*   fail(false).
*******************************************************************************************************************/
bool setPreferences(void)
{
    TouchScreenPoints_t postionsJustTouched;
    postionsJustTouched.xPosition = 0;
    postionsJustTouched.yPosition = 0;

    
    xPositionOfTouchedSpot = 0;
    xPositionOfTouchedSpot = 0;
    
    #if defined(DEBUG19)
        Serial.println(F("\nThis is the setPreferences Sub. It draws 6 boxes on the screen with text in them."));
    #endif
    
    tft.clearScreen();  // Clear the tft screen.
    
    /// Put the touch buttons on the tft screen.
    SetTimePreferences.draw();
    SetRainPreferences.draw();
    SetTempSensorsPreferences.draw();
    NotUsed1.draw();
    ResetMaster.draw();
    ResetSlave.draw();

    #if defined(DEBUG19)
        Serial.println(F("This is the setPreferences Sub. Their should be 6 boxes on the screen with text in them."));
    #endif
    
    if (waitForAnotherTouch() == true)  //  Wait for Another touch returns true if touch was found otherwise false.
        {
            postionsJustTouched.xPosition = xPositionOfTouchedSpot;
            postionsJustTouched.yPosition = yPositionOfTouchedSpot;
            // This is the first box on top from the left.
            if ( SetTimePreferences.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG19)
                        Serial.println(F("setPreferences Section - We have a hit inside the SetTimePreferences box, Setting Time Preferences!\n"));
                    #endif
                    
                    setTimePreferences();
                    
                    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                        tft.enableISR(true);  // Start up interrupts again.
                    #endif
                    
                    tft.clearScreen();  // Clear the tft screen.
                    return true;
                }
        
            // This is the second box on the top from the left.
            else if ( SetRainPreferences.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG19)
                        Serial.println(F("setPreferences Section - We have a hit inside the SetRainPreferences box, Setting Rain Preferences!\n"));
                    #endif
                    
                    setRainPreferences();  // Checks for the sensors on the one wire bus.
                    
                    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                        tft.enableISR(true);  // Start up interrupts again.
                    #endif
                    
                    tft.clearScreen();  // Clear the tft screen.
                    return true;
                }
        
            // This is the third box on the top from the left.
            else if ( SetTempSensorsPreferences.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG19)
                        Serial.println(F("setPreferences Section - We have a hit inside the SetTempSensorsPreferences box, Setting Temp Sensors Preferences!\n"));
                    #endif
                    #if defined(USINGTEMPSENSORS)
                        tftFindActiveOneWirePins();  // Finds the Active One Wire Bus Pins on the Teensy.
                    #endif
                    
                    setTemperatureCalibrationOffset();
                    
                    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                        tft.enableISR(true);  // Start up interrupts again.
                    #endif
                    
                    tft.clearScreen();  // Clear the tft screen.
                    return true;
                }
        
            // This is the third box on the top from the left.
            else if ( NotUsed1.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG19)
                        Serial.println(F("setPreferences Section - We have a hit inside the NotUsed1 button!\n"));
                    #endif
                    
                    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                        tft.enableISR(true);  // Start up interrupts again.
                    #endif
                    
                    tft.clearScreen();  // Clear the tft screen.
                    return true;
                }
        
            // This is the first box on bottom from the left.
            else if ( ResetMaster.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG19)
                        Serial.println(F("setPreferences Section - We have a hit inside the Reset Master box!\n"));
                    #endif
                    CPU_RESTART;  // This will restart the CPU and start the whole program from the beginning.
                    
                    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                        tft.enableISR(true);  // Start up interrupts again.
                    #endif
                    
                    tft.clearScreen();  // Clear the tft screen.
                    return true;
                }
        
            // This is the second box on the bottom from the left.
            else if ( ResetSlave.OnTouch(postionsJustTouched) == ON_RNDRECT )
                {
                    #if defined(DEBUG19)
                        Serial.println(F("setPreferences Flag - We have a hit inside the Reset Slave box!\n"));
                    #endif
                    dataReturnedToSlave.returnText[0] = 'R';  // .
                    dataReturnedToSlave.returnText[1] = 'S';  // .
                    dataReturnedToSlave.returnText[2] = 'T';  // .
                    
                    sendDataToSlave(CPURESET, &dataReturnedToSlave, sizeof(dataReturnedToSlave));
                    
                    #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                        tft.enableISR(true);  // Start up interrupts again.
                    #endif
                    
                    tft.clearScreen();  // Clear the tft screen.
                    return true;
                }
        }
    else
        {
            #if defined(DEBUG19)
                Serial.println(F("This is the setPreferences Sub. Waited 8 seconds and found no touch.\n"));
            #endif
        
            tft.clearScreen();  // Clear the tft screen.
        
            #if defined(USE_RA8875_TOUCH)  // We are using the resistive touch screen controller built-in to the RA8875.
                tft.touchEnable(true);  // Set the touch screen interrupt to enable.
                tft.enableISR(true);  // Start up interrupts again.
            #endif
            
            return false;
        }
    return false;
}


/*******************************************************************************************************************
* This function gets the sensor data from the BME280 Temperature, Barometric Pressure, and Relative Humidity Sensor.
* Pressure is returned in the SI units of Pascals. 100 Pascals = 1 hPa = 1 millibar. Often times barometric pressure
* is reported in millibar or inches-mercury. For future reference 1 pascal =0.000295333727 inches of mercury, or
* 1 inch Hg = 3386.39 Pascal. So if you take the pascal value of say 100734 and divide by 3389.39 you'll get 29.72
* inches-Hg.
*******************************************************************************************************************/
void getSensorData(void)
{
    float tempPressureReading, tempReadTemperature;
    //Take a reading.
    //Start with temperature, as that data is needed for accurate compensation.
    //Reading the temperature updates the compensators of the other functions
    //in the background.
    #if defined(DEBUG42)
        Serial.println(F("\nThis is the getSensorData function."));
    #endif
    
    tempReadTemperature = ( SystemSensor.readTempF() );
    
    #if defined(DEBUG42)
        Serial.print(F("This is the getSensorData function.  This is the Temperature read from the BME280 Sensor: "));
        Serial.println(tempReadTemperature,3);
    #endif
    
    sensorState.Temperature = ( tempReadTemperature );
    
    #if defined(DEBUG42)
        Serial.print(F("This is the getSensorData function.  This is the Corrected Temperature from the BME280 Sensor: "));
        Serial.println(sensorState.Temperature,3);
    #endif
    
    tempPressureReading = (SystemSensor.readFloatPressure());
    sensorState.Baro_Pressure = (tempPressureReading / 3389.39);
    
    #if defined(DEBUG42)
        Serial.print(F("This is the getSensorData function.  This is the Converted Baro Pressure from the BME280 Sensor: "));
        Serial.println(sensorState.Baro_Pressure,3);
    #endif
    
    sensorState.Relative_Humidity = (SystemSensor.readFloatHumidity());
    
    #if defined(DEBUG42)
        Serial.print(F("This is the getSensorData function.  This is the Relative Humidity from the BME280 Sensor: "));
        Serial.println(sensorState.Relative_Humidity,3);
    #endif
    
}


/*******************************************************************************************************************
* This function signals the one Second Timer routine to run when true.
* This ISR is called by interrupt every second.
*******************************************************************************************************************/
void oneSecondTimerISR(void)
{
    oneSecondFlag = true;
}


/*******************************************************************************************************************
* This function signals the two Second Timer routine to run when true.
* This ISR is called by interrupt every two seconds.
*
*******************************************************************************************************************/
void twoSecondTimerISR(void)
{
    twoSecondFlag = true;
}


/*******************************************************************************************************************
* This function signals the Print Temperature, Barometric Pressure, and Humidity routine to run when true.
* This ISR is called by interrupt every 10 seconds.
*
*******************************************************************************************************************/
void tenSecondTimerISR(void)
{
    tenSecondFlag = true;
}

/*******************************************************************************************************************
* This function signals the Update GPS Time routine to run when true.
* This ISR is called by interrupt every 30 seconds.
*
 *******************************************************************************************************************/
void thirtySecondTimerISR(void)
{
    thirtySecondFlag = true;
}

/*******************************************************************************************************************
* --------------------------------------Serial2 Events ISR's--------------------------------
*******************************************************************************************************************/
void tx2Event(void)
{
    // TX Event function will fire when it is finished sending a packet
    Event2.clear_tx_event();
    #if defined(DEBUG20)
        Serial.println(F("Event2 tx2Event fired, data transmitted.\n"));
    #endif
}

void rx2Event(void)
{
    // RX Event function moves the buffer data from the input buffer to the local buffer when it is full
    numberOfBytesFromDMAReceive = Event2.available();
    #if defined(DEBUG20)
        Serial.println(F("rx2Event Interrupt."));
        Serial.print(F("Event2.available has this many bytes available: "));
        Serial.println(numberOfBytesFromDMAReceive);
    #endif
    
    if (numberOfBytesFromDMAReceive == maxRxBufferSize)
        {
            #if defined(DEBUG20)
                Serial.println(F("Event2 rx2Event, Found a full buffer..."));
            #endif
        
            for (int i = 0; i < numberOfBytesFromDMAReceive; i++)
                {
                    receiveBuffer[i] = Event2.read();  // This moves the incoming byte to the buffer.
                }
            #if defined(DEBUG20)
                Serial.println(F("Event2 rx2Event, Data has been transferred to receiveBuffer...\n"));
            #endif
        
            incomingDMABufferFullFlag = true;
            Event2.clear_rx_event();  // Clears the Uart interrupt, clears DMA interrupt, clears the DMA complete flag.
            Event2.clear();  // Clears the circular buffer in Event2.
        }
    else
        {
            Event2.clear_rx_event();  // Clears the Uart interrupt, clears DMA interrupt, clears the DMA complete flag.
            Event2.clear();  // Clears the circular buffer in Event2.
            return;
        }
}


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/// System Setup routine.
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/

void setup()
{
    bool status;
    uint8_t tftErrorCode = 0;
    tmElements_t tm;
    
    #if defined(DEBUG)
        long unsigned debug_start = millis ();
    #endif
    
    Serial.begin(115200);
    
    #if defined(DEBUG)
        while (!Serial && ((millis () - debug_start) <= 2000)) ;
        Serial.println(F("Setup Starting the program.."));
    #endif
    
    pinMode(13, OUTPUT);  // Setup pin 13 to turn on the LED.
    digitalWrite(13, LOW);  // This turns off the LED.
    
    #if defined(USINGTEMPSENSORS)
        #if defined(DEBUG31)
            if (findActiveOneWirePins())  // Returns true if Active One Wire Bus was found.
                {
                    Serial.println(F("Found Active One Wire Bus Device(s)..."));
                }
        #endif
    
        #if defined(DEBUG33)
            Serial.println(F("Resetting the One Wire Bus..."));
        #endif
        oneWire.reset();
    
        #if defined(DEBUG33)
            Serial.println(F("Begin oneWire with new Teensy Pin..."));
        #endif
        oneWire.begin(oneWireLocationOnTeensyPin);
    
        #if defined(DEBUG33)
            Serial.println(F("Delay 2 milliseconds..."));
        #endif
        delay(2);
    
        #if defined(DEBUG33)
            Serial.println(F("Begin tempSensors..."));
        #endif
        tempSensors.begin();
    
        #if defined(DEBUG33)
            Serial.println(F("Delay 2 millicseconds..."));
        #endif
    
        delay(2);
    
        #if defined(DEBUG33)
            Serial.println(F("Set Wait For Conversion to FALSE..."));
        #endif
        tempSensors.setWaitForConversion(false);  // This flag makes the convert command return without waiting for a conversion to complete (750mSec).
    
        if (!identifyCurrentTempDevicesOnBus())
            {
                #if defined(DEBUG33)
                    Serial.println(F("No Devices found on the One Wire Bus..."));
                #endif
            }
        else
            {
                // Read ID's per sensor
                // and put them in oneWireTemps array
                for (uint8_t index = 0; index < deviceCount; index++)
                    {
                        // go through sensors
                        tempSensors.getAddress(oneWireTemps[index].addr, index);
                        oneWireTemps[index].id = tempSensors.getUserData(oneWireTemps[index].addr);
                    }
            }
    #endif
    
    #if defined(DEBUG)
        Serial.println(F("Setup finished temp sensors setup.."));
    #endif
    
    #if defined(useRS_485)
        pinMode (serial2RecieveEnablePin, OUTPUT);  // This is the RS-485 Recieve enable pin.
        digitalWrite(serial2RecieveEnablePin,  LOW);  // Start with the RS-485 Recieve enable pin enabled (Low Active for Recieve).
    
        pinMode (serial2TransmitEnablePin, OUTPUT);  // This is the RS-485 Transmit enable pin.
        digitalWrite(serial2TransmitEnablePin,  HIGH);  // Start with the RS-485 Transmit enable pin enabled (High Active for transmit).
    #endif
    
    #if defined(DEBUG)
        Serial.println(F("Setup finished RS_485 setup.."));
    #endif
    
    #if defined(USINGGPS)
        #if defined(DEBUG41)
            Serial.println(F("Using default baud (9600), trying to set baudrate to 230400..."));
            gps.begin(BaudDefault);  // Enable Teensy serial communication @ 9600 baud, default settings.
            gps.SetGPSbaud(230400, true);   // Set GPS Port Baud, Possible Baud Rate Configurations 4800~9600~19200~38400~57600~115200~230400~460800
            gps.end();                      // Disables Teensy serial communication, to re-enable serial communication, call gps.begin(Baud, bool).
    
            Serial.println(F("Using 19,200 baud, trying to set baudrate to 230400..."));
            gps.begin(19200);
            gps.SetGPSbaud(230400, true);
            gps.end();
    
            Serial.println(F("Using 38,400 baud, trying to set baudrate to 230400..."));
            gps.begin(38400);
            gps.SetGPSbaud(230400, true);
            gps.end();
    
            Serial.println(F("Using 57,600 baud, trying to set baudrate to 230400..."));
            gps.begin(57600);
            gps.SetGPSbaud(230400, true);
            gps.end();
    
            Serial.println(F("Using 115,200 baud, trying to set baudrate to 230400..."));
            gps.begin(115200);
            gps.SetGPSbaud(230400, true);
            gps.end();
    
            Serial.println(F("Using 230,400 baud, trying to set baudrate to 230400..."));
            gps.begin(230400);
            gps.SetGPSbaud(230400, true);
            gps.end();
    
            Serial.println(F("Using 460,800 baud, trying to set baudrate to 230400..."));
            gps.begin(460800);
            gps.SetGPSbaud(230400, true);
            gps.end();
    
            Serial.println(F("Using 921,600 baud, trying to set baudrate to 230400..."));
            gps.begin(921600);
            gps.SetGPSbaud(230400, true);
            gps.end();
    
            Serial.println(F("Using 230,400 baud, Polling GPS Baud Port1..."));
            // now start communication with the GPS receiver at 230400 baud,
            gps.begin(230400);                        // Enable Teensy serial communication @ given baud rate
            gps.Poll_GPSbaud_Port1(true);             // Polls the GPS baud configuration for one I/O Port, I/O Target 0x01=UART1
    
            gps.Poll_NAV_PVT();                       // Polls UBX-NAV-PVT    (0x01 0x07) Navigation Position Velocity Time Solution
    
            gps.SetRATE(1000, true);                   // Navigation/Measurement Rate Settings, e.g. 100ms => 10Hz, 200 => 5.00Hz, 1000ms => 1Hz, 10000ms => 0.1Hz
            // Possible Configurations:
            // 60=>16.67Hz, 64=>15.63Hz, 72=>13.89Hz, 80=>12.50Hz, 100=>10.00Hz, 125=>8.00Hz, 200=>5.00Hz, 250=>4.00Hz, 500=>2.00Hz
            // 800=>1.25Hz, 1000=>1.00Hz, 2000=>0.50Hz, 4000=>0.25Hz, 10000=>0.10Hz, 20000=>0.05Hz, 50000=>0.02Hz
    
            // NOTE: Dis_all_NMEA -strongly suggest changing RX buffer to 255 or more,*otherwise you will miss ACKs*on serial monitor
            gps.Dis_all_NMEA_Child_MSGs(true);       // Disable All NMEA Child Messages Command
    
            gps.SetNAV5(3, true);                     // Set Dynamic platform model Navigation Engine Settings (0:portable, 2: stationary, 3:pedestrian, Etc)
            // Possible Configurations
            // 0: portable, 2: stationary, 3: pedestrian, 4: automotive, 5: sea, 6: airborne with <1g, 7: airborne with <2g
            // 8: airborne with <4g, 9: wrist worn watch (not supported in protocol v.less than 18)
    
            // ### Periodic auto update ON,OFF Command ###
            gps.Dis_NAV_PVT(true);                  // Disable periodic auto update NAV_PVT
            //gps.Ena_NAV_PVT(true);                    // Enable periodic auto update NAV_PVT
    
            //gps.Ena_NAV_ATT(true);                  // Enable periodic auto update NAV_ATT ~ U-blox M8 from protocol version 19
            gps.Dis_NAV_ATT(true);                    // Disable periodic auto update NAV_ATT ~ ---^
    
            //gps.Ena_NAV_POSLLH(true);                 // Enable periodic auto update NAV_POSLLH
            gps.Dis_NAV_POSLLH(true);               // Disable periodic auto update NAV_POSLLH
    
        #else
            gps.begin(BaudDefault);         // Enable Teensy serial communication @ 9600 baud, default settings.
            gps.SetGPSbaud(230400, false);  // Set GPS Port Baud, Possible Baud Rate Configurations 4800~9600~19200~38400~57600~115200~230400~460800
            gps.end();                      // Disables Teensy serial communication, to re-enable serial communication, call gps.begin(Baud, bool).
            
            gps.begin(19200);
            gps.SetGPSbaud(230400, false);
            gps.end();
            
            gps.begin(38400);
            gps.SetGPSbaud(230400, false);
            gps.end();
            
            gps.begin(57600);
            gps.SetGPSbaud(230400, false);
            gps.end();
            
            gps.begin(115200);
            gps.SetGPSbaud(230400, false);
            gps.end();
            
            gps.begin(230400);
            gps.SetGPSbaud(230400, false);
            gps.end();
            
            gps.begin(460800);
            gps.SetGPSbaud(230400, false);
            gps.end();
            
            gps.begin(921600);
            gps.SetGPSbaud(230400, false);
            gps.end();
            
            // now start communication with the GPS receiver at 230400 baud,
            gps.begin(230400);                        // Enable Teensy serial communication @ given baud rate
            gps.Poll_GPSbaud_Port1(false);            // Polls the GPS baud configuration for one I/O Port, I/O Target 0x01=UART1
            
            //gps.Poll_NAV_PVT();                       // Polls UBX-NAV-PVT    (0x01 0x07) Navigation Position Velocity Time Solution
    
            gps.SetRATE(1000, false);                   // Navigation/Measurement Rate Settings, e.g. 100ms => 10Hz, 200 => 5.00Hz, 1000ms => 1Hz, 10000ms => 0.1Hz
            // Possible Configurations:
            // 60=>16.67Hz, 64=>15.63Hz, 72=>13.89Hz, 80=>12.50Hz, 100=>10.00Hz, 125=>8.00Hz, 200=>5.00Hz, 250=>4.00Hz, 500=>2.00Hz
            // 800=>1.25Hz, 1000=>1.00Hz, 2000=>0.50Hz, 4000=>0.25Hz, 10000=>0.10Hz, 20000=>0.05Hz, 50000=>0.02Hz
    
            // NOTE: Dis_all_NMEA -strongly suggest changing RX buffer to 255 or more,*otherwise you will miss ACKs*on serial monitor
            gps.Dis_all_NMEA_Child_MSGs(false);       // Disable All NMEA Child Messages Command
    
            gps.SetNAV5(3, false);                     // Set Dynamic platform model Navigation Engine Settings (0:portable, 2: stationary, 3:pedestrian, Etc)
            // Possible Configurations
            // 0: portable, 2: stationary, 3: pedestrian, 4: automotive, 5: sea, 6: airborne with <1g, 7: airborne with <2g
            // 8: airborne with <4g, 9: wrist worn watch (not supported in protocol v.less than 18)
    
            // ### Periodic auto update ON,OFF Command ###
            //gps.Ena_NAV_PVT(false);                  // Enable periodic auto update NAV_PVT
            gps.Dis_NAV_PVT(false);                    // Disable periodic auto update NAV_PVT
    
            //gps.Ena_NAV_ATT(true);                  // Enable periodic auto update NAV_ATT ~ U-blox M8 from protocol version 19
            gps.Dis_NAV_ATT(false);                    // Disable periodic auto update NAV_ATT ~ ---^
    
            //gps.Ena_NAV_POSLLH(true);                 // Enable periodic auto update NAV_POSLLH
            gps.Dis_NAV_POSLLH(false);               // Disable periodic auto update NAV_POSLLH

        #endif
    
    #endif
    
    #if defined(DEBUG)
        Serial.println(F("Setup finished GPS setup.."));
    #endif
    
    clearBuffer(receiveBuffer, sizeof(receiveBuffer));
    
    //--------------------------Uart2Event Configuration--------------------------------
    
    #if defined(DEBUG20)
        Serial.print(F("Setting the Event2 Transmit pin to: "));
        Serial.println(serial2TxPin);;
    #endif
    
    Event2.set_transmit_pin(serial2TxPin, 0);        //
    
    #if defined(DEBUG20)
        Serial.print(F("Setting the Event2 Recieve pin to: "));
        Serial.println(serial2RxPin);
    #endif
    
    Event2.set_recieve_pin(serial2RxPin);           //
    
    #if defined(DEBUG20)
        Serial.println(F("Setting the Event2 Loop Back to no loopback.."));
    #endif
    
    Event2.loopBack = false;                       // internal loopback set / "default = false"
    
    #if defined(DEBUG20)
        Serial.println(F("Setting the Event2 transmit Event Handler to tx2Event.."));
    #endif
    
    Event2.txEventHandler = tx2Event;             // event handler Serial2 TX
    
    #if defined(DEBUG20)
        Serial.println(F("Setting the Event2 recieve Event Handler to rx2Event.."));
    #endif
    
    Event2.rxEventHandler = rx2Event;             // event handler Serial2 RX
    
    #if defined(DEBUG20)
        Serial.println(F("Setting the Event2 Recieve Buffer Size Trigger to 128 bytes.."));
    #endif
    // Set trigger for 128 bytes, [Stx(byte)], [Command(byte)], [Size of Data Packet(byte)], [Data...(up to 122 bytes)], [CRC1(byte)], [CRC2(byte)], [Etx(byte)].
    Event2.rxBufferSizeTrigger = 128;
    
    //Event2.flush();  // wait for any remainding dma transfers to complete.
    Event2.clear();  // Clears the circular buffer in Event2
    
    #if defined(DEBUG20)
        Serial.println(F("Beginning Event2.."));
    #endif
    
    Event2.begin(250000);                         // start serial port
    
    #if defined(DEBUG)
        Serial.println(F("Setup finished Event 2 setup.."));
    #endif
    
    //----------------------------------------------------------------------------------
    
    /*******************************************************************************************************************
    * BMP280 Driver settings
    * Communications Interface can be I2C_MODE or SPI_MODE
    * Specify chipSelectPin using arduino pin names
    * Specify I2C address.  Can be 0x77(default) or 0x76
    *
    * For I2C, enable the following and disable the SPI section
    *******************************************************************************************************************/
    SystemSensor.settings.commInterface = I2C_MODE;
    SystemSensor.settings.I2CAddress = 0x76;
    
    /*******************************************************************************************************************
    * For SPI enable the following and disable the I2C section
    *******************************************************************************************************************/
    //mySensor.settings.commInterface = SPI_MODE;
    //mySensor.settings.chipSelectPin = 10;
    
    /*******************************************************************************************************************
    * Operation settings
    *
    * runMode can be:
    * 0, Sleep mode
    * 1 or 2, Forced mode
    * 3, Normal mode
    *******************************************************************************************************************/
    SystemSensor.settings.runMode = 3; // Normal mode
    
    /*******************************************************************************************************************
    * tStandby can be:
    * 0, 0.5ms
    * 1, 62.5ms
    * 2, 125ms
    * 3, 250ms
    * 4, 500ms
    * 5, 1000ms
    * 6, 10ms
    * 7, 20ms
    *******************************************************************************************************************/
    SystemSensor.settings.tStandby = 0;
    
    /*******************************************************************************************************************
    * Filter can be off or number of FIR coefficients to use:
    * 0, filter off
    * 1, coefficients = 2
    * 2, coefficients = 4
    * 3, coefficients = 8
    * 4, coefficients = 16
    *******************************************************************************************************************/
    SystemSensor.settings.filter = 0;
    
    /*******************************************************************************************************************
    * TempOverSample can be:
    * 0, skipped
    * 1 through 5, oversampling *1, *2, *4, *8, *16 respectively
    *******************************************************************************************************************/
    SystemSensor.settings.tempOverSample = 1;
    
    /*******************************************************************************************************************
    * PressOverSample can be:
    * 0, skipped
    * 1 through 5, oversampling *1, *2, *4, *8, *16 respectively
    *******************************************************************************************************************/
    SystemSensor.settings.pressOverSample = 1;
    
    /*******************************************************************************************************************
    * HumidOverSample can be:
    * 0, skipped
    * 1 through 5, oversampling *1, *2, *4, *8, *16 respectively
    *******************************************************************************************************************/
    SystemSensor.settings.humidOverSample = 1;
    
    /*******************************************************************************************************************
    * Calling SystemSensor.begin() causes the settings to be loaded
    *******************************************************************************************************************/
    
    SystemSensor.begin();
    
    #if defined(DEBUG15)
        Serial.print(F("Starting BME280... result of .begin() in Hex: "));
        Serial.println(SystemSensor.begin(),HEX);
    #endif
    
    delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
    
    #if defined(DEBUG15)
        Serial.print(F("Displaying ID, reset and ctrl regs\n"));
        
        Serial.print(F("ID(0xD0): 0x"));
        Serial.println(SystemSensor.readRegister(BME280_CHIP_ID_REG), HEX);
        Serial.print(F("Reset register(0xE0): 0x"));
        Serial.println(SystemSensor.readRegister(BME280_RST_REG), HEX);
        Serial.print(F("ctrl_meas(0xF4): 0x"));
        Serial.println(SystemSensor.readRegister(BME280_CTRL_MEAS_REG), HEX);
        Serial.print(F("ctrl_hum(0xF2): 0x"));
        Serial.println(SystemSensor.readRegister(BME280_CTRL_HUMIDITY_REG), HEX);
        
        Serial.print(F("\n\n"));
        
        Serial.print(F("Displaying all regs\n"));
        uint8_t memCounter = 0x80;
        uint8_t tempReadData;
        
        for(int rowi = 8; rowi < 16; rowi++ )
            {
                Serial.print(F("0x"));
                Serial.print(rowi, HEX);
                Serial.print(F("0:"));
                for(int coli = 0; coli < 16; coli++ )
                    {
                        tempReadData = SystemSensor.readRegister(memCounter);
                        Serial.print((tempReadData >> 4) & 0x0F, HEX);//Print first hex nibble
                        Serial.print(tempReadData & 0x0F, HEX);//Print second hex nibble
                        Serial.print(F(" "));
                        memCounter++;
                    }
                Serial.print(F("\n"));
            }
        
        Serial.print(F("\n\n"));
        
        Serial.print(F("Displaying concatenated calibration words\n"));
        Serial.print(F("dig_T1, uint16: "));
        Serial.println(SystemSensor.calibration.dig_T1);
        Serial.print(F("dig_T2, int16: "));
        Serial.println(SystemSensor.calibration.dig_T2);
        Serial.print(F("dig_T3, int16: "));
        Serial.println(SystemSensor.calibration.dig_T3);
        
        Serial.print(F("dig_P1, uint16: "));
        Serial.println(SystemSensor.calibration.dig_P1);
        Serial.print(F("dig_P2, int16: "));
        Serial.println(SystemSensor.calibration.dig_P2);
        Serial.print(F("dig_P3, int16: "));
        Serial.println(SystemSensor.calibration.dig_P3);
        Serial.print(F("dig_P4, int16: "));
        Serial.println(SystemSensor.calibration.dig_P4);
        Serial.print(F("dig_P5, int16: "));
        Serial.println(SystemSensor.calibration.dig_P5);
        Serial.print(F("dig_P6, int16: "));
        Serial.println(SystemSensor.calibration.dig_P6);
        Serial.print(F("dig_P7, int16: "));
        Serial.println(SystemSensor.calibration.dig_P7);
        Serial.print(F("dig_P8, int16: "));
        Serial.println(SystemSensor.calibration.dig_P8);
        Serial.print(F("dig_P9, int16: "));
        Serial.println(SystemSensor.calibration.dig_P9);
        
        Serial.print(F("dig_H1, uint8: "));
        Serial.println(SystemSensor.calibration.dig_H1);
        Serial.print(F("dig_H2, int16: "));
        Serial.println(SystemSensor.calibration.dig_H2);
        Serial.print(F("dig_H3, uint8: "));
        Serial.println(SystemSensor.calibration.dig_H3);
        Serial.print(F("dig_H4, int16: "));
        Serial.println(SystemSensor.calibration.dig_H4);
        Serial.print(F("dig_H5, int16: "));
        Serial.println(SystemSensor.calibration.dig_H5);
        Serial.print(F("dig_H6, uint8: "));
        Serial.println(SystemSensor.calibration.dig_H6);
    #endif
    
    #if defined(DEBUG)
        Serial.println(F("Setup finished Temp, Hum, press setup.."));
    #endif
    
    // this is the magic trick for printf to support float
    asm(".global _printf_float");
    
    // this is the magic trick for scanf to support float
    asm(".global _scanf_float");
    
    
    #if defined(USE_FT5206_TOUCH)  // This the the FT5206 flag.
        // These are the pins for the FT5206 capacitive controller on the 7" version (ER-TFTM070) with the RA8875 Touchscreen
        pinMode(FT5206_WAKE, OUTPUT);  // This is the WAKE input from the FT5206 Capacitive controller on the 7" touchscreen.
        digitalWrite(FT5206_WAKE, HIGH);  // Start with the ouput high.
    
        pinMode(FT5206_RST, OUTPUT);  // This is the WAKE input from the FT5206 Capacitive controller on the 7" touchscreen.
        digitalWrite(FT5206_RST, HIGH);  // Start with the ouput high.
    
        digitalWrite(FT5206_RST, LOW);  // Toggle the ouput low.
        delay(5);
        digitalWrite(FT5206_RST, HIGH);  // Set back to ouput high, This resets the FT5206 Controller.
    
        digitalWrite(FT5206_WAKE, LOW);  // Toggle the ouput low.
        delay(5);
        digitalWrite(FT5206_WAKE, HIGH);  // Set back to ouput high, This resets the FT5206 Controller.
    #endif

    #if defined(DEBUG)
        Serial.println(F("Setup finished FT5206 Touch setup.."));
    #endif
    
    /********************************************************************************************************************
    * These are the pin numbers on teensy 3.0, 3.1, & 3.2 for each of the port numbers that are available accordingly.
    *
    * PortA[4:5, 12:13] = {33, 24, 3, 4}
    * PortB[0:3, 16:19] = {16, 17, 19, 18, 0, 1, 32, 25}
    * PortC[0:11] = {15, 22, 23, 9, 10, 13, 11, 12, 28, 27, 29, 30}
    * PortD[0:7] = {2, 14, 7, 8, 6, 20, 21, 5}
    * PortE[0:1] = {31, 26}
    ********************************************************************************************************************/

    
    /****************************************************************************************************
    * IntervalTimer is generally recommended for use only in libraries and advanced applications
    * which require highly precise timing.  Up to 4 IntervalTimer objects may be active simultaneuously.
    * The interval is specified in microseconds, which may be an integer or floating point number,
    * for more highly precise timing.
    * elif (F_CPU == 96,000,000)
    * #define F_BUS 48,000,000
    * (Largest number for unsigned int = 4,294,967,295 decimal)
    * MAX_PERIOD = UINT32_MAX / (F_BUS / 1000000.0)
    * The function requires two input variables:
    * 1. The name of the function to be executed.
    * 2. The interval in microseconds.  The largest number allowed is: 89,478,485 or 89 seconds
    *
    * This function returns true if sucessful.
    * False is returned if all hardware resources are busy, or used by other IntervalTimer objects.
    *
    *****************************************************************************************************/
    
    status = oneSecondTimer.begin(oneSecondTimerISR, 1000000);  // One Second ISR to run every 1 second.
    if (status == false)
        {
            #if defined(DEBUG)
                Serial.println(F("oneSecondTimer initialization failed"));
            #endif
        }
    
    status = twoSecondTimer.begin(twoSecondTimerISR, 2000000);  // Two Second ISR to run every 2 seconds.
    if (status == false)
        {
            #if defined(DEBUG)
                Serial.println(F("twoSecondTimer initialization failed"));
            #endif
        }
    
    status = tenSecondTimer.begin(tenSecondTimerISR, 10000000);  // Ten Second ISR to run every 10 seconds.
    if (status == false)
        {
            #if defined(DEBUG)
                Serial.println(F("tenSecondTimer initialization failed"));
            #endif
        }
    
    
    status = thirtySecondTimer.begin(thirtySecondTimerISR, 3E7);  // Thirty Second ISR to run every 30 seconds, or 30 million microseconds.
    if (status == false)
        {
            #if defined(DEBUG)
                Serial.println(F("thirtySecondTimer initialization failed"));
            #endif
        }
    
    
    #if defined(DEBUG)
        Serial.println(F("Setup finished interval timers setup.."));
    #endif
    
    /*******************************************************************************************************************
    * Time uses a special time_t variable type, which is the number of seconds elapsed since 1970. Using time_t lets
    * you store or compare times as a single number, rather that dealing with 6 numbers and details like the number of
    * days in each month and leap years.
    *
    * now();
    * Reads the current time, as a time_t number.
    *
    * setTime(t);
    * Sets the current time, based on a time_t number.
    *
    * makeTime(tm);
    * Convert normal date & time to a time_t number. The time_t number is returned. The tm input is a TimeElements variable type, which has these fields:
    * tm.Second  Seconds   0 to 59
    * tm.Minute  Minutes   0 to 59
    * tm.Hour    Hours     0 to 23
    * tm.Wday    Week Day  0 to 6  (not needed for mktime)
    * tm.Day     Day       1 to 31
    * tm.Month   Month     1 to 12
    * tm.Year    Year      0 to 99 (offset from 1970)
    *
    * breakTime(t, tm);
    * Convert a time_t number to normal date & time. The tm input is a TimeElements variable which breakTime fills with the 7 numbers, computed from the "t" input.
    *
    * hour(t);
    * minute(t);
    * second(t);
    * day(t);
    * month(t);
    * year(t);
    * Convert a time_t number to a single time or data field. These can be simpler to use than breakTime() and a 7-field TimeElements variable.
    *
    * Functions for managing the timer services are:
    * setTime(t);             // set the system time to the give time t
    * setTime(hr,min,sec,day,mnth,yr); // alternative to above, yr is 2 or 4 digit yr (2010 or 10 sets year to 2010)
    * adjustTime(adjustment); // adjust system time by adding the adjustment value
    *
    * timeStatus();       // indicates if time has been set and recently synchronized
    * returns one of the following enumerations:
    * timeNotSet      // the time has never been set, the clock started at Jan 1 1970
    * timeNeedsSync   // the time had been set but a sync attempt did not succeed
    * timeSet         // the time is set and is synced
    * Time and Date values are not valid if the status is timeNotSet. Otherwise values can be used but
    * the returned time may have drifted if the status is timeNeedsSync.
    *
    * setSyncProvider(getTimeFunction);  // set the external time provider
    * setSyncInterval(interval);         // set the number of seconds between re-sync
    *******************************************************************************************************************/
    
    /*******************************************************************************************************************
    * This sets the system time clock to use the time from the internet.
    *******************************************************************************************************************/
    #if defined(timeSyncProviderInternet)
    
    #endif
    
    /*******************************************************************************************************************
    * This sets the system time clock to use the GPS time clock.
    *******************************************************************************************************************/
    #if defined(timeSyncProviderGPS)
        status = updateGPSTimeTimer.begin(updateGPSTimeISR, 60000000);  //  Update GPS Time to run every 60 seconds.
        if (status == false)
            {
                if (DEBUG) Serial.println("updateGPSTimeTimer initialization failed");
            }
    #endif
    
    
    /*******************************************************************************************************************
    * This sets the system time clock to use the Teensy's internal clock.
    ********************************************************************************************************************/
    #if defined(timeSyncProviderTeensy)
    
        #if defined(DEBUG9)
            Serial.println(F("Setting the sync provider to Teensy..."));
        #endif
    
        setSyncProvider(getTeensy3Time);  // Set the source of the time function to the Teensy internal battery backed up clock.
    
        #if defined(DEBUG9)
            Serial.println(F("Setting the Teensy clock from the DS3231..."));
        #endif
        // compactTime = setTeensy3Clock();  // Sets the Teensy's internal clock to the DS1307 time.
    #endif
    
    
    
    /*******************************************************************************************************************
    * This sets the system time clock to use the external DS3231 clock.
    ********************************************************************************************************************/
    #if defined(timeSyncProviderDS3231)
        #if defined(DEBUG9)
            Serial.println(F("Setting the sync provider to DS3231 on the I2C bus..."));
        #endif
    
        setSyncProvider(RTC.get);   // The function to get the time from the DS3231 using the DS1307 code.
    
        if(timeStatus() != timeSet)
            {
                #if defined(DEBUG9)
                    Serial.println(F("Unable to sync with the RTC"));
                #endif
            }
        else
            {
                #if defined(DEBUG9)
                    Serial.println(F("RTC has set the system time"));
                #endif
            }
    #endif
    
    /*******************************************************************************************************************
    * Get current time from DS3231 and put into variables curentTime[0] (hh), [1] (mm), & [2] (ss).
    *******************************************************************************************************************/
    
    if (RTC.read(tm))
        {
            lastTimeSetByClock = makeTime(tm);  // This changes a tmElements to a time_t (Unix time).
            currentTime[0]      = (tm.Hour);
            currentTime[1]      = (tm.Minute);
            currentTime[2]      = (tm.Second);
            currentDate[0]      = (tm.Day);
            currentDate[1]      = (tm.Month);
            currentDayOfWeek    = (tm.Wday);
            currentDate[2]      = (tmYearToCalendar(tm.Year));
        }
    else
        {
            if (RTC.chipPresent())
                {
                    Serial.println(F("The DS3231 is stopped.  Please run the SetTime"));
                    Serial.println(F("example to initialize the time and begin running."));
                    Serial.println();
                }
            else
                {
                    Serial.println(F("DS3231 read error!  Please check the circuitry."));
                    Serial.println();
                }
        }
    
    PreviousTimeForWind = lastTimeSetByClock;
    
    
    #if defined(DEBUG)
        Serial.println(F("Setup finished Time of day setup.."));
    #endif
   
    
    /********************************************************************************************************************
    *                                   Get Rain Data from EEPROM and put in RAM.
    * This is where the info concerning the rain gauge in EEPROM is placed in the appropriate variables in RAM.
    * The AT24C32 has 4096 bytes of memory storage (address 0 -> 0xFFF).
    ********************************************************************************************************************/
    float i = eepromMemory.readFloat(rainMemoryLocation);  // First 4 bytes is the rain per year in float.
    #if defined(DEBUG40)  // This the the Reading Startup Values from EEPROM debug flag.
        Serial.print(F("Read EEPROM Memory yearly rain offset, Rain Offset: "));
        Serial.println(i, 3);
    #endif
    if ( i > 0 )
        {
            rainYearlyOffset = i;
        }
    
    /********************************************************************************************************************
     *             Get Temp Sensor Calibration Data from EEPROM and put in RAM.
     * This is where the info concerning the Temp Sensor Calibration Offset in EEPROM is placed in the appropriate variables in RAM.
     * The AT24C32 has 4096 bytes of memory storage (address 0 -> 0xFFF).
     ********************************************************************************************************************/
    float j = eepromMemory.readFloat(TempCalOffsetLocation);  // First 4 bytes is Temp Cal offset in float.
    #if defined(DEBUG40)  // This the the Reading Startup Values from EEPROM debug flag.
        Serial.print(F("Read EEPROM Memory Temp Sensor Calibration Offset, Temp Sensor Calibration Offset: "));
        Serial.println(j, 3);
    #endif
    if ( j != 0 )
        {
            SystemSensor.settings.tempCorrection = j;
        }

    /********************************************************************************************************************
     *                  Get Time Zone Setting from EEPROM and put in RAM.
     * This is where the info concerning the Time Zone Setting in EEPROM is placed in the appropriate variables in RAM.
     * The AT24C32 has 4096 bytes of memory storage (address 0 -> 0xFFF).
     ********************************************************************************************************************/
    uint8_t k = eepromMemory.read(TimeZoneSettingLocation);  // This is Temp Cal offset in uint8_t.
    time_t temp_t;
    
    #if defined(DEBUG40)  // This the the Reading Startup Values from EEPROM debug flag.
        Serial.print(F("Read EEPROM Memory Time Zone Setting, This is the time zone from EEPROM: "));
        Serial.println(k);
    #endif
    
    #if defined(DEBUG40)  // This the the Reading Startup Values from EEPROM debug flag.
        Serial.print(F("Read EEPROM Memory Time Zone Setting, This is the size of the tzIndex: "));
        Serial.println( (sizeof(timezones) / 4) );  // The timezones array is an array of addresses.
    #endif
    
    if ( (k >= 0) && (k <= (sizeof(timezones) / 4) ) )  // This checks for the validity of the time zone.
        {
            tzIndex = k;
            tz = timezones[tzIndex];  // This variable is used to determine which time zone we are using.
            temp_t = ((*tz).toLocal(lastTimeSetByClock, &tcr));
            #if defined(DEBUG40)  // This the the Reading Startup Values from EEPROM debug flag.
                Serial.print(F("Read EEPROM Memory Time Zone Setting, set time to: "));
                Serial.println(tcr -> abbrev);
            #endif
        }
    else
        {
            tz = timezones[tzIndex];  // Set the time zone to the default.
            temp_t = ((*tz).toLocal(lastTimeSetByClock, &tcr));
            #if defined(DEBUG40)  // This the the Reading Startup Values from EEPROM debug flag.
                Serial.print(F("Read EEPROM Memory Time Zone Setting, EEPROM Number was not valid, setting to: "));
                Serial.println(tcr -> abbrev);
            #endif
        }
    
    
    #if defined(DEBUG)
        Serial.println(F("Setup finished EEPROM setup.."));
    #endif

    
    /**************************************************************************
    * These are the returns from tft.begin: A byte with the error code/s:
    * bit--------------------------------------------------------------------
    * 0:    The display it's not supported!
    * 1:    The MOSI or MISO or SCLK choose for Teensy it's out of permitted range!
    * 2:    The CS pin you selected it's out of permitted range!
    * 3:    You have to upgrade to Teensyduino 1.22 or better to use this feature!
    * 4:
    * 5:
    * 6:
    * 7:
    * 0b00000000 = no error
    **************************************************************************/
    tft.begin(RA8875_800x480);
    tftErrorCode = tft.errorCode();
    if (tftErrorCode != 00)
        {
            #if defined(DEBUG11)  // This the the TFT debug flag.
                Serial.println(F("Error: TFT error during tft.begin! "));
                Serial.println(tftErrorCode, HEX);
            #endif
        }
    else
        {
            tftPresentFlag = false; // There is a problem with the TFT display, or its not there.
        }
    
    tft.setFontScale(1);  // Set Font to 2x normal size (16W x 32T pixels). {0 is normal size (8w x 16t pixels)}
    tftErrorCode = tft.errorCode();
    if (tftErrorCode != 00)
        {
            #if defined(DEBUG11)  // This the the TFT debug flag.
                Serial.println(F("Error: tft.setFontScale(1)! "));
                Serial.println(tftErrorCode, HEX);
            #endif
        }
    
    tft.brightness(255);
    tftErrorCode = tft.errorCode();
    if (tftErrorCode != 00)
        {
            #if defined(DEBUG11)  // This the the TFT debug flag.
                Serial.println(F("Error: tft.brightness(255)! "));
                Serial.println(tftErrorCode, HEX);
            #endif
        }
    
    tft.setRotation(SCREEN_ROTATION);  // Set Screen Rotation to Landscape.
    tftErrorCode = tft.errorCode();
    if (tftErrorCode != 00)
        {
            #if defined(DEBUG11)  // This the the TFT debug flag.
                Serial.println(F("Error: tft.setRotation(SCREEN_ROTATION) "));
                Serial.println(tftErrorCode, HEX);
            #endif
        }
    
    #if defined(USE_FT5206_TOUCH)
    
        tft.useCapINT(FT5206_INT);
        if (tftErrorCode != 00)
            {
                #if defined(DEBUG11)  // This the the TFT debug flag.
                    Serial.println(F("Error: tft.useCapINT(RA8875_INT) "));
                    Serial.println(tftErrorCode, HEX);
                #endif
            }
    
        // we use the capacitive chip Interrupt out!  the following set the max touches (max 5)
        // it can be placed inside loop but BEFORE touched() to limit dynamically the touches (for example to 1)
        tft.setTouchLimit(MAXTOUCHLIMIT);
        if (tftErrorCode != 00)
            {
                #if defined(DEBUG11)  // This the the TFT debug flag.
                    Serial.println(F("Error: tft.setTouchLimit(MAXTOUCHLIMIT) "));
                    Serial.println(tftErrorCode, HEX);
                #endif
            }
    
        tft.enableCapISR(true);  // capacitive touch screen interrupt it's armed
        if (tftErrorCode != 00)
            {
                #if defined(DEBUG11)  // This the the TFT debug flag.
                    Serial.println(F("Error: tft.enableCapISR(true) "));
                    Serial.println(tftErrorCode, HEX);
                #endif
            }
    
    #else
        #if defined(DEBUG11)  // This the the TFT debug flag.
            Serial.println(F("You should open RA8875UserSettings.h file and uncomment USE_FT5206_TOUCH!"));
            tft.print(F("You should open RA8875UserSettings.h file and uncomment USE_FT5206_TOUCH!"));
        #endif
    #endif
    
    #if defined(USE_RA8875_TOUCH)
    
        tft.useINT(RA8875_INT);  // We use generic interrupt helper for Internal Resistive Touch Screen.
    
        tft.touchBegin();  // Enable touch support for RA8875.
    
    
        if (!tft.touchCalibrated())  // Already calibrated?
            {
                Serial.println(F("Maybe you better calibrate the Touch Screen first!"));
            }
    
        tft.touchEnable(true);  // Set the touch screen interrupt to enable.
        tft.enableISR(true);    // Start up interrupts again.
    
    #endif
    
    // Now set up the clock face:
    clockPos[0] = (tft.width() / 2) + 32;  // Clock center position in x direction (pixels).
    clockPos[1] = tft.height() / 2;  // Clock center position in y direction (pixels).
    if (tft.isPortrait())
        {
            clockPos[2] = (tft.width()/2) - 20;  // Clock radius in pixels (480/2 - 20 = 220 pixels).
        }
    else  // Must be in Landscape mode
        {
            clockPos[2] = (tft.height()/2) - 45;  // Clock radius in pixels (480 / 2 - 45 = 195 pixels).
        }
    
    oldPos[0] = clockPos[0];
    oldPos[1] = clockPos[1];
    oldPos[2] = clockPos[0];
    oldPos[3] = clockPos[1];
    oldPos[4] = clockPos[0];
    oldPos[5] = clockPos[1];
    
    drawGauge(clockPos, 0, 360,RA8875_WHITE, RA8875_WHITE, RA8875_WHITE);  // This draws the the round clock face with the major & minor tick marks.
    
    getSensorData();  // Get some data from the Temp, Press, Hum sensor.
    
    // Now set up the Wind Direction Flower:
    WindDirFlowerPos[0] = (tft.width() / 8) + 10;  // Wind Direction Flower center position in x direction (pixels).
    WindDirFlowerPos[1] = tft.height() / 2;  // Wind Direction Flower center position in y direction (pixels).
    if (tft.isPortrait())
        {
            WindDirFlowerPos[2] = 30;  // Wind Direction Flower radius in pixels.
        }
    else
        {
            WindDirFlowerPos[2] = 90;  // Wind Direction Flower radius in pixels.
        }
    
    oldWindDirFlowerPos[0] = WindDirFlowerPos[0];
    oldWindDirFlowerPos[1] = WindDirFlowerPos[1];
    
    drawWindGauge(WindDirFlowerPos, 0, 360, RA8875_WHITE, RA8875_WHITE, RA8875_WHITE);  // This draws the the round Wind Direction Flower with the major & minor tick marks.
    
    windFlowerBlanked = false;  // The Wind Direction Flower Gauge is now on the screen.
    
    #if defined(DEBUG)
        Serial.println(F("Setup finished TFT setup.."));
    #endif
    
    #if defined(USINGTEMPSENSORS)
        tempSensors.requestTemperatures();  // This requests that the Temp conversions begin without waiting for the 750msec to complete.
    #endif
    
    #if defined(USINGGPS)
        Serial1.clear();  // Clear the Serial1 buffer of all data
        gps.Poll_NAV_PVT();  // Polls UBX-NAV-PVT, this requests the data from the GPS Module.
    #endif
    
    tft.setTextColor(RA8875_LIGHT_ORANGE);
    tft.setCursor( ( XMIDDLE - Xsize ), YTOP);  // Size is set for 16 x 32 pixels.
    tft.print(F("MASTER"));
    tft.setTextColor(RA8875_WHITE);

    statusDotColor = DOT_RED;  // Set the color indicator of the status dot to red.
    printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_RED);  // Print RED dot to indicate beginning status of the corrected UTC Time from the GPS.
    
    printStatusDotOnTFT(XRIGHT - (Xsize * 17), (YLINE14 + 16), RA8875_RED);  // Print red dot to indicate no data has been received.
    
    float tempPressureReading;
    //Take a reading.
    //Start with temperature, as that data is needed for accurate compensation.
    //Reading the temperature updates the compensators of the other functions
    //in the background.
    
    /// This initializes the daily high and low for the inside temp.
        getSensorData();  // This function reads Temp, Press, & Humidity into sensor.State Struct.
        insideDailyHighTemp = sensorState.Temperature;
        insideDailyLowTemp = sensorState.Temperature;
    
    /// Enable the watchdog by calling Watchdog.enable() as below.  This will turn on the watchdog timer with a ~12 second timeout
    /// before reseting the Arduino. The estimated actual milliseconds before reset (in milliseconds) is returned.
    /// Make sure to reset the watchdog before the countdown expires or the Arduino will reset!
    #if defined(DEBUG46)  // This the the WatchDog debug flag.
        int countdownMS = Watchdog.enable(12000);
        Serial.print("Enabled the watchdog with max countdown of: ");
        Serial.print(countdownMS, DEC);
        Serial.println(" milliseconds!");
        Serial.println();
    #else
        Watchdog.enable(12000);  // Set Watchdog timer for 12 seconds.
    #endif
    
    #if defined(DEBUG)
        Serial.println(F("Setup complete.."));
    #endif
    
}


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/// This is the Main Loop Code.
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/

void loop()
{
    #if defined(DEBUG10)
        unsigned long start, executionTime;
    #endif
    
    tmElements_t tm;
    bool touchCheckStatus = false;
    
    /*******************************************************************************************************************
     *                           This section runs every second, driven by interrupt.
     *******************************************************************************************************************/
    if (oneSecondFlag == true)  // This runs every second
        {
            if (preferencesInProgressFlag == false)
                {
                    /*******************************************************************************************************************
                    * Get current time from DS3231 and put into variables curentTime[0] (hh), [1] (mm), & [2] (ss).
                    *******************************************************************************************************************/
                    if (RTC.read(tm))
                        {
                            lastTimeSetByClock = makeTime(tm);  // Converts tmElements array into a Time_T (UNIX Time), LastTimeSetByClock is a Time_T (UNIX Time) variable.
                
                            currentTime[0]      = (tm.Hour);
                            currentTime[1]      = (tm.Minute);
                            currentTime[2]      = (tm.Second);
                            currentDate[0]      = (tm.Day);
                            currentDate[1]      = (tm.Month);
                            currentDate[2]      = (tmYearToCalendar(tm.Year));
                            currentDayOfWeek    = (tm.Wday);  // Put the Day of the Week into the currentDayOfWeek Variable.
                        }
                    else
                        {
                            if (RTC.chipPresent())
                                {
                                    Serial.println(F("The DS3231 is stopped.  Please run the SetTime"));
                                    Serial.println(F("example to initialize the time and begin running."));
                                    Serial.println();
                                }
                            else
                                {
                                    Serial.println(F("DS3231 read error!  Please check the circuitry."));
                                    Serial.println();
                                }
                        }
            
                        if ((currentTime[0] >= 22) || (currentTime[0] < 8))
                            {  // Night Time Brightness is lower, with red text and a red clock face.
                                tft.brightness(5);
                                tft.setTextColor(RA8875_RED,RA8875_BLACK);  // Set text color + text background color.
                                drawGauge(clockPos, 0, 360,RA8875_RED, RA8875_RED, RA8875_RED);  // This draws the the round clock face with the major & minor tick marks.
                                switch (statusDotColor)
                                    {
                                        case DOT_RED:
                                            printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_RED);  // Print the status of the corrected UTC Time from the GPS.
                                            break;
                                        
                                        case DOT_YELLOW:
                                            printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_YELLOW);  // Print the status of the corrected UTC Time from the GPS.
                                            break;
                                        
                                        case DOT_GREEN:
                                            printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_GREEN);  // Print the status of the corrected UTC Time from the GPS.
                                            break;
                                        
                                        default:
                                            printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_BLACK);  // Print the status of the corrected UTC Time from the GPS.
                                            break;
                                    }
                            }
                        else
                            {  // Day Time brightness is full power with white text and a white clock face
                                tft.brightness(255);
                                tft.setTextColor(RA8875_WHITE,RA8875_BLACK);  // Set text color + text background color.
                                drawGauge(clockPos, 0, 360,RA8875_WHITE, RA8875_WHITE, RA8875_WHITE);  // This draws the the round clock face with the major & minor tick marks.
                                switch (statusDotColor)
                                    {
                                        case DOT_RED:
                                            printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_RED);  // Print the status of the corrected UTC Time from the GPS.
                                            break;
                                        
                                        case DOT_YELLOW:
                                            printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_YELLOW);  // Print the status of the corrected UTC Time from the GPS.
                                            break;
                                        
                                        case DOT_GREEN:
                                            printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_GREEN);  // Print the status of the corrected UTC Time from the GPS.
                                            break;
                                        
                                        default:
                                            printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_BLACK);  // Print the status of the corrected UTC Time from the GPS.
                                            break;
                                    }
                            }
            
                        drawClockHands(clockPos, currentTime, clockColors);
                        drawPrintTime(( clockPos[0] - (Xsize * 5)) , YLINE10, currentTime[0], currentTime[1], currentTime[2], currentDayOfWeek, currentDate[0], currentDate[1], currentDate[2], false);
            
                        if ( (WeatherDataStruct.windAverage > 0) )  // If the wind average is more than zero print the wind data on the TFT.
                            {
                                if ((currentTime[0] >= 22) || (currentTime[0] < 8))
                                    {  // Night Time Brightness is very low, with red text and a red clock face.
                                        tft.setTextColor(RA8875_RED,RA8875_BLACK);  // Set text color to Red + text background color to Black.
                                        drawWindGauge(WindDirFlowerPos, 0, 360,RA8875_RED, RA8875_RED, RA8875_RED);  // This draws the the round Wind Direction Flower with the major & minor tick marks.
                                    }
                                else
                                    {
                                        tft.setTextColor(RA8875_WHITE,RA8875_BLACK);  // Set text color to White + text background color to Black.
                                        drawWindGauge(WindDirFlowerPos, 0, 360, RA8875_WHITE, RA8875_WHITE, RA8875_WHITE);  // This draws the the round Wind Direction Flower with the major & minor tick marks.
                                    }
                
                                drawPrintWind(XLEFT, YTOP, WeatherDataStruct.windAverage, WeatherDataStruct.windMax, WeatherDataStruct.windApparentDir);
                                drawWindDirPointer(WindDirFlowerPos, WeatherDataStruct.windApparentDir, WindDirFlowerColors);
                                if (windFlowerBlanked == true)
                                    {
                                        windFlowerBlanked = false;
                                    }
                            }
                        else if ( (WeatherDataStruct.windAverage == 0) )  // The wind average is zero.
                            {
                                drawBlankWind(WindDirFlowerPos, XLEFT, YTOP);
                                if (windFlowerBlanked == false)
                                    {
                                        windFlowerBlanked = true;
                                    }
                
                            }
            
                    if (currentTime[2] == 0)  // Seconds = zero.
                        {  // one minute has passed.
                            if (currentTime[1] == 0)  // Minutes = zero.
                                {  // One hour has passed
                                    if (currentTime[0] == 0)  // Hours = zero, It is Midnight 00:00:00.
                                        {
                                            insideDailyHighTemp = sensorState.Temperature;  // Set inside Daily High Temp to ambient at midnight.
                                            insideDailyLowTemp = sensorState.Temperature;  // Set inside Daily Low Temp to ambient at midnight.
                        
                                            outsideDailyHighTemp = WeatherDataStruct.Temperature;  // Set outside Daily High Temp to ambient at midnight.
                                            outsideDailyLowTemp = WeatherDataStruct.Temperature;  // Set outside Daily Low Temp to ambient at midnight.
                        
                                            if (currentDate[0] == 1)  // This is the day.
                                                {
                                                    if (currentDate[1] == 10)  // This is the month of October.
                                                        {

                                                        }
                                                }
                                        }
                                    #if defined(USINGGPS)
                                        if (currentTime[0] == 2)  // Hours = two, It is 2AM 02:00:00.
                                            {
                                                updateGPSFlag = true;  // Setting this flag means that the next 2 second interrupt comes around, it will update the RTC from the GPS.
                                            }
                                    #endif
                                }
                        }
            
                        #if defined(USINGTEMPSENSORS)
                            getDS18B20TempsIntoStruct();  // Get the Actual Temps from the Sensors and Place into the Struct.
                            // Print the Temps on the TFT Screen.
                            // XRIGHT - (Xsize * 18), YTOP
                            // tft.setCursor (xPos + (Xsize * 7), yPos + (Ysize * 3));
                            printDS18B20TempsOnTFT( XRIGHT - (Xsize * 9), ( YTOP + ( ( Ysize / 4 ) * 17) ) );  // Print the DS18B20 Temps at the Right side of TFT - 9 character, & 5 1/2 lines down.
            
                            #if defined(DEBUG33)
                                // Print Temps on the Monitor
                                printDS18B20TempsOnMonitor();
            
                                Serial.println(F("\t Request Temp"));
                            #endif
                            tempSensors.requestTemperatures();  // This requests that the Temp conversions begin without waiting for the 750msec to complete.
                        #endif
            
                }  // End of if (preferencesInProgressFlag == false)
            oneSecondFlag = false;
        }  // End of if (oneSecondFlag == true)
    
    
    /*******************************************************************************************************************
    *                           This section runs every 2 seconds, driven by interrupt.
    *******************************************************************************************************************/
    if (twoSecondFlag == true)  // This runs every two seconds.
        {
            if (preferencesInProgressFlag == false)
                {
                    #if defined(USINGGPS)
                        if (updateGPSFlag == true)
                            {
                                Serial1.clear();  // Clear the Serial1 buffer of all data
                                gps.Poll_NAV_PVT();  // Polls UBX-NAV-PVT, this requests the data from the GPS Module.
                                delay(1000);  // Wait for the data to arrive from the GPS sensor.
                                if (updateTimeFromGPS())  // This gets the GMT Time from the GPS attached to Serial port 1.
                                    {
                                        #if defined(DEBUG27)
                                            Serial.println(F("Error: Did not update DS3231 Clock with GPS time!"));
                                            Serial.print(F("statusDotColor is: "));
                                            Serial.println(statusDotColor);
                                            Serial.print(F("UTCTimeCorrected flag: "));
                                            Serial.println(UTCTimeCorrected);
                                        #endif
                                    }
                                else
                                    {
                                        #if defined(DEBUG27)
                                            Serial.println(F("Updated DS3231 Clock with GPS time!"));
                                            Serial.print(F("statusDotColor is: "));
                                            Serial.println(statusDotColor);
                                            Serial.print(F("UTCTimeCorrected flag: "));
                                            Serial.println(UTCTimeCorrected);
                                        #endif
                                    }
                                
                                updateGPSFlag = false;
                            }
                        
                        updatePositionFromGPS();
                        Serial1.clear();  // Clear the Serial1 buffer of all data.
                        gps.Poll_NAV_PVT();  // Polls UBX-NAV-PVT, this requests the data from the GPS Module.
                        
                    #endif
                }
            twoSecondFlag = false;  // End of 2 second routines.
        }  // End of if (twoSecondFlag == true)  // This runs every two seconds.
    
    
    /*******************************************************************************************************************
     *                           This section runs every 10 seconds, driven by interrupt.
     *******************************************************************************************************************/
    if (tenSecondFlag == true)  // This runs every 10 seconds.
        {
            #if defined(DEBUG46)  // This the the WatchDog debug flag.
                Watchdog.reset();  // Kick the dog every 10 seconds, its set to reset the teensy in 12 seconds.
                Serial.println("Reset the WatchDog Timer");
            #else
                Watchdog.reset();  // Kick the dog every 10 seconds, its set to reset the teensy in 12 seconds.
            #endif
            
            if (preferencesInProgressFlag == false)
                {
                    getSensorData();
        
                    drawPrintInsideTempPresHum(XRIGHT - (Xsize * 18), YTOP, sensorState.Temperature, sensorState.Baro_Pressure, sensorState.Relative_Humidity);
                    drawPrintInsideHighLowTempsTFT( ( XRIGHT - (Xsize * 11) ),  ( YTOP + (Ysize * 3) ) );  // Print the days high and low on the TFT
        
                    if (slaveDataReceived)
                        {
                            // Draw the outside Temperature, Pressure, and Humidity on TFT Screen.
                            drawPrintOutsideTempPresHum(XRIGHT - (Xsize * 13), YLINE13, WeatherDataStruct.Temperature, WeatherDataStruct.Baro_Pressure, WeatherDataStruct.Relative_Humidity);
                            drawPrintOutsideHighLowTempsTFT( ( XRIGHT - (Xsize * 11) ),  YLINE12 );  // Print the days high and low on the TFT
                            slaveDataReceived = false;  // Reset the flag just in case we don't get any more data from slave.
                            // Draw the Rain Data on the TFT.
                            drawRainSensorInfo(XLEFT, YLINE13);
                        }
                    else
                        {
                            drawBlankOutsideTempPresHum(XRIGHT - (Xsize * 11), YLINE12);  // This includes the 3 lines of Temp,Pres,Hum data and the days high low temps.
                            printStatusDotOnTFT(XRIGHT - (Xsize * 17), (YLINE14 + 16), RA8875_YELLOW);  // Print yellow dot to indicate the data has stopped.
                        }

                    
                }
        
            tenSecondFlag = false;
        }
    
    
    /*******************************************************************************************************************
    *                           This section runs every 30 seconds, driven by interrupt.
    *******************************************************************************************************************/
    if (thirtySecondFlag == true)  // This runs every 30 seconds.
        {
            twelveMinuteCount++;  // Increment the twelve minute flag.
            sixtyMinuteCount++;
        
            if (twelveMinuteCount == 24)  // Twelve minutes is 24 thirty second counts.
                {
                    #if defined(USINGTEMPSENSORS)
                        tftPlotTempData();  // This plots the data with an osilloscope type display.
                    #endif
                    twelveMinuteCount = 0;  // Reset the count to zero.
                }
        
            if (sixtyMinuteCount == 2880)  // One 24 Hour day is 2880 thirty second counts.
                {
                    sixtyMinuteCount = 0;  // Reset the count to zero.
                }
        
            thirtySecondFlag = false;  // End of 30 second routines.
        }
    
    
    /*******************************************************************************************************************
    *                           This runs at the end of every loop (very fast).
    *******************************************************************************************************************/
    touchCheckStatus = checkForTouch();  // Checks for a touch on the touch screen.
    if (touchCheckStatus == true)
        {
            preferencesInProgressFlag = true;
            setPreferences();  // Set preferences.
            
            preferencesInProgressFlag = false;  // Finsihed with the preferences routine.
        
            // This returns the Master/Slave Mode indication at the top-middle of the TFT.
                tft.setTextColor(RA8875_LIGHT_ORANGE);
                tft.setCursor( ( XMIDDLE - Xsize ), YTOP);  // Size is set for 16 x 32 pixels.
                tft.print(F("MASTER"));
                tft.setTextColor(RA8875_WHITE);
        
            switch (statusDotColor)
                {
                    case DOT_RED:
                        printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_RED);  // Print the status of the corrected UTC Time from the GPS.
                        break;
                    
                    case DOT_YELLOW:
                        printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_YELLOW);  // Print the status of the corrected UTC Time from the GPS.
                        break;
                    
                    case DOT_GREEN:
                        printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_GREEN);  // Print the status of the corrected UTC Time from the GPS.
                        break;
                    
                    default:
                        printUTCStatusDotOnTFT( ( XMIDDLE + ( ( Xsize * 2 ) + 2 ) ), ( YTOP + ( Ysize * 4 ) ), RA8875_BLACK);  // Print the status of the corrected UTC Time from the GPS.
                        break;
                }
        }
    
    if (incomingDMABufferFullFlag)
        {
            processPacketFromSlave();
        
            incomingDMABufferFullFlag = false;
        }
    
    #if defined(USINGTEMPSENSORS)
        if (freshTempsAvailableFlag == true)  // This Flag means there is new temp Data from the DS18B20 Sensors.
            {
                /*
                 if (addDS18B20TempsToArray())  // Get the Temps from the struct and place into the large circular buffers.
                 {
                 Serial.println(F("Error: No sensors or more than 4 sensors!"));
                 }
                 */
                controlTheWaterRelay();  // This function controls the on/off of the Water relay to keep us cool.
        
                freshTempsAvailableFlag = false;  // Reset the New Temp Data from the DS18B20 Sensors flag.
            }
    #endif
    
}

