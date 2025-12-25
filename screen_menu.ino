#include "Globals.h"

void drawMenuScreen() {
  tft.fillScreen(TFT_BLACK);

  // Titel "MENU" mittig
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  String title = "MENU";
  int titleWidth = title.length() * 12;
  tft.setCursor((320 - titleWidth) / 2, 15);
  tft.print(title);

  // Menüpunkte – minimaler Abstand zum Titel, kompakter
  String items[] = {"UHRZEIT", "DATUM", "WECKER", "SUNRISE", "DISPLAY"};
  int startY = 42;          // sehr nah am Titel – kleiner Abstand
  int buttonWidth = 240;
  int buttonHeight = 30;    // etwas niedriger
  int spacing = 8;          // kleiner Abstand zwischen Buttons

  for (int i = 0; i < 5; i++) {
    int y = startY + i * (buttonHeight + spacing);
    int x = (320 - buttonWidth) / 2;
    tft.fillRoundRect(x, y, buttonWidth, buttonHeight, 8, TFT_DARKGREY);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);

    int textWidth = items[i].length() * 12;
    tft.setCursor(x + (buttonWidth - textWidth) / 2, y + 5);
    tft.print(items[i]);
  }

  // Kein Zurück-Button – Swipe only!
}
