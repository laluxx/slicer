#include "keychords.h"
#include "theme.h"
#include <string.h>

#define MAX_SEQUENCE_LENGTH 10
#define MAX_KEYCHORDS 10

typedef struct {
    char sequence[MAX_SEQUENCE_LENGTH];
    KeyChordAction action;
} KeyChord;

// An array of keychords, you can extend this to fit more keychords
static KeyChord keyChords[MAX_KEYCHORDS] = {
    { "spchtn", &NextTheme },
    { "spchtp", &PreviousTheme }
};

static char currentSequence[MAX_SEQUENCE_LENGTH] = {0};
static bool isSequenceActive = false;

void KeyChordSystem_Init() {
    memset(currentSequence, 0, sizeof(currentSequence));
}

void KeyChordSystem_Update() {
    if (!isSequenceActive) return;

    for (int i = 0; i < MAX_KEYCHORDS; ++i) {
        if (strcmp(currentSequence, keyChords[i].sequence) == 0) {
            keyChords[i].action();
            memset(currentSequence, 0, sizeof(currentSequence));
            isSequenceActive = false;
            break;
        }
    }
}

void KeyChordSystem_HandleKeyPress(char key) {
    int len = strlen(currentSequence);
    if (len < MAX_SEQUENCE_LENGTH - 1) {
        currentSequence[len] = key;
        currentSequence[len + 1] = '\0';
    }

    if (key == ' ') {
        isSequenceActive = true;
    }
}

void KeyChordSystem_HandleKeyRelease(char key) {
    if (key == ' ' && !isSequenceActive) {
        memset(currentSequence, 0, sizeof(currentSequence));
    }
}
