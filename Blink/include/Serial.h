//#include <Arduino.h>
#include <NilRTOS.h>
#include "config.h"

//#include "SerialUtilities.h"
//#include "params.h"

// code taken from
// https://github.com/Hackuarium/simple-spectro/tree/master/arduino/SimpleSpectro.
// Thread allowing serial over usb communication.

#ifdef THR_SERIAL

void printResult(char* data, Print* output);
void printHelp(Print* output);
void processSpecificCommand(char* data, char* paramValue, Print* output);
void printSpecificHelp(Print* output);

NIL_WORKING_AREA(waThreadSerial, 96);  // minimum 96
NIL_THREAD(ThreadSerial, arg);

void noThread(Print* output);

#endif