#include "slicer.h"

// Sprite slicing parameters
int SLICE_HORIZONTAL = 4;
int SLICE_VERTICAL = 12;
int SLICE_SIZE_X = 32;
int SLICE_SIZE_Y = 32;

int animation_rate = 15;

Rectangle GetCharacterSourceRec(int frameCounter, CharacterState state) {
    Rectangle sourceRec = { (frameCounter/animation_rate) % SLICE_HORIZONTAL * SLICE_SIZE_X, state * SLICE_SIZE_Y, SLICE_SIZE_X, SLICE_SIZE_Y };
    if (state == WALK_LEFT || state == RUN_LEFT || state == IDLE_LEFT) {
        sourceRec.width *= -1;  // Flip horizontally for left-facing states
    }
    return sourceRec;
}
