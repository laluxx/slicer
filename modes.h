#ifndef MODES_H
#define MODES_H

#include "raylib.h"  // Assume raylib is used for graphical elements
#include "screen.h"
#include "ui.h"
#include "slicer.h"
#include "log.h"
#include "panels.h"

typedef enum {
    MODE_DEFAULT,
    MODE_SLICER,
    MODE_UI_EDITOR,
    MODE_MAX
} EditorMode;

extern EditorMode currentMode;

void EditorChangeMode(EditorMode mode);
void EditorNextMode(void);
void EditorPreviousMode(void);


void DrawModeBar(void);

/* void RenderSlicerMode(Texture2D sprite); */
void RenderSlicerMode(Character* character, Texture2D sprite);


void RenderUIEditorMode(void);   // New function declaration
// UI Editor related structures and variables
typedef struct {
    char text[50];
    Rectangle bounds;
    Color color;
} UIButton;

#define BUTTON_COUNT 2
#define MAX_PLACED_BUTTONS 100

extern UIButton availableButtons[BUTTON_COUNT];
extern UIButton placedButtons[MAX_PLACED_BUTTONS];
extern int placedButtonCount;
extern UIButton *selectedForPlacement;
extern UIButton *selectedPlacedButton;



#endif  // MODES_H
