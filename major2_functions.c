#include "major2_functions.h"
#include "path.h"

//******************************GLOBAL VARS****************
bool exitFlag = 0;
int pid;

//**************************FUNCTION DEFINITIONS***********
//print current working director
void printCurrDir() {
  char currWorkingDir[1024];

  getcwd(currWorkingDir, sizeof(currWorkingDir));
  printf("CWDir: %s ", currWorkingDir);
}

//exit shell program built-in
void exitShell() {
  printf("Goodbye\n");
  exit(0);
}

//change directory built-in
void changeDir(char **parsed) {

  if(parsed[1] == NULL) {
    chdir(getenv("HOME"));
  }
  else {
    if(chdir(parsed[1]) != 0) {
      perror("chdir() error");
    }
  }

  printf("Changed directory to: ");
  printCurrDir();
  printf("\n");
  
}

//executing the other linux commands
void execOtherCmds(char **parsed) {
  pid_t pid;
  int status;

  //fork fail check
  if((pid = fork()) < 0) {     
    printf("Forking child process failed\n");
    exit(1);
  }
  //child process: run command
  else if(pid == 0) {          
    if(execvp(parsed[0], parsed) < 0) {    
      printf("\tExecvp error: command(s) not recognized\n");
      exit(1);
    }
  }
  //parent process: wait for child
  else {                                  
    while (wait(&status) != pid);
  }
}

//function to handle signals
void handleSignals(int signo) {
  static int stop;

  if (signo == SIGINT) {
    kill(pid, SIGKILL);
    printf(" Done\n");
    exit(0);
  }

  if (signo == SIGTSTP) {
    if (!stop) {
      printf("\n$>>> ");
      printCurrDir();
      printf("\nCtrl z caught: Suspend\n");
      
      if(pid != 0) {}
      
      stop = 1;
    } 
    else {
      printf("\n$>>> ");
      printCurrDir();
      printf("\nCtrl z caught: Resume\n");
      
      if(pid != 0) {}
      
      stop = 0;
      
    }
  }
}

//function that calls handle signals
void signalCall() {
  if (signal(SIGINT, handleSignals) == SIG_ERR) {
    printf("\ncan't catch SIGINT\n");
  }
    
  if (signal(SIGTSTP, handleSignals) == SIG_ERR) {
    printf("\ncan't catch SIGINT\n");
  }
    
  signal(SIGCONT, handleSignals);
  
  pid = fork();
  
  if (pid == 0) {}
  else {
    setpgid(pid,0);
  }
  
}

//LOOP HERE+++++++++++++++++++++++++++++++++++++++++++++++++
void interactiveLoop() {
  char *inputString;

  setBigPath(bigPath);

  signalCall();

  if(pid != 0) {
    while (1) {
      //print shell line
      printf("$>>> ");
      
      //print current working directory
      printCurrDir();
      
      //take input
      inputString = readLineFeed();
  
      //making sure string to parse is long enough
      if(strlen(inputString) <= 1) {
        printf("Shell: too few arguments, try again\n");
      }
      else {
        //parse user input
        parseUserInput(inputString);
      }
  
      //check if exitFlag is true, then exit shell program at end of command loop
      if(exitFlag == 1) {
        exitShell();
      }
    }
  }
  
}
//LOOP HERE+++++++++++++++++++++++++++++++++++++++++++++++++

//batch file mode
void batchMode(char **argv) {
  FILE *filePtr;
  char *fileContents;
  char *currentLine;
  int length;
  filePtr = fopen(argv[1], "r");

  if(filePtr == NULL) {
    printf("Cannot open file\n");
    exit(1);
  }

  //finding length of file
  fseek(filePtr, 0, SEEK_END);
  length = ftell(filePtr);
  fseek(filePtr, 0, SEEK_SET);

  //allocation size of string based on file length
  fileContents = malloc(sizeof(char) * (length+1));

  //using fread to read in file contents
  if (fileContents) {
    fread(fileContents, 1, length, filePtr);
  } 
  else {
    printf("Failed to malloc\n");
    exit(1);
  }
  
  fclose(filePtr);

  printf("%s\n", fileContents);

  //parsing file contents to execute commands
  while(fileContents != NULL && strlen(fileContents) > 0) {
    
    currentLine = strsep(&fileContents, "\n");
    
    parseUserInput(currentLine);
    
  }

}

