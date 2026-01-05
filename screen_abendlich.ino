#include "Globals.h"

void drawAbendlichtScreen(int duration, int brightness) {
  tft.fillScreen(TFT_BLACK);
  
  // Titel
  tft.setTextColor(TFT_RED);
  tft.setTextSize(2);
  tft.setCursor(50, 10);
  tft.print(F("ABENDLICHT"));
  
  // Stunde
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 40);
  tft.print(F("Stunde: "));
  if (sunsetSettings.hour < 10) tft.print("0");
  tft.print(sunsetSettings.hour);
  
  // + und - Buttons für Stunde
  tft.fillRoundRect(200, 35, 50, 40, 8, TFT_DARKGREY);
  tft.fillRoundRect(260, 35, 50, 40, 8, TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(215, 45);
  tft.print(F("+"));
  tft.setCursor(275, 45);
  tft.print(F("-"));
  
  // Minute
  tft.setCursor(20, 90);
  tft.print(F("Minute: "));
  if (sunsetSettings.minute < 10) tft.print("0");
  tft.print(sunsetSettings.minute);
  
  // + und - Buttons für Minute
  tft.fillRoundRect(200, 85, 50, 40, 8, TFT_DARKGREY);
  tft.fillRoundRect(260, 85, 50, 40, 8, TFT_DARKGREY);
  tft.setCursor(215, 95);
  tft.print(F("+"));
  tft.setCursor(275, 95);
  tft.print(F("-"));
  
  // Wochentage
  tft.drawRoundRect(15, 135, 290, 50, 8, TFT_WHITE);
  const char* days[] = {"Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"};
  for (int i = 0; i < 7; i++) {
    int x = 30 + i * 38;
    tft.setCursor(x, 145);
    tft.print(days[i]);
    uint16_t col = sunsetSettings.days[i] ? TFT_RED : TFT_DARKGREY;
    tft.fillRoundRect(x - 5, 165, 30, 30, 5, col);
  }
  
  // AN/AUS-BUTTON - einfach nur AN oder AUS
  uint16_t onCol = sunsetSettings.active ? TFT_RED : TFT_DARKGREY;
  tft.fillRoundRect(80, 200, 140, 40, 8, onCol);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(120, 210);
  tft.print(sunsetSettings.active ? F("AN") : F("AUS"));
}
