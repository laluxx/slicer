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
    MODE_MAX
} EditorMode;

extern EditorMode currentMode;

void EditorChangeMode(EditorMode mode);
void EditorNextMode(void);
void EditorPreviousMode(void);


void DrawModeBar(void);

/* void RenderSlicerMode(Texture2D sprite); */
void RenderSlicerMode(Character* character, Texture2D sprite);


#endif  // MODES_H
