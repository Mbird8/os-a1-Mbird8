#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int filePath(char* path, char* toFind){
    // final
    if((toFind[0]) == '\0') return ((path[0]) == '\0');
    // recurse into filePath
    if((toFind[0]) == '*') return (filePath(path, toFind+1) || ((path[0]) != '\0' && filePath(path+1, toFind)));
    return ((path[0]) != '\0' && (toFind[0]) == (path[0]) && filePath(path+1, toFind+1));
}

void find(char *path, char *toFind, int type){

    char buff[MAXPATH], *p;
    int fd;
    struct dirent de;
    struct stat st;
    //fd = open(path, 0);

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat%s\n", path);
        close(fd);
        exit(1);
    }

    if(st.type != T_DIR){
        fprintf(2, "find: <dir> not a directory\n");
        close(fd);
        exit(1);
    }

    strcpy(buff, path); // copy path into buff
    p = buff + strlen(buff); // p = end of buff

    if(p != buff && *(p-1) != '/') *(p++) = '/'; // add '/' to end+1 of buff

    while (read(fd, &de, sizeof(de)) == sizeof(de)){
//printf("inside while\n");
        if(de.inum == 0) continue; // skip 

        memmove(p, de.name, DIRSIZ); // append name to p
        p[DIRSIZ] = 0;

        if(strcmp(p, ".") == 0 || strcmp(p, "..") == 0) continue; //skip current and previous directory
        if(stat(buff, &st) < 0){
            fprintf(2, "find: cannot stat%s\n", buff);
            continue;
        }
//printf("3 if\n");
        if((type == 0 || type == st.type || (type == T_FILE && st.type == T_DEVICE)) && filePath(p, toFind)){
            printf("%s\n", buff);
        }
//printf("4 if\n");
        if(st.type == T_DIR){
            find(buff, toFind, type);
        }
        //printf("end while\n");
    }
    close(fd);
}

int main(int argc, char * argv[]){
    

    if(argc < 3 || argc > 5 || argc == 4){
        fprintf(2, "usage: find <dir> <pattern> [-type f|d]\n");
        exit(1);
    }

    // int file = open(argv[1], 0);
    // if(file == -1){
    //     fprintf(2, "Cannot open <dir>\n");
    //     exit(1);
    // }

    if(argc == 5){
        if(*argv[4] == 'f'){
            find(argv[1], argv[2], T_FILE);
        }
        else if(*argv[4] == 'd'){
            find(argv[1], argv[2], T_DIR);
            exit(0);
        }
    } 
    else if(argc == 3){
        find(argv[1], argv[2], 0);
        exit(0);
    }

    fprintf(2, "Proper usage: find <dir> <pattern> [-type f|d]\n");
        exit(1);
    
    // if(argc > 5){
    //     type = argv[4];
    // }
    // else{

    // }
}
