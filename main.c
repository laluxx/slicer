#include "raylib.h"
#include "screen.h"
#include "slicer.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sprite Slicer");
    Texture2D sprite = LoadTexture("./sprite.png");

    Vector2 position = { (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2 };
    int frameCounter = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Handle user input and update character state
        if (IsKeyDown(KEY_W))
        {
            state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_UP : IDLE_UP;
            position.y -= BASE_SPEED.y;
            lastState = state;
        }
        else if (IsKeyDown(KEY_S))
        {
            state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_DOWN : IDLE_DOWN;
            position.y += BASE_SPEED.y;
            lastState = state;
        }
        else if (IsKeyDown(KEY_A))
        {
            state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_LEFT : WALK_LEFT;
            position.x -= BASE_SPEED.x;
            lastState = state;
        }
        else if (IsKeyDown(KEY_D))
        {
            state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_RIGHT : WALK_RIGHT;
            position.x += BASE_SPEED.x;
            lastState = state;
        }
        else
        {
            state = lastState;
        }

        // Drawing
        BeginDrawing();
        ClearBackground((Color){11, 11, 11, 255});
        DrawTextureRec(sprite, GetCharacterSourceRec(frameCounter, state), position, WHITE);
        EndDrawing();

        frameCounter++;
    }

    UnloadTexture(sprite);
    CloseWindow();

    return 0;
}







/* #include "raylib.h" */
/* #include "./screen.h" */


/* // Sprite slicing parameters */
/* const int SLICE_HORIZONTAL = 4; */
/* const int SLICE_VERTICAL = 12; */
/* const int SLICE_SIZE_X = 32; */
/* const int SLICE_SIZE_Y = 32; */

/* // Movement parameters */
/* const Vector2 BASE_SPEED = { 4.0f, 4.0f }; */
/* const float SPRINT_FACTOR = 1.5f; */

/* typedef enum { */
/*     IDLE_DOWN, */
/*     WALK_RIGHT, */
/*     IDLE_UP, */
/*     RUN_DOWN, */
/*     RUN_RIGHT, */
/*     RUN_UP, */
/*     HIT_DOWN, */
/*     HIT_RIGHT, */
/*     HIT_UP, */
/*     DEATH_DOWN, */
/*     DEATH_RIGHT, */
/*     DEATH_UP, */
/*     IDLE_LEFT,      // Added */
/*     WALK_LEFT,      // Added */
/*     RUN_LEFT        // Added */
/* } CharacterState; */

/* CharacterState state = IDLE_DOWN; */
/* CharacterState lastState = IDLE_DOWN; */

/* int main(void) */
/* { */
/*     InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sprite Slicer"); */
/*     Texture2D sprite = LoadTexture("./sprite.png"); */

/*     Vector2 position = { (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2 }; */
/*     int frameCounter = 0; */

/*     SetTargetFPS(60); */

/*     while (!WindowShouldClose()) */
/*     { */
/*         // Handle user input and update character state */
/*         if (IsKeyDown(KEY_W)) */
/*         { */
/*             state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_UP : IDLE_UP; */
/*             position.y -= BASE_SPEED.y; */
/*             lastState = state; */
/*         } */
/*         else if (IsKeyDown(KEY_S)) */
/*         { */
/*             state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_DOWN : IDLE_DOWN; */
/*             position.y += BASE_SPEED.y; */
/*             lastState = state; */
/*         } */
/*         else if (IsKeyDown(KEY_A)) */
/*         { */
/*             state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_LEFT : WALK_LEFT; // Modified */
/*             position.x -= BASE_SPEED.x; */
/*             lastState = state; */
/*         } */
/*         else if (IsKeyDown(KEY_D)) */
/*         { */
/*             state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_RIGHT : WALK_RIGHT; */
/*             position.x += BASE_SPEED.x; */
/*             lastState = state; */
/*         } */
/*         else */
/*         { */
/*             state = lastState; */
/*         } */

/*         // Update source rectangle based on character state */
/*         Rectangle sourceRec = { (frameCounter/60) % SLICE_HORIZONTAL * SLICE_SIZE_X, state * SLICE_SIZE_Y, SLICE_SIZE_X, SLICE_SIZE_Y }; */
/*         if (state == WALK_LEFT || state == RUN_LEFT || state == IDLE_LEFT)  // Modified */
/*         { */
/*             sourceRec.width *= -1;  // Flip horizontally for left-facing states */
/*         } */

/*         // Drawing */
/*         BeginDrawing(); */
/*         ClearBackground((Color){11, 11, 11, 255}); */
/*         DrawTextureRec(sprite, sourceRec, position, WHITE); */
/*         EndDrawing(); */

/*         frameCounter++; */
/*     } */

/*     UnloadTexture(sprite); */
/*     CloseWindow(); */

/*     return 0; */
/* } */
