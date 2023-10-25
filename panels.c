#include "panels.h"
#include "theme.h"


// CONFIGURATION
bool showTopRightCorner;
bool showTopLeftCorner;
bool showBottomRightCorner;
bool showBottomLeftCorner;


Panel panel = {
    INITIAL_TOP_HEIGHT,
    INITIAL_BOTTOM_HEIGHT,
    INITIAL_LEFT_WIDTH,
    INITIAL_RIGHT_WIDTH,
    {0, 0, 0, 0}, // Initialize the center panel rectangle
    false,       // Center panel is not visible initially
    {0, 0, 0, 0}, // Initialize the flexible panel rectangle
    false        // Flexible panel is not visible initially
};



// Define textures for the corner images
Texture2D topLeftCornerTexture;
Texture2D topRightCornerTexture;
Texture2D bottomLeftCornerTexture;
Texture2D bottomRightCornerTexture;

void LoadCornerTextures() {
    topLeftCornerTexture = LoadTexture("./icons/ui/png/top-left-corner.png");
    topRightCornerTexture = LoadTexture("./icons/ui/png/top-right-corner.png");
    bottomLeftCornerTexture = LoadTexture("./icons/ui/png/bottom-left-corner.png");
    bottomRightCornerTexture = LoadTexture("./icons/ui/png/bottom-right-corner.png");
}


void UnloadCornerTextures() {
    UnloadTexture(topLeftCornerTexture);
    UnloadTexture(topRightCornerTexture);
    UnloadTexture(bottomLeftCornerTexture);
    UnloadTexture(bottomRightCornerTexture);
}








void PanelsReset() {
    panel = (Panel){
        INITIAL_TOP_HEIGHT,
        INITIAL_BOTTOM_HEIGHT,
        INITIAL_LEFT_WIDTH,
        INITIAL_RIGHT_WIDTH,
        {0, 0, 0, 0},
        false,
        {0, 0, 0, 0},
        false
    };
    log_add("Reset panel dimensions for MODE_DEFAULT");
}


void DrawPanels() {
    DrawRectangle(0, 0, SCREEN_WIDTH, panel.topHeight, CURRENT_THEME.panel_top);
    DrawRectangle(0, SCREEN_HEIGHT - panel.bottomHeight, SCREEN_WIDTH, panel.bottomHeight, CURRENT_THEME.panel_bottom);
    DrawRectangle(0, panel.topHeight, panel.leftWidth, SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight, CURRENT_THEME.panel_left);
    DrawRectangle(SCREEN_WIDTH - panel.rightWidth, panel.topHeight, panel.rightWidth, SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight, CURRENT_THEME.panel_right);


    // Drawing the center panel
    if (panel.centerPanelVisible) {
        panel.centerPanel.x = panel.leftWidth + CENTER_GAP;
        panel.centerPanel.y = panel.topHeight + CENTER_GAP;
        panel.centerPanel.width = SCREEN_WIDTH - panel.leftWidth - panel.rightWidth - 2 * CENTER_GAP;
        panel.centerPanel.height = SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight - 2 * CENTER_GAP;
        DrawRectangleRec(panel.centerPanel, CURRENT_THEME.panel_center);
    }
    // Drawing the flexible panel
    if (panel.flexPanelVisible) {
        DrawRectangleRec(panel.flexPanel, CURRENT_THEME.panel_center);
    }



    // Draw the corner images at the adjusted positions
    if (showTopLeftCorner){
        DrawTexture(topLeftCornerTexture, panel.leftWidth, panel.topHeight, CURRENT_THEME.panel_corners);
    }
    if (showTopRightCorner){
        DrawTexture(topRightCornerTexture, SCREEN_WIDTH - panel.rightWidth - topRightCornerTexture.width, panel.topHeight, CURRENT_THEME.panel_corners);
    }

    if (showBottomLeftCorner){
        DrawTexture(bottomLeftCornerTexture, panel.leftWidth, SCREEN_HEIGHT - panel.bottomHeight - bottomLeftCornerTexture.height, CURRENT_THEME.panel_corners);
    }

    if (showBottomRightCorner){
        DrawTexture(bottomRightCornerTexture, SCREEN_WIDTH - panel.rightWidth - bottomRightCornerTexture.width, SCREEN_HEIGHT - panel.bottomHeight - bottomRightCornerTexture.height, CURRENT_THEME.panel_corners);
    }





}

