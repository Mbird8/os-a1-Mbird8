#include "user/user.h"

int filePath(char* path, char* toFind){
    // final
    if((toFind[0]) == '\0') return ((path[0]) == '\0');
    // recurse into filePath
    if((toFind[0]) == '*') return (filePath(path, toFind+1) || ((path[0]) != '\0' && filePath(path+1, toFind)));
    return ((path[0]) != '\0' && (toFind[0]) == (path[0]) && filePath(path, toFind));
}

char find(char *path, char *toFind, char type){

    return 'a';
}

int main(int argc, char * argv[]){
    

    if(argc < 3 || argc > 5 || argc == 4){
        fprintf(2, "usage: find <dir> <pattern> [-type f|d]\n");
        exit(1);
    }

    int file = open(argv[1], 0);
    if(file == -1){
        fprintf(2, "Cannot open <dir>\n");
        exit(1);
    }

    char type = 'a';
    if(argc > 5){
        type = argv[4];
        char filePath
    }
    else{

    }
}
