#include "Globals.h"

// Deutsche Wochentage ausgeschrieben
const char* wochentage[] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};

// Funktion um Himmelsfarbe basierend auf Uhrzeit ODER Sonnenaufgang zu berechnen
uint16_t getSkyColor(int hour, int minute, int y) {
  // === WENN SONNENAUFGANG LÄUFT: Realistische Animation ===
  if (sunriseRunning && screenSunriseStartTime > 0) {
    unsigned long elapsed = millis() - screenSunriseStartTime;
    unsigned long totalDuration = alarmSettings.sunriseDuration * 60000UL;
    int progress = constrain(map(elapsed, 0, totalDuration, 0, 100), 0, 100);
    
    int r, g, b;
    
    // Phase 1 (0-20%): Dunkles Blau → Tiefes Violett/Rosa
    if (progress < 20) {
      int p = map(progress, 0, 20, 0, 100);
      r = map(p, 0, 100, 15, 80);
      g = map(p, 0, 100, 10, 20);
      b = map(p, 0, 100, 40, 60);
    }
    // Phase 2 (20-40%): Rosa/Violett → Kräftiges Orange/Rosa
    else if (progress < 40) {
      int p = map(progress, 20, 40, 0, 100);
      r = map(p, 0, 100, 80, 255);
      g = map(p, 0, 100, 20, 60);
      b = map(p, 0, 100, 60, 80);
    }
    // Phase 3 (40-60%): Orange/Rosa → Goldgelb/Orange
    else if (progress < 60) {
      int p = map(progress, 40, 60, 0, 100);
      r = map(p, 0, 100, 255, 255);
      g = map(p, 0, 100, 60, 140);
      b = map(p, 0, 100, 80, 40);
    }
    // Phase 4 (60-80%): Goldgelb → Helles Gelb/Blau
    else if (progress < 80) {
      int p = map(progress, 60, 80, 0, 100);
      r = map(p, 0, 100, 255, 200);
      g = map(p, 0, 100, 140, 180);
      b = map(p, 0, 100, 40, 150);
    }
    // Phase 5 (80-100%): Helles Gelb → Tageshimmel Blau
    else {
      int p = map(progress, 80, 100, 0, 100);
      r = map(p, 0, 100, 200, 135);
      g = map(p, 0, 100, 180, 206);
      b = map(p, 0, 100, 150, 235);
    }
    
    // Vertikaler Gradient: Oben heller, unten intensiver
    if (progress < 50) {
      // Während Sonnenaufgang: Unten wärmer/intensiver
      r = map(y, 0, 120, r + 40, r - 20);
      g = map(y, 0, 120, g + 20, g - 10);
      b = map(y, 0, 120, b + 30, b - 30);
    } else {
      // Später: Normale Gradienten
      r = map(y, 0, 120, r + 30, r - 30);
      g = map(y, 0, 120, g + 20, g - 20);
      b = map(y, 0, 120, b + 40, b - 20);
    }
    
    return tft.color565(constrain(r, 0, 255), constrain(g, 0, 255), constrain(b, 0, 255));
  }
  
  // === NORMALE ZEIT-BASIERTE FARBE ===
  int totalMinutes = hour * 60 + minute;
  
  // Nacht: 20:00 - 06:00 (1200-360 Minuten)
  if (totalMinutes >= 1200 || totalMinutes < 360) {
    int darkness = map(y, 0, 120, 15, 35);
    return tft.color565(darkness, darkness, darkness + 25);
  }
  
  // Sonnenaufgang: 06:00 - 08:00 (360-480 Minuten) - Realistischer
  if (totalMinutes >= 360 && totalMinutes < 480) {
    int progress = map(totalMinutes, 360, 480, 0, 100);
    int r, g, b;
    
    if (progress < 30) {
      // Früher Morgen: Blau → Rosa
      int p = map(progress, 0, 30, 0, 100);
      r = map(p, 0, 100, 20, 255);
      g = map(p, 0, 100, 15, 80);
      b = map(p, 0, 100, 50, 120);
    } else if (progress < 70) {
      // Rosa → Orange/Gold
      int p = map(progress, 30, 70, 0, 100);
      r = map(p, 0, 100, 255, 255);
      g = map(p, 0, 100, 80, 150);
      b = map(p, 0, 100, 120, 80);
    } else {
      // Orange → Tageshimmel
      int p = map(progress, 70, 100, 0, 100);
      r = map(p, 0, 100, 255, 135);
      g = map(p, 0, 100, 150, 206);
      b = map(p, 0, 100, 80, 235);
    }
    
    r = map(y, 0, 120, r + 40, r - 30);
    g = map(y, 0, 120, g + 25, g - 20);
    b = map(y, 0, 120, b + 35, b - 15);
    return tft.color565(constrain(r, 0, 255), constrain(g, 0, 255), constrain(b, 0, 255));
  }
  
  // Tag: 08:00 - 18:00 (480-1080 Minuten)
  if (totalMinutes >= 480 && totalMinutes < 1080) {
    int r = map(y, 0, 120, 100, 135);
    int g = map(y, 0, 120, 180, 206);
    int b = map(y, 0, 120, 255, 235);
    return tft.color565(r, g, b);
  }
  
  // Sonnenuntergang: 18:00 - 20:00 (1080-1200 Minuten) - Realistischer
  if (totalMinutes >= 1080 && totalMinutes < 1200) {
    int progress = map(totalMinutes, 1080, 1200, 0, 100);
    int r, g, b;
    
    if (progress < 40) {
      // Tageshimmel → Gold/Orange
      int p = map(progress, 0, 40, 0, 100);
      r = map(p, 0, 100, 135, 255);
      g = map(p, 0, 100, 206, 120);
      b = map(p, 0, 100, 235, 50);
    } else if (progress < 70) {
      // Gold/Orange → Rosa/Rot
      int p = map(progress, 40, 70, 0, 100);
      r = map(p, 0, 100, 255, 200);
      g = map(p, 0, 100, 120, 40);
      b = map(p, 0, 100, 50, 80);
    } else {
      // Rosa → Dunkles Blau
      int p = map(progress, 70, 100, 0, 100);
      r = map(p, 0, 100, 200, 15);
      g = map(p, 0, 100, 40, 10);
      b = map(p, 0, 100, 80, 40);
    }
    
    r = map(y, 0, 120, r + 50, r - 40);
    g = map(y, 0, 120, g + 30, g - 30);
    b = map(y, 0, 120, b + 40, b - 30);
    return tft.color565(constrain(r, 0, 255), constrain(g, 0, 255), constrain(b, 0, 255));
  }
  
  return TFT_BLACK;
}

