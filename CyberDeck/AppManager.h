#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <SPI.h>
#include <TFT_eSPI.h>
#include "HomeScreen.h"
#include "NotesApp.h"

// Add one entry here per app, and a matching case in AppManager.cpp's
// update()/switchTo() switch statements.
enum AppID {
  APP_HOME,
  APP_NOTES
};

// Owns every screen/app and decides which one is currently active,
// routing touch input to it and handling "go back to home" requests.
class AppManager {
  public:
    void begin(TFT_eSPI *tft);
    void update(); // call once per loop()

  private:
    TFT_eSPI *_tft = nullptr;
    AppID _current = APP_HOME;
    bool _wasTouched = false; // edge-detection across the whole app switcher

    HomeScreen _home;
    NotesApp _notes;

    void switchTo(AppID id);
};

#endif // APP_MANAGER_H
