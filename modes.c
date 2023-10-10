#include "modes.h"

EditorMode currentMode = MODE_DEFAULT;

bool panelsResetOnModeChange = true;



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

/* void RenderSlicerMode(Texture2D sprite) { */
/*     // Draw sprite at the center, scaled up for better visibility */
/*     float scale = 2.0f; */
/*     Rectangle spriteDest = { SCREEN_WIDTH/2 - sprite.width*scale/2, SCREEN_HEIGHT/2 - sprite.height*scale/2 - 100, sprite.width*scale, sprite.height*scale }; */
/*     DrawTexturePro(sprite, (Rectangle){ 0, 0, sprite.width, sprite.height }, spriteDest, (Vector2){ 0, 0 }, 0, WHITE); */

/*     // Draw grid over the sprite */
/*     for (int i = 0; i < SLICE_HORIZONTAL; i++) { */
/*         for (int j = 0; j < SLICE_VERTICAL; j++) { */
/*             Rectangle gridRect = { spriteDest.x + i*SLICE_SIZE_X*scale, spriteDest.y + j*SLICE_SIZE_Y*scale, SLICE_SIZE_X*scale, SLICE_SIZE_Y*scale }; */
/*             DrawRectangleLinesEx(gridRect, 1, RAYWHITE); */
/*         } */
/*     } */

/*     int smallButtonWidth = buttonWidth / 6;  // Adjusted the width */

/*     // Vertical slices controls (Left side of the sprite) */
/*     if (DrawButton("-", (Rectangle){spriteDest.x - smallButtonWidth - 10, spriteDest.y, smallButtonWidth, buttonHeight}, RED)) { */
/*         if (SLICE_VERTICAL > 1) SLICE_VERTICAL--; */
/*     } */
/*     if (DrawButton("+", (Rectangle){spriteDest.x - smallButtonWidth - 10, spriteDest.y + buttonHeight + 10, smallButtonWidth, buttonHeight}, GREEN)) { */
/*         SLICE_VERTICAL++; */
/*     } */

/*     // Horizontal slices controls (Top side of the sprite) */
/*     if (DrawButton("-", (Rectangle){spriteDest.x, spriteDest.y - buttonHeight - 10, smallButtonWidth, buttonHeight}, RED)) { */
/*         if (SLICE_HORIZONTAL > 1) SLICE_HORIZONTAL--; */
/*     } */
/*     if (DrawButton("+", (Rectangle){spriteDest.x + smallButtonWidth + 10, spriteDest.y - buttonHeight - 10, smallButtonWidth, buttonHeight}, GREEN)) { */
/*         SLICE_HORIZONTAL++; */
/*     } */

/*     // Centralized "Slice!" button, adjusted position to be a bit higher */
/*     int sliceButtonVerticalOffset = 50;  // Adjust this value to move the button up/down as needed */
/*     if (DrawButton("Slice!", (Rectangle){SCREEN_WIDTH/2 - buttonWidth/2, SCREEN_HEIGHT - (buttonHeight + buttonSpacing) - sliceButtonVerticalOffset, buttonWidth, buttonHeight}, BLUE)) { */
/*         // TODO: Add functionality to slice the sprite based on current settings */
/*     } */
/* } */

// V2
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









PixelEditorData pixelEditor = {
    .gridSize = 10,
    .backgroundColor = {11, 11, 11, 255}
};

void InitializePixelEditor(char* imagePath) {
    Image canvas = LoadImage(imagePath);
    pixelEditor.canvasTexture = LoadTextureFromImage(canvas);
    UnloadImage(canvas);  // Unload image data (no longer needed)

    // Initialize color picker
    for (int i = 0; i < 5; i++) {
        pixelEditor.colorPicker[i].rect = (Rectangle){ 10 + i*40, 10, 30, 30 };
        switch (i) {
            case 0: pixelEditor.colorPicker[i].color = RED; break;
            case 1: pixelEditor.colorPicker[i].color = GREEN; break;
            case 2: pixelEditor.colorPicker[i].color = BLUE; break;
            case 3: pixelEditor.colorPicker[i].color = YELLOW; break;
            case 4: pixelEditor.colorPicker[i].color = PURPLE; break;
        }
    }
    pixelEditor.currentColor = RED;
    pixelEditor.eraserActive = false;
}

