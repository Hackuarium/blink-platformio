
#ifdef THR_MONITORING

NIL_WORKING_AREA(waThreadMonitoring, 0);
NIL_THREAD(ThreadMonitoring, arg) {
  nilThdSleepMilliseconds(8000);  // Do not start the watchdog too quickly
  wdt_enable(WDTO_8S);            // activate the watchdog
#ifdef MONITORING_LED
  pinMode(MONITORING_LED, OUTPUT);  // diode pin out
#endif

  while (true) {
#ifdef MONITORING_LED
    digitalWrite(MONITORING_LED, HIGH);
    nilThdSleepMilliseconds(100);
    digitalWrite(MONITORING_LED, LOW);
    nilThdSleepMilliseconds(100);
#endif
    wdt_reset();  // resed avoid the automatic reboot
  }
}

#endif