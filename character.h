#ifndef CHARACTER_H
#define CHARACTER_H



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


extern const char *CharacterStateNames[];





extern const Vector2 BASE_SPEED;
extern const float SPRINT_FACTOR;

extern CharacterState state;
extern CharacterState lastState;

Rectangle GetCharacterSourceRec(int frameCounter, CharacterState state);

#endif // CHARACTER_H
