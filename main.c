#include "raylib.h"
#include "screen.h"
#include "slicer.h"
#include "character.h"
#include "stdio.h"


// separate into ui.c
#include "stdlib.h"
#define CHARACTER_INSPECTOR_TEXT_SIZE 20
void DrawCharacterInspector(Character character, Texture2D sprite, int frameCounter) {
    Vector2 bigSpritePosition = { SCREEN_WIDTH - 150, SCREEN_HEIGHT - 150 };
    Rectangle bigSpriteRec = GetCharacterSourceRec(frameCounter, character.state);

    // If the character is moving left, we'll use the right sprite but flip it horizontally.
    if (character.state == WALK_LEFT || character.state == RUN_LEFT) {
        bigSpriteRec = GetCharacterSourceRec(frameCounter, WALK_RIGHT); // Use right sprite
        bigSpriteRec.width *= -1;  // Flip horizontally
    }

    DrawTexturePro(sprite, bigSpriteRec, (Rectangle){ bigSpritePosition.x, bigSpritePosition.y, abs(bigSpriteRec.width) * 4, bigSpriteRec.height * 4 }, (Vector2){ 0, 0 }, 0, WHITE);

    // Draw the character's state as text above the character inspector
    const char *stateText = CharacterStateNames[character.state];
    Vector2 textSize = MeasureTextEx(GetFontDefault(), stateText, CHARACTER_INSPECTOR_TEXT_SIZE, 1);
    Vector2 textPosition = { bigSpritePosition.x + (abs(bigSpriteRec.width) * 4 - textSize.x) / 2, bigSpritePosition.y - textSize.y - 10 };
    DrawText(stateText, textPosition.x, textPosition.y, CHARACTER_INSPECTOR_TEXT_SIZE, WHITE);
}


// LOG
// only editor log work
#include "string.h"

/* #define LOG_MAX_LINES 1000 */
/* #define LOG_FONT_SIZE 10 */
/* #define LOG_LINE_HEIGHT 20 */

/* typedef struct { */
/*     char lines[LOG_MAX_LINES][256];  // Assuming max log line length is 256 */
/*     int count; */
/*     int scrollOffset; */
/* } EditorLog; */

/* EditorLog editorLog; */
/* int logPaddingX = 10; */
/* int logPaddingY = 10; */

/* void log_add(const char *format, ...) { */
/*     va_list args; */
/*     va_start(args, format); */
/*     if (editorLog.count < LOG_MAX_LINES) { */
/*         vsnprintf(editorLog.lines[editorLog.count], 256, format, args); */
/*         editorLog.count++; */
/*     } else { */
/*         for (int i = 1; i < LOG_MAX_LINES; i++) { */
/*             strcpy(editorLog.lines[i-1], editorLog.lines[i]); */
/*         } */
/*         vsnprintf(editorLog.lines[LOG_MAX_LINES - 1], 256, format, args); */
/*     } */
/*     va_end(args); */
/*     vprintf(format, args);  // Also write log message to stdout */
/*     printf("\n");  // New line for stdout */
/* } */

/* void log_clear() { */
/*     memset(editorLog.lines, 0, sizeof(editorLog.lines)); */
/*     editorLog.count = 0; */
/* } */

/* void DrawEditorLog(int bottomHeight) { */
/*     int startY = GetScreenHeight() - bottomHeight + editorLog.scrollOffset + logPaddingY; */
/*     BeginScissorMode(0, GetScreenHeight() - bottomHeight, GetScreenWidth(), bottomHeight); */
/*     for (int i = 0; i < editorLog.count; i++) { */
/*         DrawText(editorLog.lines[i], logPaddingX, startY + i * LOG_LINE_HEIGHT, LOG_FONT_SIZE, WHITE); */
/*     } */
/*     EndScissorMode(); */
/* } */

/* void UpdateEditorLogScroll() { */
/*     if (IsKeyDown(KEY_UP)) editorLog.scrollOffset -= LOG_LINE_HEIGHT; */
/*     if (IsKeyDown(KEY_DOWN)) editorLog.scrollOffset += LOG_LINE_HEIGHT; */
/* } */





#define LOG_MAX_LINES 1000
#define LOG_FONT_SIZE 10
#define LOG_LINE_HEIGHT 20

typedef struct {
    char lines[LOG_MAX_LINES][256];  // Assuming max log line length is 256
    int count;
    int scrollOffset;
} EditorLog;

EditorLog editorLog;
int logPaddingX = 10;
int logPaddingY = 10;

void update_log_from_file() {
    FILE *file = fopen("stdout.txt", "r");
    if (file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), file) != NULL) {
            if (editorLog.count < LOG_MAX_LINES) {
                strncpy(editorLog.lines[editorLog.count], line, 256);
                editorLog.count++;
            } else {
                for (int i = 1; i < LOG_MAX_LINES; i++) {
                    strcpy(editorLog.lines[i-1], editorLog.lines[i]);
                }
                strncpy(editorLog.lines[LOG_MAX_LINES - 1], line, 256);
            }
        }
        fclose(file);
    }
}


void log_add(const char *format, ...) {
    va_list args;
    va_start(args, format);
    if (editorLog.count < LOG_MAX_LINES) {
        vsnprintf(editorLog.lines[editorLog.count], 256, format, args);
        editorLog.count++;
    } else {
        for (int i = 1; i < LOG_MAX_LINES; i++) {
            strcpy(editorLog.lines[i-1], editorLog.lines[i]);
        }
        vsnprintf(editorLog.lines[LOG_MAX_LINES - 1], 256, format, args);
    }
    va_end(args);
    vprintf(format, args);  // Also write log message to stdout
    printf("\n");  // New line for stdout
}

