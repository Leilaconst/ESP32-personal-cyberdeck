#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SPI.h>
#include <TFT_eSPI.h>
#include "Config.h"

// Types of key, used to decide how a tap on that key is handled.
enum KeyType {
  KEY_CHAR,       // a normal letter/number/punctuation key
  KEY_SHIFT,      // toggles upper/lower case for letter keys
  KEY_BACKSPACE,  // deletes the last character in the buffer
  KEY_SPACE,      // inserts a space
  KEY_ENTER       // signals "done typing" to whoever owns the keyboard
};

// One key's on-screen hitbox plus what it types.
struct KBKey {
  int16_t x, y, w, h;   // pixel rectangle, top-left + size
  KeyType type;
  char normalChar;      // character produced when shift is off (KEY_CHAR only)
  char shiftChar;        // character produced when shift is on  (KEY_CHAR only)
};

// A reusable on-screen QWERTY keyboard. Call begin() once with a pointer
// to an already-initialized TFT_eSPI instance, draw() to render it, and
// handleTouch() every time tft.getTouch() reports a press. The typed
// text accumulates in an internal buffer, readable via getText().
class Keyboard {
  public:
    void begin(TFT_eSPI *tft);

    // Draws the full keyboard at the bottom of the screen.
    void draw();

    // Feed this a raw touch point (screen coordinates). Returns true if
    // the point landed on a key (so the caller knows something happened).
    bool handleTouch(uint16_t x, uint16_t y);

    // The text typed so far.
    const char *getText() const { return _buffer; }

    // Clears the typed text buffer (does not redraw — call draw() after
    // if you also want the on-screen text area, which lives outside
    // this class, refreshed).
    void clearText();

    // True exactly once, the frame after ENTER is tapped; reading it
    // clears the flag, so check it once per loop.
    bool enterPressed();

    // Where the keyboard starts vertically, so callers know how much
    // screen space above it is free for their own UI.
    int16_t top() const { return _kbTop; }

  private:
    TFT_eSPI *_tft = nullptr;

    static const uint8_t MAX_KEYS = 40;
    KBKey _keys[MAX_KEYS];
    uint8_t _keyCount = 0;

    int16_t _kbTop = 0;
    bool _shiftActive = false;
    bool _wasTouched = false;   // edge-detection so a held finger doesn't repeat

    char _buffer[KEYBOARD_MAX_LEN + 1] = {0};
    uint8_t _bufferLen = 0;
    bool _enterFlag = false;

    void buildLayout();
    void addCharKey(char lower, char upper, float x, float y, float w, float h);
    void addSpecialKey(KeyType type, float x, float y, float w, float h);
    void drawKey(const KBKey &key);
    const char *labelFor(const KBKey &key, char labelBuf[3]) const;
};

#endif // KEYBOARD_H
