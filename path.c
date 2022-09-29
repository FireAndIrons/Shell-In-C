#include <stdio.h>            // everything
#include <stdlib.h>           // EXIT_FAILURE
#include <string.h>           // strncmp
#include <unistd.h>           // execl
#include <ctype.h>            // processes
#include <pthread.h>           // Threads
#include "path.h"

//char bigPath [500];

//find path variable
void setBigPath(char *pj)
{
    strcpy(pj,getenv("PATH")); 
}


void remoPath(char *bigPath,char *par)
{
    char *found = strstr(bigPath,par);
    if(found==NULL)
    {
        printf("");
        return;
    }
        
    int parL= strlen(par);
    int bigL= strlen(bigPath);
    int fp = (found - bigPath);
    int lp = (bigL - fp) - parL;
    
    char *newstr = (char*)malloc(500);

    memcpy(newstr,bigPath,fp);
    memcpy(newstr+fp,bigPath+(fp+parL),lp);

    newstr[fp+lp]='\0';
    strcpy(bigPath,newstr);

    free(newstr);
}

void addPath (char *par,char *bigPath)
{
    if (strstr(bigPath,par) == NULL)//if not there
    {
        strcat(bigPath, par);
    }

    return;
}


void path (char **parsed,char *bigPath)
{

    if( parsed[1] == NULL)
    {
        printf("%s\n",bigPath);
    }
    else if(strcmp(parsed[1],"+") == 0)
    {
        addPath(parsed[2],bigPath);
    }
    else if(strcmp(parsed[1],"-") == 0)
    {
        remoPath(bigPath,parsed[2]);
    }
    else
    {
        printf("Path error\n");
    }

     return;
}



//no arge
//return the current PATH

//PATH + /bin
//append strcat()
// set envriment setenv()

//PATH - /bin
// exec(PATH)
// Linix patha fuctuion remove form string
