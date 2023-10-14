#include "modes.h"
#include <stdlib.h>

EditorMode currentMode = MODE_DEFAULT;
bool panelsResetOnModeChange = true;

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



/* // UI EDITOR */
/* UIButton availableButtons[BUTTON_COUNT] = { */
/*     {"Button 1", {0, 0, 100, 50}, RED}, */
/*     {"Incrementer", {0, 0, 150, 50}, BLUE} */
/* }; */

/* UIButton placedButtons[MAX_PLACED_BUTTONS]; */
/* int placedButtonCount = 0; */
/* UIButton *selectedForPlacement = NULL; */
/* UIButton *selectedPlacedButton = NULL; */


/* void RenderUIEditorMode() { */
/*     // Draw available buttons at the bottom for selection */
/*     int startX = 10; */
/*     int previewHeight = 60; */
/*     int startY = GetScreenHeight() - previewHeight; */

/*     for (int i = 0; i < BUTTON_COUNT; i++) { */
/*         Rectangle btnBounds = availableButtons[i].bounds; */
/*         btnBounds.y = startY; */
/*         btnBounds.x = startX; */

/*         if (DrawButton(availableButtons[i].text, btnBounds, availableButtons[i].color)) { */
/*             selectedForPlacement = &availableButtons[i]; */
/*             selectedPlacedButton = NULL; */
/*         } */

/*         startX += btnBounds.width + 10; */
/*     } */

/*     Vector2 mousePos = GetMousePosition(); */

/*     // Place a new button if one is selected for placement */
/*     if (selectedForPlacement && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mousePos.y < startY) { */
/*         UIButton newButton = *selectedForPlacement; */
/*         newButton.bounds.x = mousePos.x - newButton.bounds.width / 2;   // Centered placement */
/*         newButton.bounds.y = mousePos.y - newButton.bounds.height / 2; */
/*         placedButtons[placedButtonCount++] = newButton; */
/*         selectedPlacedButton = &placedButtons[placedButtonCount - 1]; */
/*         selectedForPlacement = NULL; */
/*     } */

/*     // Render placed buttons and handle their selection */
/*     for (int i = 0; i < placedButtonCount; i++) { */
/*         if (DrawButton(placedButtons[i].text, placedButtons[i].bounds, placedButtons[i].color)) { */
/*             selectedPlacedButton = &placedButtons[i]; */
/*         } */

/*         // Draw the 2D gizmo if this button is selected */
/*         if (selectedPlacedButton == &placedButtons[i]) { */
/*             DrawRectangleLinesEx((Rectangle) { placedButtons[i].bounds.x + placedButtons[i].bounds.width, placedButtons[i].bounds.y, 20, placedButtons[i].bounds.height }, 2, RED); */
/*             DrawRectangleLinesEx((Rectangle) { placedButtons[i].bounds.x, placedButtons[i].bounds.y - 20, placedButtons[i].bounds.width, 20 }, 2, BLUE); */
/*         } */
/*     } */

/*     // Deselect the placed button if we click outside */
/*     if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && selectedPlacedButton && !CheckCollisionPointRec(mousePos, selectedPlacedButton->bounds)) { */
/*         selectedPlacedButton = NULL; */
/*     } */
/* } */





// UI EDITOR
// The array of available buttons for selection
UIButton availableButtons[BUTTON_COUNT] = {
    {"Button 1", {0, 0, 100, 50}, RED, BUTTON_NORMAL},
    {"Incrementer", {0, 0, 150, 50}, BLUE, BUTTON_INCREMENTER},
    {"Toggle", {0, 0, 150, 20}, GREEN, BUTTON_TOGGLE}
};

// Variables to manage the placement of buttons
UIButton placedButtons[MAX_PLACED_BUTTONS];
int placedButtonCount = 0;
UIButton *selectedForPlacement = NULL;
UIButton *selectedPlacedButton = NULL;
bool isDragging = false;  // New variable to track dragging

Vector2 initialClickPosition = {0, 0};  // Where the mouse was initially pressed
float mouseHoldDuration = 0.0;  // How long the mouse has been held down
const float DRAG_START_TIME = 0.3;  // Reduced time to 0.3 seconds




// Function to draw a gizmo around the selected button
void DrawGizmo(UIButton *button) {
    DrawRectangleLinesEx((Rectangle) { button->bounds.x + button->bounds.width, button->bounds.y, 20, button->bounds.height }, 2, RED);
    DrawRectangleLinesEx((Rectangle) { button->bounds.x, button->bounds.y - 20, button->bounds.width, 20 }, 2, BLUE);
}

