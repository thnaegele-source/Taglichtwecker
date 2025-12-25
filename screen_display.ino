#include "Globals.h"

void drawDisplayScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 10);
  tft.print(F("DISPLAY EINSTELLUNG"));

  // Dunkel ab
  tft.setCursor(20, 60);
  tft.print(F("Dunkel ab: "));
  tft.print(displaySettings.darkHour);

  tft.fillRoundRect(200, 55, 50, 40, 8, TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(215, 65);
  tft.print(F("+"));

  tft.fillRoundRect(260, 55, 50, 40, 8, TFT_BLUE);
  tft.setCursor(275, 65);
  tft.print(F("-"));

  // Hell für
  tft.setTextColor(TFT_RED);
  tft.setCursor(20, 110);
  tft.print(F("Hell fuer: "));
  tft.print(brightOptions[displaySettings.brightDurationIndex]);
  tft.print(F(" s"));

  tft.setTextColor(TFT_WHITE);
  tft.fillRoundRect(200, 105, 50, 40, 8, TFT_BLUE);
  tft.setCursor(215, 115);
  tft.print(F("+"));

  tft.fillRoundRect(260, 105, 50, 40, 8, TFT_BLUE);
  tft.setCursor(275, 115);
  tft.print(F("-"));

  // Hell ab
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(20, 160);
  tft.print(F("Hell ab: "));
  tft.printf("%02d:%02d", displaySettings.alwaysOnHour, displaySettings.alwaysOnMinute);

  tft.setTextColor(TFT_WHITE);
  tft.fillRoundRect(200, 155, 50, 40, 8, TFT_BLUE);
  tft.setCursor(215, 165);
  tft.print(F("+"));

  tft.fillRoundRect(260, 155, 50, 40, 8, TFT_BLUE);
  tft.setCursor(275, 165);
  tft.print(F("-"));

  // KEIN Zurück-Button mehr – nur Swipe zurück
}
