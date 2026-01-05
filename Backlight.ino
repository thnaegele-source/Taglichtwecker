#include "Globals.h"

#define LONG_PRESS_LOOPS 20
#define DOUBLE_CLICK_TIME 500
#define DIM_SPEED 2
#define DIM_UPDATE_INTERVAL 100

// Links
static unsigned long leftReleaseTime = 0;
static unsigned long leftFirstReleaseTime = 0;
static bool leftWasPressed = false;
static int leftPressLoops = 0;
static bool leftIsDimming = false;
static int leftDimDirection = 1;
static unsigned long leftLastDimUpdate = 0;
static int leftClickCount = 0;

// Rechts
static unsigned long rightReleaseTime = 0;
static unsigned long rightFirstReleaseTime = 0;
static bool rightWasPressed = false;
static int rightPressLoops = 0;
static bool rightIsDimming = false;
static int rightDimDirection = 1;
static unsigned long rightLastDimUpdate = 0;
static int rightClickCount = 0;

void handleSideButtons() {
  int darkZoneLEDs = 32;
  int fadeZoneLEDs = 48;
  
  bool currLeft = digitalRead(TOUCH_LEFT);
  handleSensor(
    currLeft, leftWasPressed,
    leftStripOn,
    0, LEDS_PER_SIDE,
    darkZoneLEDs, fadeZoneLEDs,
    "Links",
    leftReleaseTime, leftFirstReleaseTime,
    leftPressLoops,
    leftIsDimming, leftDimDirection, leftLastDimUpdate, leftClickCount,
    TOUCH_LEFT
  );
  leftWasPressed = currLeft;
  
  bool currRight = digitalRead(TOUCH_RIGHT);
  handleSensor(
    currRight, rightWasPressed,
    rightStripOn,
    LEDS_PER_SIDE, NUM_LEDS,
    darkZoneLEDs, fadeZoneLEDs,
    "Rechts",
    rightReleaseTime, rightFirstReleaseTime,
    rightPressLoops,
    rightIsDimming, rightDimDirection, rightLastDimUpdate, rightClickCount,
    TOUCH_RIGHT
  );
  rightWasPressed = currRight;
  
  lastLeftState = currLeft;
  lastRightState = currRight;
}