// Function to render the available buttons in the bottom panel for selection
void DrawBottomPanelContent(void) {
    int startX = 10;
    int previewHeight = 60;
    int startY = GetScreenHeight() - previewHeight;

    for (int i = 0; i < BUTTON_COUNT; i++) {
        Rectangle btnBounds = availableButtons[i].bounds;
        btnBounds.y = startY;
        btnBounds.x = startX;
        bool isButtonClicked = false;

        switch (availableButtons[i].type) {
            case BUTTON_NORMAL:
                isButtonClicked = DrawButton(availableButtons[i].text, btnBounds, availableButtons[i].color);
                break;
            case BUTTON_INCREMENTER:
                int dummyValue = 5; // Just for display purposes
                DrawIncrementerButton(&dummyValue, "Value", "int", btnBounds.x, btnBounds.y);
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btnBounds)) {
                    isButtonClicked = true;
                }
                break;
            case BUTTON_TOGGLE:
                bool dummyState = false; // Just for display purposes
                dummyState = DrawToggleButton((Vector2){btnBounds.x, btnBounds.y}, dummyState, availableButtons[i].text);
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btnBounds)) {
                    isButtonClicked = true;
                }
                break;
        }

        if (isButtonClicked) {
            selectedForPlacement = &availableButtons[i];
            selectedPlacedButton = NULL;
        }

        if (selectedForPlacement == &availableButtons[i]) {
            DrawRectangleLines(btnBounds.x, btnBounds.y + btnBounds.height, btnBounds.width, 2, (Color){100, 91, 150, 255});
        }

        startX += btnBounds.width + 10;
    }
}

// Function to place the selected button on the main area
void PlaceSelectedButton(Vector2 mousePos) {
    if (selectedForPlacement && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        UIButton newButton = *selectedForPlacement;
        newButton.bounds.x = mousePos.x - newButton.bounds.width / 2;
        newButton.bounds.y = mousePos.y - newButton.bounds.height / 2;
        placedButtons[placedButtonCount++] = newButton;
        selectedPlacedButton = &placedButtons[placedButtonCount - 1];
        selectedForPlacement = NULL;
    }
}




void RenderUIEditorMode() {
    DrawBottomPanelContent();

    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < placedButtonCount; i++) {
        bool buttonClicked = false;

        switch (placedButtons[i].type) {
            case BUTTON_NORMAL:
                buttonClicked = DrawButton(placedButtons[i].text, placedButtons[i].bounds, placedButtons[i].color);
                break;
            case BUTTON_INCREMENTER:
                int dummyValue = 5; // Just for display purposes
                DrawIncrementerButton(&dummyValue, "Value", "int", placedButtons[i].bounds.x, placedButtons[i].bounds.y);
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), placedButtons[i].bounds) && !isDragging) {
                    buttonClicked = true;
                }
                break;
            case BUTTON_TOGGLE:
                bool dummyState = false; // Just for display purposes
                dummyState = DrawToggleButton((Vector2){placedButtons[i].bounds.x, placedButtons[i].bounds.y}, dummyState, placedButtons[i].text);
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), placedButtons[i].bounds) && !isDragging) {
                    buttonClicked = true;
                }
                break;
            default:
                break;
        }

        // If the mouse button is pressed on a placed button and the user is not dragging yet
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, placedButtons[i].bounds)) {
            initialClickPosition = mousePos;
            selectedPlacedButton = &placedButtons[i];
        }

        // If the mouse button is held down and the user started the drag on a placed button
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && selectedPlacedButton == &placedButtons[i] && Vector2Distance(initialClickPosition, mousePos) > 10.0) {
            isDragging = true;
        }

        if (isDragging && selectedPlacedButton == &placedButtons[i]) {
            selectedPlacedButton->bounds.x = mousePos.x - selectedPlacedButton->bounds.width / 2;
            selectedPlacedButton->bounds.y = mousePos.y - selectedPlacedButton->bounds.height / 2;
        }

        // If the mouse button is released, reset the drag state and the selected button
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            isDragging = false;
            selectedPlacedButton = NULL;
        }

        // If a button is selected, draw the gizmo on it
        if (selectedPlacedButton == &placedButtons[i]) {
            DrawGizmo(selectedPlacedButton);
        }
    }

    // Handle the placement of new buttons
    if (!isDragging) {
        PlaceSelectedButton(mousePos);
    }
}
