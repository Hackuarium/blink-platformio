#include <Arduino.h>
#include <NilRTOS.h>


#include "eeprom.h"
#include "params.h"

void resetParameters();

void printUtilitiesHelp(Print* output);

void printFreeMemory(Print* output);

void processUtilitiesCommand(char command, char* paramValue, Print* output);
