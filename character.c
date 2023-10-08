#include "raylib.h"
#include "character.h"


// Movement parameters
const Vector2 BASE_SPEED = { 4.0f, 4.0f };
const float SPRINT_FACTOR = 1.5f;


CharacterState state = IDLE_DOWN;
CharacterState lastState = IDLE_DOWN;



const char *CharacterStateNames[] = {
    "IDLE_DOWN",
    "WALK_RIGHT",
    "IDLE_UP",
    "RUN_DOWN",
    "RUN_RIGHT",
    "RUN_UP",
    "HIT_DOWN",
    "HIT_RIGHT",
    "HIT_UP",
    "DEATH_DOWN",
    "DEATH_RIGHT",
    "DEATH_UP",
    "IDLE_LEFT",
    "WALK_LEFT",
    "RUN_LEFT"
};
