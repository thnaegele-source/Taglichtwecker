#include "Globals.h"

TFT_eSPI tft = TFT_eSPI();
SPIClass touchSPI = SPIClass(VSPI);
XPT2046_Touchscreen ts(T_CS);
Adafruit_NeoPixel strip(NUM_LEDS, LED_DATA, NEO_GRB + NEO_KHZ800);
RTC_DS3231 rtc;
Preferences preferences;

int16_t touchMinX = 300, touchMaxX = 3700, touchMinY = 300, touchMaxY = 3700;
bool lightOn = false;
unsigned long sunriseStartTime = 0;
unsigned long screenSunriseStartTime = 0;
bool sunriseRunning = false;
bool fastSunrise = false;
int currentScreen = SCREEN_MAIN;
unsigned long lastTouchTime = 0;
unsigned long lastScreenTime = 0;
bool leftStripOn = false, rightStripOn = false;
bool lastLeftState = HIGH, lastRightState = HIGH;

// NEU: Dimmen & Doppelklick
int savedBrightness = 60;
bool isDimming = false;
unsigned long dimStartTime = 0;
int dimDirection = 1;
unsigned long lastLeftClick = 0;
unsigned long lastRightClick = 0;
int leftClickCount = 0;
int rightClickCount = 0;

AlarmSettings alarmSettings;
DisplaySettings displaySettings;

const int brightOptions[7] = {5, 10, 15, 30, 60, 120, 240};

uint32_t sunriseColors[10] = {
  strip.Color(0, 0, 0),
  strip.Color(10, 0, 0),
  strip.Color(30, 5, 0),
  strip.Color(60, 10, 0),
  strip.Color(100, 20, 0),
  strip.Color(150, 40, 5),
  strip.Color(200, 80, 10),
  strip.Color(255, 120, 20),
  strip.Color(255, 147, 41),
  strip.Color(255, 200, 100)
};
