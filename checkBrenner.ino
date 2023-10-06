#include "config.h"

#ifdef GAS_SENSOR_PIN

static int brenner = false;
static int lastSensor = 0;
static uint64_t lastEdgeMillis = 0, rebounceTimer = 0, brennerTimer = 0;


int checkBrenner(uint64_t now) {
  int sensor = digitalRead(GAS_SENSOR_PIN);
  int report = false;
  
  if ((brenner) && (now > brennerTimer)) {
    brenner = false;
    report = true;
  }

  if ((now > rebounceTimer) && (sensor != lastSensor)) {
    rebounceTimer = now + 50;
    if (sensor == 0) {
      if (brenner == false) report = true;
      gasCounts++;
      LOG("Counts incremented to " + String(gasCounts));
      brenner = true;
      brennerTimer = now + 60000;
      lastPeriod = (long)(now - lastEdgeMillis) / 1000;
      lastEdgeMillis = now;
      LOG("Last Period :" + String(lastPeriod));
    }
    lastSensor = sensor;
  }
  return report;
}
#endif
