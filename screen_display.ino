#include "Globals.h"

void drawDisplayScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 3);
  tft.print(F("DISPLAY EINSTELLUNG"));
  
  // === Dunkel ab === (Y: 28-103)
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 28);
  tft.print(F("Dunkel ab:"));
  
  // Zeit anzeigen
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(10, 48);
  tft.printf("%02d:%02d", displaySettings.darkHour, displaySettings.darkMinute);
  
  // H-Buttons (Stunde)
  tft.setTextSize(1);
  tft.setCursor(140, 51);
  tft.print(F("H"));
  
  tft.fillRoundRect(158, 44, 50, 28, 5, TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(173, 48);
  tft.print(F("+"));
  
  tft.fillRoundRect(215, 44, 50, 28, 5, TFT_BLUE);
  tft.setCursor(230, 48);
  tft.print(F("-"));
  
  // M-Buttons (Minute)
  tft.setTextSize(1);
  tft.setCursor(140, 81);
  tft.print(F("M"));
  
  tft.fillRoundRect(158, 76, 50, 28, 5, TFT_BLUE);
  tft.setTextSize(2);
  tft.setCursor(173, 80);
  tft.print(F("+"));
  
  tft.fillRoundRect(215, 76, 50, 28, 5, TFT_BLUE);
  tft.setCursor(230, 80);
  tft.print(F("-"));
  
  // === Hell ab === (Y: 113-188)
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.setCursor(10, 113);
  tft.print(F("Hell ab:"));
  
  // Zeit anzeigen
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(10, 133);
  tft.printf("%02d:%02d", displaySettings.alwaysOnHour, displaySettings.alwaysOnMinute);
  
  // H-Buttons (Stunde)
  tft.setTextSize(1);
  tft.setCursor(140, 136);
  tft.print(F("H"));
  
  tft.fillRoundRect(158, 129, 50, 28, 5, TFT_BLUE);
  tft.setTextSize(2);
  tft.setCursor(173, 133);
  tft.print(F("+"));
  
  tft.fillRoundRect(215, 129, 50, 28, 5, TFT_BLUE);
  tft.setCursor(230, 133);
  tft.print(F("-"));
  
  // M-Buttons (Minute)
  tft.setTextSize(1);
  tft.setCursor(140, 166);
  tft.print(F("M"));
  
  tft.fillRoundRect(158, 161, 50, 28, 5, TFT_BLUE);
  tft.setTextSize(2);
  tft.setCursor(173, 165);
  tft.print(F("+"));
  
  tft.fillRoundRect(215, 161, 50, 28, 5, TFT_BLUE);
  tft.setCursor(230, 165);
  tft.print(F("-"));
  
  // === Display hell für === (Y: 198-237)
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 198);
  tft.print(F("Display hell:"));
  
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 218);
  tft.printf("%d s", brightOptions[displaySettings.brightDurationIndex]);
  
  // Buttons nebeneinander
  tft.fillRoundRect(158, 214, 50, 28, 5, TFT_BLUE);
  tft.setTextSize(2);
  tft.setCursor(173, 218);
  tft.print(F("+"));
  
  tft.fillRoundRect(215, 214, 50, 28, 5, TFT_BLUE);
  tft.setCursor(230, 218);
  tft.print(F("-"));
  
  // === Touch Kalibrierung Button (rechts, volle Höhe) ===
  tft.fillRoundRect(273, 25, 42, 215, 8, TFT_ORANGE);
  tft.setTextColor(TFT_BLACK);
  
  // Text vertikal zentriert
  tft.setTextSize(1);
  tft.setCursor(282, 85);
  tft.print(F("Touch"));
  tft.setCursor(278, 105);
  tft.print(F("Kalib-"));
  tft.setCursor(275, 125);
  tft.print(F("rierung"));
}
