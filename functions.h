#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdbool.h>

typedef struct {
    bool isPressed;
    float initialDelay;  // Delay before the key starts repeating, in seconds
    float repeatRate;    // Time between repeats, in seconds
    float timer;         // Time since the key was first pressed
    float repeatTimer;   // Time since the key was last repeated
} KeyState;

bool IsCustomKeyDown(int key);
void UpdateKeyState(int key);
void InitKeyStates(float initialDelay, float repeatRate);

#endif // FUNCTIONS_H_
