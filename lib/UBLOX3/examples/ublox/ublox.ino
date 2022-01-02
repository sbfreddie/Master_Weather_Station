/*
  UBLOX_example3.ino - CFG-TP5 (0x06 0x31) Time Pulse Parameters
  Brian R Taylor
  brian.taylor@bolderflight.com
  2016-07-06

  Copyright (c) 2016 Bolder Flight Systems

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
  and associated documentation files (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge, publish, distribute,
  sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or
  substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
  v1.0.3
  Chris O.
  2018-03-29
  Supported Navigation IDs:
  UBX-NAV-PVT ---- (0x01 0x07) Navigation Position Velocity Time Solution
  UBX-NAV-POSLLH - (0x01 0x02) Geodetic Position Solution
  UBX-NAV-ATT ---- (0x01 0x05) Attitude Solution

  /---  High level Commands, for the user ---/
   NOTE: command(bool) == (true)Print Message Acknowledged on USB Serial Monitor
  begin(Baud)                       // Starting communication with the GPS
  end();                            // Disables Teensy serial communication, to re-enable, call begin(Baud)
  Poll_CFG_Port1(bool);             // Polls the configuration for one I/O Port, I/O Target 0x01=UART1
  Poll_NAV_PVT();                   // Polls UBX-NAV-PVT    (0x01 0x07) Navigation Position Velocity Time Solution
  Poll_NAV_POSLLH();                // Polls UBX-NAV-POSLLH (0x01 0x02) Geodetic Position Solution
  Poll_NAV_ATT();                   // Polls UBX-NAV-ATT    (0x01 0x05) Attitude Solution
  Poll_MON_IO(bool);                // Polls UBX-MON-IO (0x0A 0x02) I/O Subsystem Status
  Poll_MON_VER(bool);               // Polls UBX-MON-VER (0x0A 0x04) Receiver/Software Version
  Poll_CFG_TP5(bool);               // Polls CFG-TP5        (0x06 0x31) Poll Time Pulse 0 Parameters
  ### Periodic Auto Update ON,OFF Command ###
  Ena_NAV_PVT(bool);                // Enable periodic auto update NAV_PVT
  Dis_NAV_PVT(bool);                // Disable periodic auto update NAV_PVT
  Ena_NAV_ATT(bool);                // Enable periodic auto update NAV_ATT
  Dis_NAV_ATT(bool);                // Disable periodic auto update NAV_ATT
  Ena_NAV_POSLLH(bool);             // Enable periodic auto update NAV_POSLLH
  Dis_NAV_POSLLH(bool);             // Disable periodic auto update NAV_POSLLH
  ### u-blox Switch off all NMEA MSGs ###
  Dis_all_NMEA_Child_MSGs(bool);    // Disable All NMEA Child Messages Command
  ### High level Command Generator ###
  SetGPSbaud(uint32_t baud, bool)   // Set UBLOX GPS Port Configuration Baud rate
  SetNAV5(uint8_t dynModel, bool)   // Set Dynamic platform model Navigation Engine Settings (0:portable, 3:pedestrian, Etc)
  SetRATE(uint16_t measRate, bool)  // Set Navigation/Measurement Rate Settings (100ms=10.00Hz, 200ms=5.00Hz, 1000ms=1.00Hz, Etc)
  SetCFG_TP5(uint32_t FreqLocked, double DutyLocked, uint16_t antCableDelay, bool printACK); // UBX-CFG-TP5 (0x06 0x31) Time Pulse 0 Parameters
  Ena_Dis_MON_IO(bool En~Di, bool) // UBX-MON-IO (0x0A 0x02) Ena/Dis periodic auto update I/O Subsystem Status, bytes(received, sent), parity , framing , overrun)
*/

#include "ublox3.h"

// The elapsedMillis feature is built into Teensyduino.
// For non-Teensy boards, it is available as a library.
elapsedMillis sinceMSG_poll;

// Set this to the GPS hardware serial port you wish to use
#define GPShwSERIAL 1 // 1 = Serial1, 2 = Serial2, 3 = Serial3, 4 = Serial4 ....
uint32_t const BaudDefault = 115200; // default settings

// a uBlox object, which is on Teensy hardware
// GPS serial port
UBLOX gps(GPShwSERIAL);

// the uBlox data structure
gpsData uBloxData;

