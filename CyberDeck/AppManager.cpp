#include "AppManager.h"

void AppManager::begin(TFT_eSPI *tft) {
  _tft = tft;
  _home.begin(tft);
  _notes.begin(tft);
  switchTo(APP_HOME);
}

void AppManager::switchTo(AppID id) {
  _current = id;
  switch (_current) {
    case APP_HOME:  _home.draw();  break;
    case APP_NOTES: _notes.draw(); break;
  }
}

void AppManager::update() {
  uint16_t x, y;
  bool touched = _tft->getTouch(&x, &y);

  // Edge-detect: only react the moment a touch begins. Without this, a
  // finger held down for a few loop() iterations would register as
  // several taps in a row (e.g. typing the same letter repeatedly).
  bool isNewPress = touched && !_wasTouched;
  _wasTouched = touched;

  if (!isNewPress) return;

  switch (_current) {
    case APP_HOME:
      if (_home.hitNotesIcon(x, y)) {
        switchTo(APP_NOTES);
      }
      break;

    case APP_NOTES:
      if (_notes.hitBackButton(x, y)) {
        switchTo(APP_HOME);
      } else {
        _notes.handleTouch(x, y);
      }
      break;
  }
}
