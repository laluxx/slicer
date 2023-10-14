#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <raylib.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TEXTURES 100

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

// Function prototypes
void UpdateFileList();
Texture2D GetIconForExtension(const char* name);
void InitializeFileManager(const char* startDir);
void DrawFileManager();
void NavigateUp();
void NavigateDown();
void NavigateIn();
void NavigateOut();
void UpdateFileManager();

#endif // FILEMANAGER_H
