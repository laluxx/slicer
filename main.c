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
#include "window.h"
#include "filemanager.h"
#include "theme.h"
#include <stdbool.h>





// INSPECTORS
#define CHARACTER_INSPECTOR_TEXT_SIZE 20
void DrawCharacterCoordinatesInspector(Character character) {
    int xOffset = 10;  // Offset from the left where text starts
    int yOffset = 10;  // Offset from the top where text starts

    Vector2 positionTextPos = { SCREEN_WIDTH - 150 + xOffset, SCREEN_HEIGHT - 150 + yOffset };

    char xPosStr[20], yPosStr[20];
    snprintf(xPosStr, sizeof(xPosStr), "x: %d", (int) character.position.x);
    snprintf(yPosStr, sizeof(yPosStr), "y: %d", (int) character.position.y);

    DrawText(xPosStr, positionTextPos.x, positionTextPos.y, CHARACTER_INSPECTOR_TEXT_SIZE, CURRENT_THEME.x);
    DrawText(yPosStr, positionTextPos.x, positionTextPos.y + CHARACTER_INSPECTOR_TEXT_SIZE + 5, CHARACTER_INSPECTOR_TEXT_SIZE, CURRENT_THEME.y);
}

void DrawCharacterInspector(Character character, Texture2D sprite, int frameCounter) {
    DrawCharacterCoordinatesInspector(character);


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



// MOUSE INSPECTOR

typedef struct Cursor {
    int x;
    int y;
} Cursor;

bool cameraManagerEnabled = false;
bool coordinateLinesEnabled = true;

// Define the paths to the mouse button images
#define MOUSE_LEFT_KEY_PATH "./icons/input-prompts/KeyboardandMouse/Dark/Mouse_Left_Key_Dark.png"
#define MOUSE_MIDDLE_KEY_PATH "./icons/input-prompts/KeyboardandMouse/Dark/Mouse_Middle_Key_Dark.png"
#define MOUSE_RIGHT_KEY_PATH "./icons/input-prompts/KeyboardandMouse/Dark/Mouse_Right_Key_Dark.png"
#define MOUSE_SIMPLE_KEY_PATH "./icons/input-prompts/KeyboardandMouse/Dark/Mouse_Simple_Key_Dark.png"

// Assume these are global or loaded before your game loop
Texture2D mouseLeftKey;
Texture2D mouseMiddleKey;
Texture2D mouseRightKey;
Texture2D mouseSimpleKey;

void LoadMouseTextures() {
    mouseLeftKey = LoadTexture(MOUSE_LEFT_KEY_PATH);
    mouseMiddleKey = LoadTexture(MOUSE_MIDDLE_KEY_PATH);
    mouseRightKey = LoadTexture(MOUSE_RIGHT_KEY_PATH);
    mouseSimpleKey = LoadTexture(MOUSE_SIMPLE_KEY_PATH);
}

void UnloadMouseTextures() {
    UnloadTexture(mouseLeftKey);
    UnloadTexture(mouseMiddleKey);
    UnloadTexture(mouseRightKey);
    UnloadTexture(mouseSimpleKey);
}

void DrawMouseInspector(void) {

    if (!coordinateLinesEnabled){
        return;
    }

    Vector2 mousePosition = GetMousePosition();


    // Draw red horizontal line and blue vertical line
    DrawLine(0, (int)mousePosition.y, SCREEN_WIDTH, (int)mousePosition.y, RED);
    DrawLine((int)mousePosition.x, 0, (int)mousePosition.x, SCREEN_HEIGHT, BLUE);

    // Scale factor to adjust the size of the keys
    float scaleFactor = 0.40f;  // MOUSE SIZE
    int iconOffsetX = 20;
    int iconOffsetY = 20;

    // Draw the corresponding mouse button icon larger and at the offset mouse position when a button is pressed
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        DrawTexturePro(mouseLeftKey, (Rectangle){0, 0, mouseLeftKey.width, mouseLeftKey.height}, (Rectangle){mousePosition.x + iconOffsetX, mousePosition.y + iconOffsetY, mouseLeftKey.width*scaleFactor, mouseLeftKey.height*scaleFactor}, (Vector2){0, 0}, 0, WHITE);
    }
    if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
        DrawTexturePro(mouseMiddleKey, (Rectangle){0, 0, mouseMiddleKey.width, mouseMiddleKey.height}, (Rectangle){mousePosition.x + iconOffsetX, mousePosition.y + iconOffsetY, mouseMiddleKey.width*scaleFactor, mouseMiddleKey.height*scaleFactor}, (Vector2){0, 0}, 0, WHITE);
    }
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        DrawTexturePro(mouseRightKey, (Rectangle){0, 0, mouseRightKey.width, mouseRightKey.height}, (Rectangle){mousePosition.x + iconOffsetX, mousePosition.y + iconOffsetY, mouseRightKey.width*scaleFactor, mouseRightKey.height*scaleFactor}, (Vector2){0, 0}, 0, WHITE);
    }
}