//read line feed
char *readLineFeed() {
  char *buffer = NULL;
  ssize_t bufferSize = 0;
  
  if (getline(&buffer, &bufferSize, stdin) == -1) {
    
    if(feof(stdin)) {
      printf("Shell: readline error\n");
      exit(EXIT_FAILURE);   
    } 
    else {
      printf("Shell: readline error\n");
      exit(EXIT_SUCCESS); //we recieved an EOF
    }
  }
  
  char *newBuffer = strsep(&buffer, "\n");
  
  return newBuffer;
  
}


//function that takes potentially piped command(s) and seperates them into basic commands
//then sends basic commands to executeUserCmd
void parseUserPipedCmd(char *userPipedCmd) {
  char *userCmds[3];

  for(int i = 0; i < 3; ++i) {
  
    if(userPipedCmd == NULL) {
      return;
    }
    
    if(strlen(userPipedCmd) <= 1) {
      return;
    }
    
    userCmds[i] = strsep(&userPipedCmd, "|");
   
    executeUserCmd(userCmds[i]);
    
  }
}

//function to parse user input that potentially includes pipes and semicolons, i.e. the raw input
//seperates input from semicolons and sends them to parseUserPipedCmd
void parseUserInput(char *userInput) {
  char *userCmds[3];

  for(int i = 0; i < 3; ++i) {
    
    if(userInput == NULL) {
      return;
    }
    if(strlen(userInput) <= 1) {
      return;
    }
    
    userCmds[i] = strsep(&userInput, ";");
    
    parseUserPipedCmd(userCmds[i]);
  }
  
}

//function for parsing command words from spaces
void parseSpace(char *str, char **parsed) {
  int i;

  for (i = 0; i < MAX_LIST; i++) {
    
    parsed[i] = strsep(&str, " ");
    
    if (parsed[i] == NULL) {
      break;
    }
    if (strlen(parsed[i]) == 0) {
      i--;
    }
    
  }
  
}

//process input string for spaces
void processCmdForExecution(char *str, char **parsed) {
  if(str == NULL) {
    return;
  }
  if(strlen(str) <= 1) {
    return;
  }
  
  parseSpace(str, parsed);
  
}

//executing the built-in commands and the linux shell commands 
void executeUserCmd(char *userCmd) {
  char *parsedArgs[MAX_LIST];
  
  processCmdForExecution(userCmd, parsedArgs);
  
  runCommandMenu(parsedArgs);
  
} 

//command menu
void runCommandMenu(char **parsed) {

  int cmdSwitchCase = 0;
  char *builtInCmds[4];  

  builtInCmds[0] = "exit";
  builtInCmds[1] = "cd";
  builtInCmds[2] = "path";
  builtInCmds[3] = "myhistory";

  for(int i = 0; i < 4; ++i) {
    if(strcmp(parsed[0], builtInCmds[i]) == 0) {
      cmdSwitchCase = i + 1;
      break;
    }
  }
  
  switch(cmdSwitchCase) {
    case 1:
      exitFlag = 1;
      break;
    case 2:
      changeDir(parsed);
      break;
    case 3:
      path(parsed,bigPath);
      break;
    case 4:
      printf("Where myhistory goes.\n");
      break;
    default:
      if(strcmp(*parsed, "grep") && strcmp(*parsed, "sort")&& strcmp(*parsed, "head") && strcmp(*parsed, "find")) {
        printf("Exec %s\n", *parsed);
        execOtherCmds(parsed);
      }
      break;
  }
  
}
