// theme.c
#include "theme.h"
#include <string.h>
#include <stdio.h>

int currentThemeIndex = 0;
Theme themes[2];

Color HexToColor(const char* hexStr) {
    if (hexStr[0] == '#' && strlen(hexStr) == 7) {
        int r, g, b;
        sscanf(hexStr + 1, "%02x%02x%02x", &r, &g, &b);
        return (Color){ r, g, b, 255 };
    }
    return BLACK; // Default if the format is wrong
}

void InitializeThemes() {
    themes[0] = (Theme) {
        .panel_left    = HexToColor("#0E0E0E"),
        .panel_top     = HexToColor("#0E0E0E"),
        .panel_right   = HexToColor("#0E0E0E"),
        .panel_bottom  = HexToColor("#0E0E0E"),
        .panel_center  = HexToColor("#0E0E0E"),
        .panel_flex    = HexToColor("#0E0E0E"),
        .panel_corners = HexToColor("#0E0E0E"),
        .x = HexToColor("#e62937"),
        .y = HexToColor("#0079F1"),
    };

    themes[1] = (Theme) {
        .panel_left = HexToColor("#DDDDDD"),
        .panel_top = HexToColor("#DDDDDD"),
        .panel_right = HexToColor("#DDDDDD"),
        .panel_bottom = HexToColor("#DDDDDD"),
        .panel_center = HexToColor("#DDDDDD"),
        .panel_flex = HexToColor("#DDDDDD"),
        .panel_corners = HexToColor("#DDDDDD"),
        .x = HexToColor("#E62937"),
        .y = HexToColor("#0079F1"),
    };
}

void NextTheme() {
    currentThemeIndex++;
    if (currentThemeIndex >= sizeof(themes) / sizeof(Theme)) {
        currentThemeIndex = 0;
    }
}

void PreviousTheme() {
    currentThemeIndex--;
    if (currentThemeIndex < 0) {
        currentThemeIndex = sizeof(themes) / sizeof(Theme) - 1;
    }
}
