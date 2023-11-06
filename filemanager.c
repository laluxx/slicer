// TODO
// actually edit dirs, rename delete ad copy directories []
// if the folder is not big (<10) simply open the tree with the mouse
// while in the same current directory, []
// add mouse support to open and close folder  []
// introduce highlight for selection []
// and highlight for when overing with the mouse []
// we should use scissor mode to make it fit the left panel
// file preview [] */
// dont render the files off screen []

#include "filemanager.h"
#include "keychords.h"
#include "panels.h"
#include "ui.h"
#include <raylib.h>
#include "theme.h"

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#else
#include <unistd.h>
#endif

FileManager fm;
TextureCache textureCache[MAX_TEXTURES] = {0};

int fontSize = 20;
const int FILE_NAME_PADDING = 10;

const int ADDITIONAL_WIDTH = 41;

void AdjustLeftPanelWidthForFileNames() {
    int requiredWidth = INITIAL_LEFT_WIDTH + ADDITIONAL_WIDTH;  // Starting with the initial width and adding the additional hardcoded width
    for (int i = 0; i < fm.fileCount; ++i) {
        int width = MeasureText(fm.files[i], fontSize) + FILE_NAME_PADDING + ADDITIONAL_WIDTH;
        if (width > requiredWidth) {
            requiredWidth = width;
        }
    }

    panel.leftWidth = requiredWidth;  // Adjust the left panel width directly
}

void UpdateFileList() {
    // Free existing files in the list first
    for (int i = 0; i < fm.fileCount; i++) {
        free(fm.files[i]);
    }
    free(fm.files);
    fm.files = NULL;
    fm.fileCount = 0;

    DIR* dir = opendir(fm.currentDir);
    if (!dir) return;

    struct dirent* entry;

    // First pass: Add directories to the list
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Check if the entry is a directory
        if (entry->d_type == DT_DIR) {
            fm.fileCount++;
            fm.files = realloc(fm.files, fm.fileCount * sizeof(char*));
            fm.files[fm.fileCount - 1] = strdup(entry->d_name);
        }
    }

    // Reset the directory stream to the beginning
    rewinddir(dir);

    // Second pass: Add files to the list
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Check if the entry is a file
        if (entry->d_type != DT_DIR) {
            fm.fileCount++;
            fm.files = realloc(fm.files, fm.fileCount * sizeof(char*));
            fm.files[fm.fileCount - 1] = strdup(entry->d_name);
        }
    }

    closedir(dir);

    // Mark cache entries as unused when switching directories
    for (int i = 0; i < MAX_TEXTURES; i++) {
        textureCache[i].used = false;
    }

    // Adjust the left panel width based on the new file list
    AdjustLeftPanelWidthForFileNames();
}



Texture2D GetIconForExtension(const char* name) {
    char iconPath[1024];

    // If name is null or empty, return a default icon
    if (!name || strlen(name) == 0) {
        snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/default.png");
        return LoadTexture(iconPath);
    }

    // If no '.' in the name, treat as a folder
    if (!strchr(name, '.')) {
        if (strcmp(name, "src") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/folder-src.png");
        } else if (strcmp(name, "shaders") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/folder-shader.png");
        } else if (strcmp(name, ".git") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/folder-git.png");
        } else if (strcmp(name, "shaders") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/folder-shader.png");
        } else if (strcmp(name, "godot") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/folder-godot.png");
        } else if (strcmp(name, "screenshots") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/folder-images.png");
        } else if (strcmp(name, "icons") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/folder-images.png");
        } else if (strcmp(name, "Makefile") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/makefile.png");
        } else {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/folder-batch.png");  // Default folder icon
        }
        return LoadTexture(iconPath);
    }

    // Treat as a file and get its extension
    const char* ext = GetFileExtension(name);

    // Check for the '.' at the start of the extension and skip it
    if (ext && ext[0] == '.') {
        ext++;
    }

    // Match against hardcoded file extensions
    if (ext) {
        if (strcmp(ext, "png") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/image.png");
        } else if (strcmp(ext, "md") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/markdown.png");
        } else if (strcmp(ext, "conf") == 0 || strcmp(ext, "config") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/settings.png");
        } else if (strcmp(ext, "txt") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/document.png");
        } else if (strcmp(ext, "obj") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/3d.png");
        } else if (strcmp(ext, "sh") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/console.png");
        } else if (strcmp(ext, "gd") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/godot.png");
        } else if (strcmp(ext, "hs") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/haskell.png");
        } else if (strcmp(ext, "js") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/javascript.png");
        } else if (strcmp(ext, "ogg") == 0 || strcmp(ext, "mp3") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/lilypond.png");
        } else if (strcmp(ext, "py") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/python.png");
        } else if (strcmp(ext, "rs") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/rust.png");
        } else if (strcmp(ext, "glsl") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/shader.png");
        } else if (strcmp(ext, "mp4") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/video.png");
        } else if (strcmp(ext, "git") == 0) {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/folder-git.png");
        } else {
            snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/%s.png", ext);  // Search for the extension in the icons directory
        }
    } else {
        snprintf(iconPath, sizeof(iconPath), "./icons/file-manager-icons/24/default.png");  // Default icon if no valid extension found
    }

    return LoadTexture(iconPath);
}



