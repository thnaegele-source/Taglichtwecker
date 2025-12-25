#include "Globals.h"

void setup() {
  Serial.begin(115200);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // Backlight an
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  Wire.begin(RTC_SDA, RTC_SCL);
  if (!rtc.begin()) {
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(50, 100);
    tft.print(F("RTC FEHLER!"));
    while (1);
  }
  touchSPI.begin(14, 12, 13, T_CS);
  touchSPI.setFrequency(2500000);
  ts.begin(touchSPI);
  ts.setRotation(1);
  strip.begin();
  strip.setBrightness(0);
  strip.show();
  pinMode(TOUCH_LEFT, INPUT_PULLUP);
  pinMode(TOUCH_RIGHT, INPUT_PULLUP);
  lastLeftState = digitalRead(TOUCH_LEFT);
  lastRightState = digitalRead(TOUCH_RIGHT);
  calibrateTouch();
  currentScreen = SCREEN_MAIN;
  lastTouchTime = millis();
  lastScreenTime = millis();
  drawMainScreen();
}

void loop() {
  DateTime now = rtc.now();
  handleSideButtons();
  handleBacklight(now);
  handleTouchInput(now);
  if (sunriseRunning) updateSunrise();
  if (currentScreen != SCREEN_MAIN && millis() - lastScreenTime > AUTO_BACK_DELAY) {
    currentScreen = SCREEN_MAIN;
    drawMainScreen();
  }
  if (currentScreen == SCREEN_MAIN) {
    updateMainScreenTime(now);
  }

  // Zuverlässige Wecker-Prüfung
  static bool alarmTriggeredToday = false;
  static int lastDay = -1;

  int currentDayOfWeek = now.dayOfTheWeek();

  if (now.day() != lastDay) {
    alarmTriggeredToday = false;
    lastDay = now.day();
  }

  int adjustedDay = currentDayOfWeek - 1;
  if (adjustedDay < 0) adjustedDay = 6;

  bool correctDay = alarmSettings.days[adjustedDay];

  if (alarmSettings.active && correctDay && !alarmTriggeredToday) {
    if (now.hour() == alarmSettings.hour && now.minute() == alarmSettings.minute) {
      lightOn = true;
      fastSunrise = true;
      sunriseStartTime = millis();
      sunriseRunning = true;
      strip.setBrightness(1);
      strip.clear();
      strip.show();

      alarmTriggeredToday = true;

      drawMainScreen();
    }
  }

  delay(10);
}

void calibrateTouch() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 60);
  tft.print(F("4 Ecken beruehren:"));

  tft.setCursor(20, 100);
  tft.print(F("1. Oben-Links"));
  TS_Point p1 = waitForTouch();
  touchMinX = p1.x - 150;
  touchMinY = p1.y - 150;

  tft.fillRect(20, 100, 280, 30, TFT_BLACK);
  tft.setCursor(20, 100);
  tft.print(F("2. Oben-Rechts"));
  TS_Point p2 = waitForTouch();
  touchMaxX = p2.x + 150;

  tft.fillRect(20, 100, 280, 30, TFT_BLACK);
  tft.setCursor(20, 100);
  tft.print(F("3. Unten-Links"));
  TS_Point p3 = waitForTouch();
  if (p3.y - 150 < touchMinY) touchMinY = p3.y - 150;

  tft.fillRect(20, 100, 280, 30, TFT_BLACK);
  tft.setCursor(20, 100);
  tft.print(F("4. Unten-Rechts"));
  TS_Point p4 = waitForTouch();
  if (p4.x + 150 > touchMaxX) touchMaxX = p4.x + 150;
  touchMaxY = p4.y + 150;

  Serial.printf("KALIB: X[%d-%d] Y[%d-%d]\n", touchMinX, touchMaxX, touchMinY, touchMaxY);

  drawMainScreen();
}

TS_Point waitForTouch() {
  TS_Point p;
  do {
    while (!ts.touched()) delay(3);
    p = ts.getPoint();
  } while (p.z < TOUCH_THRESHOLD);

  Serial.printf("Raw: X=%d Y=%d Z=%d\n", p.x, p.y, p.z);
  delay(300);  // Entprellen
  return p;
}
