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
    // DOOM
    themes[0] = (Theme) {
        .panel_left    = HexToColor("#090909"),
        .panel_top     = HexToColor("#090909"),
        .panel_right   = HexToColor("#090909"),
        .panel_bottom  = HexToColor("#090909"),
        .panel_center  = HexToColor("#090909"),
        .panel_flex    = HexToColor("#090909"),
        .panel_corners = HexToColor("#090909"),
        .background    = HexToColor("#0B0B0B"),
        .minibuffer    = HexToColor("#090909"),
        .modeline      = HexToColor("#080808"),
        .modeline_highlight = HexToColor("#658B5F"),
        .x = HexToColor("#564F96"),
        .y = HexToColor("#658B5F"),
    };

    // SpaceGray
    themes[1] = (Theme) {
        .panel_left    = HexToColor("#0E0E0E"),
        .panel_top     = HexToColor("#0E0E0E"),
        .panel_right   = HexToColor("#0E0E0E"),
        .panel_bottom  = HexToColor("#0E0E0E"),
        .panel_center  = HexToColor("#0E0E0E"),
        .panel_flex    = HexToColor("#0E0E0E"),
        .panel_corners = HexToColor("#0E0E0E"),
        .minibuffer    = HexToColor("#0B0B0B"),
        .modeline    = HexToColor("#080808"),
        .background    = HexToColor("#0B0B0B"),
        .modeline_highlight = HexToColor("#0079F1"),
        .x = HexToColor("#e62937"),
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


// LUA

/* #include "lua.h" */
/* #include "lualib.h" */
/* #include "lauxlib.h" */
/* #include <stdlib.h> */



/* void LoadConfig() { */
/*     const char* homeDir = getenv("HOME"); */
/*     if (homeDir == NULL) { */
/*         perror("getenv"); */
/*         return; */
/*     } */

/*     char configPath[512]; */
/*     int written = snprintf(configPath, sizeof(configPath), "%s/.config/slicer/config.lua", homeDir); */
/*     if (written >= sizeof(configPath) || written < 0) { */
/*         fprintf(stderr, "Config path is too long or an error occurred\n"); */
/*         return; */
/*     } */

/*     lua_State* L = luaL_newstate(); */
/*     if (!L) { */
/*         perror("Error creating Lua state"); */
/*         return; */
/*     } */

/*     luaL_openlibs(L); // Load Lua libraries */

/*     // Execute the config file */
/*     if (luaL_dofile(L, configPath) != LUA_OK) { */
/*         fprintf(stderr, "Error loading config file: %s\n", lua_tostring(L, -1)); */
/*         lua_close(L); */
/*         return; */
/*     } */

/*     lua_getglobal(L, "currentThemeIndex");  // Get the variable from Lua script */
/*     if (lua_isnumber(L, -1)) { */
/*         currentThemeIndex = lua_tointeger(L, -1); */
/*         // Make sure the index is in range */
/*         currentThemeIndex = currentThemeIndex % (sizeof(themes) / sizeof(Theme)); */
/*     } else { */
/*         fprintf(stderr, "currentThemeIndex is not defined or is not a number in the config file\n"); */
/*     } */

/*     lua_close(L); // Close Lua state */
/* } */




#include <pthread.h>
#include <sys/inotify.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

// The file watcher function that runs on a separate thread
void* FileWatcher(void *arg) {
    const char *homeDir = getenv("HOME");
    if (!homeDir) {
        perror("getenv failed");
        return NULL;
    }

    char configPath[512];
    snprintf(configPath, sizeof(configPath), "%s/.config/slicer/", homeDir);

    // Set up inotify to watch the config directory
    int inotifyFd = inotify_init();
    if (inotifyFd < 0) {
        perror("inotify_init failed");
        return NULL;
    }

    int watchDesc = inotify_add_watch(inotifyFd, configPath, IN_CLOSE_WRITE);
    if (watchDesc < 0) {
        perror("inotify_add_watch failed");
        close(inotifyFd);
        return NULL;
    }

    ssize_t numRead;
    char eventBuf[4096] __attribute__ ((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;

    while (1) {
        numRead = read(inotifyFd, eventBuf, sizeof(eventBuf));
        if (numRead == 0) {
            fprintf(stderr, "read() from inotify fd returned 0!");
            break;
        }

        if (numRead < 0) {
            if (errno != EINTR) {
                perror("read");
                break;
            } else {
                continue;
            }
        }

        // Process all of the events
        for (char *ptr = eventBuf; ptr < eventBuf + numRead; ptr += sizeof(struct inotify_event) + event->len) {
            event = (const struct inotify_event *) ptr;
            // If the event is for the config file, call LoadConfig
            if (event->mask & IN_CLOSE_WRITE) {
                if (strcmp(event->name, "config.lua") == 0) {
                    LoadConfig();
                }
            }
        }
    }

    // Clean up
    close(watchDesc);
    close(inotifyFd);
    return NULL;
}

// This function should be called to start the file watcher
void StartFileWatcher() {
    pthread_t watcher_thread;
    if (pthread_create(&watcher_thread, NULL, FileWatcher, NULL) != 0) {
        perror("pthread_create failed");
    }
    pthread_detach(watcher_thread); // We won't join this thread so detach it
}



void LoadConfig() {
    const char* homeDir = getenv("HOME");
    if (homeDir == NULL) {
        perror("getenv");
        return;
    }

    char configPath[512];
    int written = snprintf(configPath, sizeof(configPath), "%s/.config/slicer/config.lua", homeDir);
    if (written >= sizeof(configPath) || written < 0) {
        fprintf(stderr, "Config path is too long or an error occurred\n");
        return;
    }

    lua_State* L = luaL_newstate();
    if (!L) {
        perror("Error creating Lua state");
        return;
    }

    luaL_openlibs(L); // Load Lua libraries

    // Execute the config file
    if (luaL_dofile(L, configPath) != LUA_OK) {
        fprintf(stderr, "Error loading config file: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return;
    }

    lua_getglobal(L, "currentThemeIndex");  // Get the variable from Lua script
    if (lua_isnumber(L, -1)) {
        currentThemeIndex = lua_tointeger(L, -1);
        // Make sure the index is in range
        currentThemeIndex = currentThemeIndex % (sizeof(themes) / sizeof(Theme));
    } else {
        fprintf(stderr, "currentThemeIndex is not defined or is not a number in the config file\n");
    }

    lua_close(L); // Close Lua state
}
