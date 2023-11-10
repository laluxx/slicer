#ifndef KEYCHORDS_H_
#define KEYCHORDS_H_

#include "stdbool.h"
#include <string.h>

#define MAX_KEYCHORD_LENGTH 128
#define LEADER_KEY KEY_SPACE

// Keychord action function type
typedef void (*KeyChordAction)(void);

// Keychord category enumeration
typedef enum {
    CATEGORY_THEME,
    CATEGORY_WINDOW,
    CATEGORY_FILE_MANAGER,
    CATEGORY_QUIT_SESSION,
    // Add more categories as needed
    CATEGORY_COUNT // This keeps track of the total number of categories
} KeyChordCategory;

// Extend the KeyChord structure to include a category
typedef struct {
    const char* keychord;
    KeyChordAction action;
    const char* description; // Description of the keychord
    KeyChordCategory category; // Category of the keychord
} KeyChord;


extern KeyChord keychords[];
extern const int keychordCount;
extern char currentKeyChord[MAX_KEYCHORD_LENGTH];
extern bool leaderKeyActive;

/* void ProcessKeyChord(const char* keys); */
void ProcessKeyChord(const char* keys, char firstChar);
void HandleKeyChords();

#endif // KEYCHORDS_H_
