#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>

#define MAXARGC 512
#define DESC_NUMB  10
int descryptors[DESC_NUMB][2];

int parsecommand(char * cmdstr);
void linesReading(char *fileName);
void initPipes();
void closePipes();

int main(int argc, char* argv[]){
    linesReading(argv[1]);
    return 0;
}

void initPipes(){
    for(int i=0; i<DESC_NUMB; i++){
        if(pipe(descryptors[i]) == -1){
            perror("Pipe");
            exit(EXIT_FAILURE);
        }
    }
}
void closePipes(){
    for(int i=0; i<DESC_NUMB; i++){
        close(descryptors[i][0]);
        close(descryptors[i][1]);
    }
}

int parsecommand(char * cmdstr)
{

    initPipes();

    char *cmdstrdup = strdup(cmdstr);
    if(cmdstrdup == NULL)
        return 0; 

    char *saveptr;
    char *ptr;
    char *cmds[MAXARGC]; 
    int cmdn;
    int argc;
    char *argv[MAXARGC];

    pid_t childProcess;
    cmdn=0;
    cmds[cmdn++] = strtok(cmdstrdup, "|\n");
    while ((cmds[cmdn++] = strtok(NULL, "|\n")) != NULL);
    cmdn--;

     for (int i = 0;i < cmdn; ++i){
         argc=0;
         argv[argc++] = strtok(cmds[i], " \n");
            while ((argv[argc++] = strtok(NULL, " \n")) != NULL);
        argc--;
        for (int i=0; i<argc; i++) printf("%s ", argv[i]);
        printf("\n");
        if ((childProcess = fork()) == -1){
            perror("Fork");
            exit(EXIT_FAILURE);
        }


        if (childProcess == 0) {

            if((i != cmdn -1))
                dup2(descryptors[i+1][1],  STDOUT_FILENO);

            dup2(descryptors[i][0], STDIN_FILENO);

            if (execvp(argv[0], argv) == -1) {
                perror("Error");
                exit(-1);
            }
        }
        close(descryptors[i+1][1]);
        
     }

     printf("All comands: %d, waiting for them to exit\n", cmdn);
        for (int i = 0; i < cmdn; ++i) {
            wait(NULL);
            printf("%d'th thread exited\n", i);
        }

        closePipes();
    return 1;
    
}

void linesReading(char *fileName){
    FILE *fp;
    char *line = NULL;
    size_t len =0;
    ssize_t read;

    fp=fopen(fileName, "r");
    if(fp==NULL)
        exit(EXIT_FAILURE);
    while((read=getline(&line, &len, fp))!=-1){

        if(line[read-1]=='\n')line[read-1]=NULL;
        if(parsecommand(line)!=1)
            break;
    }
    free(line);
    fclose(fp);
}