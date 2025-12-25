#include "Globals.h"

void drawAlarmScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE); tft.setTextSize(2);
  tft.setCursor(20, 10); tft.print(F("WECKER"));
  tft.setCursor(20, 40); tft.print(F("Stunde: "));
  tft.print(alarmSettings.hour);
  tft.fillRoundRect(200, 35, 50, 40, 8, TFT_BLUE);
  tft.setTextColor(TFT_WHITE); tft.setCursor(215, 45); tft.print(F("+"));
  tft.fillRoundRect(260, 35, 50, 40, 8, TFT_BLUE);
  tft.setCursor(275, 45); tft.print(F("-"));
  tft.setCursor(20, 90); tft.print(F("Minute: "));
  tft.print(alarmSettings.minute);
  tft.fillRoundRect(200, 85, 50, 40, 8, TFT_BLUE);
  tft.setCursor(215, 95); tft.print(F("+"));
  tft.fillRoundRect(260, 85, 50, 40, 8, TFT_BLUE);
  tft.setCursor(275, 95); tft.print(F("-"));
  tft.drawRoundRect(15, 135, 290, 50, 8, TFT_WHITE);
  const char* days[] = {"Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"};
  for (int i = 0; i < 7; i++) {
    int x = 30 + i * 38;
    tft.setCursor(x, 145); tft.print(days[i]);
    uint16_t col = alarmSettings.days[i] ? TFT_GREEN : TFT_DARKGREY;
    tft.fillRoundRect(x - 5, 165, 30, 30, 5, col);
  }
  uint16_t onCol = alarmSettings.active ? TFT_GREEN : TFT_RED;
  tft.fillRoundRect(80, 200, 140, 40, 8, onCol);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(120, 210);
  tft.print(alarmSettings.active ? F("AN") : F("AUS"));
  // Zurück-Button entfernt – Navigation per Swipe
}