void handleSensor(
  bool currState, bool wasPressed,
  bool &stripOn,
  int startLED, int endLED,
  int darkZone, int fadeZone,
  const char* side,
  unsigned long &releaseTime, unsigned long &firstReleaseTime,
  int &pressLoops,
  bool &isDimming, int &dimDirection, unsigned long &lastDimUpdate,
  int &clickCount,
  int pin
) {
  
  // === BUTTON GEDRÜCKT ===
  if (currState == HIGH && wasPressed == LOW) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(pin) == HIGH) {
      pressLoops = 0;
      Serial.printf("%s: GEDRUECKT\n", side);
    }
  }
  
  // === BUTTON GEHALTEN ===
  if (currState == HIGH && wasPressed == HIGH) {
    pressLoops++;
    
    if (pressLoops % 10 == 0) {
      Serial.printf("%s: Gehalten (Loops: %d)\n", side, pressLoops);
    }
    
    if (pressLoops >= LONG_PRESS_LOOPS && !isDimming) {
      if (stripOn && !sunriseRunning) {
        isDimming = true;
        dimDirection = 1;
        lastDimUpdate = millis();
        Serial.printf("%s: DIMMEN START\n", side);
      }
    }
    
    if (isDimming) {
      if (millis() - lastDimUpdate >= DIM_UPDATE_INTERVAL) {
        lastDimUpdate = millis();
        
        savedBrightness += (dimDirection * DIM_SPEED);
        
        if (savedBrightness >= 100) {
          savedBrightness = 100;
          dimDirection = -1;
        } else if (savedBrightness <= 10) {
          savedBrightness = 10;
          dimDirection = 1;
        }
        
        setNightlight(startLED, endLED, darkZone, fadeZone, savedBrightness);
      }
    }
  }
  
  // === BUTTON LOSGELASSEN ===
  if (currState == LOW && wasPressed == HIGH) {
    releaseTime = millis();
    Serial.printf("%s: LOSGELASSEN (Loops: %d)\n", side, pressLoops);
    
    if (pressLoops >= LONG_PRESS_LOOPS) {
      Serial.printf("%s: War LANGES DRUECKEN\n", side);
      
      if (isDimming) {
        isDimming = false;
        saveNightlightSettings();
        Serial.printf("%s: DIMMEN STOPP\n", side);
      }
      
      clickCount = 0;
      pressLoops = 0;
      return;
    }
    
    Serial.printf("%s: War KURZES DRUECKEN\n", side);
    pressLoops = 0;
    
    if (isDimming) {
      isDimming = false;
      Serial.printf("%s: DIMMEN abgebrochen\n", side);
    }
    
    if (clickCount == 1 && (releaseTime - firstReleaseTime < DOUBLE_CLICK_TIME)) {
      Serial.printf("%s: DOPPELKLICK!\n", side);
      clickCount = 0;
      
      if (sunriseRunning) {
        sunriseRunning = false;
        screenSunriseStartTime = 0;
        lightOn = false;
        
        // LEDs komplett AUS
        for (int i = 0; i < NUM_LEDS; i++) {
          strip.setPixelColor(i, 0);
        }
        strip.show();
        Serial.printf("%s: Sonnenaufgang STOPP - ALLE LEDs AUS\n", side);
        
        // WICHTIG: Button auf Bildschirm aktualisieren!
        if (currentScreen == SCREEN_MAIN) {
          drawLightButton();
        }
      } else {
        // Nachtlichter AUS
        leftStripOn = false;
        rightStripOn = false;
        stripOn = false;
        
        // ALLE LEDs erstmal AUS
        for (int i = 0; i < NUM_LEDS; i++) {
          strip.setPixelColor(i, 0);
        }
        strip.show();
        
        sunriseRunning = true;
        lightOn = true;
        fastSunrise = false;
        sunriseStartTime = millis();
        screenSunriseStartTime = millis();
        strip.setBrightness(1);
        Serial.printf("%s: Sonnenaufgang START\n", side);
        
        // WICHTIG: Button auf Bildschirm aktualisieren!
        if (currentScreen == SCREEN_MAIN) {
          drawLightButton();
        }
      }
      return;
    }
    
    clickCount = 1;
    firstReleaseTime = releaseTime;
    Serial.printf("%s: Click 1 registriert\n", side);
  }
  
  // === BUTTON NICHT GEDRÜCKT ===
  if (currState == LOW && wasPressed == LOW) {
    if (pressLoops > 0) {
      pressLoops = 0;
    }
    
    // === EINZELKLICK-TIMEOUT ===
    if (clickCount == 1 && (millis() - firstReleaseTime >= DOUBLE_CLICK_TIME)) {
      Serial.printf("%s: EINZELKLICK! stripOn war %d, setze auf %d\n", side, stripOn, !stripOn);
      clickCount = 0;
      
      if (!sunriseRunning) {
        stripOn = !stripOn;
        
        if (stripOn) {
          Serial.printf("%s: RUFE setNightlight() - LEDs %d bis %d, Helligkeit %d\n", side, startLED, endLED, savedBrightness);
          setNightlight(startLED, endLED, darkZone, fadeZone, savedBrightness);
          Serial.printf("%s: NACH setNightlight() - stripOn=%d\n", side, stripOn);
        } else {
          Serial.printf("%s: Schalte LEDs %d bis %d AUS\n", side, startLED, endLED);
          for (int i = startLED; i < endLED; i++) {
            strip.setPixelColor(i, 0);
          }
          strip.show();
          Serial.printf("%s: LEDs AUS - stripOn=%d\n", side, stripOn);
        }
      } else {
        Serial.printf("%s: Einzelklick ignoriert - Sonnenaufgang läuft\n", side);
      }
    }
  }
}

void setNightlight(int startLED, int endLED, int darkZone, int fadeZone, int brightness) {
  Serial.printf("setNightlight() GESTARTET: LEDs %d-%d, darkZone=%d, fadeZone=%d, brightness=%d\n", 
                startLED, endLED, darkZone, fadeZone, brightness);
  
  int ledsSet = 0;
  
  for (int i = startLED; i < endLED; i++) {
    int distanceFromCenter = (i < LEDS_PER_SIDE) ? 
                             (LEDS_PER_SIDE - i) : 
                             (i - LEDS_PER_SIDE);
    
    if (distanceFromCenter <= darkZone) {
      strip.setPixelColor(i, 0);
    } else if (distanceFromCenter <= darkZone + fadeZone) {
      int fadePos = distanceFromCenter - darkZone;
      float fadeFactor = (float)fadePos / fadeZone;
      
      uint8_t r = (uint8_t)(255 * fadeFactor);
      uint8_t g = (uint8_t)(147 * fadeFactor);
      uint8_t b = (uint8_t)(41 * fadeFactor);
      
      strip.setPixelColor(i, strip.Color(r, g, b));
      ledsSet++;
    } else {
      strip.setPixelColor(i, WARM_WHITE);
      ledsSet++;
    }
  }
  
  strip.setBrightness(brightness);
  strip.show();
  
  Serial.printf("setNightlight() FERTIG: %d LEDs gesetzt, brightness=%d\n", ledsSet, brightness);
}

