#include "raylib.h"
#include "screen.h"
#include "slicer.h"
#include "character.h"
#include "log.h"
#include "panels.h"
#include "stdio.h"
#include "stdlib.h"
#include "modes.h"
#include "ui.h"






// INSPECTORS
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



void DrawSlicerInspector(Texture2D sprite) {
    // Determine the starting position for the control groups
    int controlGroupXOffset = SCREEN_WIDTH - 100;  // Adjusted x offset
    int topYOffset = 10;  // Set to 10 to allow space for the text above
    int controlSpacing = 20;  // Spacing between control groups

    // Create control groups for each parameter
    DrawIncrementerButton(&SLICE_HORIZONTAL, "SLICE_HORIZONTAL", "int", controlGroupXOffset, topYOffset);
    controlGroupXOffset -= (100 + controlSpacing);  // Adjusted spacing to accommodate type rectangle and name text
    DrawIncrementerButton(&SLICE_VERTICAL, "SLICE_VERTICAL", "int", controlGroupXOffset, topYOffset);
    controlGroupXOffset -= (100 + controlSpacing);  // Adjusted spacing to accommodate type rectangle and name text
    DrawIncrementerButton(&SLICE_SIZE_X, "SLICE_SIZE_X", "int", controlGroupXOffset, topYOffset);
    controlGroupXOffset -= (100 + controlSpacing);  // Adjusted spacing to accommodate type rectangle and name text
    DrawIncrementerButton(&SLICE_SIZE_Y, "SLICE_SIZE_Y", "int", controlGroupXOffset, topYOffset);
    controlGroupXOffset -= (100 + controlSpacing);  // Adjusted spacing to accommodate type rectangle and name text
    DrawIncrementerButton(&animation_rate, "Animation Rate", "int", controlGroupXOffset, topYOffset);

    // Set the position for drawing the sprite
    int xOffset = SCREEN_WIDTH - 388;  // Moved further to the left
    int yOffset = 50;  // Moved down

    // Adjusted scale and position for better proportion with buttons and to prevent overlap
    float scale = 1.0f;
    Rectangle spriteDest = { xOffset + 130, yOffset, sprite.width * scale, sprite.height * scale };
    DrawTexturePro(sprite, (Rectangle){ 0, 0, sprite.width, sprite.height }, spriteDest, (Vector2){ 0, 0 }, 0, WHITE);

    // Draw grid over the sprite
    for (int i = 0; i < SLICE_HORIZONTAL; i++) {
        for (int j = 0; j < SLICE_VERTICAL; j++) {
            Rectangle gridRect = { spriteDest.x + i*SLICE_SIZE_X*scale, spriteDest.y + j*SLICE_SIZE_Y*scale, SLICE_SIZE_X*scale, SLICE_SIZE_Y*scale };
            DrawRectangleLinesEx(gridRect, 1, RAYWHITE);
        }
    }
}








// Define the paths to the key images
#define W_KEY_PATH "./icons/input-prompts/KeyboardandMouse/Dark/W_Key_Dark.png"
#define A_KEY_PATH "./icons/input-prompts/KeyboardandMouse/Dark/A_Key_Dark.png"
#define S_KEY_PATH "./icons/input-prompts/KeyboardandMouse/Dark/S_Key_Dark.png"
#define D_KEY_PATH "./icons/input-prompts/KeyboardandMouse/Dark/D_Key_Dark.png"

// Define the position of the WASD keys
#define WASD_POS_X 1680  // Adjusted for bottom-left placement
#define WASD_POS_Y 950  // Adjusted for bottom-left placement

// Assume these are global or loaded before your game loop
Texture2D wKey;
Texture2D aKey;
Texture2D sKey;
Texture2D dKey;

void LoadWASDTextures() {
    wKey = LoadTexture(W_KEY_PATH);
    aKey = LoadTexture(A_KEY_PATH);
    sKey = LoadTexture(S_KEY_PATH);
    dKey = LoadTexture(D_KEY_PATH);
}

