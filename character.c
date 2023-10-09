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

void UpdateCharacterState(Character* character) {
    if (IsKeyDown(KEY_W)) {
        character->state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_UP : IDLE_UP;
        character->position.y -= BASE_SPEED.y;
    } else if (IsKeyDown(KEY_S)) {
        character->state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_DOWN : IDLE_DOWN;
        character->position.y += BASE_SPEED.y;
    } else if (IsKeyDown(KEY_A)) {
        character->state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_LEFT : WALK_LEFT;
        character->position.x -= BASE_SPEED.x;
    } else if (IsKeyDown(KEY_D)) {
        character->state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_RIGHT : WALK_RIGHT;
        character->position.x += BASE_SPEED.x;
    } else {
        character->state = character->lastState;
    }
    character->lastState = character->state;
}

void DrawCharacter(Character character, Texture2D sprite, int frameCounter) {
    DrawTextureRec(sprite, GetCharacterSourceRec(frameCounter, character.state), character.position, WHITE);
}
