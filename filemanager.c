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
#include "panels.h"

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

void DrawFileManager() {
    DrawText(fm.currentDir, 15, 1040, 8, WHITE);

    int iconSize = 24;
    int iconPadding = 4;
    int iconVerticalOffset = -6;

    for (int i = 0; i < fm.fileCount; i++) {
        Texture2D icon = GetTextureFromCache(fm.files[i]);
        DrawTexture(icon, 10, 40 + i * 25 + iconVerticalOffset, WHITE);
        Color color = (i == fm.selectedIndex) ? YELLOW : WHITE;
        /* DrawText(fm.files[i], 10 + iconSize + iconPadding, 40 + i * 25, 20, color); */
        DrawText(fm.files[i], 10 + iconSize + iconPadding, 40 + i * 25, fontSize, color);

    }

    ClearUnusedTextures();
}

void NavigateUp() {
    fm.selectedIndex--;
    if (fm.selectedIndex < 0) {
        fm.selectedIndex = fm.fileCount - 1;
    }
}

void NavigateDown() {
    fm.selectedIndex++;
    if (fm.selectedIndex >= fm.fileCount) {
        fm.selectedIndex = 0;
    }
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

/* void UpdateFileManager() { */
/*     if (IsKeyPressed(KEY_H)) { */
/*         NavigateOut(); */
/*     } */
/*     if (IsKeyPressed(KEY_J)) { */
/*         NavigateDown(); */
/*     } */
/*     if (IsKeyPressed(KEY_K)) { */
/*         NavigateUp(); */
/*     } */
/*     if (IsKeyPressed(KEY_L)) { */
/*         NavigateIn(); */
/*     } */
/* } */


void UpdateFileManager() {
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
        if (IsKeyPressed(KEY_H)) {
            NavigateOut();
        }
        if (IsKeyPressed(KEY_J)) {
            NavigateDown();
        }
        if (IsKeyPressed(KEY_K)) {
            NavigateUp();
        }
        if (IsKeyPressed(KEY_L)) {
            NavigateIn();
        }
    }
}
