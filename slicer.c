#include "slicer.h"

// Sprite slicing parameters
const int SLICE_HORIZONTAL = 4;
const int SLICE_VERTICAL = 12;
const int SLICE_SIZE_X = 32;
const int SLICE_SIZE_Y = 32;
int animation_rate = 15;

Rectangle GetCharacterSourceRec(int frameCounter, CharacterState state) {
    Rectangle sourceRec = { (frameCounter/animation_rate) % SLICE_HORIZONTAL * SLICE_SIZE_X, state * SLICE_SIZE_Y, SLICE_SIZE_X, SLICE_SIZE_Y };
    if (state == WALK_LEFT || state == RUN_LEFT || state == IDLE_LEFT) {
        sourceRec.width *= -1;  // Flip horizontally for left-facing states
    }
    return sourceRec;
}
