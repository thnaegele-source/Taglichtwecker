#include "Globals.h"

void updateSunrise() {
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - sunriseStartTime;
  unsigned long totalDuration = (unsigned long)alarmSettings.sunriseDuration * 60000UL;
  
  if (elapsed >= totalDuration) {
    strip.setBrightness(alarmSettings.maxBrightness);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 220, 180));  // Warmes Taglicht
    }
    strip.show();
    sunriseRunning = false;
    lightOn = true;
    Serial.println("Sonnenaufgang BEENDET");
    return;
  }
  
  float progress = (float)elapsed / totalDuration;
  uint8_t brightness = (uint8_t)(progress * alarmSettings.maxBrightness);
  strip.setBrightness(brightness);
  
  uint8_t r, g, b;
  
  // Phase 1: Dunkles Blau → Helles Blau (0-20%)
  if (progress < 0.2f) {
    float t = progress / 0.2f;
    r = (uint8_t)(30 * t);
    g = 0;
    b = (uint8_t)(100 + 155 * t);
  } 
  // Phase 2: Blau → Violett/Rosa (20-40%)
  else if (progress < 0.4f) {
    float t = (progress - 0.2f) / 0.2f;
    r = (uint8_t)(100 + 155 * t);
    g = (uint8_t)(30 * t);
    b = 255;
  } 
  // Phase 3: Rosa → Orange (40-70%)
  else if (progress < 0.7f) {
    float t = (progress - 0.4f) / 0.3f;
    r = 255;
    g = (uint8_t)(100 + 155 * t);
    b = (uint8_t)(255 - 155 * t);
  } 
  // Phase 4: Orange → Gelb/Weiß (70-100%)
  else {
    float t = (progress - 0.7f) / 0.3f;
    r = 255;
    g = (uint8_t)(255 - 35 * t);
    b = (uint8_t)(100 + 80 * t);
  }
  
  // LEDs progressiv einschalten
  int litPixels = (int)(progress * strip.numPixels() + 1);
  for (int i = 0; i < strip.numPixels(); i++) {
    if (i < litPixels) {
      strip.setPixelColor(i, strip.Color(r, g, b));
    } else {
      strip.setPixelColor(i, 0);
    }
  }
  
  strip.show();
  
  // Debug-Ausgabe alle 10 Sekunden
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 10000) {
    Serial.printf("Sunrise: %.1f%% - Brightness: %d - RGB(%d,%d,%d)\n", 
                  progress * 100, brightness, r, g, b);
    lastDebug = millis();
  }
}
