#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <raylib.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TEXTURES 1000

// File Manager Structure
typedef struct {
    char** files;
    int fileCount;
    int selectedIndex;
    char currentDir[1024];
    char displayedDir[1024];
} FileManager;

typedef struct {
    char name[128];  // file or extension name
    Texture2D texture;
    bool used;       // flag to indicate if this cache entry was used in the current directory
} TextureCache;

extern FileManager fm;
extern TextureCache textureCache[MAX_TEXTURES];
extern bool fileManagerVisible;

// Function prototypes
void UpdateFileList();
Texture2D GetIconForExtension(const char* name);
void InitializeFileManager(const char* startDir);
void DrawFileManager();
void NavigateUp();
void NavigateDown();
void NavigateIn();
void NavigateOut();
void HandleFileManagerKeys();
void CenterTopFileImmediately();

#endif // FILEMANAGER_H



// BASE
/* void DrawFileManager() { */
/*     // This is drawn outside the scissor mode, so it's unaffected. */
/*     DrawText(fm.currentDir, 15, SCREEN_HEIGHT - minibuffer.height - modeline.height + 10, 8, WHITE); */

/*     // Start the scissor mode for restricting drawing. */
/*     BeginScissorMode(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - minibuffer.height - modeline.height); */

/*     int iconSize = 24; */
/*     int iconPadding = 4; */
/*     int iconVerticalOffset = -6; */

/*     for (int i = 0; i < fm.fileCount; i++) { */
/*         // Calculate position before drawing to determine if it's inside the scissor region. */
/*         int yPosition = 40 + i * 25 + iconVerticalOffset; */

/*         // If the icon or text would be rendered below the scissor region, skip this iteration. */
/*         if (yPosition >= SCREEN_HEIGHT - minibuffer.height - modeline.height) { */
/*             break; */
/*         } */

/*         Texture2D icon = GetTextureFromCache(fm.files[i]); */
/*         DrawTexture(icon, 10, yPosition, WHITE); */

/*         Color color = (i == fm.selectedIndex) ? YELLOW : WHITE; */
/*         DrawText(fm.files[i], 10 + iconSize + iconPadding, yPosition, fontSize, color); */
/*     } */

/*     // End the scissor mode. */
/*     EndScissorMode(); */

/*     ClearUnusedTextures(); */
/* } */
