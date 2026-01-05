#include "Globals.h"

void drawMenuScreen2() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  String title = "MENU 2";
  int titleWidth = title.length() * 12;
  tft.setCursor((320 - titleWidth) / 2, 15);
  tft.print(title);

  String items[] = {"ABENDLICHT", "SUNSET", "WIFI"};
  int startY = 42;
  int buttonWidth = 240;
  int buttonHeight = 30;
  int spacing = 8;

  for (int i = 0; i < 3; i++) {
    int y = startY + i * (buttonHeight + spacing);
    int x = (320 - buttonWidth) / 2;
    tft.fillRoundRect(x, y, buttonWidth, buttonHeight, 8, TFT_DARKGREY);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);

    int textWidth = items[i].length() * 12;
    tft.setCursor(x + (buttonWidth - textWidth) / 2, y + 5);
    tft.print(items[i]);
  }

  tft.setTextColor(TFT_LIGHTGREY);
  tft.setTextSize(1);
  tft.setCursor(70, 220);
  tft.print(F("Swipe oben: Zurueck zu MENU"));
}
