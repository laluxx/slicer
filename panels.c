#include "panels.h"
#include "theme.h"
#include "string.h"
#include <stdio.h>
/* #include <cstddef> */


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

void OpenLeftFlexiblePanel() {
    OpenFlexiblePanel(FLEX_SIZE_ONE_HALF, FLEX_POSITION_LEFT);
}

void OpenRightFlexiblePanel() {
    OpenFlexiblePanel(FLEX_SIZE_ONE_HALF, FLEX_POSITION_RIGHT);
}

void OpenTopFlexiblePanel() {
    OpenFlexiblePanel(FLEX_SIZE_ONE_HALF, FLEX_POSITION_TOP);
}

void OpenBottomFlexiblePanel() {
    OpenFlexiblePanel(FLEX_SIZE_ONE_HALF, FLEX_POSITION_BOTTOM);
}


// FRAMES

#define MAX_FRAMES 100
#define FRAME_GAP 10


Frame frames[MAX_FRAMES];
int frameCount = 0;
int selectedIndex = -1;
float masterFactor = 0.6;  // 60% for the master frame
LayoutType currentLayout = LAYOUT_MASTER_STACK; // Default layout

void HandleFrameKeys() {
    if (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) {
        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))) {
            CreateNewFrame();
        }
        else if (IsKeyPressed(KEY_Q)) {
            CloseSelectedFrame();
        }
        else if (IsKeyPressed(KEY_J)) {
            MoveFrameSelection(1);
        }
        else if (IsKeyPressed(KEY_K)) {
            MoveFrameSelection(-1);
        }
        else if (IsKeyPressed(KEY_H)) {
            masterFactor -= 0.05;
            if (masterFactor < 0.1) masterFactor = 0.1;
            ArrangeFrames();
        }
        else if (IsKeyPressed(KEY_L)) {
            masterFactor += 0.05;
            if (masterFactor > 0.9) masterFactor = 0.9;
            ArrangeFrames();
        }
        else if (IsKeyPressed(KEY_M)) {  // For switching layout mode
            currentLayout = (currentLayout + 1) % LAYOUT_COUNT;
            ArrangeFrames();
        }
        else if (IsKeyPressed(KEY_T) && selectedIndex != -1) {  // Toggle selected frame's floating/managed state
            frames[selectedIndex].isFloating = !frames[selectedIndex].isFloating;

            if (frames[selectedIndex].isFloating) {
                frames[selectedIndex].prevState = frames[selectedIndex].rect; // Store the current state before making it floating
            } else {
                frames[selectedIndex].rect = frames[selectedIndex].prevState;
            }
            ArrangeFrames();  // Re-arrange the frames immediately
        }
    }
}

Vector2 dragOffset;  // Global or member variable to store the drag offset

void HandleFrameMouseInteractions() {
    UpdateFrameFocusWithMouse();

    // If ALT is pressed
    if (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) {

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && selectedIndex != -1 && frames[selectedIndex].isFloating) {

            // On initial mouse button press, compute the drag offset
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                dragOffset.x = mousePos.x - frames[selectedIndex].rect.x;
                dragOffset.y = mousePos.y - frames[selectedIndex].rect.y;
            }

            // Use the offset to compute the new position
            Vector2 mousePos = GetMousePosition();
            frames[selectedIndex].rect.x = mousePos.x - dragOffset.x;
            frames[selectedIndex].rect.y = mousePos.y - dragOffset.y;

        } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && selectedIndex != -1 && frames[selectedIndex].isFloating) {
            Vector2 mousePos = GetMousePosition();
            frames[selectedIndex].rect.width = (mousePos.x - frames[selectedIndex].rect.x);
            frames[selectedIndex].rect.height = (mousePos.y - frames[selectedIndex].rect.y);

        } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            // Same logic for resizing the master frame as before...
            Vector2 mousePos = GetMousePosition();
            float leftBound = panel.leftWidth;
            float rightBound = SCREEN_WIDTH - panel.rightWidth;
            masterFactor = (mousePos.x - leftBound) / (rightBound - leftBound);
            if (masterFactor < 0.1f) masterFactor = 0.1f;
            if (masterFactor > 0.9f) masterFactor = 0.9f;
            ArrangeFrames();
        }
    }
}

