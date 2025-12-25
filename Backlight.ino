#include "Globals.h"

void handleSideButtons() {
  bool currLeft = digitalRead(TOUCH_LEFT);
  bool currRight = digitalRead(TOUCH_RIGHT);

  if (currLeft == LOW && lastLeftState == HIGH) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(TOUCH_LEFT) == LOW) {
      leftStripOn = !leftStripOn;
      // updateSideStrips(); entfernt – nicht mehr nötig
      // Der Sunrise-Verlauf übernimmt alles
    }
  }

  if (currRight == LOW && lastRightState == HIGH) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(TOUCH_RIGHT) == LOW) {
      rightStripOn = !rightStripOn;
      // updateSideStrips(); entfernt – nicht mehr nötig
    }
  }

  lastLeftState = currLeft;
  lastRightState = currRight;
}

static bool backlightWasOn = true;

void handleBacklight(DateTime now) {
  int curMin = now.hour() * 60 + now.minute();
  int darkMin = displaySettings.darkHour * 60 + displaySettings.darkMinute;
  int alwaysMin = displaySettings.alwaysOnHour * 60 + displaySettings.alwaysOnMinute;
  int brightSec = brightOptions[displaySettings.brightDurationIndex];
  bool shouldOn = (curMin < darkMin) || (curMin >= alwaysMin) || (millis() - lastTouchTime < brightSec * 1000L);

  if (shouldOn != backlightWasOn) {
    backlightWasOn = shouldOn;
    digitalWrite(TFT_BL, shouldOn ? HIGH : LOW);

    if (shouldOn) {
      // Beim Einschalten: Bildschirm neu zeichnen
      if (currentScreen == SCREEN_MAIN) {
        drawMainScreen();
      } else if (currentScreen == SCREEN_MENU_LIST) {
        drawMenuScreen();
      }
      // Für andere Bildschirme später erweitern
    }
  }
}

void checkAlarmTrigger(DateTime now) {
  if (alarmSettings.active && alarmSettings.days[now.dayOfTheWeek()] && !fastSunrise) {
    int wakeMin = alarmSettings.minute - alarmSettings.vorher;
    int wakeHr = alarmSettings.hour;
    if (wakeMin < 0) { wakeMin += 60; wakeHr = (wakeHr - 1 + 24) % 24; }
    int diff = (now.hour() * 60 + now.minute()) - (wakeHr * 60 + wakeMin);
    if (diff >= 0 && diff <= 1 && !sunriseRunning) {
      lightOn = true;
      fastSunrise = false;
      sunriseStartTime = millis();
      sunriseRunning = true;
      drawLightButton();
    }
  }
}
