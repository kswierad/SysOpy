#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <zconf.h>

pid_t child = -1;
void end(int signal){
    printf("Caught SIGINT, terminating.\n");
    if(child!=-1){
        kill(child,SIGTERM);
    }
    exit(0);
}
void handle_child(int signal){
        if(child == -1){
            child = fork();
            if(child==0){
                execlp("./date.sh","./date.sh",NULL);
                exit(1);
            }
        } else{
            printf("Waiting for CTRL+Z - continuation or CTRL+C - terminate program\n");
            kill(child,SIGTERM);
            child = -1;
            sigset_t mask;
            sigfillset(&mask);
            sigdelset(&mask,2);
            sigdelset(&mask,20);
            sigsuspend(&mask);
        }
    
}
int main(){
    signal(SIGINT, end);
    struct sigaction action;
    action.sa_handler = handle_child;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(20,&action,NULL);
    
    child = fork();
    if(!child){
        execlp("./date.sh","./date.sh",NULL);
        exit(1);
    }
    
    while(1){
    }

    return 0;
}