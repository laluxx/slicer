#include "whichkey.h"
#include "ui.h"
#include "screen.h"

// Initialize the which-key struct
WhichKey whichKey = {
    .active = false,
    .timer = 0.0f,
    .buffer = {0},
    .additionalHeight = 0.0f // This will be dynamically calculated
};

// Function to calculate the height needed for the which-key interface
float CalculateWhichKeyHeight(int numberOfKeyChords) {
    // Assuming 20 pixels per item and 10 pixels padding between items
    return (float)(numberOfKeyChords * 20 + (numberOfKeyChords - 1) * 10);
}


// Function to update which-key state
void UpdateWhichKey() {
    if (IsKeyPressed(LEADER_KEY)) {
        ActivateWhichKey();
    }

    if (whichKey.active) {
        whichKey.timer += GetFrameTime();
        if (whichKey.timer > 0.5f) { // Half a second delay before showing which-key
            // Clear previous buffer
            memset(whichKey.buffer, 0, sizeof(whichKey.buffer));
            // Populate whichKey.buffer with relevant keychords
            int numberOfKeyChords = 0;
            for (int i = 0; i < keychordCount; ++i) {
                if (strncmp(currentKeyChord, keychords[i].keychord, strlen(currentKeyChord)) == 0) {
                    strcat(whichKey.buffer, keychords[i].keychord);
                    strcat(whichKey.buffer, " - ");
                    strcat(whichKey.buffer, keychords[i].description);
                    strcat(whichKey.buffer, "\n");
                    numberOfKeyChords++;
                }
            }
            // Adjust the minibuffer height to make space for which-key interface
            if (numberOfKeyChords > 0) {
                whichKey.additionalHeight = CalculateWhichKeyHeight(numberOfKeyChords);
                minibuffer.height = 21.0f + whichKey.additionalHeight;
            }
            // Do not deactivate whichKey here, let DrawWhichKey() handle the display
        }
    }
}

// Function to draw which-key interface
void DrawWhichKey() {
    if (whichKey.active) {
        // Draw the which-key items
        int offsetY = SCREEN_HEIGHT - minibuffer.height + 10; // Start offset for drawing
        char* token = strtok(whichKey.buffer, "\n");
        while (token) {
            DrawText(token, 10, offsetY, 10, RAYWHITE);
            offsetY += 30; // 20 pixels for text, 10 pixels for padding
            token = strtok(NULL, "\n");
        }
    }
}

// Call this function when the leader key is pressed
void ActivateWhichKey() {
    if (!whichKey.active) { // Only activate if not already active
        whichKey.active = true;
        whichKey.timer = 0.0f;
        memset(whichKey.buffer, 0, sizeof(whichKey.buffer)); // Clear the buffer
    } else {
        // If already active and leader key is pressed again, deactivate and reset minibuffer height
        whichKey.active = false;
        minibuffer.height = 21.0f; // Reset to original height
    }
}
