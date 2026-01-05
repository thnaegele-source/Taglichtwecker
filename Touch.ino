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
        unsigned long holdTime = millis() - touchStartTime;
        if (holdTime > 500 && (millis() - lastRepeatTime) > 150) {
          lastRepeatTime = millis();
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

      // === HORIZONTALER SWIPE für Hauptnavigation ===
      if (abs(deltaY) < 100 && abs(deltaX) > 60) {
        if (currentScreen == SCREEN_MAIN) {
          if (deltaX < 0) {
            currentScreen = SCREEN_MENU_LIST;
            drawMenuScreen();
          }
        } 
        else if (currentScreen == SCREEN_MENU_LIST) {
          if (deltaX > 0) {
            currentScreen = SCREEN_MAIN;
            drawMainScreen();
          }
        }
        else if (currentScreen == SCREEN_MENU_LIST2) {
          if (deltaX > 0) {
            currentScreen = SCREEN_MAIN;
            drawMainScreen();
          }
        }
        else if (currentScreen == SCREEN_SUNSET || currentScreen == SCREEN_ABENDLICHT) {
          if (deltaX > 0) {
            currentScreen = SCREEN_MENU_LIST2;
            drawMenuScreen2();
          }
        }
        else {
          if (deltaX > 0) {
            currentScreen = SCREEN_MENU_LIST;
            drawMenuScreen();
          }
        }
        return;
      }

      // === NEU: VERTIKALER SWIPE zwischen Menu 1 und 2 ===
      if (abs(deltaX) < 100 && abs(deltaY) > 60) {
        if (currentScreen == SCREEN_MENU_LIST) {
          if (deltaY < 0) {
            currentScreen = SCREEN_MENU_LIST2;
            drawMenuScreen2();
            return;
          }
        }
        else if (currentScreen == SCREEN_MENU_LIST2) {
          if (deltaY > 0) {
            currentScreen = SCREEN_MENU_LIST;
            drawMenuScreen();
            return;
          }
        }
      }

      if (abs(deltaX) < 60 && abs(deltaY) < 60) {
        handleButtonPress(startX, startY, now);
      }
    }
  }
}

