#include "Globals.h"

void drawSunriseScreen(int duration, int maxBrightness) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 10);
  tft.print(F("Sonnenaufgang"));

  // Dauer
  tft.setCursor(20, 60);
  tft.print(F("Dauer: "));
  tft.setTextSize(2);
  tft.setCursor(110, 58);
  tft.printf("%d min", duration);

  tft.fillRoundRect(210, 55, 50, 40, 8, TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(225, 65);
  tft.print(F("+"));

  tft.fillRoundRect(270, 55, 50, 40, 8, TFT_BLUE);
  tft.setCursor(285, 65);
  tft.print(F("-"));

  // Helligkeit
  tft.setTextSize(2);
  tft.setCursor(20, 120);
  tft.print(F("Helligkeit: "));
  tft.setTextSize(2);
  tft.setCursor(150, 118);
  tft.printf("%d%%", maxBrightness);

  tft.fillRoundRect(210, 115, 50, 40, 8, TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(225, 125);
  tft.print(F("+"));

  tft.fillRoundRect(270, 115, 50, 40, 8, TFT_BLUE);
  tft.setCursor(285, 125);
  tft.print(F("-"));
}
