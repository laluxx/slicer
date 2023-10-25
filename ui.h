#ifndef UI_H
#define UI_H

#include "raylib.h"  // Include this if raylib is used for graphical elements
#include <stdbool.h>  // Include this for the bool type
#include <string.h>
#include <math.h>

typedef struct {
    Rectangle bounds;
    bool active;
    char value[32];
    int cursorPosition;
    double cursorBlinkTime;
} ValueEditor;



// Color Picker
typedef struct ColorPicker {
    Rectangle gradientBox;
    Rectangle spectrumBox;
    Rectangle selectedColorBox;
    Color selectedColor;
    float selectedHue;  // Store selected hue from the spectrum
} ColorPicker;



Color GetColorFromGradient(Rectangle gradientBox, Vector2 position, float hue);  // Passing selected hue as argument
Color GetColorFromSpectrum(Rectangle spectrumBox, Vector2 position);
/* void DrawColorPicker(ColorPicker *colorPicker); */
void DrawColorPicker(ColorPicker *colorPicker, int x, int y, float scale);
void DrawMiniBuffer(int width, float height);
void DrawModeline(int width, float minibufferHeight);






extern int BUTTON_FONT_SIZE;
extern int buttonWidth;
extern int buttonHeight;
extern int buttonSpacing;
extern int buttonYStart;

bool DrawButton(const char *text, Rectangle btnBounds, Color baseColor);
void DrawIncrementerButton(int *value, const char *name, const char *type, int x, int y);
bool DrawToggleButton(Vector2 position, bool currentState, const char *text);
bool DrawToggleButton2(Vector2 position, bool currentState, const char *text);

bool DrawIosToggleButton(Vector2 position, bool currentState);
void UnloadToggleTextures();
void LoadToggleTextures();

#endif  // UI_H
