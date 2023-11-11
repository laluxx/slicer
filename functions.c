#include "raylib.h"

// Function to check if the mouse has moved since the last frame
bool IsMouseMoving() {
    static Vector2 lastMousePos = {0, 0};  // Store the last mouse position
    Vector2 currentMousePos = GetMousePosition();

    // Check if the current mouse position is different from the last frame
    if (currentMousePos.x != lastMousePos.x || currentMousePos.y != lastMousePos.y) {
        lastMousePos = currentMousePos;  // Update the last mouse position
        return true;  // Mouse has moved
    }

    return false;  // Mouse has not moved
}
