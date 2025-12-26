#include "Globals.h"

void drawDisplayScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 5);
  tft.print(F("DISPLAY EINSTELLUNG"));
  
  // === Dunkel ab ===
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
  
  // M-Buttons (Minute) - mit 3px Abstand
  tft.setTextSize(1);
  tft.setCursor(140, 80);
  tft.print(F("M"));
  
  tft.fillRoundRect(158, 75, 50, 28, 5, TFT_BLUE);
  tft.setTextSize(2);
  tft.setCursor(173, 79);
  tft.print(F("+"));
  
  tft.fillRoundRect(215, 75, 50, 28, 5, TFT_BLUE);
  tft.setCursor(230, 79);
  tft.print(F("-"));
  
  // === Hell ab ===
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
  
  // M-Buttons (Minute) - mit 3px Abstand
  tft.setTextSize(1);
  tft.setCursor(140, 165);
  tft.print(F("M"));
  
  tft.fillRoundRect(158, 160, 50, 28, 5, TFT_BLUE);
  tft.setTextSize(2);
  tft.setCursor(173, 164);
  tft.print(F("+"));
  
  tft.fillRoundRect(215, 160, 50, 28, 5, TFT_BLUE);
  tft.setCursor(230, 164);
  tft.print(F("-"));
  
  // === Display hell für ===
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 195);
  tft.print(F("Display hell:"));
  
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 215);
  tft.printf("%d s", brightOptions[displaySettings.brightDurationIndex]);
  
  // Buttons nebeneinander
  tft.fillRoundRect(158, 211, 50, 28, 5, TFT_BLUE);
  tft.setTextSize(2);
  tft.setCursor(173, 215);
  tft.print(F("+"));
  
  tft.fillRoundRect(215, 211, 50, 28, 5, TFT_BLUE);
  tft.setCursor(230, 215);
  tft.print(F("-"));
}