void handleButtonPress(int x, int y, DateTime now) {
  // === MAIN SCREEN Buttons ===
  if (currentScreen == SCREEN_MAIN) {
    if (x >= 5 && x <= 95 && y >= 197 && y <= 235) {
      if (lightOn || sunriseRunning || sunsetRunning) {
        lightOn = false;
        sunriseRunning = false;
        sunsetRunning = false;
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
      drawSunsetButton();
      drawStatusBetweenButtons();
      return;
    }

    if (x >= 225 && x <= 315 && y >= 197 && y <= 235) {
      if (sunsetRunning) {
        sunsetRunning = false;
        lightOn = false;
        strip.setBrightness(0);
        strip.clear();
        strip.show();
        Serial.println("Sunset AUS");
      } else {
        sunsetRunning = true;
        sunriseRunning = false;
        sunsetStartTime = millis();
        strip.setBrightness(1);
        strip.clear();
        strip.show();
        Serial.println("Sunset START");
      }
      drawLightButton();
      drawSunsetButton();
      drawStatusBetweenButtons();
      return;
    }
  }

  // === MENU 1 Auswahl ===
  if (currentScreen == SCREEN_MENU_LIST) {
    for (int i = 0; i < 5; i++) {
      int yTop = 42 + i * 38;
      int yBottom = yTop + 30;
      if (x >= 40 && x <= 280 && y >= yTop && y <= yBottom) {
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

  // === MENU 2 Auswahl ===
  if (currentScreen == SCREEN_MENU_LIST2) {
    for (int i = 0; i < 3; i++) {
      int yTop = 42 + i * 38;
      int yBottom = yTop + 30;
      if (x >= 40 && x <= 280 && y >= yTop && y <= yBottom) {
        switch (i) {
          case 0: 
            currentScreen = SCREEN_ABENDLICHT;
            drawAbendlichtScreen(20, 22);
            break;
          case 1:
            currentScreen = SCREEN_SUNSET; 
            drawSunsetScreen(sunsetSettings.duration, sunsetSettings.maxBrightness); 
            break;
          case 2: 
            Serial.println("WiFi-Menü noch nicht implementiert");
            break;
        }
        return;
      }
    }
  }

  if (currentScreen == SCREEN_DISPLAY) {
    handleDisplayButtons(x, y);
    return;
  }

  if (currentScreen == SCREEN_TIME) {
    handleTimeButtons(x, y);
    return;
  }

  if (currentScreen == SCREEN_DATE) {
    handleDateButtons(x, y);
    return;
  }

  if (currentScreen == SCREEN_ALARM) {
    handleAlarmButtons(x, y);
    return;
  }

  if (currentScreen == SCREEN_SUNRISE) {
    handleSunriseButtons(x, y);
    return;
  }

  if (currentScreen == SCREEN_SUNSET) {
    handleSunsetButtons(x, y);
    return;
  }

  if (currentScreen == SCREEN_ABENDLICHT) {
    handleAbendlichtButtons(x, y);
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
  } else if (currentScreen == SCREEN_SUNSET) {
    handleSunsetButtons(x, y);
  } else if (currentScreen == SCREEN_ABENDLICHT) {
    handleAbendlichtButtons(x, y);
  }
}

void handleDisplayButtons(int x, int y) {
  if (x >= 158 && x <= 208 && y >= 44 && y <= 72) {
    displaySettings.darkHour = (displaySettings.darkHour + 1) % 24;
    drawDisplayScreen();
    return;
  }
  if (x >= 215 && x <= 265 && y >= 44 && y <= 72) {
    displaySettings.darkHour = (displaySettings.darkHour + 23) % 24;
    drawDisplayScreen();
    return;
  }
  if (x >= 158 && x <= 208 && y >= 76 && y <= 104) {
    displaySettings.darkMinute = (displaySettings.darkMinute + 1) % 60;
    drawDisplayScreen();
    return;
  }
  if (x >= 215 && x <= 265 && y >= 76 && y <= 104) {
    displaySettings.darkMinute = (displaySettings.darkMinute + 59) % 60;
    drawDisplayScreen();
    return;
  }
  if (x >= 158 && x <= 208 && y >= 129 && y <= 157) {
    displaySettings.alwaysOnHour = (displaySettings.alwaysOnHour + 1) % 24;
    drawDisplayScreen();
    return;
  }
  if (x >= 215 && x <= 265 && y >= 129 && y <= 157) {
    displaySettings.alwaysOnHour = (displaySettings.alwaysOnHour + 23) % 24;
    drawDisplayScreen();
    return;
  }
  if (x >= 158 && x <= 208 && y >= 161 && y <= 189) {
    displaySettings.alwaysOnMinute = (displaySettings.alwaysOnMinute + 1) % 60;
    drawDisplayScreen();
    return;
  }
  if (x >= 215 && x <= 265 && y >= 161 && y <= 189) {
    displaySettings.alwaysOnMinute = (displaySettings.alwaysOnMinute + 59) % 60;
    drawDisplayScreen();
    return;
  }
  if (x >= 158 && x <= 208 && y >= 214 && y <= 242) {
    displaySettings.brightDurationIndex = (displaySettings.brightDurationIndex + 1) % 7;
    drawDisplayScreen();
    return;
  }
  if (x >= 215 && x <= 265 && y >= 214 && y <= 242) {
    displaySettings.brightDurationIndex = (displaySettings.brightDurationIndex + 6) % 7;
    drawDisplayScreen();
    return;
  }
  
  if (x >= 273 && x <= 315 && y >= 25 && y <= 240) {
    calibrateTouch();
    return;
  }
}

void handleTimeButtons(int x, int y) {
  DateTime n = rtc.now();
  if (x >= 210 && x <= 260 && y >= 35 && y <= 75) {
    rtc.adjust(DateTime(n.year(), n.month(), n.day(), (n.hour() + 1) % 24, n.minute(), n.second()));
    drawTimeScreen();
    return;
  }
  if (x >= 270 && x <= 320 && y >= 35 && y <= 75) {
    rtc.adjust(DateTime(n.year(), n.month(), n.day(), (n.hour() + 23) % 24, n.minute(), n.second()));
    drawTimeScreen();
    return;
  }
  if (x >= 210 && x <= 260 && y >= 115 && y <= 155) {
    rtc.adjust(DateTime(n.year(), n.month(), n.day(), n.hour(), (n.minute() + 1) % 60, n.second()));
    drawTimeScreen();
    return;
  }
  if (x >= 270 && x <= 320 && y >= 115 && y <= 155) {
    rtc.adjust(DateTime(n.year(), n.month(), n.day(), n.hour(), (n.minute() + 59) % 60, n.second()));
    drawTimeScreen();
    return;
  }
}

void handleDateButtons(int x, int y) {
  DateTime n = rtc.now();
  if (x >= 210 && x <= 260 && y >= 55 && y <= 95) {
    rtc.adjust(DateTime(n.year(), n.month(), n.day() + 1, n.hour(), n.minute(), n.second()));
    drawDateScreen();
    return;
  }
  if (x >= 270 && x <= 320 && y >= 55 && y <= 95) {
    rtc.adjust(DateTime(n.year(), n.month(), n.day() - 1, n.hour(), n.minute(), n.second()));
    drawDateScreen();
    return;
  }
  if (x >= 210 && x <= 260 && y >= 115 && y <= 155) {
    int newMonth = n.month() + 1;
    if (newMonth > 12) newMonth = 1;
    rtc.adjust(DateTime(n.year(), newMonth, n.day(), n.hour(), n.minute(), n.second()));
    drawDateScreen();
    return;
  }
  if (x >= 270 && x <= 320 && y >= 115 && y <= 155) {
    int newMonth = n.month() - 1;
    if (newMonth < 1) newMonth = 12;
    rtc.adjust(DateTime(n.year(), newMonth, n.day(), n.hour(), n.minute(), n.second()));
    drawDateScreen();
    return;
  }
  if (x >= 210 && x <= 260 && y >= 175 && y <= 215) {
    rtc.adjust(DateTime(n.year() + 1, n.month(), n.day(), n.hour(), n.minute(), n.second()));
    drawDateScreen();
    return;
  }
  if (x >= 270 && x <= 320 && y >= 175 && y <= 215) {
    rtc.adjust(DateTime(n.year() - 1, n.month(), n.day(), n.hour(), n.minute(), n.second()));
    drawDateScreen();
    return;
  }
}

void handleAlarmButtons(int x, int y) {
  if (x >= 200 && x <= 250 && y >= 35 && y <= 75) {
    alarmSettings.hour = (alarmSettings.hour + 1) % 24;
    drawAlarmScreen();
    return;
  }
  if (x >= 260 && x <= 310 && y >= 35 && y <= 75) {
    alarmSettings.hour = (alarmSettings.hour + 23) % 24;
    drawAlarmScreen();
    return;
  }
  if (x >= 200 && x <= 250 && y >= 85 && y <= 125) {
    alarmSettings.minute = (alarmSettings.minute + 1) % 60;
    drawAlarmScreen();
    return;
  }
  if (x >= 260 && x <= 310 && y >= 85 && y <= 125) {
    alarmSettings.minute = (alarmSettings.minute + 59) % 60;
    drawAlarmScreen();
    return;
  }
  for (int i = 0; i < 7; i++) {
    int xLeft = 25 + i * 38;
    int xRight = xLeft + 30;
    if (x >= xLeft && x <= xRight && y >= 165 && y <= 195) {
      alarmSettings.days[i] = !alarmSettings.days[i];
      drawAlarmScreen();
      return;
    }
  }
  if (x >= 80 && x <= 220 && y >= 200 && y <= 240) {
    alarmSettings.active = !alarmSettings.active;
    drawAlarmScreen();
    drawStatusBetweenButtons();
    return;
  }
}

void handleSunriseButtons(int x, int y) {
  if (x >= 200 && x <= 250 && y >= 50 && y <= 100) {
    if (alarmSettings.sunriseDuration + 1 > 60) alarmSettings.sunriseDuration = 60;
    else alarmSettings.sunriseDuration += 1;
    drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness);
    return;
  }
  if (x >= 260 && x <= 310 && y >= 50 && y <= 100) {
    if (alarmSettings.sunriseDuration - 1 < 1) alarmSettings.sunriseDuration = 1;
    else alarmSettings.sunriseDuration -= 1;
    drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness);
    return;
  }
  if (x >= 200 && x <= 250 && y >= 110 && y <= 160) {
    if (alarmSettings.maxBrightness + 5 > 100) alarmSettings.maxBrightness = 100;
    else alarmSettings.maxBrightness += 5;
    drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness);
    return;
  }
  if (x >= 260 && x <= 310 && y >= 110 && y <= 160) {
    if (alarmSettings.maxBrightness - 5 < 10) alarmSettings.maxBrightness = 10;
    else alarmSettings.maxBrightness -= 5;
    drawSunriseScreen(alarmSettings.sunriseDuration, alarmSettings.maxBrightness);
    return;
  }
}

void handleSunsetButtons(int x, int y) {
  if (x >= 200 && x <= 250 && y >= 50 && y <= 100) {
    if (sunsetSettings.duration + 1 > 60) sunsetSettings.duration = 60;
    else sunsetSettings.duration += 1;
    drawSunsetScreen(sunsetSettings.duration, sunsetSettings.maxBrightness);
    return;
  }
  if (x >= 260 && x <= 310 && y >= 50 && y <= 100) {
    if (sunsetSettings.duration - 1 < 1) sunsetSettings.duration = 1;
    else sunsetSettings.duration -= 1;
    drawSunsetScreen(sunsetSettings.duration, sunsetSettings.maxBrightness);
    return;
  }
  if (x >= 200 && x <= 250 && y >= 110 && y <= 160) {
    if (sunsetSettings.maxBrightness + 5 > 100) sunsetSettings.maxBrightness = 100;
    else sunsetSettings.maxBrightness += 5;
    drawSunsetScreen(sunsetSettings.duration, sunsetSettings.maxBrightness);
    return;
  }
  if (x >= 260 && x <= 310 && y >= 110 && y <= 160) {
    if (sunsetSettings.maxBrightness - 5 < 10) sunsetSettings.maxBrightness = 10;
    else sunsetSettings.maxBrightness -= 5;
    drawSunsetScreen(sunsetSettings.duration, sunsetSettings.maxBrightness);
    return;
  }
}

void handleAbendlichtButtons(int x, int y) {
  // Stunde +
  if (x >= 200 && x <= 250 && y >= 35 && y <= 75) {
    sunsetSettings.hour = (sunsetSettings.hour + 1) % 24;
    drawAbendlichtScreen(sunsetSettings.duration, sunsetSettings.maxBrightness);
    return;
  }
  // Stunde -
  if (x >= 260 && x <= 310 && y >= 35 && y <= 75) {
    sunsetSettings.hour = (sunsetSettings.hour + 23) % 24;
    drawAbendlichtScreen(sunsetSettings.duration, sunsetSettings.maxBrightness);
    return;
  }
  // Minute +
  if (x >= 200 && x <= 250 && y >= 85 && y <= 125) {
    sunsetSettings.minute = (sunsetSettings.minute + 1) % 60;
    drawAbendlichtScreen(sunsetSettings.duration, sunsetSettings.maxBrightness);
    return;
  }
  // Minute -
  if (x >= 260 && x <= 310 && y >= 85 && y <= 125) {
    sunsetSettings.minute = (sunsetSettings.minute + 59) % 60;
    drawAbendlichtScreen(sunsetSettings.duration, sunsetSettings.maxBrightness);
    return;
  }
  
  // Wochentage
  for (int i = 0; i < 7; i++) {
    int xLeft = 25 + i * 38;
    int xRight = xLeft + 30;
    if (x >= xLeft && x <= xRight && y >= 165 && y <= 195) {
      sunsetSettings.days[i] = !sunsetSettings.days[i];
      drawAbendlichtScreen(sunsetSettings.duration, sunsetSettings.maxBrightness);
      return;
    }
  }
  
  // AN/AUS Button
  if (x >= 80 && x <= 220 && y >= 200 && y <= 240) {
    sunsetSettings.active = !sunsetSettings.active;
    drawAbendlichtScreen(sunsetSettings.duration, sunsetSettings.maxBrightness);
    drawStatusBetweenButtons();
    return;
  }
}
