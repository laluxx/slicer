#include "ui.h"
#include <raylib.h>
#include "screen.h"
#include "theme.h"

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
    /* DrawRectangleRec(nameRect, typeColor); */

    // Draw type and name texts
    DrawText(type, x, y, 10, WHITE);
    DrawText(name, x + (buttonWidth * 1.5 + textPadding + 26 - nameWidth) / 2, y + buttonHeight + textPadding, 10, WHITE);

    // Draw connecting pipe
    int pipeX = x + typeWidth / 2;
    int pipeYStart = y + nameHeight + 4;
    int pipeYEnd = nameRect.y;  // Adjusted to connect precisely with the nameRect
    int pipeWidth = 2;
    /* DrawRectangle(pipeX - pipeWidth / 2, pipeYStart, pipeWidth, pipeYEnd - pipeYStart, typeColor); */

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


/* /\* TODO smoother edges *\/ */
/* bool DrawIosToggleButton(Vector2 position, bool currentState) { */
/*     const float baseWidth = 60.0f;  // Adjusted width for more roundness */
/*     const float baseHeight = 30.0f; */
/*     const float sliderRadius = baseHeight / 2.0f; */
/*     static float sliderAnimValue = 15.0f;  // Initial value representing the initial position of the circle */

/*     Rectangle baseRect = { position.x, position.y, baseWidth, baseHeight }; */
/*     Vector2 sliderTarget = { currentState ? (position.x + baseWidth - sliderRadius) : position.x + sliderRadius }; */
/*     Vector2 sliderPosition = { position.x + sliderAnimValue, position.y + baseHeight / 2 }; */

/*     if (currentState) { */
/*         sliderAnimValue += (sliderTarget.x - sliderPosition.x) * 0.1f;  // Smooth animation */
/*     } else { */
/*         sliderAnimValue -= (sliderPosition.x - sliderTarget.x) * 0.1f;  // Smooth animation */
/*     } */

/*     Color baseColor = currentState ? GREEN : GRAY; */

/*     if (CheckCollisionPointRec(GetMousePosition(), baseRect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) { */
/*         currentState = !currentState; */
/*     } */

/*     DrawRectangleRounded(baseRect, 0.8, 20, baseColor); */
/*     DrawCircleV(sliderPosition, sliderRadius - 2, WHITE); */

/*     return currentState; */
/* } */



Texture2D toggleOnTexture;
Texture2D toggleOffTexture;
Texture2D toggleCircleTexture;

void LoadToggleTextures() {
    toggleOnTexture = LoadTexture("./icons/ui/png/toggleon.png");
    toggleOffTexture = LoadTexture("./icons/ui/png/toggleoff.png");
    toggleCircleTexture = LoadTexture("./icons/ui/png/togglecircle.png");
}

void UnloadToggleTextures() {
    UnloadTexture(toggleOnTexture);
    UnloadTexture(toggleOffTexture);
    UnloadTexture(toggleCircleTexture);
}


