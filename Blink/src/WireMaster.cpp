#include <NilRTOS.h>
#include "config.h"
//#include "WireMaster.h"
#include "params.h"

#ifdef THR_WIRE_MASTER

#ifndef I2C_HARDWARE
#define I2C_HARDWARE 1
#endif

#define I2C_TIMEOUT 10
#define I2C_SLOWMODE 1

#if I2C_HARDWARE == 1
#include "Wire.h"
//TwoWire WireM = TwoWire();
#else
// WireM.available returns a wrong value with SoftWire when device unplugged
/* 08/24/2020
#include "../SoftI2CMaster/SoftWire.h"
SoftWire WireM = SoftWire();
*/
#endif

#define WIRE_MASTER_HOT_PLUG 1  // scan for new devices preventing sleep mode
// of I2C slaves

#define WIRE_MAX_DEVICES 8

byte numberI2CDevices = 0;
byte wireDeviceID[WIRE_MAX_DEVICES];

/*
int wireReadInt(uint8_t address) {
  nilSemWait(&lockTimeCriticalZone);
  WireM.requestFrom(address, (uint8_t)2);
  if (WireM.available() != 2) {
    nilSemSignal(&lockTimeCriticalZone);
    return ERROR_VALUE;
  }
  int16_t value = (WireM.read() << 8) | WireM.read();
  nilSemSignal(&lockTimeCriticalZone);
  return value;
}

void wireWakeup(uint8_t address) {
  nilSemWait(&lockTimeCriticalZone);
  WireM.beginTransmission(address);
  WireM.endTransmission(); // Send data to I2C dev with option for a repeated
                           // start
  nilSemSignal(&lockTimeCriticalZone);
}

void wireSetRegister(uint8_t address, uint8_t registerAddress) {
  nilSemWait(&lockTimeCriticalZone);
  WireM.beginTransmission(address);
  WireM.write(registerAddress);
  WireM.endTransmission(); // Send data to I2C dev with option for a repeated
                           // start
  nilSemSignal(&lockTimeCriticalZone);
}


int wireReadIntRegister(uint8_t address, uint8_t registerAddress) {
  wireSetRegister(address, registerAddress);
  return wireReadInt(address);
}


int wireCopyParameter(uint8_t address, uint8_t registerAddress,
  uint8_t parameterID) {
  setParameter(parameterID, wireReadIntRegister(address, registerAddress));
}
*/

void wireWriteIntRegister(uint8_t address, uint8_t registerAddress, 
  int value) {
  // 08/24/2020
  /*
  nilSemWait(&lockTimeCriticalZone);
  WireM.beginTransmission(address);
  WireM.write(registerAddress);
  if (value > 255 || value < 0)
    WireM.write(value >> 8);
  WireM.write(value & 255);
  WireM.endTransmission(); // Send data to I2C dev with option for a repeated
                           // start
  nilSemSignal(&lockTimeCriticalZone);
  */
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  if (value > 255 || value < 0)
    Wire.write(value >> 8);
  Wire.write(value & 255);
  Wire.endTransmission(); // Send data to I2C dev with option for a repeated
                           // startWireM.beginTransmission(address);
  Wire.write(registerAddress);
  if (value > 255 || value < 0)
    Wire.write(value >> 8);
  Wire.write(value & 255);
  Wire.endTransmission(); // Send data to I2C dev with option for a repeated
                           // start
}

void printWireDeviceParameter(Print *output, uint8_t wireID) {
  output->println(F("I2C device: "));
  output->println(wireID);
  for (byte i = 0; i < 26; i++) {
    output->print((char)(i + 65));
    output->print(" : ");
    output->print(i);
    output->print(F(" - "));
    // 08/24/2020
    //output->println(wireReadIntRegister(wireID, i));
  }
}


void wireRemoveDevice(byte id) {
  for (byte i = id; i < numberI2CDevices - 1; i++) {
    wireDeviceID[i] = wireDeviceID[i + 1];
  }
  numberI2CDevices--;
}

