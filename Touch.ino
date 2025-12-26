#include "Globals.h"

void handleTouchInput(DateTime now) {
  static bool touching = false;
  static int startX = 0;
  static int startY = 0;
  static int lastX = 0;
  static int lastY = 0;
  static unsigned long touchStartTime = 0;
  static unsigned long lastRepeatTime = 0;

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
        touchStartTime = millis();
        lastRepeatTime = millis();
      } else {
        // Auto-Repeat: Nach 500ms Halten, dann alle 150ms wiederholen
        unsigned long holdTime = millis() - touchStartTime;
        if (holdTime > 500 && (millis() - lastRepeatTime) > 150) {
          lastRepeatTime = millis();
          // Trigger Repeat für aktuelle Position
          handleButtonRepeat(startX, startY);
        }
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
        handleButtonPress(startX, startY, now);
      }
    }
  }
}

void handleButtonPress(int x, int y, DateTime now) {
  // LICHT Button (Main Screen)
  if (currentScreen == SCREEN_MAIN) {
    if (x >= 5 && x <= 115 && y >= 170 && y <= 240) {
      if (lightOn || sunriseRunning) {
        lightOn = false;
        sunriseRunning = false;
        strip.setBrightness(0);
        strip.clear();
        strip.show();
        Serial.println("Licht AUS");
      } else {
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

  // Menüauswahl
  if (currentScreen == SCREEN_MENU_LIST) {
    for (int i = 0; i < 5; i++) {
      int yTop = 40 + i * 50 - 30;
      int yBottom = yTop + 40;
      if (x >= 20 && x <= 300 && y >= yTop && y <= yBottom) {
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

  // Display-Menü
  if (currentScreen == SCREEN_DISPLAY) {
    handleDisplayButtons(x, y);
    return;
  }

  // Zeit-Menü
  if (currentScreen == SCREEN_TIME) {
    handleTimeButtons(x, y);
    return;
  }

  // Datum-Menü
  if (currentScreen == SCREEN_DATE) {
    handleDateButtons(x, y);
    return;
  }

  // Wecker-Menü
  if (currentScreen == SCREEN_ALARM) {
    handleAlarmButtons(x, y);
    return;
  }

  // Sunrise-Menü
  if (currentScreen == SCREEN_SUNRISE) {
    handleSunriseButtons(x, y);
    return;
  }
}

void handleButtonRepeat(int x, int y) {
  if (currentScreen == SCREEN_DISPLAY) {
    handleDisplayButtons(x, y);
  } else if (currentScreen == SCREEN_TIME) {
    handleTimeButtons(x, y);
  } else if (currentScreen == SCREEN_DATE) {
    handleDateButtons(x, y);
  } else if (currentScreen == SCREEN_ALARM) {
    handleAlarmButtons(x, y);
  } else if (currentScreen == SCREEN_SUNRISE) {
    handleSunriseButtons(x, y);
  }
}

void handleDisplayButtons(int x, int y) {
  // Dunkel ab - Stunde +
  if (x >= 158 && x <= 208 && y >= 44 && y <= 72) {
    displaySettings.darkHour = (displaySettings.darkHour + 1) % 24;
    drawDisplayScreen();
    return;
  }
  // Dunkel ab - Stunde -
  if (x >= 215 && x <= 265 && y >= 44 && y <= 72) {
    displaySettings.darkHour = (displaySettings.darkHour + 23) % 24;
    drawDisplayScreen();
    return;
  }
  // Dunkel ab - Minute +
  if (x >= 158 && x <= 208 && y >= 75 && y <= 103) {
    displaySettings.darkMinute = (displaySettings.darkMinute + 1) % 60;
    drawDisplayScreen();
    return;
  }
  // Dunkel ab - Minute -
  if (x >= 215 && x <= 265 && y >= 75 && y <= 103) {
    displaySettings.darkMinute = (displaySettings.darkMinute + 59) % 60;
    drawDisplayScreen();
    return;
  }
  // Hell ab - Stunde +
  if (x >= 158 && x <= 208 && y >= 129 && y <= 157) {
    displaySettings.alwaysOnHour = (displaySettings.alwaysOnHour + 1) % 24;
    drawDisplayScreen();
    return;
  }
  // Hell ab - Stunde -
  if (x >= 215 && x <= 265 && y >= 129 && y <= 157) {
    displaySettings.alwaysOnHour = (displaySettings.alwaysOnHour + 23) % 24;
    drawDisplayScreen();
    return;
  }
  // Hell ab - Minute +
  if (x >= 158 && x <= 208 && y >= 160 && y <= 188) {
    displaySettings.alwaysOnMinute = (displaySettings.alwaysOnMinute + 1) % 60;
    drawDisplayScreen();
    return;
  }
  // Hell ab - Minute -
  if (x >= 215 && x <= 265 && y >= 160 && y <= 188) {
    displaySettings.alwaysOnMinute = (displaySettings.alwaysOnMinute + 59) % 60;
    drawDisplayScreen();
    return;
  }
  // Display hell für +
  if (x >= 158 && x <= 208 && y >= 211 && y <= 239) {
    displaySettings.brightDurationIndex = (displaySettings.brightDurationIndex + 1) % 7;
    drawDisplayScreen();
    return;
  }
  // Display hell für -
  if (x >= 215 && x <= 265 && y >= 211 && y <= 239) {
    displaySettings.brightDurationIndex = (displaySettings.brightDurationIndex + 6) % 7;
    drawDisplayScreen();
    return;
  }
}

void handleTimeButtons(int x, int y) {
  DateTime n = rtc.now();
  // Stunde +
  if (x >= 210 && x <= 260 && y >= 35 && y <= 75) {
    rtc.adjust(DateTime(n.year(), n.month(), n.day(), (n.hour() + 1) % 24, n.minute(), n.second()));
    drawTimeScreen();
    return;
  }
  // Stunde -
  if (x >= 270 && x <= 320 && y >= 35 && y <= 75) {
    rtc.adjust(DateTime(n.year(), n.month(), n.day(), (n.hour() + 23) % 24, n.minute(), n.second()));
    drawTimeScreen();
    return;
  }
  // Minute +
  if (x >= 210 && x <= 260 && y >= 115 && y <= 155) {
    rtc.adjust(DateTime(n.year(), n.month(), n.day(), n.hour(), (n.minute() + 1) % 60, n.second()));
    drawTimeScreen();
    return;
  }
  // Minute -
  if (x >= 270 && x <= 320 && y >= 115 && y <= 155) {
    rtc.adjust(DateTime(n.year(), n.month(), n.day(), n.hour(), (n.minute() + 59) % 60, n.second()));
    drawTimeScreen();
    return;
  }
}

void handleDateButtons(int x, int y) {
  DateTime n = rtc.now();
  // Tag +
  if (x >= 210 && x <= 260 && y >= 55 && y <= 95) {
    rtc.adjust(DateTime(n.year(), n.month(), n.day() + 1, n.hour(), n.minute(), n.second()));
    drawDateScreen();
    return;
  }
  // Tag -
  if (x >= 270 && x <= 320 && y >= 55 && y <= 95) {
    rtc.adjust(DateTime(n.year(), n.month(), n.day() - 1, n.hour(), n.minute(), n.second()));
    drawDateScreen();
    return;
  }
  // Monat +
  if (x >= 210 && x <= 260 && y >= 115 && y <= 155) {
    int newMonth = n.month() + 1;
    if (newMonth > 12) newMonth = 1;
    rtc.adjust(DateTime(n.year(), newMonth, n.day(), n.hour(), n.minute(), n.second()));
    drawDateScreen();
    return;
  }
  // Monat -
  if (x >= 270 && x <= 320 && y >= 115 && y <= 155) {
    int newMonth = n.month() - 1;
    if (newMonth < 1) newMonth = 12;
    rtc.adjust(DateTime(n.year(), newMonth, n.day(), n.hour(), n.minute(), n.second()));
    drawDateScreen();
    return;
  }
  // Jahr +
  if (x >= 210 && x <= 260 && y >= 175 && y <= 215) {
    rtc.adjust(DateTime(n.year() + 1, n.month(), n.day(), n.hour(), n.minute(), n.second()));
    drawDateScreen();
    return;
  }
  // Jahr -
  if (x >= 270 && x <= 320 && y >= 175 && y <= 215) {
    rtc.adjust(DateTime(n.year() - 1, n.month(), n.day(), n.hour(), n.minute(), n.second()));
    drawDateScreen();
    return;
  }
}

void handleAlarmButtons(int x, int y) {
  // Stunde +
  if (x >= 200 && x <= 250 && y >= 35 && y <= 75) {
    alarmSettings.hour = (alarmSettings.hour + 1) % 24;
    drawAlarmScreen();
    return;
  }
  // Stunde -
  if (x >= 260 && x <= 310 && y >= 35 && y <= 75) {
    alarmSettings.hour = (alarmSettings.hour + 23) % 24;
    drawAlarmScreen();
    return;
  }
  // Minute +
  if (x >= 200 && x <= 250 && y >= 85 && y <= 125) {
    alarmSettings.minute = (alarmSettings.minute + 1) % 60;
    drawAlarmScreen();
    return;
  }
  // Minute -
  if (x >= 260 && x <= 310 && y >= 85 && y <= 125) {
    alarmSettings.minute = (alarmSettings.minute + 59) % 60;
    drawAlarmScreen();
    return;
  }
  // Wochentage
  for (int i = 0; i < 7; i++) {
    int xLeft = 25 + i * 38;
    int xRight = xLeft + 30;
    if (x >= xLeft && x <= xRight && y >= 165 && y <= 195) {
      alarmSettings.days[i] = !alarmSettings.days[i];
      drawAlarmScreen();
      return;
    }
  }
  // AN/AUS
  if (x >= 80 && x <= 220 && y >= 200 && y <= 240) {
    alarmSettings.active = !alarmSettings.active;
    drawAlarmScreen();
    return;
  }
}

void handleSunriseButtons(int x, int y) {
  // Dauer +
  if (x >= 200 && x <= 270 && y >= 50 && y <= 100) {
    if (alarmSettings.sunriseDuration + 1 > 60) alarmSettings.sunriseDuration = 60;
    else alarmSettings.sunriseDuration += 1;
    drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness);
    return;
  }
  // Dauer -
  if (x >= 260 && x <= 330 && y >= 50 && y <= 100) {
    if (alarmSettings.sunriseDuration - 1 < 1) alarmSettings.sunriseDuration = 1;
    else alarmSettings.sunriseDuration -= 1;
    drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness);
    return;
  }
  // Helligkeit +
  if (x >= 200 && x <= 270 && y >= 110 && y <= 160) {
    if (alarmSettings.maxBrightness + 5 > 100) alarmSettings.maxBrightness = 100;
    else alarmSettings.maxBrightness += 5;
    drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness);
    return;
  }
  // Helligkeit -
  if (x >= 260 && x <= 330 && y >= 110 && y <= 160) {
    if (alarmSettings.maxBrightness - 5 < 10) alarmSettings.maxBrightness = 10;
    else alarmSettings.maxBrightness -= 5;
    drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness);
    return;
  }
}
