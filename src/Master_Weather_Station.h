//
// File			Master_Weather_Station.h
// Header
//
// Details		<#details#>
//
// Project		 Master_Weather_Station
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author		freddie
// 				Kingston Co.
//
// Date			2/9/19 3:33 PM
// Version		<#version#>
//
// Copyright	© freddie, 2019
// Licence    <#license#>
//
// See			ReadMe.txt for references

#include "Arduino.h"

#ifndef Master_Weather_Station_h
    #define Master_Weather_Station_h

/*******************************************************************************************************************
 * - Only modify this file to include:
 * - Include files
 * - Function definitions (prototypes)
 * - Constants
 * - External variable definitions, in the appropriate section
 *******************************************************************************************************************/

/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
///                     Include application, user and local libraries
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
/// These includes are from the Arduino program that lives in /Application/Arduino

#include <SPI.h>        // For all SPI transactions on TFT, SD Card, and Touch Screen.
#include <Time.h>       // This is from the Teensy Libraries for time keeping on the Teensy.
#include <Wire.h>       // This is the One Wire or TWI library.
#include <DS1307RTC.h>  // This is the DS1307, DS3231 time library.
#include <Stream.h>     // Needed for serial streaming stuff.
#include <EEPROMex.h>   // This library is to expand on EEPROM reading and writing capabilities.
#include <OneWire.h>

/*******************************************************************************************************************
* This definition determines whether the Teensy is using the attached GPS Controller Chip to get time, date, and
* GPS Coordinates.
* Comment this line out if not using the External GPS chip connected on a Serial Bus.
*******************************************************************************************************************/
#define USINGGPS 1      // This flag determines whether the Teensy is using the GPS controller.

#if defined(USINGGPS)
    // Set this to the GPS hardware serial port you wish to use
    #define GPSHWSERIAL Serial1 // 1 = Serial1, 2 = Serial2, 3 = Serial3, 4 = Serial4 ....

#endif

/*******************************************************************************************************************
* This definition is for the TFTM07 FT5206 Capacitive Touch Screen Controller.
* This type of touch screen controller is only presently installed on the 7" Screen ER-TFTM070.
* Comment this line out if not using the Capacitive Touch Screen Controller on the 7" TFT.
*******************************************************************************************************************/
//#define USE_FT5206_TOUCH

#define MAXTOUCHLIMIT     1  // 1...5


/*******************************************************************************************************************
* This definition is for the TFTM08 build-in 4-wire Resistive Touch Screen Controller.
* This type of touch screen controller is only presently installed on the 8" & 9" Screen ER-TFTM080-2 & ER-TFTM090-2.
* Comment this line out if not using the 4-wire REsistive Touch Screen Controller.
*******************************************************************************************************************/
#define USE_RA8875_TOUCH  // Resistive touch screen

/*******************************************************************************************************************
* This definition is for the TFTM08 build-in 4-wire Resistive Touch Screen Controller.
* This type of touch screen controller is only presently installed on the 8" & 9" Screen ER-TFTM080-2 & ER-TFTM090-2.
* This is the maximum speed my Master Weather Station can go because of cabling issues.
*******************************************************************************************************************/
//const static uint32_t MAXSPISPEED	= 12000000UL;  //don't go higher than 22000000!;


/// These are includes from outside the project ~/Documents/Arduino/libraries
//#include <ILI9341_t3.h>       // This is for the ITDB02-2.4S & ER-TFTM032-3 TFT displays.
#include <RA8875.h>             // We need the RA8875 libs for the ER-TFTM043-3 TFT Display.
#include <Adafruit_SleepyDog.h>     // This library is found the WatchDog Timer buitin to the Teensy.
#include <SparkFunBME280.h>     // Need the BME280 libs for the Temp, Humidity, & Pressure Sensor.
#include "FastCRC.h"            // This library calculates various CRC's using Teensy's builtin math functions.
//#include <SdFat.h>            // We also need the SDFat card libraries for storing the Data on the SD Card disk.
//#include <SdFatUtil.h>
//#include <SdFatConfig.h>
//#include <SerialCommand.h>    // This library is for a Serial Command line.
//#include <Sd2Card.h>          // We also need this library.
#include <DallasTemperature.h>  // This Library is for the DS18B20 Temp Sensors on the OneWire Buss.
#include <Timezone.h>           // This library is for making Time Zone adjustments on UTC Time.
#include <AT24CX.h>             // This library is for using the AT24C32 EEProm on the RTC board.

/// These are include from inside the project
#include <AverageList.h>
#include <BurstBufferDMA.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
//#include <ublox.h>            // This library is used to control the uBlox GPS chip attached to the hardware serial port.
#include <UartEvent.h>        // This is to use serial DMA for transfering serial data.
#include <circular_buffer.h>
#include <Buttons.h>          // This class is used to create various shape boxes and touch buttons.#include <Circular_Buffer.h>
#include <memcpy.h>

#include "Version.h"  // This has the most recent build version.

//#include "PString.h"
//#include "Touch.h"
//#include "EdsDS3231RTC.h"

//#include "IoInit.h"
//#include "Monitor.h"
//#include "Temp.h"
//#include "PS2Keyboard.h"  // This is from the Teensy Libraries for reading the PS2 keyboard on the Teensy.

/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
///                                     End of include libraries.
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
///                                             Typdefs
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/

//typedef unsigned char mybyte;
//mybyte getChecksum(char* str);


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
///                                             Enumerations
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
enum commandTypes  /// These are the Command Codes extracted from the received packet handler:
{
    ACKNAK                  = 0,  // This is the Acknowlege/NoAcknowledge sent to either the Master or the Slave.
    CLEARRAIN               = 1,  // This command is to clear the rain data in RAM on the Slave.
    RESETRAIN               = 2,  // This command is to reset the rain data in RAM & EEPROM on the Slave.
    SETRAINTOTAL            = 3,  // This command is to set the rain total in RAM & EEPROM on the Slave.
    SETTIME                 = 4,  // This command is to set the Slave's RTC to the Master RTC Time.
    XFERWDSTRCT             = 5,  // This command is to send the Weather Data Struct from Slave to Master.
    CPURESET                = 6,
    SETTEMPOFFSET           = 7,
    SETHUMIDITYTHRESHOLD    = 8,
    ENDOFCOMMANDS           = 8
};

