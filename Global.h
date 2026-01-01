#ifndef GLOBALS_H
#define GLOBALS_H
#include <Wire.h>
#include <RTClib.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <Adafruit_NeoPixel.h>
#include <Preferences.h>

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
#define LEDS_PER_SIDE 400
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
extern Preferences preferences;

// Variablen
extern int16_t touchMinX, touchMaxX, touchMinY, touchMaxY;
extern bool lightOn;
extern unsigned long sunriseStartTime;
extern unsigned long screenSunriseStartTime;
extern bool sunriseRunning;
extern bool fastSunrise;
extern int currentScreen;
extern unsigned long lastTouchTime;
extern unsigned long lastScreenTime;
extern bool leftStripOn, rightStripOn;
extern bool lastLeftState, lastRightState;

// NEU: Dimmen
extern int savedBrightness;
extern bool isDimming;
extern unsigned long dimStartTime;
extern int dimDirection;

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

// Funktionsdeklarationen - Main Screen
void drawMainScreen();
void drawLightButton();
void updateMainScreenTime(DateTime now);
uint16_t getSkyColor(int hour, int minute, int y);
uint16_t getGroundColor();
void getMountainColors(uint16_t &backColor, uint16_t &midColor, uint16_t &frontColor, uint16_t &nearestColor);
void drawStars();
void drawStarsWithColor(uint16_t brightColor, uint16_t dimColor);
void drawMountains();

// Funktionsdeklarationen - Andere Screens
void drawMenuScreen();
void drawTimeScreen();
void drawDateScreen();
void drawAlarmScreen();
void drawSunriseScreen(int duration, int brightness);
void drawDisplayScreen();

// Funktionsdeklarationen - Touch
void calibrateTouch();
void loadTouchCalibration();
void saveTouchCalibration();
TS_Point waitForTouch();

// Funktionsdeklarationen - Touch Input
void handleTouchInput(DateTime now);
void handleButtonPress(int x, int y, DateTime now);
void handleButtonRepeat(int x, int y);
void handleDisplayButtons(int x, int y);
void handleTimeButtons(int x, int y);
void handleDateButtons(int x, int y);
void handleAlarmButtons(int x, int y);
void handleSunriseButtons(int x, int y);

// Sensor State Struct
struct SensorState {
  unsigned long pressStartTime;
  bool wasLongPress;
  bool isPressed;
};

// Sensor Data Struct
struct SensorData {
  SensorState state;
  bool isDimming;
  int dimDirection;
  unsigned long lastDimUpdate;
  
  // Click-Tracking
  int clickCount;
  unsigned long firstClickTime;
  bool actionPending;
};

// Funktionsdeklarationen - Side Buttons
void handleSideButtons();
void handleSensor(bool currState, bool lastState, bool &stripOn, int startLED, int endLED, int darkZone, int fadeZone, const char* side, unsigned long &pressTime, unsigned long &releaseTime, unsigned long &firstReleaseTime, bool &isPressed, bool &wasLongPress, bool &isDimming, int &dimDirection, unsigned long &lastDimUpdate, int &clickCount);
void setNightlight(int startLED, int endLED, int darkZone, int fadeZone, int brightness);
void handleBacklight();
void checkAlarmTrigger();

// Funktionsdeklarationen - Nightlight
void loadNightlightSettings();
void saveNightlightSettings();
void updateDimming();

// Funktionsdeklarationen - Sunrise
void updateSunrise();

#endif
