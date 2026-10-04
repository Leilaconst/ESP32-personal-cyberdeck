#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <TFT_eSPI.h>


// ---------------------------------------------------------------------
// Display
// ---------------------------------------------------------------------
// Physical panel is 320x480. Rotation 0 in TFT_eSPI keeps it in portrait
// (320 wide x 480 tall) for most ILI9488-style drivers — change this if
// your specific panel/driver numbers rotations differently.
#define SCREEN_ROTATION 0

// ---------------------------------------------------------------------
// Touch calibration
// ---------------------------------------------------------------------
// Calibration data is stored in SPIFFS under this file name.
// The SPIFFS file name must start with "/".
#define CALIBRATION_FILE "/TouchCalData5"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

void touch_calibrate(TFT_eSPI &tft);

// Defined once in Config.cpp.
extern uint16_t calData[5];

// ---------------------------------------------------------------------
// On-screen keyboard sizing
// ---------------------------------------------------------------------
// Height, in pixels, reserved at the bottom of the screen for the
// keyboard. The remaining space above it is free for the app's own UI
// (e.g. NotesApp's text box).
#define KEYBOARD_HEIGHT 260

// Maximum characters the keyboard's internal text buffer will hold.
#define KEYBOARD_MAX_LEN 128

#endif // CONFIG_H
