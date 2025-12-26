#include "Globals.h"

void handleTouchInput(DateTime now) {
  static bool touching = false;
  static int startX = 0;
  static int startY = 0;
  static int lastX = 0;
  static int lastY = 0;

  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    if (p.z >= TOUCH_THRESHOLD) {
      lastTouchTime = millis();
      lastScreenTime = millis();

      int currentX = map(p.x, touchMinX, touchMaxX, 0, 320);
      int currentY = map(p.y, touchMinY, touchMaxY, 0, 240);

      lastX = currentX;
      lastY = currentY;

      if (!touching) {
        touching = true;
        startX = currentX;
        startY = currentY;
      }
    }
  } else {
    if (touching) {
      touching = false;

      int endX = lastX;
      int endY = lastY;

      int deltaX = endX - startX;
      int deltaY = endY - startY;

      // Swipe für Navigation
      if (abs(deltaY) < 100 && abs(deltaX) > 60) {
        if (currentScreen == SCREEN_MAIN) {
          if (deltaX < 0) {
            currentScreen = SCREEN_MENU_LIST;
            drawMenuScreen();
          }
        } else if (currentScreen == SCREEN_MENU_LIST) {
          if (deltaX > 0) {
            currentScreen = SCREEN_MAIN;
            drawMainScreen();
          }
        } else {
          if (deltaX > 0) {
            currentScreen = SCREEN_MENU_LIST;
            drawMenuScreen();
          }
        }
        return;
      }

      // Tap für Buttons (kleine Bewegung = Antippen)
      if (abs(deltaX) < 60 && abs(deltaY) < 60) {
        // LICHT Button (Main Screen)
        if (currentScreen == SCREEN_MAIN) {
          if (startX >= 5 && startX <= 115 && startY >= 170 && startY <= 240) {
            // Toggle-Funktion: An/Aus
            if (lightOn || sunriseRunning) {
              // Licht ausschalten
              lightOn = false;
              sunriseRunning = false;
              strip.setBrightness(0);
              strip.clear();
              strip.show();
              Serial.println("Licht AUS");
            } else {
              // Licht einschalten mit Sonnenaufgang
              lightOn = true;
              fastSunrise = false;
              sunriseStartTime = millis();
              sunriseRunning = true;
              strip.setBrightness(1);
              strip.clear();
              strip.show();
              Serial.println("Sonnenaufgang START");
            }
            drawLightButton();
            return;
          }
        }

        // Menüauswahl – die 5 Buttons
        if (currentScreen == SCREEN_MENU_LIST) {
          for (int i = 0; i < 5; i++) {
            int yTop = 40 + i * 50 - 30;
            int yBottom = yTop + 40;

            if (startX >= 20 && startX <= 300 && startY >= yTop && startY <= yBottom) {
              switch (i) {
                case 0: currentScreen = SCREEN_TIME; drawTimeScreen(); break;
                case 1: currentScreen = SCREEN_DATE; drawDateScreen(); break;
                case 2: currentScreen = SCREEN_ALARM; drawAlarmScreen(); break;
                case 3: currentScreen = SCREEN_SUNRISE; drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness); break;
                case 4: currentScreen = SCREEN_DISPLAY; drawDisplayScreen(); break;
              }
              return;
            }
          }
        }

        // Sunrise-Menü – Tap für + / -
        if (currentScreen == SCREEN_SUNRISE) {
          if (startX >= 200 && startX <= 270 && startY >= 50 && startY <= 100) { // Dauer +
            if (alarmSettings.sunriseDuration + 1 > 60) alarmSettings.sunriseDuration = 60;
            else alarmSettings.sunriseDuration += 1;
            drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness);
            return;
          }
          if (startX >= 260 && startX <= 330 && startY >= 50 && startY <= 100) { // Dauer -
            if (alarmSettings.sunriseDuration - 1 < 1) alarmSettings.sunriseDuration = 1;
            else alarmSettings.sunriseDuration -= 1;
            drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness);
            return;
          }
          if (startX >= 200 && startX <= 270 && startY >= 110 && startY <= 160) { // Helligkeit +
            if (alarmSettings.maxBrightness + 5 > 100) alarmSettings.maxBrightness = 100;
            else alarmSettings.maxBrightness += 5;
            drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness);
            return;
          }
          if (startX >= 260 && startX <= 330 && startY >= 110 && startY <= 160) { // Helligkeit -
            if (alarmSettings.maxBrightness - 5 < 10) alarmSettings.maxBrightness = 10;
            else alarmSettings.maxBrightness -= 5;
            drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness);
            return;
          }
        }

        // Uhrzeit-Menü – Tap für + / -
        if (currentScreen == SCREEN_TIME) {
          DateTime n = rtc.now();
          if (startX >= 210 && startX <= 260 && startY >= 35 && startY <= 75) { // Stunde +
            rtc.adjust(DateTime(n.year(), n.month(), n.day(), (n.hour() + 1) % 24, n.minute(), n.second()));
            drawTimeScreen();
            return;
          }
          if (startX >= 270 && startX <= 320 && startY >= 35 && startY <= 75) { // Stunde -
            rtc.adjust(DateTime(n.year(), n.month(), n.day(), (n.hour() + 23) % 24, n.minute(), n.second()));
            drawTimeScreen();
            return;
          }
          if (startX >= 210 && startX <= 260 && startY >= 115 && startY <= 155) { // Minute +
            rtc.adjust(DateTime(n.year(), n.month(), n.day(), n.hour(), (n.minute() + 1) % 60, n.second()));
            drawTimeScreen();
            return;
          }
          if (startX >= 270 && startX <= 320 && startY >= 115 && startY <= 155) { // Minute -
            rtc.adjust(DateTime(n.year(), n.month(), n.day(), n.hour(), (n.minute() + 59) % 60, n.second()));
            drawTimeScreen();
            return;
          }
        }

        // Datum-Menü – Tap für + / -
        if (currentScreen == SCREEN_DATE) {
          DateTime n = rtc.now();
          if (startX >= 210 && startX <= 260 && startY >= 55 && startY <= 95) { // Tag +
            rtc.adjust(DateTime(n.year(), n.month(), n.day() + 1, n.hour(), n.minute(), n.second()));
            drawDateScreen();
            return;
          }
          if (startX >= 270 && startX <= 320 && startY >= 55 && startY <= 95) { // Tag -
            rtc.adjust(DateTime(n.year(), n.month(), n.day() - 1, n.hour(), n.minute(), n.second()));
            drawDateScreen();
            return;
          }
          if (startX >= 210 && startX <= 260 && startY >= 115 && startY <= 155) { // Monat +
            int newMonth = n.month() + 1;
            if (newMonth > 12) newMonth = 1;
            rtc.adjust(DateTime(n.year(), newMonth, n.day(), n.hour(), n.minute(), n.second()));
            drawDateScreen();
            return;
          }
          if (startX >= 270 && startX <= 320 && startY >= 115 && startY <= 155) { // Monat -
            int newMonth = n.month() - 1;
            if (newMonth < 1) newMonth = 12;
            rtc.adjust(DateTime(n.year(), newMonth, n.day(), n.hour(), n.minute(), n.second()));
            drawDateScreen();
            return;
          }
          if (startX >= 210 && startX <= 260 && startY >= 175 && startY <= 215) { // Jahr +
            rtc.adjust(DateTime(n.year() + 1, n.month(), n.day(), n.hour(), n.minute(), n.second()));
            drawDateScreen();
            return;
          }
          if (startX >= 270 && startX <= 320 && startY >= 175 && startY <= 215) { // Jahr -
            rtc.adjust(DateTime(n.year() - 1, n.month(), n.day(), n.hour(), n.minute(), n.second()));
            drawDateScreen();
            return;
          }
        }

        // Wecker-Menü – Tap für + / -
        if (currentScreen == SCREEN_ALARM) {
          // Stunde +
          if (startX >= 200 && startX <= 250 && startY >= 35 && startY <= 75) {
            alarmSettings.hour = (alarmSettings.hour + 1) % 24;
            drawAlarmScreen();
            return;
          }
          // Stunde -
          if (startX >= 260 && startX <= 310 && startY >= 35 && startY <= 75) {
            alarmSettings.hour = (alarmSettings.hour + 23) % 24;
            drawAlarmScreen();
            return;
          }
          // Minute +
          if (startX >= 200 && startX <= 250 && startY >= 85 && startY <= 125) {
            alarmSettings.minute = (alarmSettings.minute + 1) % 60;
            drawAlarmScreen();
            return;
          }
          // Minute -
          if (startX >= 260 && startX <= 310 && startY >= 85 && startY <= 125) {
            alarmSettings.minute = (alarmSettings.minute + 59) % 60;
            drawAlarmScreen();
            return;
          }
          // Wochentage (Mo–So)
          for (int i = 0; i < 7; i++) {
            int xLeft = 25 + i * 38;
            int xRight = xLeft + 30;
            if (startX >= xLeft && startX <= xRight && startY >= 165 && startY <= 195) {
              alarmSettings.days[i] = !alarmSettings.days[i];
              drawAlarmScreen();
              return;
            }
          }
          // AN/AUS-Button
          if (startX >= 80 && startX <= 220 && startY >= 200 && startY <= 240) {
            alarmSettings.active = !alarmSettings.active;
            drawAlarmScreen();
            return;
          }
        }

        // Display-Menü – Tap für + / -
        if (currentScreen == SCREEN_DISPLAY) {
          if (startX >= 200 && startX <= 250 && startY >= 55 && startY <= 95) { // Dunkel ab +
            displaySettings.darkHour = (displaySettings.darkHour + 1) % 24;
            drawDisplayScreen();
            return;
          }
          if (startX >= 260 && startX <= 310 && startY >= 55 && startY <= 95) { // Dunkel ab -
            displaySettings.darkHour = (displaySettings.darkHour + 23) % 24;
            drawDisplayScreen();
            return;
          }
          if (startX >= 200 && startX <= 250 && startY >= 105 && startY <= 145) { // Hell fuer +
            displaySettings.brightDurationIndex = (displaySettings.brightDurationIndex + 1) % (sizeof(brightOptions)/sizeof(brightOptions[0]));
            drawDisplayScreen();
            return;
          }
          if (startX >= 260 && startX <= 310 && startY >= 105 && startY <= 145) { // Hell fuer -
            displaySettings.brightDurationIndex = (displaySettings.brightDurationIndex + (sizeof(brightOptions)/sizeof(brightOptions[0])) - 1) % (sizeof(brightOptions)/sizeof(brightOptions[0]));
            drawDisplayScreen();
            return;
          }
          if (startX >= 200 && startX <= 250 && startY >= 155 && startY <= 195) { // Hell ab Stunde +
            displaySettings.alwaysOnHour = (displaySettings.alwaysOnHour + 1) % 24;
            drawDisplayScreen();
            return;
          }
          if (startX >= 260 && startX <= 310 && startY >= 155 && startY <= 195) { // Hell ab Stunde -
            displaySettings.alwaysOnHour = (displaySettings.alwaysOnHour + 23) % 24;
            drawDisplayScreen();
            return;
          }
        }
      }
    }
  }
}
