#include <NilRTOS.h>
#include <avr/wdt.h>
#include <config.h>

#include "Monitoring.cpp"

/****************************************************************
                NilRTOS Multithreading Table
      The higher in the Table The higher the priority
*****************************************************************/
NIL_THREADS_TABLE_BEGIN()

// logger should have priority to prevent any corruption of flash memory

#ifdef THR_MONITORING
NIL_THREADS_TABLE_ENTRY(NULL,
                        ThreadMonitoring,
                        NULL,
                        waThreadMonitoring,
                        sizeof(waThreadMonitoring))
#endif

NIL_THREADS_TABLE_END()