void DrawEditorLog(int bottomHeight) {
    int startY = GetScreenHeight() - bottomHeight + editorLog.scrollOffset + logPaddingY;
    BeginScissorMode(0, GetScreenHeight() - bottomHeight, GetScreenWidth(), bottomHeight);
    for (int i = 0; i < editorLog.count; i++) {
        DrawText(editorLog.lines[i], logPaddingX, startY + i * LOG_LINE_HEIGHT, LOG_FONT_SIZE, WHITE);
    }
    EndScissorMode();
}

void UpdateEditorLogScroll() {
    if (IsKeyDown(KEY_UP)) editorLog.scrollOffset -= LOG_LINE_HEIGHT;
    if (IsKeyDown(KEY_DOWN)) editorLog.scrollOffset += LOG_LINE_HEIGHT;
}

















// inspector or panels better
// the inspector will be only on the panel on the right
#define PANEL_CORNER_RADIUS 20.0f
#define INNER_CORNER_RADIUS 30.0f
#define INITIAL_BOTTOM_HEIGHT 100
#define INITIAL_LEFT_WIDTH 150
#define INITIAL_RIGHT_WIDTH 280
#define INITIAL_TOP_HEIGHT 40

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

Panel panel = {
    INITIAL_TOP_HEIGHT,
    INITIAL_BOTTOM_HEIGHT,
    INITIAL_LEFT_WIDTH,
    INITIAL_RIGHT_WIDTH
};


void DrawPanels() {
    DrawRectangle(0, 0, SCREEN_WIDTH, panel.topHeight, TOP_COLOR); // Top
    DrawRectangle(0, SCREEN_HEIGHT - panel.bottomHeight, SCREEN_WIDTH, panel.bottomHeight, BOTTOM_COLOR); // Bottom
    DrawRectangle(0, panel.topHeight, panel.leftWidth, SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight, LEFT_COLOR); // Left
    DrawRectangle(SCREEN_WIDTH - panel.rightWidth, panel.topHeight, panel.rightWidth, SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight, RIGHT_COLOR); // Right
}


// TODO trying to implement a preatty inverted border radius
// to make whwats behind the panels look like a rounded window
// this is a slope not inverted border radius
/* void DrawPanels() { */
/*     // Draw the main panels */
/*     DrawRectangle(0, 0, SCREEN_WIDTH, panel.topHeight, TOP_COLOR); // Top */
/*     DrawRectangle(0, SCREEN_HEIGHT - panel.bottomHeight, SCREEN_WIDTH, panel.bottomHeight, BOTTOM_COLOR); // Bottom */
/*     DrawRectangle(0, panel.topHeight, panel.leftWidth, SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight, LEFT_COLOR); // Left */
/*     DrawRectangle(SCREEN_WIDTH - panel.rightWidth, panel.topHeight, panel.rightWidth, SCREEN_HEIGHT - panel.topHeight - panel.bottomHeight, RIGHT_COLOR); // Right */

/*     // Drawing the rounded shapes on the inner angles */
/*     // Top-left corner */
/*     DrawCircleSector((Vector2){panel.leftWidth, panel.topHeight}, INNER_CORNER_RADIUS, 0, 90, 10, LEFT_COLOR); */

/*     // Top-right corner */
/*     DrawCircleSector((Vector2){SCREEN_WIDTH - panel.rightWidth, panel.topHeight}, INNER_CORNER_RADIUS, 270, 360, 10, RIGHT_COLOR); */

/*     // Bottom-left corner */
/*     DrawCircleSector((Vector2){panel.leftWidth, SCREEN_HEIGHT - panel.bottomHeight}, INNER_CORNER_RADIUS, 90, 180, 10, LEFT_COLOR); */

/*     // Bottom-right corner */
/*     DrawCircleSector((Vector2){SCREEN_WIDTH - panel.rightWidth, SCREEN_HEIGHT - panel.bottomHeight}, INNER_CORNER_RADIUS, 180, 270, 10, RIGHT_COLOR); */
/* } */


void UpdatePanelDimensions() {
    const float DRAG_SENSITIVITY = 20.0f;  // Increased sensitivity for easier resizing

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










int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sprite Slicer");
    Texture2D sprite = LoadTexture("./sprite.png");

    Character character = { { (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2 }, IDLE_DOWN, IDLE_DOWN };
    int frameCounter = 0;

    SetTargetFPS(144);

    while (!WindowShouldClose()) {
        log_add("ciao");

        UpdatePanelDimensions();
        UpdateEditorLogScroll();
        UpdateCharacterState(&character);

        if (IsKeyDown(KEY_R) && IsKeyDown(KEY_LEFT_CONTROL)) {
            panel = (Panel){
                INITIAL_TOP_HEIGHT,
                INITIAL_BOTTOM_HEIGHT,
                INITIAL_LEFT_WIDTH,
                INITIAL_RIGHT_WIDTH
            };
        }


        BeginDrawing();
        ClearBackground((Color){11, 11, 11, 255});
        DrawPanels();
        DrawEditorLog(panel.bottomHeight);
        DrawCharacter(character, sprite, frameCounter);
        DrawCharacterInspector(character, sprite, frameCounter);

        EndDrawing();

        frameCounter++;
    }

    UnloadTexture(sprite);
    CloseWindow();

    return 0;
}
