#include "modes.h"
#include <stdlib.h>

EditorMode currentMode = MODE_DEFAULT;
bool panelsResetOnModeChange = true;



/* void DrawModeBar() { */
/*     // Constants for mode circles */
/*     const int circleRadius = 8; */
/*     const int circleSpacing = 15; */
/*     const int circleStartX = 10;  // Starting position of the first circle */
/*     const int circleY = 10;  // Vertical position of the circles */

/*     // Drawing the mode circles */
/*     for (int i = 0; i < MODE_MAX; i++) { */
/*         int circleX = circleStartX + i * (circleRadius * 2 + circleSpacing); */
/*         if (i == currentMode) { */
/*             DrawCircle(circleX, circleY, circleRadius, WHITE);  // Fill the circle for the current mode */
/*         } else { */
/*             DrawCircleLines(circleX, circleY, circleRadius, WHITE);  // Only draw the border for other modes */
/*         } */
/*     } */
/* } */


/* void DrawModeBar() { */
/*     // Constants for mode circles */
/*     const int circleRadius = 8; */
/*     const int circleSpacing = 15; */
/*     const int paddingX = 10; // Padding from left of the screen */
/*     const int paddingY = 10; // Padding from top of the screen */

/*     // Check for mouse click */
/*     Vector2 mousePos = GetMousePosition(); */
/*     if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { */
/*         for (int i = 0; i < MODE_MAX; i++) { */
/*             int circleX = paddingX + circleRadius + i * (circleRadius * 2 + circleSpacing); */
/*             float distance = sqrt(pow(mousePos.x - circleX, 2) + pow(mousePos.y - (paddingY + circleRadius), 2)); */
/*             if (distance < circleRadius) { */
/*                 EditorChangeMode((EditorMode)i); */
/*                 break; */
/*             } */
/*         } */
/*     } */

/*     // Drawing the mode circles */
/*     for (int i = 0; i < MODE_MAX; i++) { */
/*         int circleX = paddingX + circleRadius + i * (circleRadius * 2 + circleSpacing); */
/*         if (i == currentMode) { */
/*             DrawCircle(circleX, paddingY + circleRadius, circleRadius, WHITE);  // Fill the circle for the current mode */
/*         } else { */
/*             DrawCircleLines(circleX, paddingY + circleRadius, circleRadius, WHITE);  // Only draw the border for other modes */
/*         } */
/*     } */
/* } */

void DrawModeBar() {
    // Constants for mode circles
    const int circleRadius = 8;
    const int circleSpacing = 15;
    const int paddingX = 10; // Padding from left of the screen
    const int paddingY = 10; // Padding from top of the screen

    // Check for mouse click
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < MODE_MAX; i++) {
            int circleX = paddingX + circleRadius + i * (circleRadius * 2 + circleSpacing);
            float distance = sqrt(pow(mousePos.x - circleX, 2) + pow(mousePos.y - (paddingY + circleRadius), 2));
            if (distance < circleRadius) {
                EditorChangeMode((EditorMode)i);  // Delegate the mode change and associated behaviors to EditorChangeMode
                break;
            }
        }
    }

    // Check for Alt + Number key combinations
    if (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) {
        if (IsKeyPressed(KEY_ONE)) {
            EditorChangeMode(MODE_DEFAULT);  // Delegate the mode change and associated behaviors to EditorChangeMode
        } else if (IsKeyPressed(KEY_TWO)) {
            EditorChangeMode(MODE_SLICER);  // Delegate the mode change and associated behaviors to EditorChangeMode
        }
        // Add similar checks for KEY_THREE, KEY_FOUR, etc. if you add more modes in the future
    }

    // Drawing the mode circles
    for (int i = 0; i < MODE_MAX; i++) {
        int circleX = paddingX + circleRadius + i * (circleRadius * 2 + circleSpacing);
        if (i == currentMode) {
            DrawCircle(circleX, paddingY + circleRadius, circleRadius, WHITE);  // Fill the circle for the current mode
        } else {
            DrawCircleLines(circleX, paddingY + circleRadius, circleRadius, WHITE);  // Only draw the border for other modes
        }
    }
}



void EditorChangeMode(EditorMode mode) {
    currentMode = mode;
    log_add("Changed editor mode");

    if (currentMode == MODE_DEFAULT && panelsResetOnModeChange) {
        PanelsReset();
    }
}