enum packetStatus
{
    GOOD_PACKET         = 0,
    BAD_PACKET          = 1,
    PACKET_NOT_FOUND    = 2,
    BAD_CRC             = 3,
    BAD_ACK             = 4,
    BAD_COMMAND         = 5,
    BAD_PREFIX_CHAR     = 6,
    BAD_SUFFIX_CHAR     = 7,
    BAD_DATA_SIZE       = 8
};

enum numericKeypadButtonTouched
{
    ZERO_BUTTON         = 0,
    ONE_BUTTON          = 1,
    TWO_BUTTON          = 2,
    THREE_BUTTON        = 3,
    FOUR_BUTTON         = 4,
    FIVE_BUTTON         = 5,
    SIX_BUTTON          = 6,
    SEVEN_BUTTON        = 7,
    EIGHT_BUTTON        = 8,
    NINE_BUTTON         = 9,
    CLEAR_BUTTON        = 10,
    ENTER_BUTTON        = 11,
    DECIMALPNT_BUTTON   = 12,
    MINUS_SIGN_BUTTON   = 13,
    NO_BUTTON           = 14
};

enum dotColorSelection
{
    DOT_RED     = 0,
    DOT_YELLOW  = 1,
    DOT_GREEN   = 2,
    DOT_NONE    = 3
};

enum gpsCompletionCodes
{
    NO_FIX                  = 0,
    DEAD_RECKONING_ONLY     = 1,
    TWO_D_FIX               = 2,
    THREE_D_FIX             = 3,
    GNSS_DR_COMBINED        = 4,
    TIME_FIX_ONLY           = 5,
    BAD_UBLOX_PACKET        = 6,
    NOT_FULL_PACKET         = 7
};

/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
///                                             Structs
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/

/*******************************************************************************************************************
 *                           This struct is for ubx GPS data.
 * This struct is 56 bytes total.
 *******************************************************************************************************************/
struct gpsGNSSStruct
{
    uint32_t iTOW; // GPS time of week of the navigation epoch: ms
    uint16_t year; // Year (UTC)
    uint8_t month; // Month, range 1..12 (UTC)
    uint8_t day;   // Day of month, range 1..31 (UTC)
    uint8_t hour;  // Hour of day, range 0..23 (UTC)
    uint8_t min;   // Minute of hour, range 0..59 (UTC)
    uint8_t sec;   // Seconds of minute, range 0..60 (UTC)

    uint8_t validTime; // 1 = valid UTC time of day
    uint8_t validDate; // 1 = valid UTC Date

    uint8_t fullyResolved; // 1 = UTC time of day has been fully resolved (no seconds uncertainty)..

    uint32_t tAcc; // Time accuracy estimate (UTC): ns
    int32_t nano;  // Fraction of second, range -1e9 .. 1e9 (UTC): ns

    uint8_t fixType; // GNSSfix Type:
                     // 0: no fix
                     // 1: dead reckoning only
                     // 2: 2D-fix
                     // 3: 3D-fix
                     // 4: GNSS + dead reckoning combined
                     // 5: time only fix

    uint8_t numSV;   // Number of satellites used in Nav Solution
    float lon;       // Longitude: adjusted by dividing by 10 Million.
    float lat;       // Latitude: adjusted by dividing by 10 Million.
    float hMSL;      // Height above mean sea level: adjusted by Multiplied by 0.00328084.
    float gSpeed;    // Ground Speed (2-D): adjusted to m/sec by multiplied by 0.001.
    int32_t headMot; // Heading of motion (2-D): deg * 1e-5

    int32_t headVeh;    // Heading of vehicle (2-D): deg * 1e-5
    int16_t magDec;     // Magnetic declination: deg * 1e-2
    bool valid = false; // Data has just been refreshed and is valid.
} gpsUBXDataStruct;

/*******************************************************************************************************************
 * This struct is for all weather data, including Wind, Temp, Pressure, Humidity, Rainfall information.
 * This struct is 84 bytes total.
 *******************************************************************************************************************/
struct weatherSensorsData
{ // 21 variables 4 bytes each = 84 bytes.
    float windAverage;                          //32 bits (4 bytes).
    float windMax;                              //32 bits (4 bytes).
    float windApparentDir;                      //32 bits (4 bytes)
    float windTrueDir;                          //32 bits (4 bytes)
    unsigned long windLastUpdate;               //32 bits (4 bytes).
    
    float pressureInchesofMercury;              //32 bits (4 bytes).
    float temperatureCorF;                      //32 bits (4 bytes).
    float Temperature;                          //32 bits (4 bytes).
    float Baro_Pressure;                        //32 bits (4 bytes).
    float Relative_Humidity;                    //32 bits (4 bytes).
    
    float rainfallTotalInInches = 0;            // Need space for the total rainfall calculation in cubic inches.
    float hourlyRainTotal = 0;                  // Rainfall accumulated for one hour
    float dailyRainTotal = 0;                   // Rainfall accumulation for 24 hours.
    float yearlyRainTotal = 0;                  // Rainfall accumulation for one Year.
    float runningTotalWaterQuantityCuin = 0;    // This is the running total water accumulated in Cubic Inches.
    float rainfallTotalInCCs =  0;              // Need space for the total rainfall calculation in cc's.
    float runningTotalWaterQuantityCC = 0;      // This is the running total water accumulated in Milliliters.
    unsigned long rainPulseTotal = 0;           // Need space for the total number of rain pulses that have occured.
    
    time_t rainSensorTime = 0;                  // This is the current time of the last tip as set by the isr.
    time_t lastRainPulseTime = 0;               // This is the previous rain pulse time.
    time_t beginningRainFallAccumulation = 0;   // This is the time of the first rainfall pulse.
    
};

/*******************************************************************************************************************
 * This struct is for the ACK or NAK to be returned to the sender of the other data struct.
 * This struct is 3 bytes total.
 *******************************************************************************************************************/