void UpdatePanelsDimensions() {
    const float DRAG_SENSITIVITY = 20.0f;
    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (mousePos.y <= panel.topHeight + DRAG_SENSITIVITY && mousePos.y >= panel.topHeight - DRAG_SENSITIVITY) {
            panel.topHeight += GetMouseDelta().y;
        }
        if (mousePos.y >= SCREEN_HEIGHT - panel.bottomHeight - DRAG_SENSITIVITY && mousePos.y <= SCREEN_HEIGHT - panel.bottomHeight + DRAG_SENSITIVITY) {
            panel.bottomHeight -= GetMouseDelta().y;
        }
        if (mousePos.x <= panel.leftWidth + DRAG_SENSITIVITY && mousePos.x >= panel.leftWidth - DRAG_SENSITIVITY) {
            panel.leftWidth += GetMouseDelta().x;
        }
        if (mousePos.x >= SCREEN_WIDTH - panel.rightWidth - DRAG_SENSITIVITY && mousePos.x <= SCREEN_WIDTH - panel.rightWidth + DRAG_SENSITIVITY) {
            panel.rightWidth -= GetMouseDelta().x;
        }
    }
}


void UpdatePanelDimensions(char panelSide) {
    const float DRAG_SENSITIVITY = 20.0f;
    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        switch (panelSide) {
            case 'T':
                if (mousePos.y <= panel.topHeight + DRAG_SENSITIVITY && mousePos.y >= panel.topHeight - DRAG_SENSITIVITY) {
                    panel.topHeight += GetMouseDelta().y;
                }
                break;
            case 'B':
                if (mousePos.y >= SCREEN_HEIGHT - panel.bottomHeight - DRAG_SENSITIVITY && mousePos.y <= SCREEN_HEIGHT - panel.bottomHeight + DRAG_SENSITIVITY) {
                    panel.bottomHeight -= GetMouseDelta().y;
                }
                break;
            case 'L':
                if (mousePos.x <= panel.leftWidth + DRAG_SENSITIVITY && mousePos.x >= panel.leftWidth - DRAG_SENSITIVITY) {
                    panel.leftWidth += GetMouseDelta().x;
                }
                break;
            case 'R':
                if (mousePos.x >= SCREEN_WIDTH - panel.rightWidth - DRAG_SENSITIVITY && mousePos.x <= SCREEN_WIDTH - panel.rightWidth + DRAG_SENSITIVITY) {
                    panel.rightWidth -= GetMouseDelta().x;
                }
                break;
            default:
                break;
        }
    }
}

void DrawPanel(char panelSide, float dimension) {
    switch (panelSide) {
        case 'T': panel.topHeight = dimension; break;
        case 'B': panel.bottomHeight = dimension; break;
        case 'L': panel.leftWidth = dimension; break;
        case 'R': panel.rightWidth = dimension; break;
        default: break;
    }
}

void ClosePanel(char panelSide) {
    switch (panelSide) {
        case 'T': panel.topHeight = 0; break;
        case 'B': panel.bottomHeight = 0; break;
        case 'L': panel.leftWidth = 0; break;
        case 'R': panel.rightWidth = 0; break;
        default: break;
    }
}

/* void OpenFlexiblePanel(FlexPanelSize size, FlexPanelPosition position) { */
/*     panel.flexPanelVisible = true; */

/*     float flexWidth = SCREEN_WIDTH - panel.leftWidth - panel.rightWidth - 2 * CENTER_GAP; */
/*     float flexHeight = SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight - 2 * CENTER_GAP; */

/*     switch (position) { */
/*         case FLEX_POSITION_TOP: */
/*             panel.flexPanel = (Rectangle){panel.leftWidth + CENTER_GAP, panel.topHeight + CENTER_GAP, flexWidth, (size == FLEX_SIZE_ONE_HALF) ? (flexHeight / 2) : (flexHeight / 3)}; */
/*             break; */
/*         case FLEX_POSITION_BOTTOM: */
/*             panel.flexPanel = (Rectangle){panel.leftWidth + CENTER_GAP, SCREEN_HEIGHT - ((size == FLEX_SIZE_ONE_HALF) ? (flexHeight / 2) : (flexHeight / 3)) - panel.bottomHeight - CENTER_GAP, flexWidth, (size == FLEX_SIZE_ONE_HALF) ? (flexHeight / 2) : (flexHeight / 3)}; */
/*             break; */
/*         case FLEX_POSITION_LEFT: */
/*             panel.flexPanel = (Rectangle){panel.leftWidth + CENTER_GAP, panel.topHeight + CENTER_GAP, (size == FLEX_SIZE_ONE_HALF) ? (flexWidth / 2) : (flexWidth / 3), flexHeight}; */
/*             break; */
/*         case FLEX_POSITION_RIGHT: */
/*             panel.flexPanel = (Rectangle){SCREEN_WIDTH - ((size == FLEX_SIZE_ONE_HALF) ? (flexWidth / 2) : (flexWidth / 3)) - panel.rightWidth - CENTER_GAP, panel.topHeight + CENTER_GAP, (size == FLEX_SIZE_ONE_HALF) ? (flexWidth / 2) : (flexWidth / 3), flexHeight}; */
/*             break; */
/*     } */
/* } */


