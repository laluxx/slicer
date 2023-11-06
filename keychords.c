// KEYCHORDS
#include "keychords.h"
#include <string.h>
#include <raylib.h>
#include "panels.h"
#include "theme.h"
#include "functions.h"


// Keychord registry
KeyChord keychords[] = {
    {"tn",  NextTheme, "Switch to the next theme"},
    {"tp",  PreviousTheme, "Switch to the previous theme"},
    {"qr",  restart_slicer, "Restart slicer"},
    {"ofl", OpenLeftFlexiblePanel, "Open left flex panel"},
    {"ofr", OpenRightFlexiblePanel, "Open left flex panel"},
    {"oft", OpenTopFlexiblePanel, "Open left flex panel"},
    {"ofb", OpenBottomFlexiblePanel, "Open left flex panel"},
    // Add more keychords as needed
};
const int keychordCount = sizeof(keychords) / sizeof(KeyChord);

// Current keychord buffer and leader key state
char currentKeyChord[MAX_KEYCHORD_LENGTH] = {0};
bool leaderKeyActive = false;

void ProcessKeyChord(const char* keys, char firstChar) {
    // A flag to keep track of whether we found a matching keychord
    bool matchFound = false;

    // A flag to determine if there are any possible matches remaining with the current sequence
    bool possibleMatchesRemaining = false;

    // Check only if the keychord starts with the given firstChar
    for (int i = 0; i < keychordCount; ++i) {
        if (keychords[i].keychord[0] == firstChar) {
            // If the current key sequence matches the beginning of a keychord,
            // then there is a possibility of a match with further key presses
            if (strncmp(keys, keychords[i].keychord, strlen(keys)) == 0) {
                if (strlen(keys) == strlen(keychords[i].keychord)) {
                    keychords[i].action(); // Execute the associated action
                    matchFound = true;
                } else {
                    // If we're here, it means there are more characters to be input for a full match
                    possibleMatchesRemaining = true;
                }
            }
        }
    }

    // If no matching keychord is found and no more possible matches, deactivate the leader key mode
    if (!matchFound && !possibleMatchesRemaining) {
        leaderKeyActive = false; // Stop listening for keychord
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
