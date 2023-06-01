#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "Utils.h"


int create_path_to_file(char** buffer, char* file_name){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd))== NULL) {
        return 1;
    }
    *buffer = malloc(strlen(cwd) + strlen(file_name) + 2);
    strcpy(*buffer, cwd);
    strcat(*buffer, "/");
    strcat(*buffer, file_name);
    return 0;
}


int MAX(int a, int b){
    return (a > b) ? a : b;
}


int MIN(int a, int b){
    return (a < b) ? a : b;
}


int SIGNUM(int a){
    return (a > 0) ? 1 : -1;
}