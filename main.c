#include "raylib.h"
#include "screen.h"
#include "slicer.h"
#include "character.h"
#include "log.h"
#include "panels.h"
#include "stdio.h"
#include "stdlib.h"

#define CHARACTER_INSPECTOR_TEXT_SIZE 20

void DrawCharacterPositionInspector(Character character) {
    int xOffset = 10;  // Offset from the left where text starts
    int yOffset = 10;  // Offset from the top where text starts

    Vector2 positionTextPos = { SCREEN_WIDTH - 150 + xOffset, SCREEN_HEIGHT - 150 + yOffset };

    char xPosStr[20], yPosStr[20];
    snprintf(xPosStr, sizeof(xPosStr), "x: %d", (int) character.position.x);
    snprintf(yPosStr, sizeof(yPosStr), "y: %d", (int) character.position.y);

    DrawText(xPosStr, positionTextPos.x, positionTextPos.y, CHARACTER_INSPECTOR_TEXT_SIZE, RED);
    DrawText(yPosStr, positionTextPos.x, positionTextPos.y + CHARACTER_INSPECTOR_TEXT_SIZE + 5, CHARACTER_INSPECTOR_TEXT_SIZE, BLUE);
}

void DrawCharacterInspector(Character character, Texture2D sprite, int frameCounter) {
    DrawCharacterPositionInspector(character);

    Vector2 bigSpritePosition = { SCREEN_WIDTH - 100, SCREEN_HEIGHT - 180 };
    Rectangle bigSpriteRec = GetCharacterSourceRec(frameCounter, character.state);

    if (character.state == WALK_LEFT || character.state == RUN_LEFT) {
        bigSpriteRec = GetCharacterSourceRec(frameCounter, WALK_RIGHT);
        bigSpriteRec.width *= -1;
    }

    DrawTexturePro(sprite, bigSpriteRec, (Rectangle){ bigSpritePosition.x, bigSpritePosition.y, abs(bigSpriteRec.width) * 4, bigSpriteRec.height * 4 }, (Vector2){ 0, 0 }, 0, WHITE);

    const char *stateText = CharacterStateNames[character.state];
    Vector2 textSize = MeasureTextEx(GetFontDefault(), stateText, CHARACTER_INSPECTOR_TEXT_SIZE, 1);
    Vector2 textPosition = { bigSpritePosition.x + (abs(bigSpriteRec.width) * 1 - textSize.x) / 2, bigSpritePosition.y + 10 };
    DrawText(stateText, textPosition.x, textPosition.y, CHARACTER_INSPECTOR_TEXT_SIZE, WHITE);
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sprite Slicer");
    Texture2D sprite = LoadTexture("./sprite.png");

    Character character = { { (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2 }, IDLE_DOWN, IDLE_DOWN };
    int frameCounter = 0;

    SetTargetFPS(144);

    while (!WindowShouldClose()) {
        UpdatePanelDimensions();
        UpdateEditorLogScroll();
        UpdateCharacterState(&character);

        if (IsKeyPressed(KEY_R) && IsKeyDown(KEY_LEFT_CONTROL)) {
            log_add("UI resetted");
            panel = (Panel){
                INITIAL_TOP_HEIGHT,
                INITIAL_BOTTOM_HEIGHT,
                INITIAL_LEFT_WIDTH,
                INITIAL_RIGHT_WIDTH
            };
        }

        BeginDrawing();
        ClearBackground((Color){11, 11, 11, 255});
        DrawPanels();
        DrawEditorLog(panel.bottomHeight);
        DrawCharacter(character, sprite, frameCounter);
        DrawCharacterInspector(character, sprite, frameCounter);

        EndDrawing();

        frameCounter++;
    }

    UnloadTexture(sprite);
    CloseWindow();

    return 0;
}