// Funktion um Bergfarbe (= untere Bildschirmfarbe) basierend auf Tageszeit zu berechnen
uint16_t getGroundColor() {
  DateTime n = rtc.now();
  int totalMinutes = n.hour() * 60 + n.minute();
  
  int greenAmount = 0; // 0 = schwarz, 100 = grün
  
  // Während Sonnenaufgang: Grün langsam einblenden
  if (sunriseRunning && screenSunriseStartTime > 0) {
    unsigned long elapsed = millis() - screenSunriseStartTime;
    unsigned long totalDuration = alarmSettings.sunriseDuration * 60000UL;
    int progress = constrain(map(elapsed, 0, totalDuration, 0, 100), 0, 100);
    
    // Grün erscheint ab 60% des Sonnenaufgangs
    if (progress > 60) {
      greenAmount = map(progress, 60, 100, 0, 100);
    }
  }
  // Normale Tageszeit
  else if (totalMinutes >= 480 && totalMinutes < 1080) {
    // Tag: Volle grüne Farbe
    greenAmount = 100;
  }
  else if (totalMinutes >= 360 && totalMinutes < 480) {
    // Morgen 06:00-08:00: Langsam grün werden
    greenAmount = map(totalMinutes, 360, 480, 0, 100);
  }
  else if (totalMinutes >= 1080 && totalMinutes < 1200) {
    // Abend 18:00-20:00: Grün verschwindet
    greenAmount = map(totalMinutes, 1080, 1200, 100, 0);
  }
  
  // Farben interpolieren zwischen schwarz und grün
  uint8_t r = map(greenAmount, 0, 100, 5, 15);
  uint8_t g = map(greenAmount, 0, 100, 5, 35);
  uint8_t b = map(greenAmount, 0, 100, 8, 10);
  
  return tft.color565(r, g, b);
}

