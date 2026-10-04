// CyberDeck firmware — main entry point.
//
// This file just brings up the display and touch controller and then
// hands control to AppManager, which owns the home screen and every
// app (see AppManager.h/.cpp to add more apps).

#include <SPI.h>
#include <TFT_eSPI.h>
#include "Config.h"
#include "AppManager.h"

TFT_eSPI tft = TFT_eSPI();
AppManager appManager;
Keyboard keyboard;

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(SCREEN_ROTATION);
  tft.fillScreen(TFT_BLACK);

  touch_calibrate(tft);

  appManager.begin(&tft);
  //keyboard.begin(&tft);
}

void loop() {
  appManager.update();
}
