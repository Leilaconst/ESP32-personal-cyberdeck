#include "HomeScreen.h"

void HomeScreen::draw() {
  int16_t W = _tft->width();
  int16_t H = _tft->height();

  _tft->fillScreen(TFT_BLACK);
  _tft->setTextColor(TFT_GREEN, TFT_BLACK);
  _tft->setTextDatum(TC_DATUM); // top-center
  _tft->setTextSize(3);
  _tft->drawString("CyberDeck", W / 2, 20);

  // One icon for now: a simple labeled box. As more apps are added,
  // lay them out in a grid here and give each its own hitbox + hitTest.
  _iconW = 120;
  _iconH = 100;
  _iconX = (W - _iconW) / 2;
  _iconY = 140;

  _tft->fillRoundRect(_iconX, _iconY, _iconW, _iconH, 10, TFT_DARKGREY);
  _tft->drawRoundRect(_iconX, _iconY, _iconW, _iconH, 10, TFT_LIGHTGREY);
  _tft->setTextColor(TFT_WHITE, TFT_DARKGREY);
  _tft->setTextDatum(MC_DATUM);
  _tft->setTextSize(2);
  _tft->drawString("Notes", _iconX + _iconW / 2, _iconY + _iconH / 2);
}

bool HomeScreen::hitNotesIcon(uint16_t x, uint16_t y) const {
  return x >= _iconX && x <= _iconX + _iconW &&
         y >= _iconY && y <= _iconY + _iconH;
}
