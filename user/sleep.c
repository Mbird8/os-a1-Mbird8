#include "user/user.h"
int main(int argc, char *argv[]){
    
    int ticks = atoi(argv[1]);

    if(argc < 2 || argc > 2 || ticks <= 0){
        fprintf(2, "usage: sleep <ticks> \n");
        exit(1);
    }
    printf("Waiting for %d ticks", ticks);
    pause(ticks);
    printf("\nFinished waiting\n");
    exit(0);
}