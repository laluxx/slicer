#include "window.h"  // Include the header file

// Function to Initialize a Window
Window CreateWindow(Vector2 position, Vector2 size, const char *title) {
    Window window = { position, size, title, true, true, false, false };  // Initialize isEnabled to false
    return window;
}

// Function to Draw the Window
void DrawWindow(Window *window) {
    if (!window->isEnabled || window->isClosed) return;  // Check both isClosed and isEnabled

    // Draw the window frame with 60% transparency
    Color windowColor = { DARKGRAY.r, DARKGRAY.g, DARKGRAY.b, 153 };  // 153 out of 255 is ~60% opacity
    DrawRectangle(window->position.x, window->position.y, window->size.x, window->size.y, windowColor);

    // Draw the title bar with full opacity
    Rectangle titleBarRect = { window->position.x, window->position.y, window->size.x, 20 };  // Assuming a 20 pixel high title bar
    DrawRectangleRec(titleBarRect, DARKGRAY);
    DrawText(window->title, window->position.x + 5, window->position.y + 5, 10, BLACK);

    // Draw close button
    Rectangle closeButtonRect = { window->position.x + window->size.x - 25, window->position.y + 5, 20, 10 };  // Assuming a 20x10 close button
    DrawRectangleRec(closeButtonRect, RED);
    DrawText("X", closeButtonRect.x + 7, closeButtonRect.y + 1, 10, BLACK);
}


// Function to Update the Window
void UpdateWindow(Window *window) {
    if (!window->isEnabled || window->isClosed) return;  // Check both isClosed and isEnabled

    // Movable logic
    if (window->isMovable) {
        Rectangle titleBarRect = { window->position.x, window->position.y, window->size.x, 20 };
        if (CheckCollisionPointRec(GetMousePosition(), titleBarRect) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            window->position.x += GetMouseDelta().x;
            window->position.y += GetMouseDelta().y;
        }
    }

    // Close button logic
    Rectangle closeButtonRect = { window->position.x + window->size.x - 25, window->position.y + 5, 20, 10 };
    if (CheckCollisionPointRec(GetMousePosition(), closeButtonRect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        window->isEnabled = !window->isEnabled;
    }

    // Resizable logic
    // ... (if needed)
}
