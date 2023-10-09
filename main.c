#include "raylib.h"
#include "screen.h"
#include "slicer.h"
#include "character.h"
#include "log.h"
#include "panels.h"
#include "stdio.h"
#include "stdlib.h"





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
// INSPECTORS END



// EDITOR MODES

typedef enum {
    MODE_DEFAULT,
    MODE_SLICER,
    MODE_PIXEL_EDITOR,
    MODE_MAX
} EditorMode;

EditorMode currentMode = MODE_DEFAULT;


void EditorChangeMode(EditorMode mode) {
    currentMode = mode;
    log_add("Changed editor mode");
}

void EditorNextMode() {
    currentMode = (currentMode + 1) % MODE_MAX;
    log_add("Switched to next mode");
}

void EditorPreviousMode() {
    currentMode = (currentMode - 1 + MODE_MAX) % MODE_MAX;
    log_add("Switched to previous mode");
}






void RenderSlicerMode(Texture2D sprite) {
    // Draw sprite at the center, scaled up for better visibility
    float scale = 2.0f;
    Rectangle spriteDest = { SCREEN_WIDTH/2 - sprite.width*scale/2, SCREEN_HEIGHT/2 - sprite.height*scale/2 - 100, sprite.width*scale, sprite.height*scale };
    DrawTexturePro(sprite, (Rectangle){ 0, 0, sprite.width, sprite.height }, spriteDest, (Vector2){ 0, 0 }, 0, WHITE);

    // Draw grid over the sprite
    for (int i = 0; i < SLICE_HORIZONTAL; i++) {
        for (int j = 0; j < SLICE_VERTICAL; j++) {
            Rectangle gridRect = { spriteDest.x + i*SLICE_SIZE_X*scale, spriteDest.y + j*SLICE_SIZE_Y*scale, SLICE_SIZE_X*scale, SLICE_SIZE_Y*scale };
            DrawRectangleLinesEx(gridRect, 1, RAYWHITE);
        }
    }

    int smallButtonWidth = buttonWidth / 6;  // Adjusted the width

    // Vertical slices controls (Left side of the sprite)
    if (DrawButton("-", (Rectangle){spriteDest.x - smallButtonWidth - 10, spriteDest.y, smallButtonWidth, buttonHeight}, RED)) {
        if (SLICE_VERTICAL > 1) SLICE_VERTICAL--;
    }
    if (DrawButton("+", (Rectangle){spriteDest.x - smallButtonWidth - 10, spriteDest.y + buttonHeight + 10, smallButtonWidth, buttonHeight}, GREEN)) {
        SLICE_VERTICAL++;
    }

    // Horizontal slices controls (Top side of the sprite)
    if (DrawButton("-", (Rectangle){spriteDest.x, spriteDest.y - buttonHeight - 10, smallButtonWidth, buttonHeight}, RED)) {
        if (SLICE_HORIZONTAL > 1) SLICE_HORIZONTAL--;
    }
    if (DrawButton("+", (Rectangle){spriteDest.x + smallButtonWidth + 10, spriteDest.y - buttonHeight - 10, smallButtonWidth, buttonHeight}, GREEN)) {
        SLICE_HORIZONTAL++;
    }

    // Centralized "Slice!" button, adjusted position to be a bit higher
    int sliceButtonVerticalOffset = 50;  // Adjust this value to move the button up/down as needed
    if (DrawButton("Slice!", (Rectangle){SCREEN_WIDTH/2 - buttonWidth/2, SCREEN_HEIGHT - (buttonHeight + buttonSpacing) - sliceButtonVerticalOffset, buttonWidth, buttonHeight}, BLUE)) {
        // TODO: Add functionality to slice the sprite based on current settings
    }
}




// PIXEL EDITOR MODE
typedef struct {
    Color color;
    Rectangle rect;
} ColorPicker;

typedef struct {
    Texture2D canvasTexture;
    ColorPicker colorPicker[5];
    Color currentColor;
    bool eraserActive;
    int gridSize;
    Color backgroundColor;
} PixelEditorData;

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
// PIXEL EDITOR MODE END



int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sprite Slicer");
    Texture2D sprite = LoadTexture("./sprite.png");
    Character character = { { (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2 }, IDLE_DOWN, IDLE_DOWN };
    int frameCounter = 0;
    SetTargetFPS(144);

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
                DrawSlicerInspector(sprite);  // Call the function here
                DrawEditorLog(panel.bottomHeight);
                DrawCharacter(character, sprite, frameCounter);
                DrawCharacterInspector(character, sprite, frameCounter);
                void DrawSlicerInspector(Texture2D sprite);
                break;
            case MODE_SLICER:
                DrawPanels();
                RenderSlicerMode(sprite);
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
