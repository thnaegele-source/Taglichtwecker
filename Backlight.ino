#include "Globals.h"

void handleSideButtons() {
  bool currLeft = digitalRead(TOUCH_LEFT);
  bool currRight = digitalRead(TOUCH_RIGHT);
  
  // Bei 160 LEDs/Meter und 5m = 800 LEDs
  // 20cm dunkel = 32 LEDs (160 * 0.20)
  // 30cm Fade = 48 LEDs (160 * 0.30)
  int darkZoneLEDs = 32;    // Die inneren 20cm bleiben aus
  int fadeZoneLEDs = 48;     // 30cm sanfter Übergang
  
  // Linker Touch-Sensor
  if (currLeft == LOW && lastLeftState == HIGH) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(TOUCH_LEFT) == LOW) {
      leftStripOn = !leftStripOn;
      
      if (leftStripOn) {
        for (int i = 0; i < LEDS_PER_SIDE; i++) {
          int distanceFromCenter = LEDS_PER_SIDE - i;  // Abstand zur Mitte
          
          if (distanceFromCenter <= darkZoneLEDs) {
            // Innere 20cm: komplett dunkel
            strip.setPixelColor(i, 0);
          } else if (distanceFromCenter <= darkZoneLEDs + fadeZoneLEDs) {
            // Fade-Zone: sanft heller werden nach außen
            int fadePos = distanceFromCenter - darkZoneLEDs;
            float fadeFactor = (float)fadePos / fadeZoneLEDs;
            
            uint8_t r = (uint8_t)(255 * fadeFactor);
            uint8_t g = (uint8_t)(147 * fadeFactor);
            uint8_t b = (uint8_t)(41 * fadeFactor);
            
            strip.setPixelColor(i, strip.Color(r, g, b));
          } else {
            // Äußerer Bereich: volle Helligkeit
            strip.setPixelColor(i, WARM_WHITE);
          }
        }
        strip.setBrightness(60);  // Etwas heller als vorher
        strip.show();
        Serial.println("Linkes Licht AN (20cm dunkel, Fade nach außen)");
      } else {
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
      
      if (rightStripOn) {
        for (int i = LEDS_PER_SIDE; i < NUM_LEDS; i++) {
          int distanceFromCenter = i - LEDS_PER_SIDE;  // Abstand zur Mitte
          
          if (distanceFromCenter <= darkZoneLEDs) {
            // Innere 20cm: komplett dunkel
            strip.setPixelColor(i, 0);
          } else if (distanceFromCenter <= darkZoneLEDs + fadeZoneLEDs) {
            // Fade-Zone: sanft heller werden nach außen
            int fadePos = distanceFromCenter - darkZoneLEDs;
            float fadeFactor = (float)fadePos / fadeZoneLEDs;
            
            uint8_t r = (uint8_t)(255 * fadeFactor);
            uint8_t g = (uint8_t)(147 * fadeFactor);
            uint8_t b = (uint8_t)(41 * fadeFactor);
            
            strip.setPixelColor(i, strip.Color(r, g, b));
          } else {
            // Äußerer Bereich: volle Helligkeit
            strip.setPixelColor(i, WARM_WHITE);
          }
        }
        strip.setBrightness(60);  // Etwas heller als vorher
        strip.show();
        Serial.println("Rechtes Licht AN (20cm dunkel, Fade nach außen)");
      } else {
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
