#ifndef WINDOW_H
#define WINDOW_H

#include "raylib.h"  // Assuming raylib is being used, adjust include as necessary

// Window Struct Definition
typedef struct Window {
    Vector2 position;
    Vector2 size;
    const char *title;
    bool isMovable;
    bool isResizable;
    bool isClosed;
    bool isEnabled;
} Window;

// Function Prototypes
Window CreateWindow(Vector2 position, Vector2 size, const char *title);
void DrawWindow(Window *window);
void UpdateWindow(Window *window);

#endif  // WINDOW_H
