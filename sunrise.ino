#include "Globals.h"

void updateSunrise() {
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - sunriseStartTime;
  unsigned long totalDuration = (unsigned long)alarmSettings.sunriseDuration * 60000UL;
  
  if (elapsed >= totalDuration) {
    strip.setBrightness(alarmSettings.maxBrightness);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 180, 100));
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
  
  int numPixels = strip.numPixels();
  int waggonLength = numPixels / 3; // 267 LEDs
  int transitionLength = 100; // Größere Farbverläufe
  
  struct Color {
    uint8_t r, g, b;
  };
  
  Color colors[6] = {
    {10, 5, 120},     // Blau
    {20, 30, 240},    // Lila (noch bläulicher: mehr Blau, weniger Rot)
    {255, 0, 0},      // Rot
    {255, 50, 0},     // Orange
    {255, 160, 10},   // Gelb (weniger grünlich: G von 180 auf 160)
    {255, 180, 100}   // Tageslicht
  };
  
  // Zuglänge
  float segmentLength = waggonLength + transitionLength;
  float trainLength = segmentLength * 5 + numPixels;
  
  // Front startet bei 0
  float trainFront = progress * trainLength;
  
  for (int i = 0; i < numPixels; i++) {
    float distanceFromFront = trainFront - i;
    
    uint8_t r, g, b;
    
    if (distanceFromFront < 0) {
      r = 0; g = 0; b = 0;
    }
    else if (distanceFromFront < segmentLength * 5) {
      int segment = (int)(distanceFromFront / segmentLength);
      float posInSegment = distanceFromFront - (segment * segmentLength);
      
      if (posInSegment < waggonLength) {
        r = colors[segment].r;
        g = colors[segment].g;
        b = colors[segment].b;
      }
      else {
        float t = (posInSegment - waggonLength) / transitionLength;
        t = (1.0 - cos(t * 3.14159)) / 2.0;
        
        r = (uint8_t)(colors[segment].r + (colors[segment+1].r - colors[segment].r) * t);
        g = (uint8_t)(colors[segment].g + (colors[segment+1].g - colors[segment].g) * t);
        b = (uint8_t)(colors[segment].b + (colors[segment+1].b - colors[segment].b) * t);
      }
    }
    else {
      r = 255; g = 180; b = 100;
    }
    
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  
  strip.show();
  
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 1000) {
    Serial.printf("Sek %.0f - Front: %.1f\n", elapsed/1000.0, trainFront);
    lastDebug = millis();
  }
}
