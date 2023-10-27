#include "modes.h"
#include "screen.h"
#include <stdlib.h>
#include <math.h> // for ui lerp

EditorMode currentMode = MODE_DASHBOARD;
bool panelsResetOnModeChange = true;





// Placeholder function. Adjust this to return the name of the mode based on your implementation.
const char* GetModeName(EditorMode mode) {
    switch(mode) {
        case MODE_DEFAULT: return "Default";
        case MODE_UI_EDITOR: return "UI Editor";
        case MODE_EDITOR: return "Editor";
        case MODE_DASHBOARD: return "Dashboard";
        // ... other cases
        default: return "Unknown";
    }
}



void DrawModeBar() {
    // Constants for mode squares
    const int squareSize = 8;
    const int squareSpacing = 15;
    const int totalWidth = MODE_MAX * squareSize + (MODE_MAX - 1) * squareSpacing;
    const int paddingX = (SCREEN_WIDTH - totalWidth) / 2;

    // This adjustment positions the mode squares right above the modeline.
    const int paddingY = SCREEN_HEIGHT - minibuffer.height - modeline.height + 17 - squareSize;

    static int previousMode = -1;
    static float fadeFactor = 0.0f;

    if (previousMode != currentMode) {
        previousMode = currentMode;
        fadeFactor = 0.0f;
    }

    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < MODE_MAX; i++) {
            int squareX = paddingX + i * (squareSize + squareSpacing);
            if (mousePos.x >= squareX && mousePos.x <= squareX + squareSize &&
                mousePos.y >= paddingY && mousePos.y <= paddingY + squareSize) {
                currentMode = i;
                break;
            }
        }
    }

    for (int i = 0; i < MODE_MAX; i++) {
        int squareX = paddingX + i * (squareSize + squareSpacing);
        if (i == currentMode) {
            DrawRectangle(squareX, paddingY, squareSize, squareSize, ColorLerp(CURRENT_THEME.x, CURRENT_THEME.y, fadeFactor));
        } else {
            DrawRectangle(squareX, paddingY, squareSize, squareSize, CURRENT_THEME.x);
        }
    }

    fadeFactor += 0.02f;
    if (fadeFactor > 1.0f) {
        fadeFactor = 1.0f;
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

    if (currentMode == MODE_DASHBOARD && panelsResetOnModeChange) {
        PanelsReset();
    }

    if (currentMode == MODE_EDITOR && panelsResetOnModeChange) {
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




// DASHBOARD

#define MAX_RECENT_FILES 5
#define MAX_PROJECTS 5
const char* recentFiles[MAX_RECENT_FILES] = {
    "~/xos/slicer/modes.h",
    "~/xos/slicer/modes.c",
    "~/xos/slicer/main.c",
    "~/xos/slicer/",
    "~/.config/slicer/.local/etc/workspaces/autosave"
};

const char* projects[MAX_PROJECTS] = {
    "~/xos/slicer/",
    "~/Desktop/pulls/dotfiles/",
    "~/Desktop/pulls/dotfiles/polybar-collection/",
    "~/xos/graphene/",
    "~/xos/suckless/dwm/"
};



void RenderCenteredDashboard() {
    int y;

    // Drawing the title "SLICER"
    y = SCREEN_HEIGHT * 0.1;  // Starting at 10% from the top
    DrawText("SLICER", SCREEN_WIDTH / 2 - MeasureText("SLICER", 50) / 2, y, 50, CURRENT_THEME.x);

    // Drawing the subtitle "Welcome to Slicer Dashboard"
    y += 80;  // Adjust this value for vertical spacing
    DrawText("Welcome to Slicer Dashboard", SCREEN_WIDTH / 2 - MeasureText("Welcome to Slicer Dashboard", 20) / 2, y, 20, RAYWHITE);

    // Drawing "Slicer started in..." section
    y += 60;  // Adjust this value for vertical spacing
    char startText[] = "Slicer started in 2.213421 seconds";
    DrawText(startText, SCREEN_WIDTH / 2 - MeasureText(startText, 20) / 2, y, 20, RAYWHITE);

    // Drawing "Recent Files" section
    y += 60;  // Adjust this value for vertical spacing
    DrawText("Recent Files:", SCREEN_WIDTH / 2 - MeasureText("Recent Files:", 20) / 2, y, 20, CURRENT_THEME.y);
    y += 30;  // Adjust this value for vertical spacing
    for (int i = 0; i < MAX_RECENT_FILES; i++) {
        DrawText(recentFiles[i], SCREEN_WIDTH / 2 - MeasureText(recentFiles[i], 20) / 2, y, 20, RAYWHITE);
        y += 30;  // Incrementing y-position for the next item
    }

    // Drawing "Projects" section
    y += 40;  // Adjust this value for vertical spacing
    DrawText("Projects:", SCREEN_WIDTH / 2 - MeasureText("Projects:", 20) / 2, y, 20, CURRENT_THEME.y);
    y += 30;  // Adjust this value for vertical spacing
    for (int i = 0; i < MAX_PROJECTS; i++) {
        DrawText(projects[i], SCREEN_WIDTH / 2 - MeasureText(projects[i], 20) / 2, y, 20, RAYWHITE);
        y += 30;  // Incrementing y-position for the next item
    }
}


void RenderDashboard() {
    int screenWidth = GetScreenWidth();
    int y = 30;  // Initial vertical padding from the top, reduced for a closer alignment to the top

    // Assuming a left-aligned position, let's set a constant horizontal starting point
    int startX = 20;  // Initial horizontal padding from the left

    // Drawing the title "SLICER"
    DrawText("SLICER", startX, y, 50, CURRENT_THEME.x);

    // Drawing the subtitle "Welcome to Slicer Dashboard"
    y += 60;
    DrawText("Welcome to Slicer Dashboard", startX, y, 20, RAYWHITE);

    // Drawing "Slicer started in..." section
    y += 60;
    char startText[] = "Slicer started in 2.213421 seconds";
    DrawText(startText, startX, y, 20, RAYWHITE);

    // Drawing "Recent Files" section
    y += 60;
    DrawText("Recent Files:", startX, y, 20, CURRENT_THEME.y);
    y += 30;
    for (int i = 0; i < MAX_RECENT_FILES; i++) {
        DrawText(recentFiles[i], startX, y, 20, RAYWHITE);
        y += 30;
    }

    // Drawing "Projects" section
    y += 40;
    DrawText("Projects:", startX, y, 20, CURRENT_THEME.y);
    y += 30;
    for (int i = 0; i < MAX_PROJECTS; i++) {
        DrawText(projects[i], startX, y, 20, RAYWHITE);
        y += 30;
    }
}
