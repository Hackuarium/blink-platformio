#include <NilRTOS.h>

#include "SerialUtilities.h"
#include "params.h"

// code taken from
// https://github.com/Hackuarium/simple-spectro/tree/master/arduino/SimpleSpectro.
// Thread allowing serial over usb communication.

#ifdef THR_SERIAL

#define SERIAL_BUFFER_LENGTH 32
#define SERIAL_MAX_PARAM_VALUE_LENGTH 32
char serialBuffer[SERIAL_BUFFER_LENGTH];
uint8_t serialBufferPosition = 0;

void printResult(char* data, Print* output);
void printHelp(Print* output);
void processSpecificCommand(char* data, char* paramValue, Print* output);
void printSpecificHelp(Print* output);

NIL_WORKING_AREA(waThreadSerial, 96);  // minimum 96
NIL_THREAD(ThreadSerial, arg) {
  Serial.begin(9600);
  while (true) {
    while (Serial.available()) {
      // get the new byte:
      char inChar = (char)Serial.read();

      if (inChar == 13 || inChar == 10) {
        // this is a carriage return;
        if (serialBufferPosition > 0) {
          printResult(serialBuffer, &Serial);
        }
        serialBufferPosition = 0;
        serialBuffer[0] = '\0';
      } else {
        if (serialBufferPosition < SERIAL_BUFFER_LENGTH) {
          serialBuffer[serialBufferPosition] = inChar;
          serialBufferPosition++;
          if (serialBufferPosition < SERIAL_BUFFER_LENGTH) {
            serialBuffer[serialBufferPosition] = '\0';
          }
        }
      }
    }
    nilThdSleepMilliseconds(1);
  }
}

void printResult(char* data, Print* output);

void printHelp(Print* output);

void noThread(Print* output);

#endif