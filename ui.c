#include "ui.h"

// BUTTONS
int BUTTON_FONT_SIZE = 20;
int buttonWidth = 220;
int buttonHeight = 30;
int buttonSpacing = 10;
int buttonYStart;  // Declaration without initialization

bool DrawButton(const char *text, Rectangle btnBounds, Color baseColor) {
    bool clicked = false;

    // Define shadow colors based on the base color
    Color shadowDark = (Color){ baseColor.r/1.5, baseColor.g/1.5, baseColor.b/1.5, baseColor.a };
    Color shadowLight = (Color){ baseColor.r*1.5 > 255 ? 255 : baseColor.r*1.5, baseColor.g*1.5 > 255 ? 255 : baseColor.g*1.5, baseColor.b*1.5 > 255 ? 255 : baseColor.b*1.5, baseColor.a };

    if (CheckCollisionPointRec(GetMousePosition(), btnBounds)) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            // Draw shadows for pressed state
            DrawRectangle(btnBounds.x, btnBounds.y + 3, btnBounds.width, btnBounds.height, shadowDark);
            DrawRectangleRec(btnBounds, baseColor);
        } else {
            // Draw shadows for hover state
            DrawRectangle(btnBounds.x, btnBounds.y + 1, btnBounds.width, btnBounds.height, shadowLight);
            DrawRectangleRec(btnBounds, baseColor);

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                clicked = true;
            }
        }
    } else {
        // Draw shadows for default state
        DrawRectangle(btnBounds.x, btnBounds.y + 2, btnBounds.width, btnBounds.height, shadowDark);
        DrawRectangleRec(btnBounds, baseColor);
    }

    DrawText(text, btnBounds.x + (btnBounds.width - MeasureText(text, BUTTON_FONT_SIZE)) / 2,
             btnBounds.y + btnBounds.height / 2 - BUTTON_FONT_SIZE / 2, BUTTON_FONT_SIZE, BLACK);

    return clicked;
}


/* void DrawIncrementerButton(int *value, const char *name, const char *type, int x, int y) { */
/*     const int buttonHeight = 20; */
/*     const int buttonWidth = 30; */
/*     const int textPadding = 5; */
/*     const int roundness = 5;  // Controls the roundness of the corners */

/*     // Determine color based on type */
/*     Color typeColor = RED;  // Default to red for int */
/*     if (strcmp(type, "float") == 0) typeColor = BLUE; */
/*     else if (strcmp(type, "char") == 0) typeColor = GREEN; */

/*     // Draw type, name, and value texts */
/*     DrawText(type, x, y, 10, BLACK); */
/*     DrawText(name, x, y + buttonHeight + textPadding, 10, BLACK); */

/*     // Create rectangles for the buttons and value display */
/*     Rectangle minusRect = { x + MeasureText(type, 10) + textPadding, y, buttonWidth, buttonHeight }; */
/*     Rectangle plusRect = { minusRect.x + buttonWidth + textPadding, y, buttonWidth, buttonHeight }; */
/*     Rectangle valueRect = { minusRect.x + buttonWidth, y, textPadding, buttonHeight }; */
/*     Rectangle fill = { minusRect.x + (buttonWidth * 3 / 4), y, plusRect.x - minusRect.x - (buttonWidth / 2), buttonHeight }; */



/*     // Check for mouse interactions for incrementing and decrementing */
/*     Vector2 mousePos = GetMousePosition(); */
/*     if (CheckCollisionPointRec(mousePos, minusRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (*value > 1)) (*value)--; */
/*     if (CheckCollisionPointRec(mousePos, plusRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) (*value)++; */

/*     // Draw the buttons and value display */
/*     DrawRectangleRounded(minusRect, roundness, 16, (Color){50, 50, 50, 255}); */
/*     DrawRectangleRounded(plusRect, roundness, 16, (Color){50, 50, 50, 255}); */
/*     DrawRectangle(fill.x, fill.y, fill.width, fill.height, WHITE); */
/*     DrawRectangle(valueRect.x, valueRect.y, valueRect.width, valueRect.height, WHITE); */
/*     DrawText(TextFormat("%d", *value), valueRect.x + valueRect.width / 2 - MeasureText(TextFormat("%d", *value), 10) / 2, y + buttonHeight / 2 - 10 / 2, 10, BLACK); */
/*     DrawText("-", minusRect.x + buttonWidth / 2 - MeasureText("-", 10) / 2, y + buttonHeight / 2 - 10 / 2, 10, WHITE); */
/*     DrawText("+", plusRect.x + buttonWidth / 2 - MeasureText("+", 10) / 2, y + buttonHeight / 2 - 10 / 2, 10, WHITE); */
/* } */



