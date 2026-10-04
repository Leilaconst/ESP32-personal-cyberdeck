#include "Keyboard.h"

// Colors — tweak freely to match your device's theme.
#define KB_BG        TFT_BLACK
#define KEY_FILL     TFT_DARKGREY
#define KEY_FILL_HOT TFT_DARKGREEN   // shift/enter/backspace fill
#define KEY_PRESSED  TFT_GREEN
#define KEY_BORDER   TFT_LIGHTGREY
#define KEY_TEXT     TFT_WHITE

void Keyboard::begin(TFT_eSPI *tft) {
  _tft = tft;
  buildLayout();
}

// ---------------------------------------------------------------------
// Layout
// ---------------------------------------------------------------------
// Builds a classic QWERTY grid sized to the display's actual width/height.
//
// Row layout (10 "columns" wide, colW = screen width / 10):
//   Row 0: 1 2 3 4 5 6 7 8 9 0                (10 keys, 1 col each)
//   Row 1: q w e r t y u i o p                (10 keys, 1 col each)
//   Row 2:  a s d f g h j k l                 (9 keys, offset half a col to center)
//   Row 3: [SHIFT 1.5] z x c v b n m [BKSP 1.5]
//   Row 4: [,] [SPACE x6] [.] [ENTER x2]
void Keyboard::buildLayout() {
  int16_t W = _tft->width();
  int16_t H = _tft->height();
  _kbTop = H - KEYBOARD_HEIGHT;

  float colW = W / 10.0f;
  float rowH = KEYBOARD_HEIGHT / 5.0f;
  _keyCount = 0;

  const char *row0 = "1234567890";
  const char *row1 = "qwertyuiop";
  const char *row2 = "asdfghjkl";
  const char *row3 = "zxcvbnm";

  // Row 0: numbers. Shift doesn't change these.
  for (uint8_t i = 0; i < 10; i++) {
    addCharKey(row0[i], row0[i], i * colW, _kbTop + 0 * rowH, colW, rowH);
  }

  // Row 1: qwertyuiop
  for (uint8_t i = 0; i < 10; i++) {
    char c = row1[i];
    addCharKey(c, toupper(c), i * colW, _kbTop + 1 * rowH, colW, rowH);
  }

  // Row 2: asdfghjkl, centered with a half-column offset
  for (uint8_t i = 0; i < 9; i++) {
    char c = row2[i];
    addCharKey(c, toupper(c), colW * 0.5f + i * colW, _kbTop + 2 * rowH, colW, rowH);
  }

  // Row 3: SHIFT, zxcvbnm, BACKSPACE
  float x = 0;
  addSpecialKey(KEY_SHIFT, x, _kbTop + 3 * rowH, colW * 1.5f, rowH);
  x += colW * 1.5f;
  for (uint8_t i = 0; i < 7; i++) {
    char c = row3[i];
    addCharKey(c, toupper(c), x, _kbTop + 3 * rowH, colW, rowH);
    x += colW;
  }
  addSpecialKey(KEY_BACKSPACE, x, _kbTop + 3 * rowH, colW * 1.5f, rowH);

  // Row 4: comma, SPACE (wide), period, ENTER
  x = 0;
  addCharKey(',', ',', x, _kbTop + 4 * rowH, colW, rowH);
  x += colW;
  addSpecialKey(KEY_SPACE, x, _kbTop + 4 * rowH, colW * 6.0f, rowH);
  x += colW * 6.0f;
  addCharKey('.', '.', x, _kbTop + 4 * rowH, colW, rowH);
  x += colW;
  addSpecialKey(KEY_ENTER, x, _kbTop + 4 * rowH, colW * 2.0f, rowH);
}

void Keyboard::addCharKey(char lower, char upper, float x, float y, float w, float h) {
  if (_keyCount >= MAX_KEYS) return;
  KBKey &k = _keys[_keyCount++];
  k.x = (int16_t)round(x);
  k.y = (int16_t)round(y);
  k.w = (int16_t)round(w);
  k.h = (int16_t)round(h);
  k.type = KEY_CHAR;
  k.normalChar = lower;
  k.shiftChar = upper;
}

