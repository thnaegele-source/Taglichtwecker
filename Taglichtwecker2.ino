#include "Globals.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== TAGLICHTWECKER START ===");
  
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  
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
  
  // === RTC LED AUSSCHALTEN ===
  rtc.writeSqwPinMode(DS3231_OFF);
  rtc.disableAlarm(1);
  rtc.disableAlarm(2);
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  Serial.println("RTC Alarme deaktiviert und Square Wave aus");
  
  touchSPI.begin(14, 12, 13, T_CS);
  touchSPI.setFrequency(2500000);
  ts.begin(touchSPI);
  ts.setRotation(1);
  
  strip.begin();
  strip.setBrightness(0);
  strip.clear();  // WICHTIG: Alle LEDs auf 0!
  strip.show();
  
  pinMode(TOUCH_LEFT, INPUT_PULLUP);
  pinMode(TOUCH_RIGHT, INPUT_PULLUP);
  lastLeftState = digitalRead(TOUCH_LEFT);
  lastRightState = digitalRead(TOUCH_RIGHT);
  
  // === WICHTIG: Strip-Status explizit initialisieren ===
  leftStripOn = false;
  rightStripOn = false;
  sunriseRunning = false;
  lightOn = false;
  Serial.println("Strip-Status initialisiert: leftStripOn=0, rightStripOn=0");
  
  // Alarm-Einstellungen initialisieren
  alarmSettings.hour = 7;
  alarmSettings.minute = 0;
  alarmSettings.active = false;
  alarmSettings.vorher = 0;
  alarmSettings.sunriseDuration = 20;
  alarmSettings.maxBrightness = 80;
  for (int i = 0; i < 7; i++) {
    alarmSettings.days[i] = false;
  }
  
  // Display-Einstellungen initialisieren
  displaySettings.darkHour = 22;
  displaySettings.darkMinute = 0;
  displaySettings.brightDurationIndex = 2;
  displaySettings.alwaysOnHour = 7;
  displaySettings.alwaysOnMinute = 0;
  
  // NEU: Sunset-Einstellungen initialisieren
  sunsetSettings.hour = 22;             // Standard: 22:00 Uhr
  sunsetSettings.minute = 0;
  sunsetSettings.duration = 20;         // 20 Minuten Standard
  sunsetSettings.maxBrightness = 30;    // Niedrigere Helligkeit zum Einschlafen
  sunsetSettings.active = false;        // Standardmäßig aus
  for (int i = 0; i < 7; i++) {
    sunsetSettings.days[i] = false;     // Alle Tage deaktiviert
  }
  sunsetRunning = false;
  sunsetStartTime = 0;
  screenSunsetStartTime = 0;
  Serial.println("Sunset-Settings initialisiert");
  
  // Touch-Kalibrierung laden oder durchführen
  Serial.println("Lade Touch-Kalibrierung...");
  loadTouchCalibration();
  
  currentScreen = SCREEN_MAIN;
  lastTouchTime = millis();
  lastScreenTime = millis();
  drawMainScreen();
  
  // NEU: Gespeicherte Helligkeit laden
  loadNightlightSettings();
  
  Serial.println("=== SETUP ABGESCHLOSSEN ===\n");
}

void loop() {
  DateTime now = rtc.now();
  
  // === DEBUG: Alle 2 Sekunden Status ausgeben ===
  static unsigned long lastStatusDebug = 0;
  if (millis() - lastStatusDebug > 2000) {
    Serial.printf("LOOP STATUS: sunriseRunning=%d, sunsetRunning=%d, leftStripOn=%d, rightStripOn=%d, lightOn=%d\n", 
                  sunriseRunning, sunsetRunning, leftStripOn, rightStripOn, lightOn);
    lastStatusDebug = millis();
  }
  
  handleSideButtons();
  // updateDimming();  // AUSKOMMENTIERT - wird in handleSideButtons() gemacht!
  handleBacklight();
  handleTouchInput(now);
  checkAlarmTrigger();
  checkSunsetTrigger();  // NEU: Sunset-Timer prüfen
  
  // NEU: Beide Animationen aktualisieren
  if (sunriseRunning) updateSunrise();
  if (sunsetRunning) updateSunset();  // NEU
  
  if (currentScreen != SCREEN_MAIN && millis() - lastScreenTime > AUTO_BACK_DELAY) {
    currentScreen = SCREEN_MAIN;
    drawMainScreen();
  }
  
  if (currentScreen == SCREEN_MAIN) {
    updateMainScreenTime(now);
  }
  
  delay(10);
}

