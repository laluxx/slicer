#include "slicer.h"

// Sprite slicing parameters
const int SLICE_HORIZONTAL = 4;
const int SLICE_VERTICAL = 12;
const int SLICE_SIZE_X = 32;
const int SLICE_SIZE_Y = 32;

// Movement parameters
const Vector2 BASE_SPEED = { 4.0f, 4.0f };
const float SPRINT_FACTOR = 1.5f;

CharacterState state = IDLE_DOWN;
CharacterState lastState = IDLE_DOWN;

Rectangle GetCharacterSourceRec(int frameCounter, CharacterState state) {
    Rectangle sourceRec = { (frameCounter/60) % SLICE_HORIZONTAL * SLICE_SIZE_X, state * SLICE_SIZE_Y, SLICE_SIZE_X, SLICE_SIZE_Y };
    if (state == WALK_LEFT || state == RUN_LEFT || state == IDLE_LEFT) {
        sourceRec.width *= -1;  // Flip horizontally for left-facing states
    }
    return sourceRec;
}
