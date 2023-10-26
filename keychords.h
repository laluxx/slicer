#ifndef KEYCHORDS_H
#define KEYCHORDS_H

#include <stdbool.h>

typedef void (*KeyChordAction)(void);

// Initialize the keychord system
void KeyChordSystem_Init();

// Update the keychord system, to be called in your game's main loop
void KeyChordSystem_Update();

// Handle a key press
void KeyChordSystem_HandleKeyPress(char key);

// Handle a key release
void KeyChordSystem_HandleKeyRelease(char key);

#endif // KEYCHORDS_H