void calibrateTouch() {
  Serial.println("\n>>> Starte Touch-Kalibrierung <<<");
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 90);
  tft.print(F("Touch-Kalibrierung"));
  tft.setTextSize(1);
  tft.setCursor(50, 115);
  tft.print(F("Kreise in den Ecken druecken"));
  delay(1500);
  
  // 1. Oben-Links
  tft.fillScreen(TFT_BLACK);
  tft.drawCircle(15, 15, 10, TFT_RED);
  tft.drawCircle(15, 15, 8, TFT_RED);
  tft.fillCircle(15, 15, 3, TFT_RED);
  TS_Point p1 = waitForTouch();
  touchMinX = p1.x - 150;
  touchMinY = p1.y - 150;
  tft.fillCircle(15, 15, 12, TFT_GREEN);
  delay(300);
  
  // 2. Oben-Rechts
  tft.fillScreen(TFT_BLACK);
  tft.drawCircle(305, 15, 10, TFT_RED);
  tft.drawCircle(305, 15, 8, TFT_RED);
  tft.fillCircle(305, 15, 3, TFT_RED);
  TS_Point p2 = waitForTouch();
  touchMaxX = p2.x + 150;
  tft.fillCircle(305, 15, 12, TFT_GREEN);
  delay(300);
  
  // 3. Unten-Links
  tft.fillScreen(TFT_BLACK);
  tft.drawCircle(15, 225, 10, TFT_RED);
  tft.drawCircle(15, 225, 8, TFT_RED);
  tft.fillCircle(15, 225, 3, TFT_RED);
  TS_Point p3 = waitForTouch();
  if (p3.y - 150 < touchMinY) touchMinY = p3.y - 150;
  tft.fillCircle(15, 225, 12, TFT_GREEN);
  delay(300);
  
  // 4. Unten-Rechts
  tft.fillScreen(TFT_BLACK);
  tft.drawCircle(305, 225, 10, TFT_RED);
  tft.drawCircle(305, 225, 8, TFT_RED);
  tft.fillCircle(305, 225, 3, TFT_RED);
  TS_Point p4 = waitForTouch();
  if (p4.x + 150 > touchMaxX) touchMaxX = p4.x + 150;
  touchMaxY = p4.y + 150;
  tft.fillCircle(305, 225, 12, TFT_GREEN);
  delay(300);
  
  Serial.printf("Kalibriert: X[%d-%d] Y[%d-%d]\n", touchMinX, touchMaxX, touchMinY, touchMaxY);
  
  // Kalibrierwerte speichern
  saveTouchCalibration();
  
  // Erfolgsmeldung
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(3);
  tft.setCursor(40, 100);
  tft.print(F("Fertig!"));
  delay(1000);
  
  Serial.println("<<< Kalibrierung abgeschlossen >>>\n");
  
  drawDisplayScreen();
}

TS_Point waitForTouch() {
  TS_Point p;
  do {
    while (!ts.touched()) delay(3);
    p = ts.getPoint();
  } while (p.z < TOUCH_THRESHOLD);
  
  Serial.printf("Raw: X=%d Y=%d Z=%d\n", p.x, p.y, p.z);
  delay(300);
  return p;
}

void loadTouchCalibration() {
  Serial.println("Oeffne Preferences Namespace 'touch'...");
  preferences.begin("touch", false);
  
  bool calibrated = preferences.getBool("calibrated", false);
  
  Serial.printf("Calibrated-Flag: %d\n", calibrated);
  
  if (calibrated) {
    touchMinX = preferences.getInt("minX", 300);
    touchMaxX = preferences.getInt("maxX", 3700);
    touchMinY = preferences.getInt("minY", 300);
    touchMaxY = preferences.getInt("maxY", 3700);
    
    Serial.println(">>> Touch-Kalibrierung aus Flash geladen:");
    Serial.printf("    X[%d-%d] Y[%d-%d]\n", touchMinX, touchMaxX, touchMinY, touchMaxY);
  } else {
    Serial.println(">>> KEINE gespeicherte Kalibrierung gefunden!");
    Serial.println(">>> Fuehre manuelle Kalibrierung durch...");
    preferences.end();
    calibrateTouch();
    return;
  }
  
  preferences.end();
  Serial.println("Preferences geschlossen.\n");
}

void saveTouchCalibration() {
  Serial.println("Speichere Kalibrierung in Flash...");
  preferences.begin("touch", false);
  
  preferences.putBool("calibrated", true);
  preferences.putInt("minX", touchMinX);
  preferences.putInt("maxX", touchMaxX);
  preferences.putInt("minY", touchMinY);
  preferences.putInt("maxY", touchMaxY);
  
  preferences.end();
  
  Serial.println(">>> Kalibrierung erfolgreich gespeichert!");
  Serial.printf("    Gespeichert: X[%d-%d] Y[%d-%d]\n", touchMinX, touchMaxX, touchMinY, touchMaxY);
}
