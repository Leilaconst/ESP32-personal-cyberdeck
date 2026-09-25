#include "NotesApp.h"

void NotesApp::begin(TFT_eSPI *tft) {
  _tft = tft;
  _keyboard.begin(tft);

  _backX = 10;
  _backY = 10;

  _textAreaY = _backY + _backH + 10;
  _textAreaH = _keyboard.top() - _textAreaY - 10;
}

void NotesApp::draw() {
  _tft->fillScreen(TFT_BLACK);

  // Back button
  _tft->fillRoundRect(_backX, _backY, _backW, _backH, 6, TFT_DARKGREY);
  _tft->drawRoundRect(_backX, _backY, _backW, _backH, 6, TFT_LIGHTGREY);
  _tft->setTextColor(TFT_WHITE, TFT_DARKGREY);
  _tft->setTextDatum(MC_DATUM);
  _tft->setTextSize(2);
  _tft->drawString("<", _backX + _backW / 2, _backY + _backH / 2);

  // Text area border, then the keyboard below it.
  _tft->drawRect(10, _textAreaY, _tft->width() - 20, _textAreaH, TFT_LIGHTGREY);
  refreshTextArea();
  _keyboard.draw();
}

void NotesApp::refreshTextArea() {
  // Clear just the inside of the text box, then redraw the typed text.
  _tft->fillRect(11, _textAreaY + 1, _tft->width() - 22, _textAreaH - 2, TFT_BLACK);
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setTextDatum(TL_DATUM); // top-left
  _tft->setTextSize(2);
  _tft->drawString(_keyboard.getText(), 16, _textAreaY + 8);
}

bool NotesApp::hitBackButton(uint16_t x, uint16_t y) const {
  return x >= _backX && x <= _backX + _backW &&
         y >= _backY && y <= _backY + _backH;
}

void NotesApp::handleTouch(uint16_t x, uint16_t y) {
  if (_keyboard.handleTouch(x, y)) {
    refreshTextArea();
  }
  // ENTER doesn't do anything special here yet (Notes is single-field);
  // future apps (e.g. a chat/console app) can check
  // _keyboard.enterPressed() here to "submit" the typed line.
}