bool DrawIosToggleButton(Vector2 position, bool currentState) {
    const float baseWidth = 60.0f;
    const float baseHeight = 30.0f;
    const float sliderRadius = toggleCircleTexture.width / 2.0f;  // Assuming the texture is a square
    const float sliderPadding = 5.0f;

    static float sliderAnimValue = 0.0f;
    static bool initialized = false;
    if (!initialized) {
        sliderAnimValue = sliderRadius + sliderPadding;
        initialized = true;
    }

    Rectangle baseRect = { position.x, position.y, baseWidth, baseHeight };
    Vector2 sliderTarget = { currentState ? (position.x + baseWidth - sliderRadius - sliderPadding) : (position.x + sliderRadius + sliderPadding) };
    Vector2 sliderPosition = { position.x + sliderAnimValue, position.y + baseHeight / 2 };

    if (currentState) {
        sliderAnimValue += (sliderTarget.x - sliderPosition.x) * 0.1f;  // Smooth animation
    } else {
        sliderAnimValue -= (sliderPosition.x - sliderTarget.x) * 0.1f;  // Smooth animation
    }

    if (CheckCollisionPointRec(GetMousePosition(), baseRect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        currentState = !currentState;
    }

    if (currentState) {
        DrawTexture(toggleOnTexture, position.x, position.y, WHITE);
    } else {
        DrawTexture(toggleOffTexture, position.x, position.y, WHITE);
    }

    DrawTexture(toggleCircleTexture, sliderPosition.x - sliderRadius, sliderPosition.y - (toggleCircleTexture.height / 2.0f), WHITE);

    return currentState;
}




// COLOR PICKER

void DrawColorPicker(ColorPicker *colorPicker, int x, int y, float scale) {
    Vector2 gradientSelectorPos = { x, y };
    Vector2 spectrumSelectorPos = { x + scale * colorPicker->spectrumBox.x, y + scale * 50 };

    Rectangle gradientBoxAdjusted = { x + scale * colorPicker->gradientBox.x, y + scale * colorPicker->gradientBox.y, scale * colorPicker->gradientBox.width, scale * colorPicker->gradientBox.height };
    Rectangle spectrumBoxAdjusted = { x + scale * colorPicker->spectrumBox.x, y + scale * colorPicker->spectrumBox.y, scale * colorPicker->spectrumBox.width, scale * colorPicker->spectrumBox.height };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), gradientBoxAdjusted)) {
            gradientSelectorPos = GetMousePosition();
            colorPicker->selectedColor = GetColorFromGradient(gradientBoxAdjusted, gradientSelectorPos, colorPicker->selectedHue);
        } else if (CheckCollisionPointRec(GetMousePosition(), spectrumBoxAdjusted)) {
            spectrumSelectorPos = GetMousePosition();
            colorPicker->selectedColor = GetColorFromSpectrum(spectrumBoxAdjusted, spectrumSelectorPos);
            colorPicker->selectedHue = colorPicker->selectedColor.r;  // Assuming hue is stored in red component
        }
    }

    Color topColor = ColorFromHSV(colorPicker->selectedHue, 1.0f, 1.0f);
    DrawRectangleGradientV(gradientBoxAdjusted.x, gradientBoxAdjusted.y, gradientBoxAdjusted.width, gradientBoxAdjusted.height, WHITE, topColor);
    DrawRectangleGradientH(gradientBoxAdjusted.x, gradientBoxAdjusted.y, gradientBoxAdjusted.width, gradientBoxAdjusted.height, BLACK, BLANK);

    // Drawing the hue gradient manually
    for (int i = 0; i < spectrumBoxAdjusted.height; i++) {
        float normalizedY = (float)i / spectrumBoxAdjusted.height;
        int hue = normalizedY * 360;
        Color color = ColorFromHSV(hue, 1.0f, 1.0f);
        DrawRectangle(spectrumBoxAdjusted.x, spectrumBoxAdjusted.y + i, spectrumBoxAdjusted.width, scale, color);
    }

    DrawRectangleRec((Rectangle){ x + scale * colorPicker->selectedColorBox.x, y + scale * colorPicker->selectedColorBox.y, scale * colorPicker->selectedColorBox.width, scale * colorPicker->selectedColorBox.height }, colorPicker->selectedColor);

    DrawText(TextFormat("HEX: #%02X%02X%02X", colorPicker->selectedColor.r, colorPicker->selectedColor.g, colorPicker->selectedColor.b), x + scale * colorPicker->selectedColorBox.x, y + scale * colorPicker->selectedColorBox.y + scale * colorPicker->selectedColorBox.height + 10, 20 * scale, WHITE);
    DrawText(TextFormat("R: %d\nG: %d\nB: %d", colorPicker->selectedColor.r, colorPicker->selectedColor.g, colorPicker->selectedColor.b), x + scale * colorPicker->selectedColorBox.x, y + scale * colorPicker->selectedColorBox.y + scale * colorPicker->selectedColorBox.height + 40, 20 * scale, WHITE);
}



Color GetColorFromGradient(Rectangle gradientBox, Vector2 position, float hue) {
    float gradientWidth = gradientBox.width;
    float gradientHeight = gradientBox.height;

    float normalizedX = (position.x - gradientBox.x) / gradientWidth;
    float normalizedY = (position.y - gradientBox.y) / gradientHeight;

    // Use the hue to get the selected color
    Color color = ColorFromHSV(hue, normalizedX, 1.0f - normalizedY);

    return color;
}

Color GetColorFromSpectrum(Rectangle spectrumBox, Vector2 position) {
    float spectrumHeight = spectrumBox.height;
    float normalizedY = (position.y - spectrumBox.y) / spectrumHeight;

    int hue = normalizedY * 360;

    Color color = ColorFromHSV(hue, 1.0f, 1.0f);

    return color;
}


// MODELINE

float modelineHeight = 30.0f;

void DrawModeline(int width, float minibufferHeight) {
    Color modelineColor = CURRENT_THEME.modeline;  // Example color; adjust as necessary
    DrawRectangle(0, SCREEN_HEIGHT - minibufferHeight - modelineHeight, width, modelineHeight, modelineColor);
}


// MINIBUFFER
void DrawMiniBuffer(int width, float height) {
    Color minibufferColor = CURRENT_THEME.minibuffer  ; // Or any other color you'd like
    DrawRectangle(0, SCREEN_HEIGHT - height, width, height, minibufferColor);
}
