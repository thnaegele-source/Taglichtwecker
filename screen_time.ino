#include "Globals.h"

void drawTimeScreen() {
  tft.fillScreen(TFT_BLACK);
  DateTime n = rtc.now();

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 10);
  tft.print(F("UHRZEIT EINSTELLEN"));

  // Stunde
  tft.setCursor(20, 60);
  tft.print(F("Stunde: "));
  tft.setTextSize(3);
  tft.setCursor(160, 55);
  tft.printf("%02d", n.hour());

  // + und - für Stunde – weiter links
  tft.fillRoundRect(210, 55, 50, 40, 8, TFT_BLUE);   // +
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(230, 65);
  tft.print(F("+"));

  tft.fillRoundRect(270, 55, 50, 40, 8, TFT_BLUE);   // -
  tft.setCursor(290, 65);
  tft.print(F("-"));

  // Minute
  tft.setTextSize(2);
  tft.setCursor(20, 120);
  tft.print(F("Minute: "));
  tft.setTextSize(3);
  tft.setCursor(160, 115);
  tft.printf("%02d", n.minute());

  // + und - für Minute – weiter links
  tft.fillRoundRect(210, 115, 50, 40, 8, TFT_BLUE);  // +
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(230, 125);
  tft.print(F("+"));

  tft.fillRoundRect(270, 115, 50, 40, 8, TFT_BLUE);  // -
  tft.setCursor(290, 125);
  tft.print(F("-"));

  // Kein Zurück-Button – Navigation per Swipe
}
