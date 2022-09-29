#ifndef PATH_H
#define PATH_H
static char bigPath [500];
void setBigPath(char *pj);
void addPath (char *par,char *bigPath);
void remoPath(char *bigPath,char *par);
void path (char **parsed,char *bigPath);

#endif 
