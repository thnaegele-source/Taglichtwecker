#include "Globals.h"

void drawSunsetScreen(int duration, int brightness) {
  tft.fillScreen(TFT_BLACK);

  // Titel
  tft.setTextColor(TFT_RED);
  tft.setTextSize(2);
  tft.setCursor(80, 10);
  tft.print(F("SUNSET"));

  // Dauer
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 55);
  tft.print(F("Dauer:"));
  tft.setCursor(110, 55);
  tft.print(duration);
  tft.print(F(" min"));
  
  // + und - Buttons für Dauer
  tft.fillRoundRect(200, 50, 50, 50, 8, TFT_DARKGREY);
  tft.fillRoundRect(260, 50, 50, 50, 8, TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(218, 62);
  tft.print(F("+"));
  tft.setCursor(278, 62);
  tft.print(F("-"));

  // Helligkeit
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 115);
  tft.print(F("Max:"));
  tft.setCursor(110, 115);
  tft.print(brightness);
  tft.print(F(" %"));
  
  // + und - Buttons für Helligkeit
  tft.fillRoundRect(200, 110, 50, 50, 8, TFT_DARKGREY);
  tft.fillRoundRect(260, 110, 50, 50, 8, TFT_DARKGREY);
  tft.setTextSize(3);
  tft.setCursor(218, 122);
  tft.print(F("+"));
  tft.setCursor(278, 122);
  tft.print(F("-"));

  // Info-Text
  tft.setTextColor(TFT_LIGHTGREY);
  tft.setTextSize(1);
  tft.setCursor(10, 180);
  tft.print(F("Rotes Licht zum Einschlafen"));
  
  // Hinweis
  tft.setCursor(70, 220);
  tft.print(F("Swipe: Zurueck zu MENU 2"));
}