Texture2D GetTextureFromCache(const char* name) {
    // Check if texture is already loaded
    for (int i = 0; i < MAX_TEXTURES; i++) {
        if (strcmp(textureCache[i].name, name) == 0) {
            textureCache[i].used = true;
            return textureCache[i].texture;
        }
    }

    // If not found in cache, load texture and store it
    for (int i = 0; i < MAX_TEXTURES; i++) {
        if (textureCache[i].name[0] == '\0') {
            textureCache[i].texture = GetIconForExtension(name);
            strcpy(textureCache[i].name, name);
            textureCache[i].used = true;
            return textureCache[i].texture;
        }
    }

    // If cache is full, return a default texture
    Texture2D defaultTexture = LoadTexture("./icons/file-manager-icons/24/default.png");
    return defaultTexture;
}

void ClearUnusedTextures() {
    for (int i = 0; i < MAX_TEXTURES; i++) {
        if (!textureCache[i].used && textureCache[i].name[0] != '\0') {
            UnloadTexture(textureCache[i].texture);
            textureCache[i].name[0] = '\0';
        }
    }
}

void InitializeFileManager(const char* startDir) {
    fm.files = NULL;
    fm.fileCount = 0;
    fm.selectedIndex = 0;
    getcwd(fm.currentDir, sizeof(fm.currentDir));
    UpdateFileList();
}

// BLOCK
float currentFileOffsetY = 0.0f;
float lerpRate = 0.03f;
int iconSize = 24;
int iconPadding = 8;
int iconVerticalOffset = -6;

void DrawFileManager() {
    // Variables local to this function
    int textWidth;
    int yPosition;

    // Draw the current directory outside of scissor mode.
    /* DrawText(fm.currentDir, 8, SCREEN_HEIGHT - minibuffer.height - modeline.height + 10, fontSize, WHITE); */

    int smallerFontSize = fontSize * 0.50;  // Adjust this multiplier as needed
    DrawText(fm.currentDir, 40, SCREEN_HEIGHT - minibuffer.height - modeline.height + 10, smallerFontSize, WHITE);

    // Target Y position where we want the selected file/folder to appear.
    float targetYPosition = (SCREEN_HEIGHT - minibuffer.height - modeline.height) / 2;

    // Calculate the actual Y position of the selected file/folder based on its index.
    float actualYPosition = 40 + fm.selectedIndex * 25;

    // Calculate the offset needed to make the selected file/folder appear at the target Y position.
    float desiredOffset = targetYPosition - actualYPosition;

    // Exponential moving average for smoother lerp.
    currentFileOffsetY = lerpRate * desiredOffset + (1 - lerpRate) * currentFileOffsetY;

    // Apply the scissor mode for restricting drawing.
    BeginScissorMode(0, 0, panel.leftWidth, SCREEN_HEIGHT - minibuffer.height - modeline.height);

    for (int i = 0; i < fm.fileCount; i++) {
        // Adjust the position using the current offset.
        yPosition = 40 + i * 25 + iconVerticalOffset + currentFileOffsetY;

        if (yPosition < 0 || yPosition >= SCREEN_HEIGHT - minibuffer.height - modeline.height) {
            continue;
        }

        Texture2D icon = GetTextureFromCache(fm.files[i]);
        textWidth = MeasureText(fm.files[i], fontSize);

        if (i == fm.selectedIndex) {
            // Background for the icon
            DrawRectangle(0, yPosition - 2, iconSize + iconPadding, iconSize + 4, CURRENT_THEME.panel_left);

            // Extended rectangle for the text up to the edge of the left panel
            DrawRectangle(iconSize + iconPadding, yPosition - 2, panel.leftWidth - (iconSize + 2 * iconPadding), iconSize + 4, CURRENT_THEME.x);
        }

        DrawTexture(icon, 10, yPosition, WHITE);
        DrawText(fm.files[i], 10 + iconSize + iconPadding, yPosition, fontSize, (i == fm.selectedIndex) ? WHITE : CURRENT_THEME.y);
    }

    EndScissorMode();
    ClearUnusedTextures();
}


