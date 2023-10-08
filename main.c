#include "raylib.h"
#include "screen.h"
#include "slicer.h"
#include "character.h"
#include "stdio.h"

#define CHARACTER_INSPECTOR_TEXT_SIZE 20

typedef struct {
    Vector2 position;
    CharacterState state;
    CharacterState lastState;
} Character;

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


void DrawCharacterInspector(Character character, Texture2D sprite, int frameCounter) {
    Vector2 bigSpritePosition = { SCREEN_WIDTH - 150, SCREEN_HEIGHT - 150 };
    Rectangle bigSpriteRec = GetCharacterSourceRec(frameCounter, character.state);
    if (character.state == WALK_LEFT || character.state == RUN_LEFT) {
        bigSpriteRec.width *= -1;  // Flip horizontally for left-facing states
    }
    DrawTexturePro(sprite, bigSpriteRec, (Rectangle){ bigSpritePosition.x, bigSpritePosition.y, bigSpriteRec.width * 4, bigSpriteRec.height * 4 }, (Vector2){ 0, 0 }, 0, WHITE);

    // Draw the character's state as text above the character inspector
    const char *stateText = CharacterStateNames[character.state];
    Vector2 textSize = MeasureTextEx(GetFontDefault(), stateText, CHARACTER_INSPECTOR_TEXT_SIZE, 1);
    Vector2 textPosition = { bigSpritePosition.x + (bigSpriteRec.width * 4 - textSize.x) / 2, bigSpritePosition.y - textSize.y - 10 };
    DrawText(stateText, textPosition.x, textPosition.y, CHARACTER_INSPECTOR_TEXT_SIZE, WHITE);
}



int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sprite Slicer");
    Texture2D sprite = LoadTexture("./sprite.png");

    Character character = { { (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2 }, IDLE_DOWN, IDLE_DOWN };
    int frameCounter = 0;

    SetTargetFPS(144);

    while (!WindowShouldClose()) {
        UpdateCharacterState(&character);

        BeginDrawing();
        ClearBackground((Color){11, 11, 11, 255});

        DrawCharacter(character, sprite, frameCounter);
        DrawCharacterInspector(character, sprite, frameCounter);

        EndDrawing();

        frameCounter++;
    }

    UnloadTexture(sprite);
    CloseWindow();

    return 0;
}




// missing left
/* #include "raylib.h" */
/* #include "screen.h" */
/* #include "slicer.h" */
/* #include "character.h" */

/* #define TEXT_SIZE 20 */

/* void DrawDirectionText(const char* direction) { */
/*     Vector2 textSize = MeasureTextEx(GetFontDefault(), direction, TEXT_SIZE, 1); */
/*     DrawText(direction, SCREEN_WIDTH - textSize.x - 10, SCREEN_HEIGHT - 200 - textSize.y, TEXT_SIZE, WHITE);  // Adjust the Y position as needed */
/* } */

/* int main(void) { */
/*     InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sprite Slicer"); */
/*     Texture2D sprite = LoadTexture("./sprite.png"); */

/*     Vector2 position = { (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2 }; */
/*     Vector2 bigSpritePosition = { SCREEN_WIDTH - 150, SCREEN_HEIGHT - 150 };  // Adjust as needed */
/*     int frameCounter = 0; */

/*     SetTargetFPS(144); */

/*     while (!WindowShouldClose()) { */
/*         // Handle user input and update character state */
/*         if (IsKeyDown(KEY_W)) { */
/*             state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_UP : IDLE_UP; */
/*             position.y -= BASE_SPEED.y; */
/*             lastState = state; */
/*         } */
/*         else if (IsKeyDown(KEY_S)) { */
/*             state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_DOWN : IDLE_DOWN; */
/*             position.y += BASE_SPEED.y; */
/*             lastState = state; */
/*         } */
/*         else if (IsKeyDown(KEY_A)) { */
/*             state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_LEFT : WALK_LEFT; */
/*             position.x -= BASE_SPEED.x; */
/*             lastState = state; */
/*         } */
/*         else if (IsKeyDown(KEY_D)) { */
/*             state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_RIGHT : WALK_RIGHT; */
/*             position.x += BASE_SPEED.x; */
/*             lastState = state; */
/*         } */
/*         else { */
/*             state = lastState; */
/*         } */

/*         // Drawing */
/*         BeginDrawing(); */
/*         ClearBackground((Color){11, 11, 11, 255}); */

/*         if (state == WALK_RIGHT || state == RUN_RIGHT) { */
/*             DrawDirectionText("Moving Right"); */
/*         } else if (state == WALK_LEFT || state == RUN_LEFT) { */
/*             DrawDirectionText("Moving Left"); */
/*         } else if (state == IDLE_UP || state == RUN_UP) { */
/*             DrawDirectionText("Moving Up"); */
/*         } else if (state == IDLE_DOWN || state == RUN_DOWN) { */
/*             DrawDirectionText("Moving Down"); */
/*         } */

/*         Rectangle bigSpriteRec = GetCharacterSourceRec(frameCounter, state); */
/*         DrawTexturePro(sprite, bigSpriteRec, (Rectangle){ bigSpritePosition.x, bigSpritePosition.y, bigSpriteRec.width * 4, bigSpriteRec.height * 4 }, (Vector2){ 0, 0 }, 0, WHITE); */

/*         DrawTextureRec(sprite, GetCharacterSourceRec(frameCounter, state), position, WHITE); */
/*         EndDrawing(); */

/*         frameCounter++; */
/*     } */

/*     UnloadTexture(sprite); */
/*     CloseWindow(); */

/*     return 0; */
/* } */



// base
/* #include "raylib.h" */
/* #include "screen.h" */
/* #include "slicer.h" */
/* #include "character.h" */

/* int main(void) */
/* { */
/*     InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sprite Slicer"); */
/*     Texture2D sprite = LoadTexture("./sprite.png"); */

/*     Vector2 position = { (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2 }; */
/*     int frameCounter = 0; */

/*     SetTargetFPS(144); */

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
/*             state = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? RUN_LEFT : WALK_LEFT; */
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

/*         // Drawing */
/*         BeginDrawing(); */
/*         ClearBackground((Color){11, 11, 11, 255}); */
/*         DrawTextureRec(sprite, GetCharacterSourceRec(frameCounter, state), position, WHITE); */
/*         EndDrawing(); */

/*         frameCounter++; */
/*     } */

/*     UnloadTexture(sprite); */
/*     CloseWindow(); */

/*     return 0; */
/* } */
