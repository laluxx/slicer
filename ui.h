#ifndef UI_H
#define UI_H

#include "raylib.h"  // Include this if raylib is used for graphical elements
#include <stdbool.h>  // Include this for the bool type
#include <string.h>
extern int BUTTON_FONT_SIZE;
extern int buttonWidth;
extern int buttonHeight;
extern int buttonSpacing;
extern int buttonYStart;

bool DrawButton(const char *text, Rectangle btnBounds, Color baseColor);
void DrawIncrementerButton(int *value, const char *name, const char *type, int x, int y);
bool DrawToggleButton(Vector2 position, bool currentState, const char *text);
bool DrawToggleButton2(Vector2 position, bool currentState, const char *text);
#endif  // UI_H