// Funktion um Bergfarbe basierend auf Tageszeit zu berechnen
void getMountainColors(uint16_t &backColor, uint16_t &midColor, uint16_t &frontColor, uint16_t &nearestColor) {
  // Hinterste Berge (sehr hell grau)
  backColor = tft.color565(90, 90, 100);
  
  // Mittlere Berge (mittelgrau)
  midColor = tft.color565(55, 55, 65);
  
  // Vordere Berge (dunkelgrau)
  frontColor = tft.color565(25, 25, 35);
  
  // Vorderste Hügel = unterer Bildschirmbereich (identisch!)
  nearestColor = getGroundColor();
}

// Funktion um Sterne zu zeichnen (nur nachts oder am Anfang des Sonnenaufgangs)
void drawStars() {
  // Sterne nur in der Nacht oder in den ersten 25% des Sonnenaufgangs
  if (sunriseRunning && screenSunriseStartTime > 0) {
    unsigned long elapsed = millis() - screenSunriseStartTime;
    unsigned long totalDuration = alarmSettings.sunriseDuration * 60000UL;
    int progress = constrain(map(elapsed, 0, totalDuration, 0, 100), 0, 100);
    
    if (progress > 25) return; // Keine Sterne nach 25% des Sonnenaufgangs
    
    // Sterne verblassen während Sonnenaufgang
    int brightness = map(progress, 0, 25, 255, 0);
    uint16_t starColor = tft.color565(brightness, brightness, brightness);
    uint16_t starColorDim = tft.color565(brightness/2, brightness/2, brightness/2);
    
    drawStarsWithColor(starColor, starColorDim);
  } else {
    // Normale Nachtsterne
    drawStarsWithColor(TFT_WHITE, TFT_LIGHTGREY);
  }
}

void drawStarsWithColor(uint16_t brightColor, uint16_t dimColor) {
  int starPositions[][2] = {
    {20, 15}, {45, 25}, {80, 10}, {110, 30}, {150, 20},
    {190, 35}, {220, 15}, {260, 25}, {290, 18}, {310, 40},
    {35, 50}, {70, 60}, {120, 55}, {170, 65}, {210, 58},
    {250, 70}, {285, 62}, {55, 85}, {95, 90}, {140, 80},
    {180, 95}, {230, 88}, {270, 100}, {300, 85}, {15, 105}
  };
  
  for (int i = 0; i < 25; i++) {
    int x = starPositions[i][0];
    int y = starPositions[i][1];
    
    if (i % 5 == 0) {
      tft.fillCircle(x, y, 2, brightColor);
    } else if (i % 3 == 0) {
      tft.fillCircle(x, y, 1, dimColor);
      tft.drawPixel(x, y, brightColor);
    } else {
      tft.drawPixel(x, y, dimColor);
    }
  }
}

void drawMountains() {
  uint16_t backColor, midColor, frontColor, nearestColor;
  getMountainColors(backColor, midColor, frontColor, nearestColor);
  
  // Hinterste Berge (sehr hell grau)
  tft.fillTriangle(0, 120, 90, 50, 180, 120, backColor);
  tft.fillTriangle(140, 120, 230, 55, 320, 120, backColor);
  
  // Mittlere Berge (mittelgrau)
  tft.fillTriangle(-20, 120, 70, 65, 160, 120, midColor);
  tft.fillTriangle(100, 120, 190, 45, 280, 120, midColor);
  tft.fillTriangle(240, 120, 310, 70, 340, 120, midColor);
  
  // Vordere Berge (dunkelgrau)
  tft.fillTriangle(-10, 120, 80, 75, 170, 120, frontColor);
  tft.fillTriangle(120, 120, 200, 60, 280, 120, frontColor);
  
  // Vorderste Hügel (schwarz nachts, dunkelgrün tagsüber)
  tft.fillTriangle(-20, 120, 60, 95, 140, 120, nearestColor);
  tft.fillTriangle(180, 120, 260, 90, 340, 120, nearestColor);
}

