// ini.h
#include <stdio.h>

FILE *ini_open(char *fileName);
char *ini_get(FILE *file, char *sectionToFind, char *keyToFind);
void ini_close(FILE *);