struct masterReturnCommand
{
    char returnText[3];                          // This is the info return to the slave (ACK or NAK).
};


/*******************************************************************************************************************
 *                           This struct is for the tm_elements data.
 * This struct is 8 bytes total.
 *******************************************************************************************************************/
struct timeElementsStruct
{
    uint8_t Second;
    uint8_t Minute;
    uint8_t Hour;
    uint8_t Wday;   // day of week, sunday is day 1
    uint8_t Day;
    uint8_t Month;
    uint16_t Year;   // offset from 1970;
};


/*******************************************************************************************************************
 *                           This struct is for the One Wire Temp Sensors data.
 *******************************************************************************************************************/
struct ds18b20DataStruct
{
    int id;
    DeviceAddress addr;  // This is an 8 byte array of uint_8's.
    float tempData;
};


/*******************************************************************************************************************
 *                           This struct is for the Rain Sensor data.
 * This struct is 44 bytes total.
 *******************************************************************************************************************/
struct rainSensorData
{ //44 bytes
    
    unsigned long rainPulseTotal = 0;           // Need space for the total number of rain pulses that have occured.
    time_t rainSensorTime = 0;                  // This is the current time of the last tip as set by the isr.
    time_t lastRainPulseTime = 0;               // This is the previous rain pulse time.
    time_t beginningRainFallAccumulation = 0;   // This is the time of the first rainfall pulse.
    float hourlyRainTotal = 0;                  // Rainfall accumulated for one hour
    float dailyRainTotal = 0;                   // Rainfall accumulation for 24 hours.
    float yearlyRainTotal = 0;                  // Rainfall accumulation for one Year.
    float rainfallTotalInInches = 0;            // Need space for the total rainfall calculation in cubic inches.
    float runningTotalWaterQuantityCuin = 0;    // This is the running total water accumulated in Cubic Inches.
    float rainfallTotalInCCs =  0;              // Need space for the total rainfall calculation in cc's.
    float runningTotalWaterQuantityCC = 0;      // This is the running total water accumulated in Milliliters.
    
};


/*******************************************************************************************************************
*                           This struct is for the TFT touchscreen points touched.
* This struct is 4 bytes total.
*******************************************************************************************************************/
// Touch screen struct.
struct touchedPoints
{
    
    int16_t xPosition;
    int16_t yPosition;
    
};

/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
///                                             Constants
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/

///             Debug constants.

//#define DEBUG       true             // Turn on/off wait for usb monitor, and verify setup completes.
//#define DEBUG_PRINT 0               // Set SD card format Debug option to 1 to see more info.
//#define DEBUG1    true              // Turn on/off main functions debug messages.
//#define DEBUG2    true              // Turn on/off Wind Speed interrupt debugging.
//#define DEBUG3    true             // Turn on/off Wind Direction interrupt messages.
//#define DEBUG4    true               // Keyboard input debugging.
//#define DEBUG5    true               // Set keyboard time debugging.
//#define DEBUG6    true               // Show Free RAM Memory.
//#define DEBUG7    true               // Set Run 1 Frequency debugging.
//#define DEBUG8    true               // Set Run Multi Frequencies debugging.
//#define DEBUG9    true              // Set DS3231 Chip debugging.
//#define DEBUG10   true            // Set GPS Chip debugging.
//#define DEBUG11   true            // Set TFT debugging.
//#define DEBUG12   true              // Set Touch Controller debugging.
//#define DEBUG13   true              // Set GPS serial buffer data dump to screen.
//#define DEBUG14   true              // Set debug data printout for atmosphereic pressure debugging.
//#define DEBUG15   true            // Set debug data printout for atmosphereic pressure debugging.
//#define DEBUG16   true            // Set debug data printout for Rainfall sensor debugging.
//#define DEBUG17   true           // Set the debug for UartEvent debugging, transfering data struct from one Teensy to another.
//#define DEBUG18   true           // Set the debug flag for Rain Gauge functions.
//#define DEBUG19   true                // Set the debug flag for the RA8875 Touch Screen Preferences section.
//#define DEBUG20   true                // Set the debug flag for UartEvent ISR's.
//#define DEBUG21   true                // Set the debug flag for UartEvent Send Data To Slave from the Master over the Uart.
//#define DEBUG22   true                // Set the debug flag for UartEvent recieve data from Master from the Uart.
//#define DEBUG23   true                // Set debug for crcCheck from the FastCRC libarary.
//#define DEBUG24   true                // Set debug for crcCreate from the FastCRC libarary.
//#define DEBUG25   true                // Set Main Loop Master/Slave Messages sending and recieving debugging.
//#define DEBUG26   true                // Set Main Loop Master/Slave Messages sending and recieving, Print the incoming buffer debugging.
//#define DEBUG27   true                // Set Debug for GPS ublox debugging..
//#define DEBUG28   true                // Set Debugging for GPS updateTimeFromGPS Function.
//#define DEBUG29   true                // Set Debugging for GPS updateTimeFromGPS Function showing raw data directly from GPS Chip via Serial Port 1.
//#define DEBUG30   true                // Set Debugging for Set Time and Time Zone Subs.
//#define DEBUG31   true                // Set Search for One Wire Bus DS18B20 Temp Sensor(s) true.
//#define DEBUG32   true                // Set Change the ID Numbers of the One Wire Bus Temp Sensor(s) True.
//#define DEBUG33   true                // Set Temp Sensors on the One Wire Bus Debugging True.
//#define DEBUG34   true                // Set Debugging for Temperature Calibration Offset.
//#define DEBUG35   true                // Set Debugging for the wait For Another Touch Function.
//#define DEBUG36   true                // Set Debugging for the 12 button Numeric Keypad Function.
//#define DEBUG37   true                // Set Debugging tft Print Keypad AnswerFunction.
//#define DEBUG38   true                // Set Debugging TFT get Number From Numeric Keypad Function.
//#define DEBUG39   true                // Set Debugging set Master Yearly Rain Offset Function.
//#define DEBUG40   true                // Set Debugging Reading Startup Values from EEPROM.
//#define DEBUG41   true                // Set Debugging GPS Startup functions in setup routine.
//#define DEBUG42   true                // Set Debugging for the BME280 Sensor Raad Function.
//#define DEBUG43   true                // Set Debugging for the Process Data From Slave Function.
//#define DEBUG44   true                // Set Debugging for the Set Day of the Week Function.
//#define DEBUG45   true                // Set Debugging for the Rain Preferences Function.
//#define DEBUG46   true                // Set Debugging for the WatchDog Setup function.
//#define DEBUG47     true              // Set Debugging for the  GPS status at the end of Loop
//#define DEBUG48     true              // Show the GPS receive buffer Data Contents and return.
//#define DEBUG49     true              // Show the GPS restart info.
//#define DEBUG50     true              // Disply the actual Data coming from the GPS in the restart Subroutine.
//#define DEBUG51     true              // Disply the size of the tcr abbrev in the drawPrintTime/// Subroutine.

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