void wireInsertDevice(byte id, byte newDevice) {
  // Serial.println(id);

  if (numberI2CDevices < WIRE_MAX_DEVICES) {
    for (byte i = id + 1; i < numberI2CDevices - 1; i++) {
      wireDeviceID[i] = wireDeviceID[i + 1];
    }
    wireDeviceID[id] = newDevice;
    numberI2CDevices++;
  }
}

boolean wireDeviceExists(byte id) {
  for (byte i = 0; i < numberI2CDevices; i++) {
    if (wireDeviceID[i] == id)
      return true;
  }
  return false;
}

void wireUpdateList(Print* output) {
  // 16ms
  
  // 08/24/2020
  //extern Serial_ Serial;

  byte _data;
  byte currentPosition = 0;
  // I2C Module Scan, from_id ... to_id
  for (byte i = 0; i <= 127; i++) {
    // 08/24/2020: Undeclared Semahore 
    //nilSemWait(&lockTimeCriticalZone);
    // 08/24/2020
    //WireM.beginTransmission(i);
    //WireM.write(&_data, 0);
    Wire.beginTransmission(i);
    Wire.write(&_data, 0);
    // I2C Module found out!
    // 08/24/2020
    //if (WireM.endTransmission() == 0) {
    if (Wire.endTransmission() == 0) {

      // 08/24/2020
      Serial.print("I2C device found at address 0x");
      if (i < 16) {
          Serial.print("0");
        }
        Serial.print(i, HEX);
        Serial.println("  !");

      // there is a device, we need to check if we should add or remove a
      // previous device
      if (currentPosition < numberI2CDevices &&
          wireDeviceID[currentPosition] ==
              i) { // it is still the same device that is at the same position,
                   // nothing to do
        currentPosition++;
      } else if (currentPosition < numberI2CDevices &&
                 wireDeviceID[currentPosition] <
                     i) { // some device(s) disappear, we need to delete them
        wireRemoveDevice(currentPosition);
        i--;
      } else if (currentPosition >= numberI2CDevices ||
                 wireDeviceID[currentPosition] > i) { // we need to add a device
        wireInsertDevice(currentPosition, i);
        currentPosition++;
      }
    }
    // 08/24/2020: Undeclared Semaphore 
    //nilSemSignal(&lockTimeCriticalZone);
    nilThdSleepMilliseconds(1);
  }
  while (currentPosition < numberI2CDevices) {
    wireRemoveDevice(currentPosition);
  }
}

void printWireInfo(Print *output) {
  wireUpdateList(output);
  output->println("I2C");

  for (byte i = 0; i < numberI2CDevices; i++) {
    output->print(i);
    output->print(F(": "));
    output->print(wireDeviceID[i]);
    output->print(F(" - "));
    output->println(wireDeviceID[i], BIN);
  }
}

void printWireHelp(Print *output) {
  output->println(F("(il) List devices"));
  output->println(F("(ip) List parameters"));
}

void processWireCommand(char command, char *paramValue,
  Print *output) { // char and char* ??
  switch (command) {
  case 'p':
    if (paramValue[0] == '\0') {
      output->println(F("Missing device ID"));
    } else {
      printWireDeviceParameter(output, atoi(paramValue));
    }
    break;
  case 'l':
    printWireInfo(output);
    break;
  default:
    printWireHelp(output);
  }
}

NIL_THREAD(ThreadWireMaster, arg) {

  nilThdSleepMilliseconds(1000);

  extern Serial_ Serial;

  unsigned int wireEventStatus = 0;

  // 08/24/2020
  //WireM.begin();
  Wire.begin();

  while (true) {
    while (!Serial); // Leonardo: wait for serial monitor
    Serial.println("\nI2C Scanner");

#ifdef WIRE_MASTER_HOT_PLUG
    // allows to log when devices are plugged in / out
    // not suitable for i2c slave sleep mode
    if (wireEventStatus % 25 == 0) {
      wireUpdateList(&Serial);
    }
#endif
    wireEventStatus++;

    nilThdSleepMilliseconds(2000);
  }
}


#endif