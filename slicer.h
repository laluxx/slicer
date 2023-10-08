#ifndef SLICER_H
#define SLICER_H

#include "raylib.h"

// Sprite slicing parameters
extern const int SLICE_HORIZONTAL;
extern const int SLICE_VERTICAL;
extern const int SLICE_SIZE_X;
extern const int SLICE_SIZE_Y;

// Movement parameters
extern const Vector2 BASE_SPEED;
extern const float SPRINT_FACTOR;

typedef enum {
    IDLE_DOWN,
    WALK_RIGHT,
    IDLE_UP,
    RUN_DOWN,
    RUN_RIGHT,
    RUN_UP,
    HIT_DOWN,
    HIT_RIGHT,
    HIT_UP,
    DEATH_DOWN,
    DEATH_RIGHT,
    DEATH_UP,
    IDLE_LEFT,
    WALK_LEFT,
    RUN_LEFT
} CharacterState;

extern CharacterState state;
extern CharacterState lastState;

Rectangle GetCharacterSourceRec(int frameCounter, CharacterState state);

#endif // SLICER_H