/*******************************************************************************************************************
* These definitions are for the TFTM101 in (X = 1024, Y = 600 mode which has 128 Columns with fontscale 0, 64
* columns with fontscale 1, and 32 columns with fontscale 2.
* It has 37.5 rows with fontscale 0, 18.75 rows with fontscale 1, and 9.375 rows with fontscale 2)  when the display
* is in the Landscape mode.
*******************************************************************************************************************/
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


#define windSpeedPin 2              // pin3 (Yellow wire from Peet Bros anomometer).
#define windSpeedInterrupt 2        // INT1

#define windDirPin 3                // pin 2 (Green wire from Peet Bros anomometer).
#define windDirInterrupt 3          // INT0

#define windResetPin 4              // This pin is an output to reset the LS221 one-shots which are connected to the Peet Bros Anemometer.

#define rainSensorPin 5             // This pin is an input from the rain sensor, momentary contact to Gnd.
#define rainSensorPinInterrupt 5    // This pin is an input from the rain sensor, momentary contact to Gnd.

/*******************************************************************************************************************
 * These definitions are for the SerialOut ports 2 & 3 which have an RS485 full duplex driver/reciever.
 *
 *******************************************************************************************************************/
#define useRS_485                       // We are going to use the RS-485 transmit/recieve hardware.

#define serial2RecieveEnablePin 33      // RS-485 input port enable pin attached to Serial2 Reciever (Low Active).
#define serial2TransmitEnablePin 32     // RS-485 output port enable pin attached to Serial2 transmitter (High Active).
#define serial2RxPin 26                 // RS-485 input port data pin attached to Serial2 reciever.
#define serial2TxPin 31                 // RS-485 output port data pin attached to Serial2 transmitter.

#define serial3RecieveEnablePin 9       // RS-485 input port enable pin attached to Serial2 Reciever (Low Active).
#define serial3TransmitEnablePin 10     // RS-485 output port enable pin attached to Serial2 transmitter (High Active).
#define serial3RxPin 7                  // RS-485 input port data pin attached to Serial2 reciever.
#define serial3TxPin 8                  // RS-485 output port data pin attached to Serial2 transmitter.


/*******************************************************************************************************************
 * Setting the sync provider to use for the Teensy.
 *
 *******************************************************************************************************************/
//#define timeSyncProviderTeensy
//#define timeSyncProviderGPS
#define timeSyncProviderDS3231
//#define timeSyncProviderInternet

/*******************************************************************************************************************
 * These definitions are for the TFTM08 in (X = 800, Y = 480) mode which has 50 Columns, and 15 rows of text when
 * the display is in the Landscape mode.
 * Font size is 1 or 16 wide x 32 high pixels.
 *******************************************************************************************************************/

/*******************************************************************************************************************
 * These definitions are for 50 Rows of text when the display is in the Landscape mode. (X = 800, Y = 480).
 * Font size is 1 or 16 wide x 32 high pixels.
 *******************************************************************************************************************/
#define XLEFT    0  // Left side of screen.

#define XCOL1 0
#define XCOL2  Xsize
#define XCOL3  (2 * Xsize)
#define XCOL4  (3 * Xsize)
#define XCOL5  (4 * Xsize)
#define XCOL6  (5 * Xsize)
#define XCOL7  (6 * Xsize)
#define XCOL8  (7 * Xsize)
#define XCOL9  (8 * Xsize)
#define XCOL10  (9 * Xsize)
#define XCOL11  (10 * Xsize)
#define XCOL12  (11 * Xsize)
#define XCOL13  (12 * Xsize)
#define XCOL14  (13 * Xsize)
#define XCOL15  (14 * Xsize)
#define XCOL16  (15 * Xsize)
#define XCOL17  (16 * Xsize)
#define XCOL18  (17 * Xsize)
#define XCOL19  (18 * Xsize)
#define XCOL20  (19 * Xsize)
#define XCOL21  (20 * Xsize)
#define XCOL22  (21 * Xsize)
#define XCOL23  (22 * Xsize)
#define XCOL24  (23 * Xsize)

#define XMIDDLE 399  // Middle of the screen horizonal.

#define XCOL25  (24 * Xsize)
#define XCOL26  (25 * Xsize)
#define XCOL27  (26 * Xsize)
#define XCOL28  (27 * Xsize)
#define XCOL29  (28 * Xsize)
#define XCOL30  (29 * Xsize)
#define XCOL31  (30 * Xsize)
#define XCOL32  (31 * Xsize)
#define XCOL33  (32 * Xsize)
#define XCOL34  (33 * Xsize)
#define XCOL35  (34 * Xsize)
#define XCOL36  (35 * Xsize)
#define XCOL37  (36 * Xsize)
#define XCOL38  (37 * Xsize)
#define XCOL39  (38 * Xsize)
#define XCOL40  (39 * Xsize)
#define XCOL41  (40 * Xsize)
#define XCOL42  (41 * Xsize)
#define XCOL43  (42 * Xsize)
#define XCOL44  (43 * Xsize)
#define XCOL45  (44 * Xsize)
#define XCOL46  (45 * Xsize)
#define XCOL47  (46 * Xsize)
#define XCOL48  (47 * Xsize)
#define XCOL49  (48 * Xsize)
#define XCOL50  (49 * Xsize)

