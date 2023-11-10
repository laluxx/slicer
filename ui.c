#include <raylib.h>
#include "keychords.h"
#include "screen.h"
#include "theme.h"
#include <stdio.h>
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

void DrawFPSWidget() {
    // Constants for easy customization:
    const int fpsWidth = 50;
    const int fontSize = 8;
    const Color textColor = themes[currentThemeIndex].modeline;

    // FPS dependent colors
    const Color goodFPSColor = GREEN;
    const Color mediumFPSColor = YELLOW;
    const Color badFPSColor = RED;

    // Calculated values:
    const int currentFPS = GetFPS();
    const int fpsPaddingX = SCREEN_WIDTH - fpsWidth;
    const int paddingY = SCREEN_HEIGHT - minibuffer.height - modeline.height;
    char fpsText[10];
    sprintf(fpsText, "%d FPS", currentFPS);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), fpsText, fontSize, 1);
    int textX = fpsPaddingX + (fpsWidth - textSize.x) / 2;
    int textY = paddingY + (modeline.height - textSize.y) / 2;

    // Determine background color based on FPS:
    Color backgroundColor;
    if (currentFPS >= 60) {
        backgroundColor = goodFPSColor;
    } else if (currentFPS >= 30) {
        backgroundColor = ColorLerp(mediumFPSColor, goodFPSColor, (float)(currentFPS - 30) / 30);
    } else {
        backgroundColor = ColorLerp(badFPSColor, mediumFPSColor, (float)currentFPS / 30);
    }
    backgroundColor = ColorLerp(backgroundColor, themes[currentThemeIndex].x, 0.5);

    // Check if widget is clicked:
    Vector2 mousePos = GetMousePosition();
    static int currentFPSTarget = 144;  // Static variable to store the current target FPS
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        mousePos.x >= fpsPaddingX && mousePos.x <= fpsPaddingX + fpsWidth &&
        mousePos.y >= paddingY && mousePos.y <= paddingY + modeline.height) {

        // Toggle FPS target based on the current target
        switch(currentFPSTarget) {
            case 144: currentFPSTarget = 15; break;
            case 15: currentFPSTarget = 30; break;
            case 30: currentFPSTarget = 60; break;
            case 60: currentFPSTarget = 144; break;
            default: currentFPSTarget = 144; break;  // Default case if for some reason the FPS isn't in the list
        }
        SetTargetFPS(currentFPSTarget);  // Update the FPS target in your engine
    }

    // Drawing the FPS widget:
    DrawRectangle(fpsPaddingX, paddingY, fpsWidth, modeline.height, backgroundColor);
    DrawText(fpsText, textX, textY, fontSize, textColor);
}


Color ColorLerp(Color a, Color b, float f) {
    return (Color) {
        a.r + f * (b.r - a.r),
        a.g + f * (b.g - a.g),
        a.b + f * (b.b - a.b),
        255
    };
}


// ALT HOLDING INDICATOR
AltIndicator altIndicator = {
    .targetWidth = 0.0f,
    .currentWidth = 0.0f,
    .wasActive = false
};

#define INDICATOR_GROWTH_RATE 10.0f        // Speed of indicator stretching
#define INDICATOR_X_OFFSET 16.0f               // Distance from the left of the screen
#define INDICATOR_Y_OFFSET 0.0f                // Distance from the vertical center of the modeline (can be adjusted)
#define INDICATOR_INITIAL_SIZE 10.0f           // Initial size when ALT is first pressed
#define INDICATOR_MAX_SIZE 14.0f               // Max size when ALT is held down
#define INDICATOR_CENTER_OFFSET_FACTOR 0.5f    // Factor to adjust the vertical centering in the modeline


