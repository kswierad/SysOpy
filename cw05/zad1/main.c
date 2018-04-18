
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
#define MAX_PROGS 5

char **to_array(char *line)
{
    char **result = calloc(MAX_NUM_ARGS, sizeof(char *));
    char *buffer = calloc(MAX_NUM_ARGS, sizeof(char));
    for (int i = 0; i < MAX_NUM_ARGS; i++)
    {
        result[i] = calloc(MAX_NUM_ARGS, sizeof(char));
    }
    int i = 1;
    buffer = strtok(line, " \n");
    strcpy(result[0], buffer);
    buffer = strtok(NULL, " \n");
    while (buffer)
    {
        strcpy(result[i], buffer);
        buffer = strtok(NULL, " \n");
        i++;
    }
    free(buffer);
    result[i] = NULL;
    return result;
}

int execute_line(FILE *file_handle)
{
    
    int pipes[2][2];
    int i = 0;
    char *buffer_loaded = calloc(MAX_LINE_LENGTH, sizeof(char));
    char *buffer_progs = calloc(MAX_LINE_LENGTH, sizeof(char));
    char *progs[MAX_PROGS];
    for(i=0; i < MAX_PROGS; i++) progs[i] = calloc(MAX_LINE_LENGTH, sizeof(char));
    i=0;
    size_t size = MAX_LINE_LENGTH;
    if (getline(&buffer_loaded, &size, file_handle)==-1){
    
        return 1;
    }
    buffer_progs = strtok(buffer_loaded, "|");
    while (buffer_progs && i < MAX_PROGS)
    {
        strcpy(progs[i], buffer_progs);
        buffer_progs = strtok(NULL, "|");
        i++;
    }
   

    for (int j = 0; j < i; j++){
         

        if (j > 1) {
                close(pipes[j % 2][0]);
                close(pipes[j % 2][1]);
        }

        if (pipe(pipes[j%2]) == -1) {
            printf("Error on pipe.\n");
            _exit(EXIT_FAILURE);
        }
        //printf("%s \n",progs[j]);
        char **args_array = to_array(progs[j]);
        
        //for(int l=0;l < 3; l++) printf("%s \n",args_array[l]);
        pid_t pid = vfork();
        int status;
        if (pid){
            
            /*waitpid(pid, &status, WUNTRACED);
            
            if (status){
                printf("Executing command %s", args_array[0]);
                int iter = 1;
                if (args_array[0] != NULL)
                    printf(", with arguments ");
                while (args_array[i] != NULL){
                    printf("%s ", args_array[iter]);
                    iter++;
                }
                printf(" failed with status %i. \n", status);
                return 1;
            }*/
            
        }
        else{
            //printf("EXEC %s \n",args_array[0]);
            //close(pipes[j % 2][0]);
            if (j < i - 1) {    
                close(pipes[j % 2][0]);
                if (dup2(pipes[j % 2][1], STDOUT_FILENO) < 0){
                     printf("Error on dup.\n");
                     exit(1);
                }
            }


            if (j > 0) {
                close(pipes[(j + 1) % 2][1]);
                if (dup2(pipes[(j + 1) % 2][0], STDIN_FILENO) < 0){
                    printf("Error on dup.\n");
                    exit(1);
                }
                
            }

            
            execvp(args_array[0], args_array);
            exit(1);
        }
        
    }
    //close(pipes[j % 2][0]);
    //close(pipes[j % 2][1]);
    free(buffer_loaded);
    free(buffer_progs);
    for(i=0; i < MAX_PROGS; i++) free(progs[i]);
    return 0;
}

int main(int argc, char *argv[])
{
    FILE *file_handle = fopen(argv[1], "r");
    if (!file_handle)
    {
        printf("Couldn't open %s, shutting down.", argv[1]);
        return 1;
    }

    while (!execute_line(file_handle));
    fclose(file_handle);
    return 0;
}