void DrawIncrementerButton2(int *value, const char *name, const char *type, int x, int y) {
    int buttonWidth = 20;
    int buttonHeight = 20;
    int valueRectWidth = 30;
    int typeRectWidth = 25;  // Set width for type rectangle

    // Determine color based on type
    Color typeColor = RED;  // Default to red for int
    if (strcmp(type, "float") == 0) typeColor = BLUE;
    else if (strcmp(type, "char") == 0) typeColor = GREEN;

    // Draw type rectangle with type text
    DrawRectangle(x, y, typeRectWidth, buttonHeight - 4, typeColor);  // Adjusted position to align with name text
    DrawText(type, x + 5, y + 5, 10, BLACK);  // Moved text slightly to the right for centering

    // Draw name text below type rectangle
    DrawText(name, x, y + buttonWidth + 5, 10, BLACK);  // Adjusted font size to match other text

    // Draw buttons and value rectangle
    int buttonsXOffset = x + typeRectWidth + 5;  // Start buttons to the right of type rectangle
    if (DrawButton("-", (Rectangle){ buttonsXOffset, y, buttonWidth, buttonHeight }, RED) && (*value > 1)) (*value)--;
    DrawRectangle(buttonsXOffset + buttonWidth, y, valueRectWidth, buttonHeight, WHITE);
    DrawText(TextFormat("%d", *value), buttonsXOffset + buttonWidth + 5, y + 5, 10, BLACK);
    if (DrawButton("+", (Rectangle){ buttonsXOffset + buttonWidth + valueRectWidth, y, buttonWidth, buttonHeight }, GREEN)) (*value)++;
}




void DrawIncrementerButton(int *value, const char *name, const char *type, int x, int y) {
    const int buttonHeight = 20, buttonWidth = 30, textPadding = 6, roundness = 5, nameHeight = 10;
    const float holdThreshold = 0.5f;
    int nameWidth = MeasureText(name, 10);
    int typeWidth = MeasureText(type, 10);
    Color typeColor = (strcmp(type, "float") == 0) ? BLUE : RED;  // Assume RED for other types

    // Draw rectangles below texts
    Rectangle typeRect = {
        x - 2,
        y + nameHeight,
        typeWidth + 4,
        4
    };
    int nameRectPadding = 2;  // Small padding for the rectangle under the name
    Rectangle nameRect = {
        x + (buttonWidth * 1.5 + textPadding + 26 - nameWidth) / 2 - nameRectPadding,
        y + buttonHeight + textPadding + nameHeight - nameRectPadding * 2 -0,  // Adjusted to cover text height
        nameWidth + 2 * nameRectPadding,
        nameHeight + 2 /* * nameRectPadding */  // Adjusted to cover text height
    };
    DrawRectangleRec(typeRect, typeColor);
    DrawRectangleRec(nameRect, typeColor);

    // Draw type and name texts
    DrawText(type, x, y, 10, WHITE);
    DrawText(name, x + (buttonWidth * 1.5 + textPadding + 26 - nameWidth) / 2, y + buttonHeight + textPadding, 10, WHITE);

    // Draw connecting pipe
    int pipeX = x + typeWidth / 2;
    int pipeYStart = y + nameHeight + 4;
    int pipeYEnd = nameRect.y;  // Adjusted to connect precisely with the nameRect
    int pipeWidth = 2;
    DrawRectangle(pipeX - pipeWidth / 2, pipeYStart, pipeWidth, pipeYEnd - pipeYStart, typeColor);

    // Button rectangles
    Rectangle minusRect = { x + typeWidth + textPadding, y, buttonWidth, buttonHeight };
    Rectangle plusRect = { minusRect.x + buttonWidth + textPadding, y, buttonWidth, buttonHeight };
    Rectangle valueRect = { minusRect.x + buttonWidth, y, textPadding, buttonHeight };
    Rectangle fill = { minusRect.x + (buttonWidth * 3 / 4), y, plusRect.x - minusRect.x - (buttonWidth / 2), buttonHeight };

    // Interaction
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, minusRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (*value > 1)) {
        (*value)--;
    }
    if (CheckCollisionPointRec(mousePos, plusRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        (*value)++;
    }

    // Draw buttons and value display
    DrawRectangleRounded(minusRect, roundness, 16, (Color){50, 50, 50, 255});
    DrawRectangleRounded(plusRect, roundness, 16, (Color){50, 50, 50, 255});
    DrawRectangle(fill.x, fill.y, fill.width, fill.height, WHITE);
    DrawRectangle(valueRect.x, valueRect.y, valueRect.width, valueRect.height, WHITE);
    DrawText(TextFormat("%d", *value), valueRect.x + valueRect.width / 2 - MeasureText(TextFormat("%d", *value), 10) / 2, y + buttonHeight / 2 - 10 / 2, 10, BLACK);

    // Draw text for + and - buttons
    Color minusTextColor = CheckCollisionPointRec(mousePos, minusRect) && IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? RED : WHITE;
    Color plusTextColor = CheckCollisionPointRec(mousePos, plusRect) && IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? GREEN : WHITE;
    DrawText("-", minusRect.x + buttonWidth / 2 - MeasureText("-", 10) / 2, y + buttonHeight / 2 - 10 / 2, 10, minusTextColor);
    DrawText("+", plusRect.x + buttonWidth / 2 - MeasureText("+", 10) / 2, y + buttonHeight / 2 - 10 / 2, 10, plusTextColor);
}




