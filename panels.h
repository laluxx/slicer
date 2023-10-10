#ifndef PANELS_H
#define PANELS_H

#include "raylib.h"
#include "screen.h"

#define PANEL_CORNER_RADIUS 20.0f
#define INNER_CORNER_RADIUS 30.0f
#define INITIAL_BOTTOM_HEIGHT 80
#define INITIAL_LEFT_WIDTH 150
#define INITIAL_RIGHT_WIDTH 280
#define INITIAL_TOP_HEIGHT 45

// COLORS
#define TOP_COLOR (Color){105, 58, 14, 255}
#define BOTTOM_COLOR (Color){85, 48, 10, 255}
#define LEFT_COLOR (Color){75, 38, 5, 255}
#define RIGHT_COLOR (Color){95, 50, 12, 255}

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
