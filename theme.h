// theme.h
#ifndef THEME_H
#define THEME_H

#include "raylib.h"

typedef struct {
    Color background;
    Color modeline;
    Color modeline_highlight;
    Color minibuffer;
    Color panel_left;
    Color panel_top;
    Color panel_right;
    Color panel_bottom;
    Color panel_center;
    Color panel_flex;
    Color panel_corners;
    Color x;
    Color y;
} Theme;

extern Theme themes[];
extern int currentThemeIndex;

#define CURRENT_THEME (themes[currentThemeIndex])

Color HexToColor(const char* hexStr);
void InitializeThemes();
void NextTheme();
void PreviousTheme();

#endif
