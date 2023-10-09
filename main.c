#include "raylib.h"
#include "screen.h"
#include "slicer.h"
#include "character.h"
#include "stdio.h"

#define CHARACTER_INSPECTOR_TEXT_SIZE 20


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
