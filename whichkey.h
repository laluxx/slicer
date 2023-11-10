#ifndef WHICHKEY_H_
#define WHICHKEY_H_

#include "keychords.h"

typedef struct WhichKey {
    bool active;
    float timer;
    char buffer[MAX_KEYCHORD_LENGTH];
    float additionalHeight; // Height of the which-key display area
} WhichKey;

extern WhichKey whichKey;

void UpdateWhichKey();
void DrawWhichKey();
void ActivateWhichKey();
float CalculateWhichKeyHeight(int numberOfKeyChords);

#endif // WHICHKEY_H_