void setup() {
  // serial to display data
  Serial.begin(115200); // Teensy Serial object always communicates at 12 Mbit/sec USB speed.
  while ( !Serial && (millis() < 10000)) ; // wait until serial monitor is open or timeout 10 seconds

  // -- AutoBauding test --
  // Try communication with the GPS
  // receiver at 9600 baud, default settings
  // then set GPS UART Baud to 460800
  gps.begin(BaudDefault);                   // Enable Teensy serial communication @ 9600 baud, default settings.
  gps.SetGPSbaud(BaudDefault, false);           // Set GPS Port Baud, Possible Baud Rate Configurations 4800~9600~19200~38400~57600~115200~230400~460800-921600
  gps.end();                                // Disables Teensy serial communication, to re-enable serial communication, call gps.begin(Baud, bool);.
  gps.begin(19200);
  gps.SetGPSbaud(BaudDefault, false);
  gps.end();
  gps.begin(38400);
  gps.SetGPSbaud(BaudDefault, false);
  gps.end();
  gps.begin(57600);
  gps.SetGPSbaud(BaudDefault, false);
  gps.end();
  gps.begin(115200);
  gps.SetGPSbaud(BaudDefault, false);
  gps.end();
  gps.begin(230400);
  gps.SetGPSbaud(BaudDefault, false);
  gps.end();
  gps.begin(460800);
  gps.SetGPSbaud(BaudDefault, false);
  gps.end();
  gps.begin(921600);
  gps.SetGPSbaud(BaudDefault, true); // 1843200 // experimental baud rate 1500000
  gps.end();
  // now start communication with the GPS
  // receiver at 1500000 baud,
  gps.begin(BaudDefault);                       // Enable Teensy serial communication @ given baud rate

  //gps.SetRATE(1000, false);                 // Navigation/Measurement Rate Settings, e.g. 100ms => 10Hz, 200 => 5.00Hz, 1000ms => 1Hz, 10000ms => 0.1Hz
  // Possible Configurations:
  // 60=>16.67Hz, 64=>15.63Hz, 72=>13.89Hz, 80=>12.50Hz, 100=>10.00Hz, 125=>8.00Hz, 200=>5.00Hz, 250=>4.00Hz, 500=>2.00Hz
  // 800=>1.25Hz, 1000=>1.00Hz, 2000=>0.50Hz, 4000=>0.25Hz, 10000=>0.10Hz, 20000=>0.05Hz, 50000=>0.02Hz

  // NOTE: Dis_all_NMEA -strongly suggest changing RX buffer to 255 or more,*otherwise you will miss ACKs*on serial monitor
  gps.Dis_all_NMEA_Child_MSGs(false);       // Disable All NMEA Child Messages Command

  gps.SetNAV5(3, false);                    // Set Dynamic platform model Navigation Engine Settings (0:portable, 2: stationary, 3:pedestrian, Etc)
  // Possible Configurations
  // 0: portable, 2: stationary, 3: pedestrian, 4: automotive, 5: sea, 6: airborne with <1g, 7: airborne with <2g
  // 8: airborne with <4g, 9: wrist worn watch (not supported in protocol v.less than 18)

  // ### Periodic auto update ON,OFF Command ###
  gps.Ena_NAV_PVT(true);                    // Enable periodic auto update NAV_PVT
  //gps.Dis_NAV_PVT(false);                 // Disable periodic auto update NAV_PVT

  //gps.Ena_NAV_ATT(true);                  // Enable periodic auto update NAV_ATT ~ U-blox M8 from protocol version 19
  gps.Dis_NAV_ATT(false);                   // Disable periodic auto update NAV_ATT ~ ---^

  //gps.Ena_NAV_POSLLH(true);               // Enable periodic auto update NAV_POSLLH
  gps.Dis_NAV_POSLLH(false);                // Disable periodic auto update NAV_POSLLH

  gps.Ena_Dis_MON_IO(true, true);           // Ena/Dis periodic auto update I/O Subsystem Status, bytes(received, sent), parity errors, framing errors, overrun errors)

  gps.Poll_MON_IO(false);                   // Polls UBX-MON-IO (0x0A 0x02) I/O Subsystem Status
  gps.Poll_MON_VER(false);                  // Polls UBX-MON-VER (0x0A 0x04) Receiver/Software Version

  // A UBlox GPS Module Primer for beginners https://forum.pjrc.com/threads/46058-A-UBlox-GPS-Module-Primer-for-beginners
  //gps.SetCFG_TP5(2000, 50.0000, 50, true);  // UBX-CFG-TP5 (0x06 0x31) - Set Time Pulse 0 Parameters.
  // Possible Configurations:
  // SetCFG_TP5(FreqLocked- 1Hz ~ 24000000Hz, DutyLocked- 0.000000% ~ 100.000000%, antCableDelay- 0~32767ns, print usb ACK- true or false);

  //gps.Poll_CFG_TP5(false);                  // Polls CFG-TP5        (0x06 0x31) Poll Time Pulse 0 Parameters
}

void loop() {
  // put your main code here, to run repeatedly:
  // The elapsedMillis feature is built into Teensyduino.
  // For non-Teensy boards, it is available as a library.
  // #### Polling Mechanism ####
  if (sinceMSG_poll >= 10000) { // SEND poll message request every 10 sec.
    sinceMSG_poll = 0; // reset since
    gps.Poll_NAV_PVT();
    //gps.Poll_CFG_TP5(true);                 // Polls CFG-TP5        (0x06 0x31) Poll Time Pulse 0 Parameters
  }

  if (gps.read(&uBloxData) ) {
    sinceMSG_poll = 0; // reset since

    Serial.print(uBloxData.utcYear);  ///< [year], Year (UTC)
    Serial.print("\t");
    Serial.print(uBloxData.utcMonth); ///< [month], Month, range 1..12 (UTC)
    Serial.print("\t");
    Serial.print(uBloxData.utcDay);   ///< [day], Day of month, range 1..31 (UTC)
    Serial.print("\t");
    Serial.print(uBloxData.utcHour);  ///< [hour], Hour of day, range 0..23 (UTC)
    Serial.print("\t");
    Serial.print(uBloxData.utcMin);   ///< [min], Minute of hour, range 0..59 (UTC)
    Serial.print("\t");
    Serial.print(uBloxData.utcSec);   ///< [s], Seconds of minute, range 0..60 (UTC)
    Serial.print("\t");
    Serial.print(uBloxData.numSV);    ///< [ND], Number of satellites used in Nav Solution
    Serial.print("\t");
    Serial.print(uBloxData.lat,10);   ///< [deg], Latitude
    Serial.print("\t");
    Serial.print(uBloxData.lon,10);   ///< [deg], Longitude
    Serial.print("\t");
    Serial.println(uBloxData.hMSL);   ///< [m], Height above mean sea level

  } else {
    if (sinceMSG_poll == 5000) {
      sinceMSG_poll = 5001; // reset since
      gps.Poll_MON_IO(false);
    }
  }
}
