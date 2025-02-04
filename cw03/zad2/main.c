
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_LINE_LENGTH 256
#define MAX_NUM_ARGS 32

char** to_array(char* line){
    char** result =  calloc(MAX_NUM_ARGS,sizeof(char*));
    char* buffer = calloc(MAX_NUM_ARGS,sizeof(char));
    for(int i=0; i < MAX_NUM_ARGS; i++){
         result[i] = calloc(MAX_NUM_ARGS,sizeof(char));
    }
    int i=1;
    buffer = strtok(line," \n");
    strcpy(result[0],buffer);
    buffer = strtok(NULL," \n");
    while(buffer){
        strcpy(result[i],buffer);
        buffer = strtok(NULL," \n");
        i++;
    }
    result[i] = NULL;
    return result;
}

int execute_line(FILE* file_handle){
    char* buffer = calloc(MAX_LINE_LENGTH,sizeof(char));
    if(!fgets(buffer,MAX_LINE_LENGTH,file_handle)) return 1;
    char** args_array = to_array(buffer);
    pid_t pid = fork();
    int status;
    if(pid){
        waitpid( pid, &status, WUNTRACED );
        if(status) {
            printf("Executing command %s", args_array[0]);
            int i = 1;
            if(args_array[0] != NULL) printf(", with arguments ");
            while(args_array[i] != NULL) {
                printf("%s ", args_array[i]);
                i++;
            }
            printf(" failed with status %i. \n", status);
            return 1;
        }
        return 0;

    } else {
        execvp(args_array[0],args_array);
        exit(1);
    }

    fclose(file_handle);
    return 0;
}


int main(int argc, char* argv[]){
    FILE* file_handle = fopen(argv[1],"r");
    if(!file_handle){
        printf("Couldn't open %s, shutting down.",argv[1]);
        return 1;


    }
    
    while(!execute_line(file_handle));
    fclose(file_handle);
    return 0;
}