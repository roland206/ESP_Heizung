#include "config.h"
#ifdef BUTTON_PIN

static int lastButtonState;
static uint64_t rebTimer = 0;
int checkButton(uint64_t now) {
  
    int taster     = digitalRead(BUTTON_PIN);
    if (lastButtonState == taster) rebTimer = now + 50;
    if (now > rebTimer) {
      lastButtonState = taster;
      rebTimer = now + 50;
      if (lastButtonState == PRESSED) {
        LOG("Button Pressed");
        button = PRESSED;
      }
    }
    return (button == PRESSED);
}
#endif
