#include "commands.h"
#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include "panels.h"
#include "filemanager.h"

void restart_slicer(){
/* system("rm slicer"); */
  system("make");
  // Restart the game engine
  char *args[] = {"./slicer", NULL}; // Assuming the binary name is "slicer"
  execv(args[0], args);

  // If execv fails, you can handle the error here
  perror("Failed to reload the game engine");
  exit(1);
}


void window_next(){
  MoveFrameSelection(1);
}

void toggle_filemanager() {
  if (fileManagerVisible){
    panel.leftWidth = 0;
    fileManagerVisible = 0;
  }else{
    panel.leftWidth = INITIAL_LEFT_WIDTH;
    fileManagerVisible = 1;
  }
}
