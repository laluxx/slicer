#ifndef UI_H
#define UI_H

#include "keychords.h"
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


typedef struct {
    char keys[256];
    int length;
} KeyIndicator;


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




/* // MODELINE && MINIBUFFER */
#define MAX_SCREENKEY_LENGTH 128

typedef struct {
    float height;
    bool isDragging;
    float initialClickOffset;
} Modeline;

typedef struct {
    float height;
    char content[MAX_SCREENKEY_LENGTH];
    float timer;
} Minibuffer;


typedef struct {
    float targetWidth;
    float currentWidth;
    float targetHeight;  // New member
    float currentHeight; // New member
    bool wasActive;
} AltIndicator;


extern Modeline modeline;      // Declares the modeline global variable but does not define it
extern Minibuffer minibuffer;  // Declares the minibuffer global variable but does not define it

void UpdateModelinePosition();
void DrawModeline();
void DrawMiniBuffer();
void UpdateMinibufferSreenKey();
void DrawFPSWidget();
Color ColorLerp(Color a, Color b, float f);
void UpdateAltIndicator();
void DrawAltIndicator();



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
void LoadConfigIconTexture();
void UnloadConfigIconTexture();





#endif  // UI_H
