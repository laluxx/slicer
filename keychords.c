// KEYCHORDS
#include "keychords.h"
#include <string.h>
#include <raylib.h>
#include "panels.h"
#include "theme.h"
#include "commands.h"

// Keychord registry with categories
KeyChord keychords[] = {
    {"tn", NextTheme, "Switch to the next theme", CATEGORY_THEME},
    {"tp", PreviousTheme, "Switch to the previous theme", CATEGORY_THEME},
    {"qr", restart_slicer, "Restart slicer", CATEGORY_QUIT_SESSION},
    {"wc", CloseSelectedFrame, "Window close", CATEGORY_WINDOW},
    {"ww", window_next, "Window next", CATEGORY_WINDOW},
    {"fm", toggle_filemanager, "Toggle filemanager", CATEGORY_FILE_MANAGER},
    // Add more keychords as needed
};

const int keychordCount = sizeof(keychords) / sizeof(KeyChord);

// Current keychord buffer and leader key state
char currentKeyChord[MAX_KEYCHORD_LENGTH] = {0};
bool leaderKeyActive = false;
bool keychordExecuted = 0;


void ProcessKeyChord(const char* keys, char firstChar) {
    bool matchFound = false;
    bool possibleMatchesRemaining = false;

    for (int i = 0; i < keychordCount; ++i) {
        if (keychords[i].keychord[0] == firstChar) {
            if (strncmp(keys, keychords[i].keychord, strlen(keys)) == 0) {
                if (strlen(keys) == strlen(keychords[i].keychord)) {
                    keychords[i].action(); // Execute the associated action
                    matchFound = true;
                    keychordExecuted = true; // Set the flag to indicate successful execution
                    break; // We found a match, so we can break out of the loop
                } else {
                    possibleMatchesRemaining = true;
                }
            }
        }
    }

    // If a matching keychord is found or no more possible matches, deactivate the leader key mode
    if (matchFound || !possibleMatchesRemaining) {
        leaderKeyActive = false;
        currentKeyChord[0] = '\0'; // Clear the current keychord
    }
}



void HandleKeyChords() {
    static char firstChordChar = '\0'; // To store the first character after the leader key

    if (IsKeyPressed(LEADER_KEY)) {
        leaderKeyActive = true;
        firstChordChar = '\0'; // Reset the first character
        currentKeyChord[0] = '\0';
    } else if (leaderKeyActive) {
        int key = GetCharPressed();
        if ((key >= 32) && (key <= 125)) {
            char keyStr[2] = {(char)key, '\0'};
            strcat(currentKeyChord, keyStr);

            // If it's the first character after the leader key, store it
            if (strlen(currentKeyChord) == 1) {
                firstChordChar = keyStr[0];
            }

            // Now we can process the current buffer
            ProcessKeyChord(currentKeyChord, firstChordChar); // Pass the first chord character
            // Reset if a complete keychord was processed
            if (!leaderKeyActive) {
                firstChordChar = '\0'; // Clear the first character
            }
        }
    }
}
