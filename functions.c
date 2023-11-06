#include "functions.h"
#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>

void restart_slicer(){

  system("make");
  // Restart the game engine
  char *args[] = {"./slicer", NULL}; // Assuming the binary name is "slicer"
  execv(args[0], args);

  // If execv fails, you can handle the error here
  perror("Failed to reload the game engine");
  exit(1);
}