void UnloadWASDTextures() {
    UnloadTexture(wKey);
    UnloadTexture(aKey);
    UnloadTexture(sKey);
    UnloadTexture(dKey);
}

void DrawWASDInspector(void) {
    // Determine the color based on key state
    Color wColor = IsKeyDown(KEY_W) ? WHITE : GRAY;
    Color aColor = IsKeyDown(KEY_A) ? WHITE : GRAY;
    Color sColor = IsKeyDown(KEY_S) ? WHITE : GRAY;
    Color dColor = IsKeyDown(KEY_D) ? WHITE : GRAY;

    // Scale factor to adjust the size of the keys
    float scaleFactor = 0.45f;  // Adjust this value to make the keys smaller or larger
    int buttonsProportion = 10;
    // Draw the keys with the determined color
    DrawTexturePro(wKey, (Rectangle){0, 0, wKey.width, wKey.height}, (Rectangle){WASD_POS_X, WASD_POS_Y + buttonsProportion - wKey.height*scaleFactor, wKey.width*scaleFactor, wKey.height*scaleFactor}, (Vector2){0, 0}, 0, wColor);
    DrawTexturePro(aKey, (Rectangle){0, 0, aKey.width, aKey.height}, (Rectangle){WASD_POS_X - aKey.width*scaleFactor + buttonsProportion, WASD_POS_Y, aKey.width*scaleFactor, aKey.height*scaleFactor}, (Vector2){0, 0}, 0, aColor);
    DrawTexturePro(sKey, (Rectangle){0, 0, sKey.width, sKey.height}, (Rectangle){WASD_POS_X, WASD_POS_Y - 45 + sKey.height * scaleFactor, sKey.width * scaleFactor, sKey.height * scaleFactor}, (Vector2){0, 0}, 0, sColor);
    DrawTexturePro(dKey, (Rectangle){0, 0, dKey.width, dKey.height}, (Rectangle){WASD_POS_X + dKey.width*scaleFactor -buttonsProportion, WASD_POS_Y, dKey.width*scaleFactor, dKey.height*scaleFactor}, (Vector2){0, 0}, 0, dColor);
}

// INSPECTORS END





int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sprite Slicer");
    Texture2D sprite = LoadTexture("./sprite.png");
    Character character = { { (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2 }, IDLE_DOWN, IDLE_DOWN };
    int frameCounter = 0;
    SetTargetFPS(144);

    // Load WASD textures
    LoadWASDTextures();

    // SLICE MODE
    buttonYStart = SCREEN_HEIGHT - (buttonHeight + buttonSpacing) * 6;

    InitializePixelEditor("./sprite.png");  // Initialize the pixel editor

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

        if (IsKeyPressed(KEY_N)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                EditorPreviousMode();
            } else {
                EditorNextMode();
            }
        }

        BeginDrawing();
        ClearBackground((Color){11, 11, 11, 255});

        switch (currentMode) {
            case MODE_DEFAULT:
                DrawPanels();
                DrawWASDInspector();
                DrawSlicerInspector(sprite);  // Call the function here
                DrawEditorLog(panel.bottomHeight);
                DrawCharacter(character, sprite, frameCounter);
                DrawCharacterInspector(character, sprite, frameCounter);
                void DrawSlicerInspector(Texture2D sprite);
                break;
            case MODE_SLICER:
                DrawCharacter(character, sprite, frameCounter);
                RenderSlicerMode(&character, sprite);

                break;
            case MODE_PIXEL_EDITOR:
                UpdatePixelEditor();  // Update the pixel editor
                RenderPixelEditorMode();    // Draw the pixel editor UI
                break;
            default:
                break;
        }
        EndDrawing();
        frameCounter++;
    }

    // Freeing memory
    UnloadTexture(pixelEditor.canvasTexture);  // Unload the canvas texture
    UnloadTexture(sprite);
    CloseWindow();
    return 0;
}