void EditorNextMode() {
    currentMode = (currentMode + 1) % MODE_MAX;
    log_add("Switched to next mode");

    if (currentMode == MODE_DEFAULT && panelsResetOnModeChange) {
        PanelsReset();
    }
}

void EditorPreviousMode() {
    currentMode = (currentMode - 1 + MODE_MAX) % MODE_MAX;
    log_add("Switched to previous mode");

    if (currentMode == MODE_DEFAULT && panelsResetOnModeChange) {
        PanelsReset();
    }
}

void RenderSlicerMode(Character* character, Texture2D sprite) {
    const int infoX = 20;
    const int infoY = 40;

    static int framesCounter = 0;
    static int animation_rate = 8;

    float scale = 2.0f;
    int buttonWidth = 180;
    int buttonHeight = 30;
    int smallButtonWidth = buttonWidth / 6;

    Rectangle spriteDest = {
        infoX + smallButtonWidth * 2 + 30,
        infoY,
        sprite.width * scale,
        sprite.height * scale
    };

    framesCounter++;
    if (framesCounter >= (60/animation_rate)) {
        framesCounter = 0;
        UpdateCharacterState(character);
    }

    if (IsKeyPressed(KEY_RIGHT) && animation_rate < MAX_FRAME_SPEED) animation_rate++;
    if (IsKeyPressed(KEY_LEFT) && animation_rate > MIN_FRAME_SPEED) animation_rate--;

    BeginDrawing();
    ClearBackground((Color){0x0B, 0x0B, 0x0B, 0xFF});

    /* DrawCharacter(*character, sprite, framesCounter); */

    DrawTexturePro(sprite, (Rectangle){0, 0, sprite.width, sprite.height}, spriteDest, (Vector2){0, 0}, 0, WHITE);

    for (int i = 0; i < SLICE_HORIZONTAL; i++) {
        for (int j = 0; j < SLICE_VERTICAL; j++) {
            Rectangle gridRect = {
                spriteDest.x + i * SLICE_SIZE_X * scale,
                spriteDest.y + j * SLICE_SIZE_Y * scale,
                SLICE_SIZE_X * scale,
                SLICE_SIZE_Y * scale
            };
            DrawRectangleLinesEx(gridRect, 1, RED);
        }
    }

    Rectangle srcRec = GetCharacterSourceRec(framesCounter, character->state);
    DrawRectangleLines(
        spriteDest.x + (int)srcRec.x * scale,
        spriteDest.y + (int)srcRec.y * scale,
        SLICE_SIZE_X * scale,
        SLICE_SIZE_Y * scale,
        GREEN
    );

    if (DrawButton("-", (Rectangle){spriteDest.x - smallButtonWidth - 10, spriteDest.y, smallButtonWidth, buttonHeight}, RED)) {
        if (SLICE_VERTICAL > 1) SLICE_VERTICAL--;
    }
    if (DrawButton("+", (Rectangle){spriteDest.x - smallButtonWidth - 10, spriteDest.y + buttonHeight + 10, smallButtonWidth, buttonHeight}, GREEN)) {
        SLICE_VERTICAL++;
    }

    if (DrawButton("-", (Rectangle){spriteDest.x, spriteDest.y - buttonHeight - 10, smallButtonWidth, buttonHeight}, RED)) {
        if (SLICE_HORIZONTAL > 1) SLICE_HORIZONTAL--;
    }
    if (DrawButton("+", (Rectangle){spriteDest.x + smallButtonWidth + 10, spriteDest.y - buttonHeight - 10, smallButtonWidth, buttonHeight}, GREEN)) {
        SLICE_HORIZONTAL++;
    }

    DrawText("FRAME SPEED: ", spriteDest.x + spriteDest.width + 10, spriteDest.y, 10, RED);
    DrawText(TextFormat("%02i FPS", animation_rate), spriteDest.x + spriteDest.width + 120, spriteDest.y, 10, RED);

    for (int i = 0; i < MAX_FRAME_SPEED; i++) {
        if (i < animation_rate) DrawRectangle(spriteDest.x + spriteDest.width + 10 + 21*i, spriteDest.y + 40, 20, 20, RED);
        DrawRectangleLines(spriteDest.x + spriteDest.width + 10 + 21*i, spriteDest.y + 40, 20, 20, GREEN);
    }

    // Displaying the character's current state
    DrawText(CharacterStateNames[character->state], spriteDest.x, spriteDest.y + spriteDest.height + 10, 10, GREEN);

    EndDrawing();
}