void drawMainScreen() {
  tft.setTextFont(1);
  tft.setTextSize(1);
  tft.setTextDatum(TL_DATUM);
  
  DateTime n = rtc.now();
  int totalMinutes = n.hour() * 60 + n.minute();
  bool isNight = (totalMinutes >= 1200 || totalMinutes < 360);
  
  // === Himmel mit Gradienten zeichnen (berücksichtigt Sonnenaufgang) ===
  for (int y = 0; y < 120; y++) {
    uint16_t color = getSkyColor(n.hour(), n.minute(), y);
    tft.drawFastHLine(0, y, 320, color);
  }
  
  // === Sterne zeichnen (nur nachts oder während Sonnenaufgang verblassend) ===
  if (isNight || (sunriseRunning && screenSunriseStartTime > 0)) {
    drawStars();
  }
  
  // === Berge ===
  drawMountains();
  
  // === Unterer Bereich - GLEICHE FARBE wie vorderste Hügel! ===
  uint16_t groundColor = getGroundColor();
  tft.fillRect(0, 120, 320, 120, groundColor);
  
  // === Uhrzeit ===
  tft.setTextFont(4);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, groundColor);  // Hintergrund = groundColor
  tft.setCursor(65, 123);
  if (n.hour() < 10) tft.print("0");
  tft.print(n.hour());
  tft.print(":");
  if (n.minute() < 10) tft.print("0");
  tft.print(n.minute());
  
  tft.setTextFont(1);
  tft.setTextSize(1);
  
  // === Wochentag + Datum ===
  tft.setTextFont(4);
  tft.setTextColor(TFT_LIGHTGREY, groundColor);  // Hintergrund = groundColor
  tft.setCursor(5, 168);
  tft.print(wochentage[n.dayOfTheWeek()]);
  tft.print(", ");
  if (n.day() < 10) tft.print("0");
  tft.print(n.day());
  tft.print(".");
  if (n.month() < 10) tft.print("0");
  tft.print(n.month());
  tft.print(".");
  tft.print(n.year());
  
  tft.setTextFont(1);
  
  // NEU: Beide Buttons zeichnen
  drawLightButton();   // Links unten
  drawSunsetButton();  // Rechts unten
  drawStatusBetweenButtons();  // Status in der Mitte
  
  tft.setTextFont(1);
  tft.setTextSize(1);
}

void drawLightButton() {
  // GEÄNDERT: Gelb wenn entweder lightOn ODER sunriseRunning
  uint16_t col = (lightOn || sunriseRunning) ? TFT_YELLOW : TFT_DARKGREY;
  tft.fillRoundRect(5, 197, 90, 38, 10, col);
  
  tft.setTextFont(2);
  tft.setTextColor(TFT_BLACK, col);
  tft.setCursor(18, 209);
  tft.print(F("LICHT"));
  
  tft.setTextFont(1);
}

// NEU: Sunset Button rechts unten
void drawSunsetButton() {
  uint16_t col = sunsetRunning ? TFT_RED : TFT_DARKGREY;
  tft.fillRoundRect(225, 197, 90, 38, 10, col);
  
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE, col);
  tft.setCursor(235, 209);
  tft.print(F("SLEEP"));
  
  tft.setTextFont(1);
}

// NEU: Status-Anzeige zwischen den Buttons
void drawStatusBetweenButtons() {
  // NUR auf dem Hauptbildschirm zeichnen!
  if (currentScreen != SCREEN_MAIN) return;
  
  uint16_t groundColor = getGroundColor();
  
  tft.setTextFont(2);
  tft.setTextSize(1);
  
  // Wecker-Status
  tft.setTextColor(TFT_CYAN, groundColor);
  tft.setCursor(105, 202);
  tft.print(alarmSettings.active ? F("Wecker AN ") : F("Wecker AUS"));
  
  // Sunset-Status
  tft.setTextColor(TFT_ORANGE, groundColor);
  tft.setCursor(105, 218);
  if (sunsetRunning) {
    tft.print(F("Sunset "));
    tft.print(sunsetSettings.duration);
    tft.print(F("m   "));
  } else if (sunsetSettings.active) {
    // Geplanter Sunset anzeigen
    if (sunsetSettings.hour < 10) tft.print("0");
    tft.print(sunsetSettings.hour);
    tft.print(F(":"));
    if (sunsetSettings.minute < 10) tft.print("0");
    tft.print(sunsetSettings.minute);
    tft.print(F(" "));
  } else {
    tft.print(F("Sunset OFF"));
  }
  
  tft.setTextFont(1);
  tft.setTextSize(1);
}