void Keyboard::addSpecialKey(KeyType type, float x, float y, float w, float h) {
  if (_keyCount >= MAX_KEYS) return;
  KBKey &k = _keys[_keyCount++];
  k.x = (int16_t)round(x);
  k.y = (int16_t)round(y);
  k.w = (int16_t)round(w);
  k.h = (int16_t)round(h);
  k.type = type;
  k.normalChar = 0;
  k.shiftChar = 0;
}

// ---------------------------------------------------------------------
// Drawing
// ---------------------------------------------------------------------
const char *Keyboard::labelFor(const KBKey &key, char labelBuf[3]) const {
  switch (key.type) {
    case KEY_CHAR:
      labelBuf[0] = _shiftActive ? key.shiftChar : key.normalChar;
      labelBuf[1] = 0;
      return labelBuf;
    case KEY_SHIFT:      return _shiftActive ? "^!" : "^";
    case KEY_BACKSPACE:  return "<-";
    case KEY_SPACE:      return "SPACE";
    case KEY_ENTER:      return "OK";
  }
  return "";
}

void Keyboard::drawKey(const KBKey &key) {
  uint16_t fill = (key.type == KEY_CHAR) ? KEY_FILL : KEY_FILL_HOT;
  // Small 2px gap between keys so they read as separate buttons.
  _tft->fillRoundRect(key.x + 2, key.y + 2, key.w - 4, key.h - 4, 6, fill);
  _tft->drawRoundRect(key.x + 2, key.y + 2, key.w - 4, key.h - 4, 6, KEY_BORDER);

  char buf[3];
  const char *label = labelFor(key, buf);
  _tft->setTextColor(KEY_TEXT, fill);
  _tft->setTextDatum(MC_DATUM); // middle-center
  _tft->setTextSize(key.type == KEY_CHAR ? 2 : 1);
  _tft->drawString(label, key.x + key.w / 2, key.y + key.h / 2);
}

void Keyboard::draw() {
  _tft->fillRect(0, _kbTop, _tft->width(), KEYBOARD_HEIGHT, KB_BG);
  for (uint8_t i = 0; i < _keyCount; i++) {
    drawKey(_keys[i]);
  }
}

// ---------------------------------------------------------------------
// Touch handling
// ---------------------------------------------------------------------
// Called by NotesApp once per NEW press: AppManager::update() already
// filters out held-finger frames, so no edge detection is needed here.
bool Keyboard::handleTouch(uint16_t x, uint16_t y) {
  bool hit = false;

  for (uint8_t i = 0; i < _keyCount; i++) {
    KBKey &k = _keys[i];
    // Half-open rectangle: neighbouring keys don't overlap by a pixel.
    if (x < k.x || x >= k.x + k.w || y < k.y || y >= k.y + k.h) continue;

    hit = true;

    // Quick visual feedback: flash the key, then restore it.
    _tft->fillRoundRect(k.x + 2, k.y + 2, k.w - 4, k.h - 4, 6, KEY_PRESSED);
    delay(60);
    drawKey(k); // restores normal fill + label

    switch (k.type) {
      case KEY_CHAR: {
        char c = _shiftActive ? k.shiftChar : k.normalChar;
        if (_bufferLen < KEYBOARD_MAX_LEN) {
          _buffer[_bufferLen++] = c;
          _buffer[_bufferLen] = 0;
        }
        // Auto-unshift after one character, like a phone keyboard.
        if (_shiftActive) {
          _shiftActive = false;
          draw(); // relabel all letter keys back to lowercase
        }
        break;
      }
      case KEY_SHIFT:
        _shiftActive = !_shiftActive;
        draw(); // relabel letter keys for the new shift state
        break;
      case KEY_BACKSPACE:
        if (_bufferLen > 0) {
          _buffer[--_bufferLen] = 0;
        }
        break;
      case KEY_SPACE:
        if (_bufferLen < KEYBOARD_MAX_LEN) {
          _buffer[_bufferLen++] = ' ';
          _buffer[_bufferLen] = 0;
        }
        break;
      case KEY_ENTER:
        _enterFlag = true;
        break;
    }

    break; // stop scanning once we've matched a key
  }

  return hit;
}

void Keyboard::clearText() {
  _bufferLen = 0;
  _buffer[0] = 0;
}

bool Keyboard::enterPressed() {
  bool wasPressed = _enterFlag;
  _enterFlag = false; // reading it clears it — check once per loop
  return wasPressed;
}
