#include<WireMaster.h>
#include<config.h>
#include <NilRTOS.h>

#ifdef THR_WIRE_MASTER

NIL_WORKING_AREA(waThreadWireMaster, 200);
NIL_THREAD(ThreadWireMaster, arg);


#endif