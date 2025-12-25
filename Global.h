#ifndef GLOBALS_H
#define GLOBALS_H

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <Adafruit_NeoPixel.h>
#include <RTClib.h>
#include <Wire.h>

// PINS
#define TFT_CS     15
#define TFT_DC     2
#define TFT_RST    4
#define TFT_BL     27
#define T_CS       5
#define LED_DATA   25
#define RTC_SDA    21
#define RTC_SCL    22
#define TOUCH_LEFT   16
#define TOUCH_RIGHT  17

// KONSTANTEN
#define NUM_LEDS 800
#define AUTO_BACK_DELAY 20000
#define TOUCH_THRESHOLD 12
#define LEDS_PER_SIDE 300
#define DEBOUNCE_DELAY 50

// BILDSCHIRME
#define SCREEN_MAIN       0
#define SCREEN_MENU_LIST  1
#define SCREEN_TIME       2
#define SCREEN_DATE       3
#define SCREEN_ALARM      4
#define SCREEN_SUNRISE    5
#define SCREEN_DISPLAY    6

// Objekte
extern TFT_eSPI tft;
extern SPIClass touchSPI;
extern XPT2046_Touchscreen ts;
extern Adafruit_NeoPixel strip;
extern RTC_DS3231 rtc;

// Variablen
extern int16_t touchMinX, touchMaxX, touchMinY, touchMaxY;

extern bool lightOn;
extern unsigned long sunriseStartTime;
extern bool sunriseRunning;
extern bool fastSunrise;

extern int currentScreen;
extern unsigned long lastTouchTime;
extern unsigned long lastScreenTime;

extern bool leftStripOn, rightStripOn;
extern bool lastLeftState, lastRightState;

struct AlarmSettings {
  int hour;
  int minute;
  bool days[7];
  int vorher;
  int sunriseDuration;
  int maxBrightness;
  bool active;
};

struct DisplaySettings {
  int darkHour;
  int darkMinute;
  int brightDurationIndex;
  int alwaysOnHour;
  int alwaysOnMinute;
};

extern AlarmSettings alarmSettings;
extern DisplaySettings displaySettings;

extern const int brightOptions[7];

extern uint32_t sunriseColors[10];

#define WARM_WHITE strip.Color(255, 147, 41)

#endif
