#include<config.h>
#include<NilRTOS.h>
#include<WireMaster.h>

#ifndef I2C_HARDWARE
#define I2C_HARDWARE 1
#endif

#define I2C_TIMEOUT 10
#define I2C_SLOWMODE 1

#ifndef WIRE_MASTER_HOT_PLUG
#define WIRE_MASTER_HOT_PLUG 1  // scan for new devices preventing sleep mode
// of I2C slaves
#endif

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

#ifdef THR_WIRE_MASTER
#ifdef THR_WIRE_M

NIL_THREAD(ThreadWireMaster, arg) {

  nilThdSleepMilliseconds(1000);

  extern Serial_ Serial;

  unsigned int wireEventStatus = 0;

  Wire.begin();

  while (true) {
    while (!Serial); // Leonardo: wait for serial monitor
    Serial.println("\nI2C Scanner");

#ifdef WIRE_MASTER_HOT_PLUG
    Serial.println("\nI2C");
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
#endif