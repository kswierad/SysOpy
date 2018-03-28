
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

char* time_limit;
char* mem_limit;

void print_usage_info(struct rusage* usage) {
    printf("System time: %i s %li us \n User time: %i s %li us \n",
           (int)usage->ru_stime.tv_sec, (long)usage->ru_stime.tv_usec,
           (int)usage->ru_utime.tv_sec, (long)usage->ru_utime.tv_usec);
}

void set_limits(char* mem_limit, char* time_limit){
  long mem_lim = strtol(mem_limit, NULL, 10)* 1048576;
  long time_lim = strtol(time_limit, NULL, 10);

  struct rlimit cpu_time_limit;
  struct rlimit memory_limit;

  cpu_time_limit.rlim_max = (rlim_t) time_lim;
  cpu_time_limit.rlim_cur = (rlim_t) time_lim;

  memory_limit.rlim_max = (rlim_t) mem_lim;
  memory_limit.rlim_cur = (rlim_t) mem_lim;

  setrlimit(RLIMIT_CPU, &cpu_time_limit);
  setrlimit(RLIMIT_AS, &memory_limit);
}

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
    struct rusage child_rusage;
    getrusage(RUSAGE_CHILDREN,&child_rusage);
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
        
        getrusage(RUSAGE_CHILDREN,&child_rusage);
        print_usage_info(&child_rusage);
        
        return 0;

    } else {
        set_limits(mem_limit,time_limit);
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
    time_limit = argv[2];
    mem_limit =  argv[3];
    while(!execute_line(file_handle));
    fclose(file_handle);
    return 0;
}