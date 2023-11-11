#include "raylib.h"
#include "functions.h"
// Where i extend raylib

// TODO
#define MAX_KEYS 512  // Total number of keys to track
KeyState keyStates[MAX_KEYS];

// Initialize all key states
void InitKeyStates(float initialDelay, float repeatRate) {
    for (int i = 0; i < MAX_KEYS; i++) {
        keyStates[i].isPressed = false;
        keyStates[i].initialDelay = initialDelay;
        keyStates[i].repeatRate = repeatRate;
        keyStates[i].timer = 0.0f;
        keyStates[i].repeatTimer = 0.0f;
    }
}

// Update the state of a specific key
void UpdateKeyState(int key) {
    if (IsKeyDown(key)) {
        if (!keyStates[key].isPressed) {
            keyStates[key].isPressed = true;
            keyStates[key].timer = 0.0f;
            keyStates[key].repeatTimer = 0.0f;
        } else {
            keyStates[key].timer += GetFrameTime();
            keyStates[key].repeatTimer += GetFrameTime();
        }
    } else {
        keyStates[key].isPressed = false;
    }
}

// Check if the key should be considered "down" based on custom logic
bool IsCustomKeyDown(int key) {
    if (keyStates[key].isPressed) {
        if (keyStates[key].timer < keyStates[key].initialDelay) {
            return true;  // Key is freshly pressed
        } else if (keyStates[key].repeatTimer >= keyStates[key].repeatRate) {
            keyStates[key].repeatTimer = 0.0f;  // Reset repeat timer
            return true;  // Key is repeating
        }
    }
    return false;
}