void UpdateFrameFocusWithMouse() {
    static Vector2 lastMousePos = {0, 0};  // Store the last mouse position
    Vector2 mousePos = GetMousePosition();

    // Check if mouse has moved
    if (mousePos.x != lastMousePos.x || mousePos.y != lastMousePos.y) {
        // Iterate through all frames
        for (int i = 0; i < frameCount; i++) {
            // Check if mouse position is within frame rectangle
            if (CheckCollisionPointRec(mousePos, frames[i].rect)) {
                if (selectedIndex != i) { // Check to avoid redundant updates
                    if (selectedIndex != -1) {
                        frames[selectedIndex].isSelected = false; // Deselect the previous frame
                    }
                    selectedIndex = i;
                    frames[i].isSelected = true; // Select the current frame
                    break; // Exit loop once a frame is focused
                }
            }
        }

        lastMousePos = mousePos;  // Update the last mouse position
    }
}

void CreateNewFrame() {
    if (frameCount < MAX_FRAMES) {
        frames[frameCount].isSelected = false;
        frameCount++;
        ArrangeFrames();
        if (selectedIndex == -1) {
            selectedIndex = 0;
            frames[0].isSelected = true;
        }
    }
}

void CloseSelectedFrame() {
    if (selectedIndex != -1) {
        for (int i = selectedIndex; i < frameCount - 1; i++) {
            frames[i] = frames[i + 1];
        }
        frameCount--;
        if (frameCount == 0) {
            selectedIndex = -1;
        } else {
            selectedIndex %= frameCount;
            frames[selectedIndex].isSelected = true;
        }
        ArrangeFrames();
    }
}

void MoveFrameSelection(int direction) {
    if (selectedIndex != -1) {
        frames[selectedIndex].isSelected = false;
        selectedIndex = (selectedIndex + direction + frameCount) % frameCount;
        frames[selectedIndex].isSelected = true;
    }
}

void ArrangeFrames() {
    // Dimensions of the main workspace area
    Rectangle workspace = {
        panel.leftWidth,
        panel.topHeight,
        SCREEN_WIDTH - panel.leftWidth - panel.rightWidth,
        SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight
    };

    // If only one frame exists
    if (frameCount == 1) {
        if (!frames[0].isFloating) {
            frames[0].rect = workspace;
        }
    } else {
        switch (currentLayout) {
            case LAYOUT_MASTER_STACK:
                if (!frames[0].isFloating) {
                    frames[0].rect = (Rectangle){
                        workspace.x,
                        workspace.y,
                        workspace.width * masterFactor,
                        workspace.height
                    };
                }
                float stackedFrameHeight = (workspace.height - FRAME_GAP * (frameCount - 2)) / (frameCount - 1);
                for (int i = 1; i < frameCount; i++) {
                    if (!frames[i].isFloating) {
                        frames[i].rect = (Rectangle){
                            frames[0].rect.x + frames[0].rect.width + FRAME_GAP,
                            workspace.y + (i-1) * (stackedFrameHeight + FRAME_GAP),
                            workspace.width - frames[0].rect.width - FRAME_GAP,
                            stackedFrameHeight
                        };
                    }
                }
                break;

            case LAYOUT_COLUMNS:
                float columnWidth = (workspace.width - FRAME_GAP * (frameCount - 1)) / frameCount;
                for (int i = 0; i < frameCount; i++) {
                    if (!frames[i].isFloating) {
                        frames[i].rect = (Rectangle){
                            workspace.x + i * (columnWidth + FRAME_GAP),
                            workspace.y,
                            columnWidth,
                            workspace.height
                        };
                    }
                }
                break;

            default:
                break;
        }
    }
}

void DrawFrames() {
    for (int i = 0; i < frameCount; i++) {
        Color frameColor = frames[i].isSelected ? CURRENT_THEME.x : CURRENT_THEME.y;
        DrawRectangleRec(frames[i].rect, frameColor);

        // If this frame has the config buffer, draw its contents
        if(frames[i].buffer.isOpen && strcmp(frames[i].buffer.title, "Config Buffer") == 0) {
            DrawText("Hello World", frames[i].rect.x + 10, frames[i].rect.y + 10, 20, RAYWHITE);
        }
    }
}


// BUFFERS TODO
// buffers live in frame space
// Handle functions incapsulate their keybinds directly
// all the handle functions will be grouped in the hadlebuffers function
void HandleHelpBuffer() {
    static bool wasCtrlHPressedLastFrame = false;

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_D)) {
        if (!wasCtrlHPressedLastFrame) {
            if (frameCount < MAX_FRAMES) {
                frames[frameCount].isSelected = false;
                frames[frameCount].buffer.title = "Config Buffer";
                frames[frameCount].buffer.isOpen = true;

                frameCount++;
                ArrangeFrames();
                if (selectedIndex == -1) {
                    selectedIndex = 0;
                    frames[0].isSelected = true;
                }
            }
            wasCtrlHPressedLastFrame = true;
        }
    } else {
        wasCtrlHPressedLastFrame = false;
    }
}
