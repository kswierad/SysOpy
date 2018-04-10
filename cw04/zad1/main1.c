#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <zconf.h>

void catch(int signal){
    printf("Caught SIGINT, terminating.\n");
    exit(0);
}
int wait;
void catch_sigstp(int signal){
    /*struct sigaction act;
    act.sa_handler = catch_sigstp;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(20,&act,NULL);*/
    if(wait){
        wait = 0;
    } else{ 
        wait = 1;
        printf("Waiting for CTRL+Z - continuation or CTRL+C - terminate program\n");
        sigset_t mask;
        sigfillset(&mask);
        sigdelset(&mask,2);
        sigdelset(&mask,20);
        sigsuspend(&mask);
    }
}
int main(){
    signal(2, catch);
    struct sigaction act;
    act.sa_handler = catch_sigstp;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(20,&act,NULL);
    wait = 0;
    while(1){
        pid_t pid;
        pid = fork();
        if(!pid){
            execlp("date","date",NULL);
            exit(1);
        }
        sleep(1);
    }

    return 0;
}