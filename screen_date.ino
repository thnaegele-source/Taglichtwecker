#include "Globals.h"

void drawDateScreen() {
  tft.fillScreen(TFT_BLACK);
  DateTime n = rtc.now();

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 10);
  tft.print(F("DATUM EINSTELLEN"));

  // Tag
  tft.setCursor(20, 60);
  tft.print(F("Tag: "));
  tft.setTextSize(3);
  tft.setCursor(120, 55);  // weiter links
  tft.printf("%02d", n.day());

  // + und - für Tag – rechts
  tft.fillRoundRect(210, 55, 50, 40, 8, TFT_BLUE);   // +
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(230, 65);
  tft.print(F("+"));

  tft.fillRoundRect(270, 55, 50, 40, 8, TFT_BLUE);   // -
  tft.setCursor(290, 65);
  tft.print(F("-"));

  // Monat
  tft.setTextSize(2);
  tft.setCursor(20, 120);
  tft.print(F("Monat: "));
  tft.setTextSize(3);
  tft.setCursor(120, 115);  // weiter links
  tft.printf("%02d", n.month());

  // + und - für Monat – rechts
  tft.fillRoundRect(210, 115, 50, 40, 8, TFT_BLUE);  // +
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(230, 125);
  tft.print(F("+"));

  tft.fillRoundRect(270, 115, 50, 40, 8, TFT_BLUE);  // -
  tft.setCursor(290, 125);
  tft.print(F("-"));

  // Jahr
  tft.setTextSize(2);
  tft.setCursor(20, 180);
  tft.print(F("Jahr: "));
  tft.setTextSize(3);
  tft.setCursor(120, 175);  // weiter links
  tft.printf("%04d", n.year());

  // + und - für Jahr – rechts
  tft.fillRoundRect(210, 175, 50, 40, 8, TFT_BLUE);  // +
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(230, 185);
  tft.print(F("+"));

  tft.fillRoundRect(270, 175, 50, 40, 8, TFT_BLUE);  // -
  tft.setCursor(290, 185);
  tft.print(F("-"));

  // Kein Zurück-Button – Navigation per Swipe
}
