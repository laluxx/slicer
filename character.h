#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"

#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1

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

typedef struct {
    Vector2 position;
    CharacterState state;
    CharacterState lastState;
} Character;

// Function prototypes
void UpdateCharacterState(Character* character);
void DrawCharacter(Character character, Texture2D sprite, int frameCounter);
void DrawCharacterInspector(Character character, Texture2D sprite, int frameCounter);

extern const char *CharacterStateNames[];

extern const Vector2 BASE_SPEED;
extern const float SPRINT_FACTOR;

extern CharacterState state;
extern CharacterState lastState;

Rectangle GetCharacterSourceRec(int frameCounter, CharacterState state);

#endif // CHARACTER_H