void OpenFlexiblePanel(FlexPanelSize size, FlexPanelPosition position) {
    panel.flexPanelVisible = true;

    float flexWidth = SCREEN_WIDTH - panel.leftWidth - panel.rightWidth - 2 * CENTER_GAP;
    float flexHeight = SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight - 2 * CENTER_GAP;

    switch (position) {
        case FLEX_POSITION_TOP:
            if (size == FLEX_SIZE_ONE_HALF) {
                panel.flexPanel = (Rectangle){panel.leftWidth + CENTER_GAP, panel.topHeight + CENTER_GAP, flexWidth, flexHeight / 2};
            } else if (size == FLEX_SIZE_ONE_THIRD) {
                panel.flexPanel = (Rectangle){panel.leftWidth + CENTER_GAP, panel.topHeight + CENTER_GAP, flexWidth, flexHeight / 3};
            } else { // FLEX_SIZE_ONE_QUARTER
                panel.flexPanel = (Rectangle){panel.leftWidth + CENTER_GAP, panel.topHeight + CENTER_GAP, flexWidth, flexHeight / 4};
            }
            break;

        case FLEX_POSITION_BOTTOM:
            if (size == FLEX_SIZE_ONE_HALF) {
                panel.flexPanel = (Rectangle){panel.leftWidth + CENTER_GAP, SCREEN_HEIGHT - (flexHeight / 2) - panel.bottomHeight - CENTER_GAP, flexWidth, flexHeight / 2};
            } else if (size == FLEX_SIZE_ONE_THIRD) {
                panel.flexPanel = (Rectangle){panel.leftWidth + CENTER_GAP, SCREEN_HEIGHT - (flexHeight / 3) - panel.bottomHeight - CENTER_GAP, flexWidth, flexHeight / 3};
            } else { // FLEX_SIZE_ONE_QUARTER
                panel.flexPanel = (Rectangle){panel.leftWidth + CENTER_GAP, SCREEN_HEIGHT - (flexHeight / 4) - panel.bottomHeight - CENTER_GAP, flexWidth, flexHeight / 4};
            }
            break;

        case FLEX_POSITION_LEFT:
            if (size == FLEX_SIZE_ONE_HALF) {
                panel.flexPanel = (Rectangle){panel.leftWidth + CENTER_GAP, panel.topHeight + CENTER_GAP, flexWidth / 2, flexHeight};
            } else if (size == FLEX_SIZE_ONE_THIRD) {
                panel.flexPanel = (Rectangle){panel.leftWidth + CENTER_GAP, panel.topHeight + CENTER_GAP, flexWidth / 3, flexHeight};
            } else { // FLEX_SIZE_ONE_QUARTER
                panel.flexPanel = (Rectangle){panel.leftWidth + CENTER_GAP, panel.topHeight + CENTER_GAP, flexWidth / 4, flexHeight};
            }
            break;

        case FLEX_POSITION_RIGHT:
            if (size == FLEX_SIZE_ONE_HALF) {
                panel.flexPanel = (Rectangle){SCREEN_WIDTH - (flexWidth / 2) - panel.rightWidth - CENTER_GAP, panel.topHeight + CENTER_GAP, flexWidth / 2, flexHeight};
            } else if (size == FLEX_SIZE_ONE_THIRD) {
                panel.flexPanel = (Rectangle){SCREEN_WIDTH - (flexWidth / 3) - panel.rightWidth - CENTER_GAP, panel.topHeight + CENTER_GAP, flexWidth / 3, flexHeight};
            } else { // FLEX_SIZE_ONE_QUARTER
                panel.flexPanel = (Rectangle){SCREEN_WIDTH - (flexWidth / 4) - panel.rightWidth - CENTER_GAP, panel.topHeight + CENTER_GAP, flexWidth / 4, flexHeight};
            }
            break;
    }
}



void CloseFlexiblePanel() {
    panel.flexPanelVisible = false;
}
