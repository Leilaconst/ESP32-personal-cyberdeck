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

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(SCREEN_ROTATION);
  tft.fillScreen(TFT_BLACK);

  // Applies the calibration numbers from Config.h. If you haven't run
  // TFT_eSPI's Touch_calibrate example yet, touch coordinates will be
  // inaccurate until you do — see README.md.
  tft.setTouch(calData);

  appManager.begin(&tft);
}

void loop() {
  appManager.update();
}
