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



typedef enum {
    LAYOUT_MASTER_STACK,
    LAYOUT_COLUMNS,
    LAYOUT_COUNT // Sentinel for wrap-around
} LayoutType;

typedef struct {
    char* title;
    bool isOpen;
} Buffer;


typedef struct {
    Rectangle rect;
    bool isSelected;
    Buffer buffer;
    bool isFloating;                  // To determine if the frame is floating
    Rectangle prevState;             // To store the previous position and size of the frame before it started floating
} Frame;


extern Panel panel;

extern bool showTopRightCorner;
extern bool showTopLeftCorner;
extern bool showBottomRightCorner;
extern bool showBottomLeftCorner;


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

// FRAMES
void ArrangeFrames();
void HandleFrameKeyBindings();
void CreateNewFrame();
void CloseSelectedFrame();
void DrawFrames();
void UpdateFrameFocusWithMouse();
void HandleFrameMouseInteractions();
// BUFFERS
void HandleHelpBuffer();

#endif // PANELS_H