void HandlePixelEditorInput(void) {
    Vector2 mousePosition = GetMousePosition();

    // Color Picker and Eraser Input Handling
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < 5; i++) {
            if (CheckCollisionPointRec(mousePosition, pixelEditor.colorPicker[i].rect)) {
                pixelEditor.currentColor = pixelEditor.colorPicker[i].color;
                pixelEditor.eraserActive = false;
            }
        }

        if (CheckCollisionPointRec(mousePosition, (Rectangle){210, 10, 30, 30})) {
            pixelEditor.eraserActive = !pixelEditor.eraserActive;
        }
    }

    // Pixel Editing Input Handling
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, (Rectangle){0, 50, pixelEditor.canvasTexture.width, pixelEditor.canvasTexture.height})) {
        Vector2 canvasPos = { mousePosition.x, mousePosition.y - 50 };
        int gridX = (int)canvasPos.x / pixelEditor.gridSize;
        int gridY = (int)canvasPos.y / pixelEditor.gridSize;

        // Load image from file, apply changes and update texture
        Image canvas = LoadImage("./sprite.png");

        // Pixel Editing
        if (pixelEditor.eraserActive) {
            for (int i = 0; i < pixelEditor.gridSize; i++) {
                for (int j = 0; j < pixelEditor.gridSize; j++) {
                    ImageDrawPixel(&canvas, gridX*pixelEditor.gridSize + i, gridY*pixelEditor.gridSize + j, pixelEditor.backgroundColor);
                }
            }
        } else {
            for (int i = 0; i < pixelEditor.gridSize; i++) {
                for (int j = 0; j < pixelEditor.gridSize; j++) {
                    ImageDrawPixel(&canvas, gridX*pixelEditor.gridSize + i, gridY*pixelEditor.gridSize + j, pixelEditor.currentColor);
                }
            }
        }

        // Updating Texture2D from Image
        UnloadTexture(pixelEditor.canvasTexture);  // Unload the old texture
        pixelEditor.canvasTexture = LoadTextureFromImage(canvas);  // Load the new texture from the edited image

        // Freeing the memory of the Image
        UnloadImage(canvas);
    }
}

void DrawPixelEditorGrid(void) {
    for (int i = 0; i <= pixelEditor.canvasTexture.width; i += pixelEditor.gridSize) {
        DrawLine(i, 50, i, 50 + pixelEditor.canvasTexture.height, DARKGRAY);
    }
    for (int j = 0; j <= pixelEditor.canvasTexture.height; j += pixelEditor.gridSize) {
        DrawLine(0, 50 + j, pixelEditor.canvasTexture.width, 50 + j, DARKGRAY);
    }
}

void DrawColorPicker(void) {
    for (int i = 0; i < 5; i++) {
        DrawRectangleRec(pixelEditor.colorPicker[i].rect, pixelEditor.colorPicker[i].color);
    }
}

void DrawEraserButton(void) {
    DrawRectangle(210, 10, 30, 30, pixelEditor.eraserActive ? DARKGRAY : LIGHTGRAY);
    DrawText("E", 220, 15, 20, pixelEditor.eraserActive ? RAYWHITE : BLACK);
}

void UpdatePixelEditor(void) {
    HandlePixelEditorInput();
}

void RenderPixelEditorMode() {
    BeginDrawing();
    ClearBackground(pixelEditor.backgroundColor);
    DrawTexture(pixelEditor.canvasTexture, 0, 50, WHITE);
    DrawPixelEditorGrid();
    DrawColorPicker();
    DrawEraserButton();
    EndDrawing();
}
