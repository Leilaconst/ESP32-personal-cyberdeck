#include "NotesApp.h"
#include <string.h>

// ---------------------------------------------------------------------
// Line breaking
// ---------------------------------------------------------------------
// Given the text starting at s, decides how much goes on this line.
// Returns how many characters to DRAW; `consumed` is how many to skip
// afterward (one more than drawn when we break on a space, so the space
// doesn't start the next line).
//
// Rules: if everything fits, take it all. Otherwise break at the last
// space that fits (word wrap). If a single word is longer than the line,
// split it at the line width (hard wrap).
static uint8_t lineBreak(const char *s, uint8_t cpl, uint8_t &consumed) {
  uint8_t len = strlen(s);

  if (len <= cpl) {            // the rest fits on one line
    consumed = len;
    return len;
  }
  if (s[cpl] == ' ') {         // line ends exactly at a word boundary
    consumed = cpl + 1;
    return cpl;
  }
  for (int i = cpl - 1; i > 0; i--) {   // search backward for a space
    if (s[i] == ' ') {
      consumed = i + 1;
      return i;
    }
  }
  consumed = cpl;              // no space found: hard-split the word
  return cpl;
}

// ---------------------------------------------------------------------
// Setup and drawing
// ---------------------------------------------------------------------
void NotesApp::begin(TFT_eSPI *tft) {
  _tft = tft;
  _keyboard.begin(tft);

  _backX = 10;
  _backY = 10;

  _textAreaY = _backY + _backH + 10;
  _textAreaH = _keyboard.top() - _textAreaY - 10;

  // Box interior is (width - 22) px wide; text starts 5 px in, so leave
  // 10 px of total horizontal padding.
  int innerW = _tft->width() - 22 - 10;
  int cpl = innerW / CHAR_W;
  if (cpl < 1) cpl = 1;
  if (cpl > MAX_LINE_CHARS) cpl = MAX_LINE_CHARS;
  _charsPerLine = cpl;

  // Interior height minus top/bottom padding, divided by line pitch.
  int lines = (_textAreaH - 2 - 12) / LINE_H;
  if (lines < 1) lines = 1;
  _maxLines = lines;
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
  // Clear just the inside of the text box.
  _tft->fillRect(11, _textAreaY + 1, _tft->width() - 22, _textAreaH - 2, TFT_BLACK);
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setTextDatum(TL_DATUM); // top-left
  _tft->setTextSize(2);

  const char *text = _keyboard.getText();

  // Pass 1: count how many lines the text needs.
  uint8_t total = 0;
  for (const char *p = text;;) {
    uint8_t consumed;
    lineBreak(p, _charsPerLine, consumed);
    total++;
    p += consumed;
    if (*p == 0) break;
  }

  // If the text is taller than the box, skip the oldest lines so the
  // newest text (where you're typing) stays visible.
  uint8_t skip = (total > _maxLines) ? (total - _maxLines) : 0;

  // Pass 2: draw the visible lines.
  char line[MAX_LINE_CHARS + 1];
  uint8_t row = 0;
  uint8_t index = 0;
  for (const char *p = text;;) {
    uint8_t consumed;
    uint8_t shown = lineBreak(p, _charsPerLine, consumed);

    if (index >= skip) {
      memcpy(line, p, shown);
      line[shown] = 0;
      _tft->drawString(line, 16, _textAreaY + 6 + row * LINE_H);
      row++;
    }
    index++;

    p += consumed;
    if (*p == 0) break;
  }
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
