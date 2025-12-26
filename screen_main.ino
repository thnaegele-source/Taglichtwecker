#include "Globals.h"

void drawMainScreen() {
  tft.fillScreen(TFT_BLACK);
  DateTime n = rtc.now();
  
  // Uhrzeit – groß, perfekt mittig, höher
  tft.setTextSize(5);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(40, 50);
  tft.printf("%02d:%02d", n.hour(), n.minute());
  
  // Datum – mittig, darunter, voll sichtbar
  tft.setTextSize(2);
  tft.setCursor(70, 130);
  tft.printf("%02d.%02d.%04d", n.day(), n.month(), n.year());
  
  // Wecker-Status – rechts platziert
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(2);
  tft.setCursor(170, 170);
  tft.print(F("Wecker: "));
  tft.print(alarmSettings.active ? F("AN") : F("AUS"));
  
  // LICHT Button
  drawLightButton();
}

void drawLightButton() {
  uint16_t col = lightOn ? TFT_YELLOW : TFT_DARKGREY;
  tft.fillRoundRect(5, 180, 110, 60, 12, col);
  tft.setTextColor(TFT_BLACK, col);
  tft.setTextSize(2);
  tft.setCursor(25, 200);
  tft.print(F("LICHT"));
}

void updateMainScreenTime(DateTime now) {
  static int lastHour = -1, lastMinute = -1;
  
  if (now.hour() != lastHour || now.minute() != lastMinute) {
    lastHour = now.hour();
    lastMinute = now.minute();
    
    // Alte Uhrzeit überschreiben
    tft.fillRect(30, 45, 260, 60, TFT_BLACK);
    tft.setTextSize(5);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(40, 50);
    tft.printf("%02d:%02d", now.hour(), now.minute());
  }
}