void updateMainScreenTime(DateTime now) {
  static int lastHour = -1, lastMinute = -1;
  static unsigned long lastSunriseUpdate = 0;
  static int lastSunriseProgress = -1;
  
  // === FONT-RESET AM ANFANG - WICHTIG! ===
  tft.setTextFont(1);
  tft.setTextSize(1);
  tft.setTextDatum(TL_DATUM);
  
  // Bildschirm während Sonnenaufgang: Nur bei merklicher Farbänderung aktualisieren
  if (sunriseRunning && screenSunriseStartTime > 0) {
    unsigned long elapsed = millis() - screenSunriseStartTime;
    unsigned long totalDuration = alarmSettings.sunriseDuration * 60000UL;
    int progress = constrain(map(elapsed, 0, totalDuration, 0, 100), 0, 100);
    
    // Nur updaten wenn sich Progress um mindestens 5% geändert hat ODER 10 Sekunden vergangen sind
    if ((progress - lastSunriseProgress >= 5) || (millis() - lastSunriseUpdate > 10000)) {
      lastSunriseUpdate = millis();
      lastSunriseProgress = progress;
      
      // Nur Himmel-Bereich und Berge neu zeichnen
      DateTime n = rtc.now();
      int totalMinutes = n.hour() * 60 + n.minute();
      bool isNight = (totalMinutes >= 1200 || totalMinutes < 360);
      
      // Himmel neu zeichnen
      for (int y = 0; y < 120; y++) {
        uint16_t color = getSkyColor(n.hour(), n.minute(), y);
        tft.drawFastHLine(0, y, 320, color);
      }
      
      // Sterne
      if (isNight || (sunriseRunning && screenSunriseStartTime > 0)) {
        drawStars();
      }
      
      // Berge
      drawMountains();
      
      // Unteren Bereich neu zeichnen (inklusive Text)
      uint16_t groundColor = getGroundColor();
      tft.fillRect(0, 120, 320, 120, groundColor);
      
      // === FONT EXPLIZIT SETZEN VOR JEDEM TEXT ===
      // Text neu zeichnen - Uhrzeit
      tft.setTextFont(4);
      tft.setTextSize(2);
      tft.setTextDatum(TL_DATUM);
      tft.setTextColor(TFT_WHITE, groundColor);
      tft.setCursor(65, 123);
      if (n.hour() < 10) tft.print("0");
      tft.print(n.hour());
      tft.print(":");
      if (n.minute() < 10) tft.print("0");
      tft.print(n.minute());
      
      // Datum
      tft.setTextFont(4);
      tft.setTextSize(1);  // Wichtig: Size 1 für kleinere Texte!
      tft.setTextDatum(TL_DATUM);
      tft.setTextColor(TFT_LIGHTGREY, groundColor);
      tft.setCursor(5, 168);
      tft.print(wochentage[n.dayOfTheWeek()]);
      tft.print(", ");
      if (n.day() < 10) tft.print("0");
      tft.print(n.day());
      tft.print(".");
      if (n.month() < 10) tft.print("0");
      tft.print(n.month());
      tft.print(".");
      tft.print(n.year());
      
      // Reset am Ende
      tft.setTextFont(1);
      tft.setTextSize(1);
      
      // NEU: Beide Buttons aktualisieren
      drawLightButton();
      drawSunsetButton();
      drawStatusBetweenButtons();
    }
    return;
  }
  
  // Sonnenaufgang beendet: Reset
  if (!sunriseRunning && lastSunriseProgress != -1) {
    lastSunriseProgress = -1;
  }
  
  // Normale Minutenaktualisierung
  if (now.hour() != lastHour || now.minute() != lastMinute) {
    lastHour = now.hour();
    lastMinute = now.minute();
    drawMainScreen();
  }
}