// TODO SLIDER or POTENZIOMETRO
/* typedef struct Slider { */
/*     Vector2 position; */
/*     int width; */
/*     int height; */
/*     float value;  // value between 0 and 1 */
/* } Slider; */

/* bool CheckCollisionPointRec(Vector2 point, Rectangle rec);  // Assuming this function is provided by Raylib */

/* float DrawSlider(Slider *slider) { */
/*     // Calculate positions and sizes */
/*     Rectangle bar = { slider->position.x, slider->position.y + slider->height/2 - 2, slider->width, 4 };  // Adjust these numbers to get the appearance you want */
/*     Rectangle handle = { slider->position.x + slider->value * slider->width - slider->height/2, slider->position.y, slider->height, slider->height };  // Makes a square handle */

/*     // Check for mouse interaction */
/*     if (CheckCollisionPointRec(GetMousePosition(), bar) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) { */
/*         float newValue = (GetMouseX() - slider->position.x) / slider->width; */
/*         slider->value = Clamp(newValue, 0.0f, 1.0f);  // Clamp ensures the value is between 0 and 1 */
/*     } */

/*     // Draw the slider */
/*     DrawRectangleRec(bar, DARKGRAY);  // Draw the bar */
/*     DrawRectangleRec(handle, (CheckCollisionPointRec(GetMousePosition(), handle) ? LIGHTGRAY : GRAY));  // Lighter color if mouse is over the handle */

/*     return slider->value;  // Return the current value of the slider */
/* } */

bool DrawToggleButton(Vector2 position, bool currentState, const char *text) {
    const int buttonHeight = 20;
    const int squareSize = 10;
    Rectangle buttonRect = { position.x + squareSize + 5, position.y, 100, buttonHeight }; // Adjusted width to 150 to accommodate text
    Rectangle squareRect = { position.x, position.y + 5, squareSize, squareSize };

    bool isMouseOver = CheckCollisionPointRec(GetMousePosition(), (Rectangle){position.x, position.y, buttonRect.width + squareSize + 5, buttonHeight});
    Color buttonColor = (currentState) ? (Color){50, 50, 50, 255} : (Color){30, 30, 30, 255};  // Darker colors for button rectangle
    Color squareColor = (currentState) ? GREEN : RED;

    if (isMouseOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentState = !currentState;
    }

    DrawRectangleRec(buttonRect, buttonColor);
    DrawRectangleRec(squareRect, squareColor);
    DrawText(text, buttonRect.x + 5, buttonRect.y + 5, 10, WHITE);  // Adjusted y position to center text vertically

    return currentState;
}


// FAKE GLOW TOGGLE
bool DrawToggleButton2(Vector2 position, bool currentState, const char *text) {
    const int buttonHeight = 20;
    const int squareSize = 10;
    const int glowOffset = 5;  // Adjust this value to control the glow size
    Rectangle buttonRect = { position.x + squareSize + 5, position.y, 100, buttonHeight };
    Rectangle squareRect = { position.x, position.y + 5, squareSize, squareSize };
    Rectangle glowRect = { position.x - glowOffset, position.y + 5 - glowOffset,
                           squareSize + 2 * glowOffset, squareSize + 2 * glowOffset };  // Glow rectangle

    bool isMouseOver = CheckCollisionPointRec(GetMousePosition(),
                  (Rectangle){position.x, position.y, buttonRect.width + squareSize + 5, buttonHeight});
    Color buttonColor = (currentState) ? (Color){50, 50, 50, 255} : (Color){30, 30, 30, 255};
    Color squareColor = (currentState) ? GREEN : RED;
    Color glowColor = (Color){0, 255, 0, 80};  // Semi-transparent green

    if (isMouseOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentState = !currentState;
    }

    if (currentState) {
        DrawRectangleRec(glowRect, glowColor);  // Draw glow if button is on
    }
    DrawRectangleRec(buttonRect, buttonColor);
    DrawRectangleRec(squareRect, squareColor);
    DrawText(text, buttonRect.x + 5, buttonRect.y + 5, 10, WHITE);

    return currentState;
}


