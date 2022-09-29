#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "major2_functions.h"

//**************************MAIN**************************
int main(int argc, char **argv) {
  
  //Run interactive mode loop
  if(argc == 1) {
    interactiveLoop();
  }
  //Load any batch files
  else {
    batchMode(argv);
  }

  return 0;
}

