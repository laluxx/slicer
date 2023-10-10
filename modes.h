#ifndef MODES_H
#define MODES_H

#include "raylib.h"  // Assume raylib is used for graphical elements
#include "screen.h"
#include "ui.h"
#include "slicer.h"
#include "log.h"

typedef enum {
    MODE_DEFAULT,
    MODE_SLICER,
    MODE_PIXEL_EDITOR,
    MODE_MAX
} EditorMode;

extern EditorMode currentMode;

void EditorChangeMode(EditorMode mode);
void EditorNextMode(void);
void EditorPreviousMode(void);

/* void RenderSlicerMode(Texture2D sprite); */
void RenderSlicerMode(Character* character, Texture2D sprite);

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

extern PixelEditorData pixelEditor;

void InitializePixelEditor(char* imagePath);
void HandlePixelEditorInput(void);
void DrawPixelEditorGrid(void);
void DrawColorPicker(void);
void DrawEraserButton(void);
void UpdatePixelEditor(void);
void RenderPixelEditorMode(void);

#endif  // MODES_H
