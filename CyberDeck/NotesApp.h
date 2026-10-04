#ifndef NOTES_APP_H
#define NOTES_APP_H

#include <SPI.h>
#include <TFT_eSPI.h>
#include "Keyboard.h"

// A minimal "app": a text box above the on-screen keyboard, plus a back
// button. This is the pattern to copy for future apps — own your slice
// of the screen above the keyboard (or wherever you need), and expose
// a way for AppManager to detect "user wants to leave this app."
class NotesApp {
  public:
    void begin(TFT_eSPI *tft);
    void draw();

    // Feed every NEW touch point here; routes it to the keyboard.
    void handleTouch(uint16_t x, uint16_t y);

    // True if (x, y) is on the back button — checked by AppManager
    // BEFORE calling handleTouch(), so it can switch screens instead.
    bool hitBackButton(uint16_t x, uint16_t y) const;

    // Redraws just the text area, wrapping text onto multiple lines.
    void refreshTextArea();

  private:
    TFT_eSPI *_tft = nullptr;
    Keyboard _keyboard;

    int16_t _backX = 0, _backY = 0, _backW = 60, _backH = 40;
    int16_t _textAreaY = 0, _textAreaH = 0;

    // Text layout, computed in begin() from the screen size.
    // Built-in font 1 is 6x8 px per character; at size 2 that's 12x16.
    static const uint8_t CHAR_W = 12;        // pixel width of one character
    static const uint8_t LINE_H = 18;        // line pitch (16 px glyph + 2 px gap)
    static const uint8_t MAX_LINE_CHARS = 64; // size of the line buffer
    uint8_t _charsPerLine = 1;
    uint8_t _maxLines = 1;
};

#endif // NOTES_APP_H
