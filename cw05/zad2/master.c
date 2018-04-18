#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>

#define BUFFER_SIZE 256


int main(int argc, char *argv[]) {
    if (argc < 2){
        printf("Too little arguments! \n");
        exit(1);
    }

    if (mkfifo(argv[1], S_IRUSR | S_IWUSR) == -1){
        printf("Making FIFO failed! \n");
        exit(1);
    }

    FILE *fifo = fopen(argv[1], "r");
    if (!fifo){
        printf("Opening FIFO failed! \n");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, fifo)) {
        printf("%s", buffer);
    }
    printf("Master: Ended reading FIFO\n");

    fclose(fifo);
    if (remove(argv[1])){
        printf("Removing FIFO failed! \n");
        exit(1);
    }
    return 0;
}