#include "Globals.h"

void updateSunset() {
  // NEU: Sunrise stoppen wenn Sunset läuft
  if (sunriseRunning) {
    sunriseRunning = false;
    screenSunriseStartTime = 0;
    Serial.println("Sunrise gestoppt - Sunset hat Priorität");
  }
  
  // Startzeit beim ersten Aufruf setzen
  static bool initialized = false;
  
  if (sunsetRunning && !initialized) {
    sunsetStartTime = millis();
    screenSunsetStartTime = millis();
    initialized = true;
    Serial.println("Sonnenuntergang initialisiert!");
    Serial.println("Bildschirm-Sonnenuntergang gestartet!");
  }
  
  if (!sunsetRunning) {
    initialized = false;
    screenSunsetStartTime = 0;
    return;
  }
  
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - sunsetStartTime;
  unsigned long totalDuration = (unsigned long)sunsetSettings.duration * 60000UL;
  
  if (elapsed >= totalDuration) {
    // Ende: Dunkles Rot (zum Einschlafen)
    strip.setBrightness(sunsetSettings.maxBrightness);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(80, 0, 0));  // Dunkles warmes Rot
    }
    strip.show();
    sunsetRunning = false;
    screenSunsetStartTime = 0;
    lightOn = true;
    Serial.println("Sonnenuntergang BEENDET - Einschlaflicht aktiv");
    return;
  }
  
  float progress = (float)elapsed / totalDuration;
  uint8_t brightness = (uint8_t)(progress * sunsetSettings.maxBrightness);
  strip.setBrightness(brightness);
  
  int numPixels = strip.numPixels();
  int waggonLength = numPixels / 3; // 267 LEDs pro Waggon
  int transitionLength = 150; // Übergänge zwischen Waggons
  
  struct Color {
    uint8_t r, g, b;
  };
  
  // Sunset-Farben: Von hellem Tageslicht zu dunklem Rot
  // UMGEKEHRTE Reihenfolge zu Sunrise!
  Color colors[6] = {
    {255, 180, 100},  // Tageslicht
    {255, 140, 60},   // Helles Orange
    {255, 80, 20},    // Orange
    {200, 30, 10},    // Rötliches Orange
    {150, 10, 5},     // Dunkles Rot
    {80, 0, 0}        // Sehr dunkles Rot (Einschlaflicht)
  };
  
  // Zuglänge berechnen
  float segmentLength = waggonLength + transitionLength;
  float trainLength = segmentLength * 5 + numPixels;
  
  // Zugfront startet bei 0
  float trainFront = progress * trainLength;
  
  for (int i = 0; i < numPixels; i++) {
    float distanceFromFront = trainFront - i;
    
    uint8_t r, g, b;
    
    if (distanceFromFront < 0) {
      // Zug hat diese LED noch nicht erreicht
      r = 0; g = 0; b = 0;
    }
    else if (distanceFromFront < segmentLength * 5) {
      // In einem der 5 Farb-Waggons
      int segment = (int)(distanceFromFront / segmentLength);
      float posInSegment = distanceFromFront - (segment * segmentLength);
      
      if (posInSegment < waggonLength) {
        // Hauptfarbe des Waggons
        r = colors[segment].r;
        g = colors[segment].g;
        b = colors[segment].b;
      }
      else {
        // Übergang zur nächsten Farbe
        float t = (posInSegment - waggonLength) / transitionLength;
        t = (1.0 - cos(t * 3.14159)) / 2.0; // Cosinus-Glättung
        
        r = (uint8_t)(colors[segment].r + (colors[segment+1].r - colors[segment].r) * t);
        g = (uint8_t)(colors[segment].g + (colors[segment+1].g - colors[segment].g) * t);
        b = (uint8_t)(colors[segment].b + (colors[segment+1].b - colors[segment].b) * t);
      }
    }
    else {
      // Im finalen Einschlaflicht-Waggon
      r = 80; g = 0; b = 0;
    }
    
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  
  strip.show();
  
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 1000) {
    Serial.printf("Sunset - Sek %.0f - Front: %.1f\n", elapsed/1000.0, trainFront);
    lastDebug = millis();
  }
}
