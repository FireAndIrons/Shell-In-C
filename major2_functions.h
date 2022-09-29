#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <wait.h>

#define BUFFER_SIZE 512
#define MAX_LIST 100

//**************************FUNCTION DECLARES*************
//LINE READ AND PARSE
void interactiveLoop();
void batchMode(char **argv);
char *readLineFeed();

void processCmdForExecution(char *str, char **parsed);
void parseSpace(char *str, char **parsed);

void parseUserInput(char *userInput);
void executeUserCmd(char *userCmd);

//OTHER
void printDirectory();
void execOtherCmds(char **parsed);

//COLLAB EFFORTS
void handleSignals(int signo);
void singnalCall();

//BUILT IN COMMANDS
void runCommandMenu(char **parsed);
void changeDir(char **parsed);
void exitShell();
