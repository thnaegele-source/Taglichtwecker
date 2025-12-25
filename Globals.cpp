#include "Globals.h"

// === OBJEKTE DEFINIEREN ===
TFT_eSPI tft = TFT_eSPI();
SPIClass touchSPI(HSPI);
XPT2046_Touchscreen ts(T_CS);
Adafruit_NeoPixel strip(NUM_LEDS, LED_DATA, NEO_GRB + NEO_KHZ800);
RTC_DS3231 rtc;

// === VARIABLEN DEFINIEREN ===
int16_t touchMinX = 200, touchMaxX = 3900;
int16_t touchMinY = 200, touchMaxY = 3900;

bool lightOn = false;
unsigned long sunriseStartTime = 0;
bool sunriseRunning = false;
bool fastSunrise = false;

int currentScreen = SCREEN_MAIN;
unsigned long lastTouchTime = 0;
unsigned long lastScreenTime = 0;

bool leftStripOn = false;
bool rightStripOn = false;
bool lastLeftState = HIGH;
bool lastRightState = HIGH;

AlarmSettings alarmSettings;
DisplaySettings displaySettings;

const int brightOptions[7] = {15, 20, 30, 45, 60, 90, 120};

uint32_t sunriseColors[10] = {
  strip.Color(25, 0, 50),
  strip.Color(60, 0, 80),
  strip.Color(100, 0, 60),
  strip.Color(140, 10, 40),
  strip.Color(200, 40, 20),
  strip.Color(240, 90, 10),
  strip.Color(255, 140, 0),
  strip.Color(255, 180, 50),
  strip.Color(255, 210, 100),
  strip.Color(255, 235, 180)
};
