/*
AP33772S.h - Header file for the AP33772S USB-C PD 3.1 Sink Controller Arduino Library.

Version: 1.0.0
(c) 2024 CentyLab
www.centylab.com

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __AP33772S__
#define __AP33772S__

#if ARDUINO >= 100
#include "Arduino.h"
#include "Wire.h"
#else
#include "WProgram.h"
#endif

#define MAX_PDO_ENTRIES 13  // Define the maximum number of PDO entries you expect

#define AP33772S_ADDRESS 0x52
#define READ_BUFF_LENGTH 128
#define WRITE_BUFF_LENGTH 6
#define SRCPDO_LENGTH 28

#define CMD_STATUS    0x01 //Reset to 0 after very Read
#define CMD_MASK      0x02
#define CMD_OPMODE    0x03
#define CMD_CONFIG    0x04
#define CMD_PDCONFIG  0x05
#define CMD_SYSTEM    0x06
// Temperature setting register
#define CMD_TR25     0x0C 
#define CMD_TR50     0x0D 
#define CMD_TR75     0x0E
#define CMD_TR100    0x0F

//Power reading related
#define CMD_VOLTAGE   0x11
#define CMD_CURRENT   0x12
#define CMD_TEMP      0x13
#define CMD_VREQ      0x14
#define CMD_IREQ      0x15


#define CMD_VSELMIN   0x16 //Minimum Selection Voltage
#define CMD_UVPTHR    0x17
#define CMD_OVPTHR    0x18
#define CMD_OCPTHR    0x19
#define CMD_OTPTHR    0x1A
#define CMD_DRTHR     0x1B

#define CMD_SRCPDO    0x20

#define CMD_PD_REQMSG 0x31
#define CMD_PD_CMDMSG 0x32
#define CMD_PD_MSGRLT 0x33

//Timer for AVS reminder signal
#define ALARM_NUM1 1 // Timer 1
#define ALARM_IRQ1 TIMER_IRQ_1
#define DELAY1 500000 // In usecond , 0.5s

typedef enum
{
  STARTED_MSK   = 1 << 0,     // 0000 0001
  READY_MSK     = 1 << 1,     // 0000 0010
  NEWPDO_MSK    = 1 << 2,     // 0000 0100
  UVP_MSK       = 1 << 3,     // 0001 0000
  OVP_MSK       = 1 << 4,     // 0010 0000
  OCP_MSK       = 1 << 5,     // 0100 0000
  OTP_MSK       = 1 << 6      // 1000 0000
} AP33772_MASK;

typedef struct
{
  union
  {
    struct
    {
      byte newNegoSuccess : 1;
      byte newNegoFail : 1;
      byte negoSuccess : 1;
      byte negoFail : 1;
      byte reserved_1 : 4;
    };
    byte negoEvent;
  };
  union
  {
    struct
    {
      byte ovp : 1;
      byte ocp : 1;
      byte otp : 1;
      byte dr : 1;
      byte reserved_2 : 4;
    };
    byte protectEvent;
  };
} EVENT_FLAG_T;

//DONE
typedef struct {
  union {
    struct {
      unsigned int voltage_max: 8;   // Bits 7:0, VOLTAGE_MAX field
      unsigned int peak_current: 2;  // Bits 9:8, PEAK_CURRENT field
      unsigned int current_max: 4;   // Bits 13:10, CURRENT_MAX field
      unsigned int type: 1;          // Bit 14, TYPE field
      unsigned int detect: 1;        // Bit 15, DETECT field
    } fixed;
    struct {
      unsigned int voltage_max: 8;   // Bits 7:0, VOLTAGE_MAX field
      unsigned int voltage_min: 2;   // Bits 9:8, VOLTAGE_MIN field
      unsigned int current_max: 4;   // Bits 13:10, CURRENT_MAX field
      unsigned int type: 1;          // Bit 14, TYPE field
      unsigned int detect: 1;        // Bit 15, DETECT field
    } pps;
  struct {
      unsigned int voltage_max: 8;   // Bits 7:0, VOLTAGE_MAX field
      unsigned int voltage_min: 2;   // Bits 9:8, VOLTAGE_MIN field
      unsigned int current_max: 4;   // Bits 13:10, CURRENT_MAX field
      unsigned int type: 1;          // Bit 14, TYPE field
      unsigned int detect: 1;        // Bit 15, DETECT field
    } avs;
  struct {
      byte byte0;
      byte byte1;
  };
  };
  unsigned long data;
} SRC_SPRandEPR_PDO_Fields;

//DONE
typedef struct {
  union {
    struct {
      unsigned int VOLTAGE_SEL: 8;  // Bits 7:0, Output Voltage Select
      unsigned int CURRENT_SEL: 4;  // Bits 11:8, Operating Current Select
      unsigned int PDO_INDEX: 4;  // Bits 15:12, Source PDO index select
      
    } REQMSG_Fields;
    struct {
      byte byte0;
      byte byte1;
    };
  unsigned long data;
  };
} RDO_DATA_T;

class AP33772S
{
public:
  AP33772S(TwoWire &wire = Wire);
  void begin();
  void displayPDOInfo(int pdoIndex);
  void displayProfiles();
  void mapPPSAVSInfo();
  void setFixPDO(int pdoIndex, int max_current);
  void setPPSPDO(int pdoIndex, int target_voltage, int max_current);
  void setAVSPDO(int pdoIndex, int target_voltage, int max_current);
  // void setVoltage(int targetVoltage); // Unit in mV
  void setNTC(int TR25, int TR50, int TR75, int TR100);
  bool setOutput(uint8_t flag);
  // void setMask(AP33772_MASK flag);
  // void clearMask(AP33772_MASK flag);

  // Monitor functions
  int readTemp();
  int readVoltage();
  int readCurrent();

  // Adjustment functions
  int readVREQ();
  int readIREQ();
  int readVSELMIN();
  void setVSELMIN(int voltage);
  int readUVPTHR();
  void setUVPTHR(int value);
  int readOVPTHR();
  void setOVPTHR(int value);
  int readOCPTHR();
  void setOCPTHR(int value);
  int readOTPTHR();
  void setOTPTHR(int value);
  int readDRTHR();
  void setDRTHR(int value);

  // Advance function to handle in other lib

  int getNumPDO();
  int getPPSIndex();
  int getAVSIndex();
  
  byte existPPS = 0; // PPS flag for setVoltage()
  byte existAVS = 0; // AVS flag for setVoltage()

private:
  static void i2c_read(byte slvAddr, byte cmdAddr, byte len);
  static void i2c_write(byte slvAddr, byte cmdAddr, byte len);
  TwoWire *_i2cPort = &Wire;
  static byte readBuf[READ_BUFF_LENGTH];
  static byte writeBuf[WRITE_BUFF_LENGTH];

  int _indexPPSUser = -1; // for getPPSIndex();
  int _indexAVSUser = -1; // for getAVSIndex();

  EVENT_FLAG_T event_flag = {0};
  RDO_DATA_T rdoData = {0};

  //Use for timer;
  static int _voltageAVSbyte;
  static int _currentAVSbyte;
  static int _indexAVS;

  //static void timerISR1();
  //void setupAVSTimer();
  //void cancelAVSTimer();

  SRC_SPRandEPR_PDO_Fields SRC_SPRandEPRpdoArray[MAX_PDO_ENTRIES] = {0}; 

  //Helper functions
  void displaySPRVoltageMin(unsigned int current_max);
  void displayEPRVoltageMin(unsigned int current_max);
  void displayCurrentRange(unsigned int current_max);
  int currentMap(int current);

};



#endif
