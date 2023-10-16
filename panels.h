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

// COMFY DARK
#define TOP_COLOR (Color){14, 14, 14, 255}  // #0E0E0E
#define BOTTOM_COLOR (Color){14, 14, 14, 255}  // #0E0E0E
#define LEFT_COLOR (Color){14, 14, 14, 255}  // #0E0E0E
#define RIGHT_COLOR (Color){14, 14, 14, 255}  // #0E0E0E
#define CENTER_COLOR (Color){14, 14, 14, 255}  // #0E0E0E

#define CENTER_GAP 10.0f

typedef enum {
    FLEX_SIZE_ONE_THIRD,
    FLEX_SIZE_ONE_HALF,
    FLEX_SIZE_ONE_QUARTER
} FlexPanelSize;


typedef enum {
    FLEX_POSITION_TOP,
    FLEX_POSITION_BOTTOM,
    FLEX_POSITION_LEFT,
    FLEX_POSITION_RIGHT
} FlexPanelPosition;

typedef struct {
    float topHeight;
    float bottomHeight;
    float leftWidth;
    float rightWidth;
    Rectangle centerPanel;
    bool centerPanelVisible;
    Rectangle flexPanel;
    bool flexPanelVisible;
} Panel;

extern Panel panel;

void DrawPanels();
void UpdatePanelsDimensions();
void DrawPanel(char panelSide, float dimension);
void ClosePanel(char panelSide);
void UpdatePanelDimensions(char panelSide);
void PanelsReset();
void OpenFlexiblePanel(FlexPanelSize size, FlexPanelPosition position);
void CloseFlexiblePanel();
void AdjustLeftPanelWidthForFileNames();
void LoadCornerTextures();
void UnloadCornerTextures();


#endif // PANELS_H