// Function to draw the cursor coordinates and toggle buttons
void DrawCursorCoordinatesInspector(Cursor cursor) {
    int xOffset = 10;  // Offset from the left where text starts
    int yOffset = 10;  // Offset from the top where text starts

    Vector2 positionTextPos = { SCREEN_WIDTH - 370 + xOffset, SCREEN_HEIGHT - 1030 + yOffset };

    char xPosStr[20], yPosStr[20];
    snprintf(xPosStr, sizeof(xPosStr), "%d", cursor.x);
    snprintf(yPosStr, sizeof(yPosStr), "%d", cursor.y);

    DrawText("x: ", positionTextPos.x, positionTextPos.y, CHARACTER_INSPECTOR_TEXT_SIZE, CURRENT_THEME.x);
    DrawText(xPosStr, positionTextPos.x + MeasureText("x: ", CHARACTER_INSPECTOR_TEXT_SIZE), positionTextPos.y, CHARACTER_INSPECTOR_TEXT_SIZE, CURRENT_THEME.x);

    DrawText("y: ", positionTextPos.x, positionTextPos.y + CHARACTER_INSPECTOR_TEXT_SIZE + 5, CHARACTER_INSPECTOR_TEXT_SIZE, CURRENT_THEME.y);
    DrawText(yPosStr, positionTextPos.x + MeasureText("y: ", CHARACTER_INSPECTOR_TEXT_SIZE), positionTextPos.y + CHARACTER_INSPECTOR_TEXT_SIZE + 5, CHARACTER_INSPECTOR_TEXT_SIZE, CURRENT_THEME.y);

    // Draw toggle buttons
    coordinateLinesEnabled = DrawIosToggleButton((Vector2){ SCREEN_WIDTH - 390, SCREEN_HEIGHT - 910 }, coordinateLinesEnabled);
}

void DrawCursorCoordinatesLines(Cursor cursor) {
    int lineThickness = 2; // default value

    Vector2 startVertical, endVertical, startHorizontal, endHorizontal;

    if (cursor.x < panel.leftWidth) {
        startVertical.x = endVertical.x = panel.leftWidth + lineThickness / 2.0; // Adjust for thickness
    } else if (cursor.x > SCREEN_WIDTH - panel.rightWidth) {
        startVertical.x = endVertical.x = SCREEN_WIDTH - panel.rightWidth - lineThickness / 2.0; // Adjust for thickness
    } else {
        startVertical.x = endVertical.x = cursor.x;
    }

    startVertical.y = panel.topHeight;
    endVertical.y = SCREEN_HEIGHT - panel.bottomHeight;

    if (cursor.y < panel.topHeight) {
        startHorizontal.y = endHorizontal.y = panel.topHeight + lineThickness / 2.0; // Adjust for thickness
    } else if (cursor.y > SCREEN_HEIGHT - panel.bottomHeight) {
        startHorizontal.y = endHorizontal.y = SCREEN_HEIGHT - panel.bottomHeight - lineThickness / 2.0; // Adjust for thickness
    } else {
        startHorizontal.y = endHorizontal.y = cursor.y;
    }

    startHorizontal.x = panel.leftWidth;
    endHorizontal.x = SCREEN_WIDTH - panel.rightWidth;

    if (coordinateLinesEnabled) {
        if (cursor.y < panel.topHeight || cursor.y > SCREEN_HEIGHT - panel.bottomHeight) {
            DrawLineEx(startVertical, endVertical, lineThickness, CURRENT_THEME.y);
            DrawLineEx(startHorizontal, endHorizontal, lineThickness, CURRENT_THEME.x);
        } else {
            DrawLineEx(startHorizontal, endHorizontal, lineThickness, CURRENT_THEME.x);
            DrawLineEx(startVertical, endVertical, lineThickness, CURRENT_THEME.y);
        }
    }
}