void CenterTopFileImmediately() {
    // Calculate the Y position of the top file
    float topFileYPosition = 40; // Assuming the first file starts at Y = 40

    // Calculate the target Y position for the center of the viewable area
    float targetYPosition = (SCREEN_HEIGHT - minibuffer.height - modeline.height) / 2;

    // Set the currentFileOffsetY to center the top file without lerping
    currentFileOffsetY = targetYPosition - topFileYPosition;

    // If the offset would be negative (top file above the center), set it to 0
    if (currentFileOffsetY < 0) {
        currentFileOffsetY = 0;
    }
}



// COMMANDS
// TODO
// all commands should be usable ivoking M-x

void NavigateUp() {
    fm.selectedIndex--;
    if (fm.selectedIndex < 0) {
        fm.selectedIndex = fm.fileCount - 1;
    }
    /* CenterTopFileImmediately(); */
}

void NavigateDown() {
    fm.selectedIndex++;
    if (fm.selectedIndex >= fm.fileCount) {
        fm.selectedIndex = 0;
    }
    /* CenterTopFileImmediately(); */
}

void NavigateIn() {
    char newPath[1024];
    sprintf(newPath, "%s/%s", fm.currentDir, fm.files[fm.selectedIndex]);

    DIR* dir = opendir(newPath);
    if (dir) {
        closedir(dir);
        strcpy(fm.currentDir, newPath);
        fm.selectedIndex = 0;
        UpdateFileList();
    }
    CenterTopFileImmediately();
}

void NavigateOut() {
    // Store the current directory name before navigating out
    char* currentDirName = strrchr(fm.currentDir, '/');
    if (currentDirName) {
        currentDirName++;  // skip the '/'
    } else {
        currentDirName = fm.currentDir;  // handle case when fm.currentDir is just a name without '/'
    }

    char previousDirName[1024];
    strncpy(previousDirName, currentDirName, sizeof(previousDirName) - 1);
    previousDirName[sizeof(previousDirName) - 1] = '\0';

    // Navigate out
    char* lastSlash = strrchr(fm.currentDir, '/');
    if (lastSlash) {
        *lastSlash = '\0';
    }

    UpdateFileList();

    // Set selectedIndex to the previously entered directory
    for (int i = 0; i < fm.fileCount; i++) {
        if (strcmp(fm.files[i], previousDirName) == 0) {
            fm.selectedIndex = i;
            break;
        }
    }
}


float navigationDelay = 0.07f; // 200 milliseconds delay
float timeSinceLastNavigation = 0.0f;


void HandleFileManagerKeys() {
    timeSinceLastNavigation += GetFrameTime();

    if (!leaderKeyActive){
        // Check if neither ALT key is pressed
        if (!IsKeyDown(KEY_LEFT_ALT) && !IsKeyDown(KEY_RIGHT_ALT)) {
            if (IsKeyPressed(KEY_H)) {
              NavigateOut();
            }
            if (IsKeyPressed(KEY_L)) {
              NavigateIn();
            }

            if (timeSinceLastNavigation >= navigationDelay) {
              if (IsKeyDown(KEY_J)) {
                NavigateDown();
                timeSinceLastNavigation = 0.0f; // Reset timer after navigation
              }
              if (IsKeyDown(KEY_K)) {
                NavigateUp();
                timeSinceLastNavigation = 0.0f; // Reset timer after navigation
              }
            }
        }
    }
}