#define XRIGHT 799  // Extreme right of screen.

/*******************************************************************************************************************
 * These definitions are for 15 Rows of text when the display is in the Landscape mode. (X = 800, Y = 480).
 * Font size is 1 or 16 wide x 32 high pixels.
 *******************************************************************************************************************/
#define YTOP 0  // Top of screen.

#define YLINE1 0
#define YLINE2  Ysize
#define YLINE3  (2 * Ysize)
#define YLINE4  (3 * Ysize)
#define YLINE5  (4 * Ysize)
#define YLINE6  (5 * Ysize)
#define YLINE7  (6 * Ysize)

#define YMIDDLE 239  // Middle of the screen vertically
#define YLINE8  (7 * Ysize)
#define YLINE9  (8 * Ysize)
#define YLINE10  (9 * Ysize)
#define YLINE11  (10 * Ysize)
#define YLINE12  (11 * Ysize)
#define YLINE13  (12 * Ysize)
#define YLINE14  (13 * Ysize)
#define YLINE15  (14 * Ysize)

#define YBOTTOM 479  // Bottom row of screen.

/*******************************************************************************************************************
 * These definitions are for the Button Class.
 *******************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 3 defines are for the 2 button Panel.
/// There are 2 Buttons.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set Width, Height, and round rectangle radius of the buttons.
#define TwoButtonPanel_Width ( 20 * Xsize )    // Allow room for 20 characters wide.
#define TwoButtonPanel_Height (6 * Ysize)      //  Allow room for 6 characters high.
#define TwoButtonPanel_radius 8                // The radius of the rounded rectangle.

// Set location for the first button
#define TwoButtonPanel1st_XPosition ( ( XMIDDLE - (X_CONST / 4) ) - ( TwoButtonPanel_Width / 2 ) )
#define TwoButtonPanel1st_YPosition ( ( YMIDDLE - (Y_CONST / 4) ) - ( TwoButtonPanel_Height / 2 ) )

// Set location for the second button
#define TwoButtonPanel2nd_XPosition ( ( XMIDDLE + (X_CONST / 2) ) - ( TwoButtonPanel_Width / 2 ) )
#define TwoButtonPanel2nd_YPosition ( ( YMIDDLE - (Y_CONST / 2) ) - ( TwoButtonPanel_Height / 2 ) )

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 5 defines are for the 4 button Panel.
/// There are 2 rows of 2 columns of Buttons.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set Width, Height, and round rectangle radius of the buttons.
#define FourButtonPanel_Width ( 20 * Xsize )    // Allow room for 20 characters wide.
#define FourButtonPanel_Height (6 * Ysize)      //  Allow room for 6 characters high.
#define FourButtonPanel_radius 8                // The radius of the rounded rectangle.

// Set location for first button.
#define FourButtonPanel1st_XPosition ( ( XMIDDLE - (X_CONST / 4) ) - ( FourButtonPanel_Width / 2 ) )
#define FourButtonPanel1st_YPosition ( ( YMIDDLE - (Y_CONST / 4) ) - ( FourButtonPanel_Height / 2 ) )

// Set Next location for the second button
#define FourButtonPanel2nd_XPosition ( ( XMIDDLE + (X_CONST / 4) ) - ( FourButtonPanel_Width / 2 ) )
#define FourButtonPanel2nd_YPosition ( ( YMIDDLE - (Y_CONST / 4) ) - ( FourButtonPanel_Height / 2 ) )

// Set Next location for the third button
#define FourButtonPanel3rd_XPosition ( ( XMIDDLE - (X_CONST / 4) ) - ( FourButtonPanel_Width / 2 ) )
#define FourButtonPanel3rd_YPosition ( ( YMIDDLE + (Y_CONST / 4) ) - ( FourButtonPanel_Height / 2 ) )

// Set Next location for the fourth button
#define FourButtonPanel4th_XPosition ( ( XMIDDLE + (X_CONST / 4) ) - ( FourButtonPanel_Width / 2 ) )
#define FourButtonPanel4th_YPosition ( ( YMIDDLE + (Y_CONST / 4) ) - ( FourButtonPanel_Height / 2 ) )


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 7 defines are for the 6 button Panel.
/// There are 2 rows of 3 columns of Buttons.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set Width, Height, and round rectangle radius of the buttons.
#define SixButtonPanel_Width ( 12 * Xsize )    // Allow room for 12 characters wide.
#define SixButtonPanel_Height (6 * Ysize)      //  Allow room for 6 characters high.
#define SixButtonPanel_radius 8                // The radius of the rounded rectangle.

// Set location for first button.
#define SixButtonPanel1st_XPosition ( ( XMIDDLE - (X_CONST / 4) ) - ( SixButtonPanel_Width / 2 ) )
#define SixButtonPanel1st_YPosition ( ( YMIDDLE - (Y_CONST / 4) ) - ( SixButtonPanel_Height / 2 ) )

// Set Next location for the second button.
#define SixButtonPanel2nd_XPosition ( ( XMIDDLE - ( SixButtonPanel_Width / 2 ) ) )
#define SixButtonPanel2nd_YPosition ( ( YMIDDLE - (Y_CONST / 4) ) - ( SixButtonPanel_Height / 2 ) )

// Set Next location for the third button.
#define SixButtonPanel3rd_XPosition ( ( XMIDDLE + (X_CONST / 4) ) - ( SixButtonPanel_Width / 2 ) )
#define SixButtonPanel3rd_YPosition ( ( YMIDDLE - (Y_CONST / 4) ) - ( SixButtonPanel_Height / 2 ) )

// Set Next location for the forth button.
#define SixButtonPanel4th_XPosition ( (XMIDDLE - (X_CONST / 4) ) - ( SixButtonPanel_Width / 2 ) )
#define SixButtonPanel4th_YPosition ( ( YMIDDLE + (Y_CONST / 4) ) - ( SixButtonPanel_Height / 2 ) )

// Set Next location for the fifth button.
#define SixButtonPanel5th_XPosition ( ( XMIDDLE - ( SixButtonPanel_Width / 2 ) ) )
#define SixButtonPanel5th_YPosition ( ( YMIDDLE + (Y_CONST / 4) ) - ( SixButtonPanel_Height / 2 ) )

// Set Next location for the sixth button.
#define SixButtonPanel6th_XPosition ( ( XMIDDLE + (X_CONST / 4) ) - ( SixButtonPanel_Width / 2 ) )
#define SixButtonPanel6th_YPosition ( ( YMIDDLE + (Y_CONST / 4) ) - ( SixButtonPanel_Height / 2 ) )


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 9 defines are for the 8 button Panel.
/// There are 2 rows of 4 columns of Buttons.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set Width, Height, and round rectangle radius of the buttons.
#define EightButtonPanel_Width ( 10 * Xsize )    // Allow room for 10 characters wide.
#define EightButtonPanel_Height (6 * Ysize)      //  Allow room for 6 characters high.
#define EightButtonPanel_radius 8                // The radius of the rounded rectangle.

// This is the first row.
// Set location for first button.
#define EightButtonPanel1st_XPosition ( ( XMIDDLE - ( (X_CONST / 8) * 3 ) ) - ( EightButtonPanel_Width / 2 ) )
#define EightButtonPanel1st_YPosition ( ( YMIDDLE - (Y_CONST / 4) ) - ( EightButtonPanel_Height / 2 ) )

// Set Next location for the second button.
#define EightButtonPanel2nd_XPosition ( ( XMIDDLE - ( X_CONST / 8) ) - ( EightButtonPanel_Width / 2 ) )
#define EightButtonPanel2nd_YPosition ( ( YMIDDLE - (Y_CONST / 4) ) - ( EightButtonPanel_Height / 2 ) )

// Set Next location for the third button.
#define EightButtonPanel3rd_XPosition ( ( XMIDDLE + (X_CONST / 8) ) - ( EightButtonPanel_Width / 2 ) )
#define EightButtonPanel3rd_YPosition ( ( YMIDDLE - (Y_CONST / 4) ) - ( EightButtonPanel_Height / 2 ) )

// Set Next location for the forth button.
#define EightButtonPanel4th_XPosition ( (XMIDDLE + ( (X_CONST / 8) * 3 ) ) - ( EightButtonPanel_Width / 2 ) )
#define EightButtonPanel4th_YPosition ( ( YMIDDLE - (Y_CONST / 4) ) - ( EightButtonPanel_Height / 2 ) )

// This is the second row.
// Set Next location for the fifth button.
#define EightButtonPanel5th_XPosition ( ( XMIDDLE - ( (X_CONST / 8) * 3 ) ) - ( EightButtonPanel_Width / 2 ) )
#define EightButtonPanel5th_YPosition ( ( YMIDDLE + (Y_CONST / 4) ) - ( EightButtonPanel_Height / 2 ) )

// Set Next location for the sixth button.
#define EightButtonPanel6th_XPosition ( ( XMIDDLE - (X_CONST / 8) ) - ( EightButtonPanel_Width / 2 ) )
#define EightButtonPanel6th_YPosition ( ( YMIDDLE + (Y_CONST / 4) ) - ( EightButtonPanel_Height / 2 ) )

// Set Next location for the seventh button.
#define EightButtonPanel7th_XPosition ( ( XMIDDLE + (X_CONST / 8) ) - ( EightButtonPanel_Width / 2 ) )
#define EightButtonPanel7th_YPosition ( ( YMIDDLE + (Y_CONST / 4) ) - ( EightButtonPanel_Height / 2 ) )

// Set Next location for the eight button.
#define EightButtonPanel8th_XPosition ( ( XMIDDLE + ( (X_CONST / 8) * 3 ) ) - ( EightButtonPanel_Width / 2 ) )
#define EightButtonPanel8th_YPosition ( ( YMIDDLE + (Y_CONST / 4) ) - ( EightButtonPanel_Height / 2 ) )


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 13 defines are for the 12 button Panel.
/// There are 3 rows of 4 columns of Buttons.
/// Xsize is 16 pixels for the 800 x 480 tft screen.
/// Ysize is 32 pixels for the 800 x 480 tft screen.
/// Xmiddle is 399 for the 800 x 480 tft screen.
/// Ymiddle is 239 for the 800 x 480 tft screen.
/// X_const is 800 for the 800 x 480 tft screen.
/// Y_const is 480 for the 800 x 480 tft screen.
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set Width of buttons, Number of Columns, and spacing between buttons Horizontally.
#define TwelveBtPnl_NumberOfColumns ( 4 )     // This is the number of columns Horizontally.
#define TwelveBtPnl_Width ( 10 * Xsize )      // Allow room for 10 characters wide (or 160 pixels).
#define TwelveBtPnl_Width_Spacing ( ( X_CONST -  ( TwelveBtPnl_Width * TwelveBtPnl_NumberOfColumns ) ) / ( TwelveBtPnl_NumberOfColumns + 1 ) )

// Set Height of buttons, Number of Rows, and spacing between the buttons vertically.
#define TwelveBtPnl_NumberOfRows ( 3 )        // This is the number of rows vertically.
#define TwelveBtPnl_Height ( 4 * Ysize )      //  Allow room for 4 characters high (or 128 pixels).
#define TwelveBtPnl_Height_Spacing ( ( Y_CONST - ( TwelveBtPnl_Height * TwelveBtPnl_NumberOfRows ) ) / ( TwelveBtPnl_NumberOfRows + 1 ) )

// Set round rectangle radius of the buttons.
#define TwelveBtPnl_radius 8                  // The radius of the rounded rectangle.

// This is the first Row of three.
// Set location for first button.
#define TwelveBtPnl1st_XPosition ( XLEFT + TwelveBtPnl_Width_Spacing )
#define TwelveBtPnl1st_YPosition ( YTOP + TwelveBtPnl_Height_Spacing )


// Set Next location for the second button.
#define TwelveBtPnl2nd_XPosition ( XLEFT + ( ( TwelveBtPnl_Width_Spacing * 2 ) + TwelveBtPnl_Width ) )
#define TwelveBtPnl2nd_YPosition ( YTOP + TwelveBtPnl_Height_Spacing )


// Set Next location for the third button.
#define TwelveBtPnl3rd_XPosition ( XLEFT + ( ( TwelveBtPnl_Width_Spacing * 3 ) + ( TwelveBtPnl_Width * 2 ) ) )
#define TwelveBtPnl3rd_YPosition ( YTOP + TwelveBtPnl_Height_Spacing )


// Set Next location for the forth button.
#define TwelveBtPnl4th_XPosition ( XLEFT + ( XLEFT + ( ( TwelveBtPnl_Width_Spacing * 4 ) + ( TwelveBtPnl_Width * 3 ) ) ) )
#define TwelveBtPnl4th_YPosition ( YTOP + TwelveBtPnl_Height_Spacing )


// This is the Second Row of three.
// Set Next location for the fifth button.
#define TwelveBtPnl5th_XPosition ( XLEFT + TwelveBtPnl_Width_Spacing )
#define TwelveBtPnl5th_YPosition ( YTOP + ( ( TwelveBtPnl_Height_Spacing * 2 ) + TwelveBtPnl_Height ) )


// Set Next location for the sixth button.
#define TwelveBtPnl6th_XPosition ( XLEFT + ( ( TwelveBtPnl_Width_Spacing * 2 ) + TwelveBtPnl_Width ) )
#define TwelveBtPnl6th_YPosition ( YTOP + ( ( TwelveBtPnl_Height_Spacing * 2 ) + TwelveBtPnl_Height ) )


// Set Next location for the seventh button.
#define TwelveBtPnl7th_XPosition ( XLEFT + ( ( TwelveBtPnl_Width_Spacing * 3 ) + ( TwelveBtPnl_Width * 2 ) ) )
#define TwelveBtPnl7th_YPosition ( YTOP + ( ( TwelveBtPnl_Height_Spacing * 2 ) + TwelveBtPnl_Height ) )


// Set Next location for the eight button.
#define TwelveBtPnl8th_XPosition ( XLEFT + ( XLEFT + ( ( TwelveBtPnl_Width_Spacing * 4 ) + ( TwelveBtPnl_Width * 3 ) ) ) )
#define TwelveBtPnl8th_YPosition ( YTOP + ( ( TwelveBtPnl_Height_Spacing * 2 ) + TwelveBtPnl_Height ) )


// This is the Third Row of three.
// Set Next location for the ninth button.
#define TwelveBtPnl9th_XPosition ( XLEFT + TwelveBtPnl_Width_Spacing )
#define TwelveBtPnl9th_YPosition ( YTOP + ( ( TwelveBtPnl_Height_Spacing * 3 ) + ( TwelveBtPnl_Height * 2 ) ) )


// Set Next location for the tenthbutton.
#define TwelveBtPnl10th_XPosition ( XLEFT + ( ( TwelveBtPnl_Width_Spacing * 2 ) + TwelveBtPnl_Width ) )
#define TwelveBtPnl10th_YPosition ( YTOP + ( ( TwelveBtPnl_Height_Spacing * 3 ) + ( TwelveBtPnl_Height * 2 ) ) )


// Set Next location for the eleventh button.
#define TwelveBtPnl11th_XPosition ( XLEFT + ( ( TwelveBtPnl_Width_Spacing * 3 ) + ( TwelveBtPnl_Width * 2 ) ) )
#define TwelveBtPnl11th_YPosition ( YTOP + ( ( TwelveBtPnl_Height_Spacing * 3 ) + ( TwelveBtPnl_Height * 2 ) ) )


// Set Next location for the twelveth button.
#define TwelveBtPnl12th_XPosition ( XLEFT + ( XLEFT + ( ( TwelveBtPnl_Width_Spacing * 4 ) + ( TwelveBtPnl_Width * 3 ) ) ) )
#define TwelveBtPnl12th_YPosition ( YTOP + ( ( TwelveBtPnl_Height_Spacing * 3 ) + ( TwelveBtPnl_Height * 2 ) ) )


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// These next 15 defines are for the 14 button Numeric Keypad Panel.
///   ____________________
///   | 8 | 9 | blank | C |
///   --------------------
///   | 4 | 5 |   6   | 7 |
///   ---------------------
///   | 0 | 1 |   2   | 3 |
///   ---------------------
///   | Enter  |    .     |
///   ---------------------
/// There are 4 rows of 4 columns of Buttons.
////////////////////////////////////////////////////////////////////////////////////////////////////////


/// Set location of the Message filled Round Rectangle.
#define NumericKeypadMessage_XPosition ( XLEFT )
#define NumericKeypadMessage_YPosition ( YTOP )
#define NumericKeypadMessage_Width ( 24 * Xsize )
#define NumericKeypadMessage_Height (2 * Ysize)
#define NumericKeypadMessage_radius 8

// Set Width, Height, and round rectangle radius of the buttons, this button will be square 64 x 64 pixels.
#define NumericKeypad_Width ( 6 * Xsize )    // Allow room for 3 characters wide.
#define NumericKeypad_Height (3 * Ysize)      //  Allow room for 1 characters high.
#define NumericKeypad_ButtonSpacing_X ( ( ( X_CONST / 2 ) - ( NumericKeypad_Width * 4 ) ) / 3 )   // Allow 48 pixels between the buttons horizontally.
#define NumericKeypad_ButtonSpacing_Y ( ( Y_CONST - ( NumericKeypad_Height * 4 ) ) / 5 )   // Allow 48 pixels between the buttons horizontally.
#define NumericKeypad_radius 8                // The radius of the rounded rectangle.

// Set Width, Height of round rectangle radius of the button, this button will be rectangle 128w x 64h pixels.
#define NumericKeypad13th_Width ( ( NumericKeypad_Width * 3 ) )    // This button is 3 buttons wide.
#define NumericKeypad13th_Height (3 * Ysize)      //  Allow room for 1 characters high.


// Set location for first button.
#define NumericKeypad1st_XPosition ( XMIDDLE)
#define NumericKeypad1st_YPosition ( YTOP + NumericKeypad_ButtonSpacing_Y )

// Set Next location for the second button.
#define NumericKeypad2nd_XPosition ( ( XMIDDLE + NumericKeypad_ButtonSpacing_X ) + ( NumericKeypad_Width ) )
#define NumericKeypad2nd_YPosition ( YTOP + NumericKeypad_ButtonSpacing_Y )

// Set Next location for the third button.
#define NumericKeypad3rd_XPosition ( ( XMIDDLE + ( NumericKeypad_ButtonSpacing_X * 2 ) ) + ( NumericKeypad_Width * 2 ) )
#define NumericKeypad3rd_YPosition ( YTOP + NumericKeypad_ButtonSpacing_Y )

// Set Next location for the forth button.
#define NumericKeypad4th_XPosition ( ( XMIDDLE + ( NumericKeypad_ButtonSpacing_X * 3 ) ) + ( NumericKeypad_Width * 3 ) )
#define NumericKeypad4th_YPosition ( YTOP + NumericKeypad_ButtonSpacing_Y )

// Set Next location for the fifth button.
#define NumericKeypad5th_XPosition ( XMIDDLE)
#define NumericKeypad5th_YPosition ( YTOP + ( NumericKeypad_ButtonSpacing_Y * 2 ) + ( NumericKeypad_Height ) )

// Set Next location for the sixth button.
#define NumericKeypad6th_XPosition ( ( XMIDDLE + NumericKeypad_ButtonSpacing_X ) + ( NumericKeypad_Width ) )
#define NumericKeypad6th_YPosition ( YTOP + ( NumericKeypad_ButtonSpacing_Y * 2 ) + ( NumericKeypad_Height ) )

// Set Next location for the seventh button.
#define NumericKeypad7th_XPosition ( ( XMIDDLE + ( NumericKeypad_ButtonSpacing_X * 2 ) ) + ( NumericKeypad_Width * 2 ) )
#define NumericKeypad7th_YPosition ( YTOP + ( NumericKeypad_ButtonSpacing_Y * 2 ) + ( NumericKeypad_Height ) )

// Set Next location for the eight button.
#define NumericKeypad8th_XPosition ( ( XMIDDLE + ( NumericKeypad_ButtonSpacing_X * 3 ) ) + ( NumericKeypad_Width * 3 ) )
#define NumericKeypad8th_YPosition ( YTOP + ( NumericKeypad_ButtonSpacing_Y * 2 ) + ( NumericKeypad_Height ) )

// Set Next location for the ninth button.
#define NumericKeypad9th_XPosition ( XMIDDLE)
#define NumericKeypad9th_YPosition ( YTOP + ( NumericKeypad_ButtonSpacing_Y * 3 ) + ( NumericKeypad_Height * 2 )  )

// Set Next location for the tenth button.
#define NumericKeypad10th_XPosition ( ( XMIDDLE + NumericKeypad_ButtonSpacing_X ) + ( NumericKeypad_Width ) )
#define NumericKeypad10th_YPosition ( YTOP + ( NumericKeypad_ButtonSpacing_Y * 3 ) + ( NumericKeypad_Height * 2 )  )

// Set Next location for the eleventh button.
#define NumericKeypad11th_XPosition ( ( XMIDDLE + ( NumericKeypad_ButtonSpacing_X * 2 ) ) + ( NumericKeypad_Width * 2 ) )
#define NumericKeypad11th_YPosition ( YTOP + ( NumericKeypad_ButtonSpacing_Y * 3 ) + ( NumericKeypad_Height * 2 )  )

// Set Next location for the twelveth button.
#define NumericKeypad12th_XPosition ( ( XMIDDLE + ( NumericKeypad_ButtonSpacing_X * 3 ) ) + ( NumericKeypad_Width * 3 ) )
#define NumericKeypad12th_YPosition ( YTOP + ( NumericKeypad_ButtonSpacing_Y * 3 ) + ( NumericKeypad_Height * 2 )  )

// Set Next location for the Thirteenth button, this is the enter button.
#define NumericKeypad13th_XPosition ( XMIDDLE + NumericKeypad_ButtonSpacing_X )
#define NumericKeypad13th_YPosition ( YTOP + ( NumericKeypad_ButtonSpacing_Y * 4 ) + ( NumericKeypad_Height * 3 )   )

// Set Next location for the Fourteenth button, this is the "." decimal point button.
#define NumericKeypad14th_XPosition ( ( XMIDDLE + ( NumericKeypad_ButtonSpacing_X * 3 ) ) + ( NumericKeypad_Width * 3 ) )
#define NumericKeypad14th_YPosition ( YTOP + ( NumericKeypad_ButtonSpacing_Y * 4 ) + ( NumericKeypad_Height * 3 )   )

/// Set location of the Answer filled Round Rectangle.
#define NumericKeypadAnswer_XPosition ( XMIDDLE - (Xsize * 20) )
#define NumericKeypadAnswer_YPosition ( YMIDDLE - Ysize )
#define NumericKeypadAnswer_Width ( 15 * Xsize )
#define NumericKeypadAnswer_Height (2 * Ysize)
#define NumericKeypadAnswer_radius 8


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
///                                             Project/System dependent defines
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/

/**********************************************************************************************************************
* CPU Restart macro
*
**********************************************************************************************************************/
#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

/**********************************************************************************************************************
* MIN/MAX/ABS macros
*
**********************************************************************************************************************/
#define MIN(a,b)            ((a<b)?(a):(b))
#define MAX(a,b)            ((a>b)?(a):(b))
#define ABS(x)                ((x>0)?(x):(-x))


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
///                                             UART Configurations
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
///                                             Rprintf Defines
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/

#define RPRINTF_COMPLEX

#define RPRINTF_FLOAT


/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/
///                                             Variables
/*ΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩΩ*/


//Do not add code below this line


#endif
