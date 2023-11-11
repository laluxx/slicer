#ifndef WHICHKEY_H_
#define WHICHKEY_H_

#include "keychords.h"

typedef struct WhichKey {
    bool active;
    float timer;
    char buffer[MAX_KEYCHORD_LENGTH];
    float height;
} WhichKey;

extern WhichKey whichKey;

void UpdateWhichKey();
void DrawWhichKey();
void ActivateWhichKey();
float CalculateWhichKeyHeight(int numberOfKeyChords);

#endif // WHICHKEY_H_
