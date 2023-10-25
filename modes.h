#ifndef MODES_H
#define MODES_H

#include "raylib.h"
#include "screen.h"
#include "ui.h"
#include "slicer.h"
#include "log.h"
#include "panels.h"
#include "theme.h"

typedef enum {
    MODE_DASHBOARD,
    MODE_DEFAULT,
    MODE_UI_EDITOR,
    MODE_PLAY,
    MODE_MAX
} EditorMode;

extern EditorMode currentMode;

void EditorChangeMode(EditorMode mode);
void EditorNextMode(void);
void EditorPreviousMode(void);
void DrawModeBar(void);


// UI EDITOR
typedef enum {
    BUTTON_NORMAL,
    BUTTON_INCREMENTER,
    BUTTON_TOGGLE
} ButtonType;

// UI Editor related structures and variables
typedef struct {
    char text[50];
    Rectangle bounds;
    Color color;
    ButtonType type;
} UIButton;


#define BUTTON_COUNT 4
#define MAX_PLACED_BUTTONS 100

extern UIButton availableButtons[BUTTON_COUNT];
extern UIButton placedButtons[MAX_PLACED_BUTTONS];
extern int placedButtonCount;
extern UIButton *selectedForPlacement;
extern UIButton *selectedPlacedButton;

void RenderUIEditorMode(void);   // Main function
void DrawBottomPanelContent(void);  // Draw available buttons at the bottom
void DrawGizmo(UIButton *button);   // Draw 2D gizmo for the selected button
void PlaceSelectedButton(Vector2 mousePos);  // Place the selected button onto the main screen

#endif  // MODES_H
