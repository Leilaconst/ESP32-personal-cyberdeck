#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include <SPI.h>
#include <TFT_eSPI.h>

// The launcher grid shown when no app is open. Currently has a single
// "Notes" icon — add more icons here as you add more apps.
class HomeScreen {
  public:
    void begin(TFT_eSPI *tft) { _tft = tft; }
    void draw();

    // Returns true if (x, y) landed on the Notes icon.
    bool hitNotesIcon(uint16_t x, uint16_t y) const;

  private:
    TFT_eSPI *_tft = nullptr;

    // Icon hitbox, computed in draw() so it adapts to screen size.
    int16_t _iconX = 0, _iconY = 0, _iconW = 0, _iconH = 0;
};

#endif // HOME_SCREEN_H
