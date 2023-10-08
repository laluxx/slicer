#include "raylib.h"
#include "screen.h"
#include "slicer.h"
#include "character.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sprite Slicer");
    Texture2D sprite = LoadTexture("./sprite.png");

    Vector2 position = { (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2 };
    int frameCounter = 0;

    SetTargetFPS(144);

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
