#include "raylib.h"
#include "character.h"


// Movement parameters
const Vector2 BASE_SPEED = { 4.0f, 4.0f };
const float SPRINT_FACTOR = 1.5f;


CharacterState state = IDLE_DOWN;
CharacterState lastState = IDLE_DOWN;