// TODO movable rect(camera) ontop of another rect(screen prportion)
// this will visualize the proportion of the screen, camera and let the camera be move by dragging the camera rect
void DrawCameraInspector(){
    cameraManagerEnabled = DrawToggleButton((Vector2){ SCREEN_WIDTH - 295, SCREEN_HEIGHT - 350 }, cameraManagerEnabled, "Camera Manager");
}
// INSPECTORS END



// CAMERA MANAGER
#define LERP_AMOUNT 0.030f

// CameraManager START
typedef struct {
    Camera2D camera;
    float lerpAmount;
} CameraManager;

CameraManager cameraManager;

float Lerp(float start, float end, float amount) {
    return start + amount * (end - start);
}


void InitializeCameraManager(CameraManager *cm, Vector2 initialPosition) {
    cm->camera.target = initialPosition;
    cm->camera.offset = (Vector2){SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f};
    cm->camera.rotation = 0.0f;
    cm->camera.zoom = 1.0f;
    cm->lerpAmount = LERP_AMOUNT;
}

void UpdateCameraManager(CameraManager *cm, Vector2 targetPosition) {
    cm->camera.target.x = Lerp(cm->camera.target.x, targetPosition.x, cm->lerpAmount);
    cm->camera.target.y = Lerp(cm->camera.target.y, targetPosition.y, cm->lerpAmount);
}
// CameraManager END



// Initialize the ColorPicker structure
ColorPicker colorPicker = {
    .gradientBox = (Rectangle){100, 100, 256, 256},
    .spectrumBox = (Rectangle){100 + 256 + 10, 100, 20, 256},
    .selectedColorBox = (Rectangle){100 + 256 + 10 + 20 + 10, 100, 50, 50},
    .selectedColor = BLACK  // Default selected color
};


