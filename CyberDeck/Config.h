#ifndef CONFIG_H
#define CONFIG_H

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
// Run TFT_eSPI's bundled "Touch_calibrate" example once on your actual
// hardware. It prints 5 numbers — paste them here, in order, replacing
// the placeholders below. Skipping this step means touch coordinates
// will be inaccurate or inverted.
static uint16_t calData[5] = { 0, 0, 0, 0, 0 };

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
