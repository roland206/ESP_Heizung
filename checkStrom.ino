#include "config.h"

#ifdef STROM_SENSOR_PIN

int waitForState = 1;
uint64_t lastEdgeMillis = 0;
uint64_t rebounceTimer = 0;

int checkStrom(uint64_t now) {

  if (now > rebounceTimer) return false;

  
  if (digitalRead(STROM_SENSOR_PIN) == waitForState) {
    stromCounts += (waitForState) ? 1 : 0;
    rebounceTimer = now + 50;
    waitForState = !waitForState;
    if (waitForState) {
      lastPeriod = (long)(now - lastEdgeMillis) / 1000;
      lastEdgeMillis = now;
    }
  }

  return false;
}
#endif