int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Slicer");
    SetExitKey(0); // Disable the exit key
    InitializeFileManager("."); // move in filemanager.c
    Texture2D sprite = LoadTexture("./sprite.png"); // move in character.c
    LoadToggleTextures();
    LoadCornerTextures();
    InitializeThemes();






    Window myWindow;  // Declare myWindow with type Window // move in window.c
    myWindow = CreateWindow((Vector2){200, 100}, (Vector2){400, 300}, "My Window"); // move in window.c


    Character character = { { (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2 }, IDLE_DOWN, IDLE_DOWN };
    int frameCounter = 0;
    SetTargetFPS(144);

    // Load WASD textures
    LoadWASDTextures();
    LoadMouseTextures();


    // CAMERA MANAGER
    InitializeCameraManager(&cameraManager, character.position);  // Initialize the camera manager

    while (!WindowShouldClose()) {
        Cursor cursor;
        cursor.x = GetMouseX();  // Update cursor x position with current mouse x position
        cursor.y = GetMouseY();  // Update cursor y position with current mouse y position

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

        // Check for F5 key press
        if (IsKeyPressed(KEY_F5)) {
            // Save any necessary game state here

            // Run the make command
            system("make");

            // Restart the game engine
            char *args[] = { "./slicer", NULL };  // Assuming the binary name is "slicer"
            execv(args[0], args);

            // If execv fails, you can handle the error here
            perror("Failed to reload the game engine");
            exit(1);
        }




        // THEME KEYBINDS
        if (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) {  // Check if Alt key is pressed
            if (IsKeyPressed(KEY_MINUS)) {    // Check if '-' key is pressed
                PreviousTheme();    // Go to the previous theme
            } else if (IsKeyPressed(KEY_EQUAL)) {    // Check if '=' key is pressed
                NextTheme();    // Go to the next theme
            }
        }



        if (IsKeyPressed(KEY_N)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                EditorPreviousMode();
            } else {
                EditorNextMode();
            }
        }


        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseFlexiblePanel();
        }

        if (IsKeyPressed(KEY_C)) {
            cameraManagerEnabled = !cameraManagerEnabled;
        }

        if (IsKeyPressed(KEY_O)) {
            panel.centerPanelVisible = !panel.centerPanelVisible;
        }


        if (cameraManagerEnabled) {
            UpdateCameraManager(&cameraManager, character.position);
        }

        BeginDrawing();
        ClearBackground(CURRENT_THEME.background);

        if (cameraManagerEnabled) {
            BeginMode2D(cameraManager.camera);
        }

        switch (currentMode) {
            case MODE_DASHBOARD:
            ClosePanel('T');  // Close top panel
            ClosePanel('B');  // Close right panel
            ClosePanel('R');  // Close right panel
            showBottomRightCorner = 0;
            showBottomLeftCorner = 0;
            showTopLeftCorner = 0;
            showTopRightCorner = 0;


            DrawPanel('L', 580.0f); // for a fixed left panel
            DrawPanels();


            HandleFrameKeyBindings();
            DrawFrames();

            RenderDashboard();
            float minibufferHeight = 21.0f; // Default height, change as needed

            DrawModeline(SCREEN_WIDTH, minibufferHeight);
            DrawMiniBuffer(SCREEN_WIDTH, minibufferHeight);
            break;


            case MODE_DEFAULT:
            showBottomRightCorner = 1;
            showBottomLeftCorner = 1;
            showTopLeftCorner = 1;
            showTopRightCorner = 1;

            DrawPanel('R', 280.0f); // for a fixed right panel


            HandleFrameKeyBindings();
            DrawFrames();

            UpdatePanelsDimensions();
            DrawMouseInspector();
            DrawCursorCoordinatesLines(cursor);
            DrawCharacter(character, sprite, frameCounter);
            DrawPanels();


            minibufferHeight = 21.0f; // Default height, change as needed

            DrawModeline(SCREEN_WIDTH, minibufferHeight);
            DrawMiniBuffer(SCREEN_WIDTH, minibufferHeight);

            // buttons
            panel.centerPanelVisible = DrawToggleButton(
                (Vector2){SCREEN_WIDTH - 294, SCREEN_HEIGHT - 570},
                panel.centerPanelVisible,
                panel.centerPanelVisible ? "Hide Center Panel"
                                         : "Show Center Panel");

            DrawFPS(120, 10);

            UpdateFileManager();
            DrawFileManager();

            DrawModeBar();

            DrawCameraInspector();
            DrawCursorCoordinatesInspector(cursor);
            DrawWASDInspector();
            DrawSlicerInspector(sprite);             // Call the function here
            /* DrawEditorLog(panel.bottomHeight); */ // TODO
            /* OpenFlexiblePanel(FLEX_SIZE_ONE_QUARTER, FLEX_POSITION_BOTTOM);
             */
            DrawCharacterInspector(character, sprite, frameCounter);

            break;


            case MODE_EDITOR:
            showBottomRightCorner = 0;
            showBottomLeftCorner = 0;
            showTopLeftCorner = 0;
            showTopRightCorner = 0;

            ClosePanel('T');
            ClosePanel('R');
            ClosePanel('B');


            UpdatePanelsDimensions();
            DrawPanels();



            HandleFrameKeyBindings();
            DrawFrames();






            minibufferHeight = 21.0f;

            DrawModeline(SCREEN_WIDTH, minibufferHeight);
            DrawMiniBuffer(SCREEN_WIDTH, minibufferHeight);


            UpdateFileManager();
            DrawFileManager();

            DrawModeBar();

            break;








            case MODE_UI_EDITOR:
                showBottomLeftCorner = false;
                showTopLeftCorner = false;
                showTopRightCorner = false;

                UpdatePanelsDimensions();
                ClosePanel('T');  // Close top panel
                /* ClosePanel('R');  // Close right panel */
                ClosePanel('L');  // Close left panela


                HandleFrameKeyBindings();
                DrawFrames();

                DrawPanel('B', 70.0f);  // for a fixed bottom panel
                DrawPanel('R', 370.0f);  // for a fixed right panel
                DrawPanels();  // Draw all panels
                DrawFPS(120, 10);
                RenderUIEditorMode();
                DrawColorPicker(&colorPicker, 1460, -70, 1);

                DrawModeBar();
            case MODE_PLAY:
                UpdatePanelsDimensions();
                ClosePanel('T');  // Close top panel
                ClosePanel('R');  // Close right panel
                ClosePanel('L');  // Close left panel
                ClosePanel('B');  // Close bottom panel
                DrawPanels();  // Draw all panels
                DrawCharacter(character, sprite, frameCounter);
            default:
                break;
        }

        if (cameraManagerEnabled) {
            EndMode2D();
        }

        EndDrawing();
        frameCounter++;
    }

    // Freeing memory
    UnloadToggleTextures();
    UnloadCornerTextures();
    UnloadTexture(sprite);
    UnloadMouseTextures();
    CloseWindow();
    return 0;
}
