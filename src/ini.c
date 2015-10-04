#include "str.h"

#include <stdio.h>
#include <stdlib.h>

FILE *ini_open(char *fileName) {
    return fopen(fileName, "r");
}

char *ini_get(FILE *file, char *sectionToFind, char *keyToFind) {
    fseek(file, 0, SEEK_SET);
    int sect = 0;
    while (!feof(file)) {
        char *line = (char *)malloc(1024);
        line = str_trim(fgets(line, 1024, file));
        int len = str_len(line);
        if (len > 0) {
            if (len > 0 && line[0] == '[' && line[len - 1] == ']' ) {
                if (str_cmp(str_substr(line, 1, len - 2), sectionToFind) == 0)
                    sect = 1;
                else sect = 0;
            }
            int sepPos = str_indexof(line, "=");
            if (sect == 1 && sepPos != -1) {
                char *key = str_trim(str_substr(line, 0, sepPos - 1));
                char *value = str_trim(str_substr(line, sepPos + 1, len - sepPos));
                if (strcmp(key, keyToFind) == 0) {
                    return value;
                }
            }
        }
    }
    return NULL;
}

void ini_close(FILE *file) {
    fclose(file);
}