void updateDimming() {
}

void loadNightlightSettings() {
  preferences.begin("nightlight", true);
  savedBrightness = preferences.getInt("brightness", 60);
  preferences.end();
  Serial.printf("Helligkeit geladen: %d\n", savedBrightness);
}

void saveNightlightSettings() {
  preferences.begin("nightlight", false);
  preferences.putInt("brightness", savedBrightness);
  preferences.end();
  Serial.printf("Helligkeit gespeichert: %d\n", savedBrightness);
}

static bool backlightWasOn = true;

void handleBacklight() {
  DateTime now = rtc.now();
  
  int curMin = now.hour() * 60 + now.minute();
  int darkMin = displaySettings.darkHour * 60 + displaySettings.darkMinute;
  int alwaysMin = displaySettings.alwaysOnHour * 60 + displaySettings.alwaysOnMinute;
  int brightSec = brightOptions[displaySettings.brightDurationIndex];
  
  bool shouldOn;
  
  if (alwaysMin < darkMin) {
    shouldOn = (curMin >= alwaysMin && curMin < darkMin);
  } else {
    shouldOn = (curMin >= alwaysMin || curMin < darkMin);
  }
  
  if (millis() - lastTouchTime < brightSec * 1000L) {
    shouldOn = true;
  }
  
  if (shouldOn != backlightWasOn) {
    backlightWasOn = shouldOn;
    digitalWrite(TFT_BL, shouldOn ? HIGH : LOW);
    
    if (shouldOn) {
      if (currentScreen == SCREEN_MAIN) {
        drawMainScreen();
      } else if (currentScreen == SCREEN_MENU_LIST) {
        drawMenuScreen();
      }
    }
  }
}

void checkAlarmTrigger() {
  DateTime now = rtc.now();
  
  static bool alarmTriggeredThisMinute = false;
  static int lastMinute = -1;
  
  if (now.minute() != lastMinute) {
    alarmTriggeredThisMinute = false;
    lastMinute = now.minute();
  }
  
  if (!alarmSettings.active || alarmTriggeredThisMinute) return;
  
  int adjustedDay = now.dayOfTheWeek() - 1;
  if (adjustedDay < 0) adjustedDay = 6;
  
  if (!alarmSettings.days[adjustedDay]) return;
  
  int wakeMin = alarmSettings.minute - alarmSettings.vorher;
  int wakeHr = alarmSettings.hour;
  if (wakeMin < 0) { 
    wakeMin += 60; 
    wakeHr = (wakeHr - 1 + 24) % 24; 
  }
  
  if (now.hour() == wakeHr && now.minute() == wakeMin && !sunriseRunning) {
    Serial.printf("WECKER AUSGELÖST! %02d:%02d\n", now.hour(), now.minute());
    
    leftStripOn = false;
    rightStripOn = false;
    
    lightOn = true;
    fastSunrise = false;
    sunriseStartTime = millis();
    screenSunriseStartTime = millis();
    sunriseRunning = true;
    strip.setBrightness(1);
    strip.clear();
    strip.show();
    alarmTriggeredThisMinute = true;
    drawLightButton();
  }
}

void checkSunsetTrigger() {
  DateTime now = rtc.now();
  
  static bool sunsetTriggeredThisMinute = false;
  static int lastMinute = -1;
  
  if (now.minute() != lastMinute) {
    sunsetTriggeredThisMinute = false;
    lastMinute = now.minute();
  }
  
  if (!sunsetSettings.active || sunsetTriggeredThisMinute) return;
  
  int adjustedDay = now.dayOfTheWeek() - 1;
  if (adjustedDay < 0) adjustedDay = 6;
  
  if (!sunsetSettings.days[adjustedDay]) return;
  
  if (now.hour() == sunsetSettings.hour && now.minute() == sunsetSettings.minute && !sunsetRunning) {
    Serial.printf("SUNSET AUSGELÖST! %02d:%02d\n", now.hour(), now.minute());
    
    // Andere Lichtquellen ausschalten
    leftStripOn = false;
    rightStripOn = false;
    sunriseRunning = false;
    
    // Sunset starten
    sunsetRunning = true;
    sunsetStartTime = millis();
    screenSunsetStartTime = millis();
    strip.setBrightness(1);
    strip.clear();
    strip.show();
    sunsetTriggeredThisMinute = true;
    
    // UI aktualisieren
    if (currentScreen == SCREEN_MAIN) {
      drawSunsetButton();
      drawStatusBetweenButtons();
    }
  }
}
