/* #include "panels.h" */

/* Panel panel = { */
/*     INITIAL_TOP_HEIGHT, */
/*     INITIAL_BOTTOM_HEIGHT, */
/*     INITIAL_LEFT_WIDTH, */
/*     INITIAL_RIGHT_WIDTH */
/* }; */


/* void DrawPanel(char panelSide) { */
/*     switch (panelSide) { */
/*         case 'T': panel.topVisible = true; break; */
/*         case 'B': panel.bottomVisible = true; break; */
/*         case 'L': panel.leftVisible = true; break; */
/*         case 'R': panel.rightVisible = true; break; */
/*         default: break; */
/*     } */
/* } */


/* void DrawPanels() { */
/*     DrawRectangle(0, 0, SCREEN_WIDTH, panel.topHeight, TOP_COLOR); */
/*     DrawRectangle(0, SCREEN_HEIGHT - panel.bottomHeight, SCREEN_WIDTH, panel.bottomHeight, BOTTOM_COLOR); */
/*     DrawRectangle(0, panel.topHeight, panel.leftWidth, SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight, LEFT_COLOR); */
/*     DrawRectangle(SCREEN_WIDTH - panel.rightWidth, panel.topHeight, panel.rightWidth, SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight, RIGHT_COLOR); */
/* } */



/* void UpdatePanelDimensions() { */
/*     const float DRAG_SENSITIVITY = 20.0f; */

/*     if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) { */
/*         Vector2 mousePos = GetMousePosition(); */

/*         if (mousePos.y <= panel.topHeight + DRAG_SENSITIVITY && mousePos.y >= panel.topHeight - DRAG_SENSITIVITY) { */
/*             panel.topHeight += GetMouseDelta().y; */
/*         } */
/*         if (mousePos.y >= SCREEN_HEIGHT - panel.bottomHeight - DRAG_SENSITIVITY && mousePos.y <= SCREEN_HEIGHT - panel.bottomHeight + DRAG_SENSITIVITY) { */
/*             panel.bottomHeight -= GetMouseDelta().y; */
/*         } */
/*         if (mousePos.x <= panel.leftWidth + DRAG_SENSITIVITY && mousePos.x >= panel.leftWidth - DRAG_SENSITIVITY) { */
/*             panel.leftWidth += GetMouseDelta().x; */
/*         } */
/*         if (mousePos.x >= SCREEN_WIDTH - panel.rightWidth - DRAG_SENSITIVITY && mousePos.x <= SCREEN_WIDTH - panel.rightWidth + DRAG_SENSITIVITY) { */
/*             panel.rightWidth -= GetMouseDelta().x; */
/*         } */
/*     } */
/* } */





#include "panels.h"

Panel panel = {
    INITIAL_TOP_HEIGHT,
    INITIAL_BOTTOM_HEIGHT,
    INITIAL_LEFT_WIDTH,
    INITIAL_RIGHT_WIDTH
};

void PanelsReset() {
    panel = (Panel){
        INITIAL_TOP_HEIGHT,
        INITIAL_BOTTOM_HEIGHT,
        INITIAL_LEFT_WIDTH,
        INITIAL_RIGHT_WIDTH
    };
    log_add("Reset panel dimensions for MODE_DEFAULT");
}


void DrawPanels() {
    DrawRectangle(0, 0, SCREEN_WIDTH, panel.topHeight, TOP_COLOR);
    DrawRectangle(0, SCREEN_HEIGHT - panel.bottomHeight, SCREEN_WIDTH, panel.bottomHeight, BOTTOM_COLOR);
    DrawRectangle(0, panel.topHeight, panel.leftWidth, SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight, LEFT_COLOR);
    DrawRectangle(SCREEN_WIDTH - panel.rightWidth, panel.topHeight, panel.rightWidth, SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight, RIGHT_COLOR);
}

void UpdatePanelsDimensions() {
    const float DRAG_SENSITIVITY = 20.0f;

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

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

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

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
