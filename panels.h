#ifndef PANELS_H
#define PANELS_H

#include "raylib.h"
#include "screen.h"

#define PANEL_CORNER_RADIUS 20.0f
#define INNER_CORNER_RADIUS 30.0f
#define INITIAL_BOTTOM_HEIGHT 80
#define INITIAL_LEFT_WIDTH 250
#define INITIAL_RIGHT_WIDTH 278
#define INITIAL_TOP_HEIGHT 45

// COLORS
// TODO create a theme sruct and related
// function to switch tame by index or by name
// utilizing a sentile inside the struct

// COMFY DARK
#define TOP_COLOR (Color){14, 14, 14, 255}  // #0E0E0E
#define BOTTOM_COLOR (Color){14, 14, 14, 255}  // #0E0E0E
#define LEFT_COLOR (Color){14, 14, 14, 255}  // #0E0E0E
#define RIGHT_COLOR (Color){14, 14, 14, 255}  // #0E0E0E





typedef struct {
    float topHeight;
    float bottomHeight;
    float leftWidth;
    float rightWidth;
} Panel;

extern Panel panel;
void DrawPanels();
void UpdatePanelsDimensions();
void DrawPanel(char panelSide, float dimension);
void ClosePanel(char panelSide);
void UpdatePanelDimensions(char panelSide);
void PanelsReset();

#endif // PANELS_H
