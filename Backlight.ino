#include "Globals.h"

void handleSideButtons() {
  bool currLeft = digitalRead(TOUCH_LEFT);
  bool currRight = digitalRead(TOUCH_RIGHT);
  
  // Linker Touch-Sensor
  if (currLeft == LOW && lastLeftState == HIGH) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(TOUCH_LEFT) == LOW) {
      leftStripOn = !leftStripOn;
      
      // Warmweißes Licht für linke Seite
      if (leftStripOn) {
        for (int i = 0; i < LEDS_PER_SIDE; i++) {
          strip.setPixelColor(i, WARM_WHITE);
        }
        strip.setBrightness(50);  // Nachttischlicht-Helligkeit
        strip.show();
        Serial.println("Linkes Licht AN");
      } else {
        // Nur linke Seite ausschalten
        for (int i = 0; i < LEDS_PER_SIDE; i++) {
          strip.setPixelColor(i, 0);
        }
        strip.show();
        Serial.println("Linkes Licht AUS");
      }
    }
  }
  
  // Rechter Touch-Sensor
  if (currRight == LOW && lastRightState == HIGH) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(TOUCH_RIGHT) == LOW) {
      rightStripOn = !rightStripOn;
      
      // Warmweißes Licht für rechte Seite
      if (rightStripOn) {
        for (int i = LEDS_PER_SIDE; i < NUM_LEDS; i++) {
          strip.setPixelColor(i, WARM_WHITE);
        }
        strip.setBrightness(50);  // Nachttischlicht-Helligkeit
        strip.show();
        Serial.println("Rechtes Licht AN");
      } else {
        // Nur rechte Seite ausschalten
        for (int i = LEDS_PER_SIDE; i < NUM_LEDS; i++) {
          strip.setPixelColor(i, 0);
        }
        strip.show();
        Serial.println("Rechtes Licht AUS");
      }
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
    }
  }
}

void checkAlarmTrigger(DateTime now) {
  static bool alarmTriggeredThisMinute = false;
  static int lastMinute = -1;
  
  // Reset bei Minutenwechsel
  if (now.minute() != lastMinute) {
    alarmTriggeredThisMinute = false;
    lastMinute = now.minute();
  }
  
  if (!alarmSettings.active || alarmTriggeredThisMinute) return;
  
  // Wochentag korrigieren: DS3231 liefert 0=Sonntag, wir wollen 0=Montag
  int adjustedDay = now.dayOfTheWeek() - 1;
  if (adjustedDay < 0) adjustedDay = 6;
  
  if (!alarmSettings.days[adjustedDay]) return;
  
  // Weckzeit mit "vorher" Minuten berechnen
  int wakeMin = alarmSettings.minute - alarmSettings.vorher;
  int wakeHr = alarmSettings.hour;
  if (wakeMin < 0) { 
    wakeMin += 60; 
    wakeHr = (wakeHr - 1 + 24) % 24; 
  }
  
  // Prüfen ob genau jetzt die Weckzeit ist
  if (now.hour() == wakeHr && now.minute() == wakeMin && !sunriseRunning) {
    Serial.printf("WECKER AUSGELOEST! %02d:%02d\n", now.hour(), now.minute());
    lightOn = true;
    fastSunrise = false;
    sunriseStartTime = millis();
    sunriseRunning = true;
    strip.setBrightness(1);
    strip.clear();
    strip.show();
    alarmTriggeredThisMinute = true;
    drawLightButton();
  }
}