void DrawAltIndicator() {
    UpdateAltIndicator();
    // Determine the center of the square relative to the width
    float halfCurrentSize = altIndicator.currentWidth * 0.5f;
    float startX = INDICATOR_X_OFFSET;  // Start X position of the indicator

    // Calculate the startY position so that the center of the indicator is aligned with the center of the modeline
    float modelineCenterY = SCREEN_HEIGHT - minibuffer.height - (modeline.height * 0.5f);
    float startY = modelineCenterY - halfCurrentSize;

    Color startColor = CURRENT_THEME.y;  // Color when the indicator is small
    Color endColor = CURRENT_THEME.x;    // Color when the indicator is at its max size

    float lerpAmount = (altIndicator.currentWidth - INDICATOR_INITIAL_SIZE) / (INDICATOR_MAX_SIZE - INDICATOR_INITIAL_SIZE);
    Color currentColor = ColorLerp(startColor, endColor, lerpAmount);

    // Draw the square expanding symmetrically from the center of the modeline
    DrawRectangle(startX, startY, altIndicator.currentWidth, altIndicator.currentWidth, currentColor);
}

void UpdateAltIndicator() {
    bool isActive = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);

    if (isActive) {
        altIndicator.targetWidth = INDICATOR_MAX_SIZE;
    } else {
        altIndicator.targetWidth = INDICATOR_INITIAL_SIZE;
    }

    // Lerp size for smooth appearance and disappearance
    altIndicator.currentWidth += (altIndicator.targetWidth - altIndicator.currentWidth) * INDICATOR_GROWTH_RATE * GetFrameTime();

    altIndicator.wasActive = isActive;
}






// MODELINE && MINIBUFFER
#define MAX_SCREENKEY_LENGTH 128

// Initialize global structs
Modeline modeline = {
    .height = 26.0f,
    .isDragging = false,
    .initialClickOffset = 0.0f
};

Minibuffer minibuffer = {
    .height = 21.0f,
    .content = "Happy Coding :3",
    .timer = 0.0f
};

// Convert the pressed key to a string
const char* KeyToString(int key) {
    switch(key) {
        case KEY_SPACE: return "spc";
        // Add other special key mappings as needed.
        default: return TextFormat("%c", key);
    }
}

void UpdateMinibufferScreenKey() {
    int key = GetKeyPressed();
    if (key > 0) { // Some key was pressed
        if(strlen(minibuffer.content) + strlen(KeyToString(key)) < MAX_SCREENKEY_LENGTH - 1) {
            strcat(minibuffer.content, KeyToString(key));
            strcat(minibuffer.content, " ");  // space between keys
        }
        minibuffer.timer = 0.0f;
    }

    minibuffer.timer += GetFrameTime();

    if (minibuffer.timer > 2.0f) { // Clear after 2 seconds of inactivity
        minibuffer.content[0] = '\0';
        minibuffer.timer = 0.0f;
    }
}

void UpdateModelinePosition() {
    Vector2 mousePos = GetMousePosition();

    // Check if the left mouse button is pressed while hovering over the modeline
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        mousePos.y > (SCREEN_HEIGHT - minibuffer.height - modeline.height) &&
        mousePos.y < (SCREEN_HEIGHT - minibuffer.height)) {
        modeline.isDragging = true;
        modeline.initialClickOffset = SCREEN_HEIGHT - mousePos.y - minibuffer.height;
    }

    // Adjust the modeline and minibuffer position if dragging
    if (modeline.isDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        minibuffer.height = SCREEN_HEIGHT - mousePos.y - modeline.initialClickOffset;
    } else if (modeline.isDragging && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        modeline.isDragging = false;
    }
}

void DrawModeline() {
    Color modelineColor = CURRENT_THEME.modeline;
    DrawRectangle(0, SCREEN_HEIGHT - minibuffer.height - modeline.height, SCREEN_WIDTH, modeline.height, modelineColor);

    // Drawing the vertical bar on the left side of the modeline
    int highlightWidth = 3;
    DrawRectangle(0, SCREEN_HEIGHT - minibuffer.height - modeline.height, highlightWidth, modeline.height, CURRENT_THEME.modeline_highlight);
}

void DrawMiniBuffer() {
    Color minibufferColor = CURRENT_THEME.minibuffer;
    DrawRectangle(0, SCREEN_HEIGHT - minibuffer.height, SCREEN_WIDTH, minibuffer.height, minibufferColor);

    // Draw the screenkey content
    /* DrawText(minibuffer.content, 6, SCREEN_HEIGHT - minibuffer.height + 6, 9, RAYWHITE); */
    DrawText(minibuffer.content, 6, SCREEN_HEIGHT - 14, 9, RAYWHITE);
}
