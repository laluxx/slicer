#ifndef KEYCHORDS_H_
#define KEYCHORDS_H_

#include "stdbool.h"

#define MAX_KEYCHORD_LENGTH 128
#define LEADER_KEY KEY_SPACE

// Keychord action function type
typedef void (*KeyChordAction)(void);

// Keychord structure
typedef struct {
    const char* keychord;
    KeyChordAction action;
    const char* description; // Description of the keychord
} KeyChord;


extern bool leaderKeyActive;

/* void ProcessKeyChord(const char* keys); */
void ProcessKeyChord(const char* keys, char firstChar);
void HandleKeyChords();

#endif // KEYCHORDS_H_
