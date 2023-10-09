#ifndef LOG_H
#define LOG_H

#include "raylib.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "screen.h"

#define LOG_MAX_LINES 1000
#define LOG_FONT_SIZE 10
#define LOG_LINE_HEIGHT 20

typedef struct {
    char lines[LOG_MAX_LINES][256];
    int count;
    int scrollOffset;
} EditorLog;

void log_add(const char *format, ...);
void log_clear();
void DrawEditorLog(int bottomHeight);
void UpdateEditorLogScroll();

#endif // LOG_H
