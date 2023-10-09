#include "log.h"

EditorLog editorLog;
int logPaddingX = 10;
int logPaddingY = 10;

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
    vprintf(format, args);
    printf("\n");
}

void log_clear() {
    memset(editorLog.lines, 0, sizeof(editorLog.lines));
    editorLog.count = 0;
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
