#define _XOPEN_SOURCE
#define _DEFAULT_SOURCE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wait.h>

struct child_info {
    pid_t pid;
    int sig_sent;
    int sig_recieved;
};
struct child_info* children;
int K;
int N;
int curr_sigs;
void control_children(int signal, siginfo_t* siginfo){
    union sigval sgv;
    sgv.sival_ptr = NULL;
    //printf("control_children start \n");
    if(curr_sigs<K){
        printf("%s %d %d [%d]\n","Received signal:",signal,getpid(),curr_sigs);
        for(int i=0; i<N; i++){
            if(siginfo->si_pid==children[i].pid) children[i].sig_recieved=1;
        }
        curr_sigs++;
    } else if(curr_sigs == K){
        curr_sigs++;
        for(int i=0; i < N; i++){
            if(siginfo->si_pid==children[i].pid) children[i].sig_recieved=1;
            if(children[i].sig_recieved){
                //printf("%s %d %d\n","Sending signal:",SIGUSR2,children[i].pid);
                sigqueue(children[i].pid,SIGUSR2,sgv);
                children[i].sig_sent=1;
            }
        }
    } else {
        for(int i=0; i < N; i++){
            if(siginfo->si_pid==children[i].pid) children[i].sig_recieved=1;
            if(!children[i].sig_sent && children[i].sig_recieved){
                sigqueue(children[i].pid,SIGUSR2,sgv);
                children[i].sig_sent=1;
            }
        }
    }
    //printf("control_children stop \n");
}

void kill_em_all(int signal){

    printf("%s %d %d\n","Received  kill signal:",signal,getpid());

    for(int i=0; i < N; i++){
        kill(children[i].pid,SIGTERM);
    }
    exit(0);
}
int stop;
void start(int signal){
    //printf("start \n");
    //printf("%s %d %d\n","Received signal:",signal,getpid());
    stop =0;
    //printf("stop \n");
}
void catch_rt(int signal, siginfo_t* siginfo){
    //printf("catch rt start \n");
    pid_t child_pid = siginfo->si_pid;
    printf("Got signal SIGMIN+%i, from child pid: %i \n", signal - SIGRTMIN, child_pid);
    int exit_status;
    //waitpid(child_pid,&exit_status, WNOHANG ); 
    //exit_status = WIFEXITED(exit_status);
    //printf("Child pid: %i, exited with status: %i \n", child_pid, exit_status); 
    //printf("catch rt stop \n");
}   

int main(int argc, char * argv[]){
    curr_sigs=0;
    printf("JESTEM %d \n",getpid());

    
    N = atoi(argv[1]);
    K = atoi(argv[2]);
    children = calloc(N,sizeof(struct child_info));
    struct sigaction act;
    
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    
    act.sa_handler = kill_em_all;
    sigaction(2,&act,NULL);

    act.sa_flags = SA_SIGINFO;
    act.sa_handler = control_children;
    sigaction(SIGUSR1,&act,NULL);

    act.sa_handler = catch_rt;
    
    
    for(int i=SIGRTMIN; i<SIGRTMAX; i++){ 
        sigaction(i,&act,NULL);
    }
    for(int i=0;i<N;i++){
        children[i].pid = 0;
        children[i].sig_sent= 0;
    }

    for(int i=0; i<N;i++){
        children[i].pid = fork();
        //printf("FORKUJE \n");
        if(children[i].pid == 0){
            stop = 1;
            srand(time(NULL));
            struct sigaction act;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;
            act.sa_handler = start;

            union sigval sgv;
            sgv.sival_ptr = NULL;

            sigaction(SIGUSR2,&act,NULL);
            int seconds = rand()%10 + 1;
            sleep(seconds);
            sigqueue(getppid(),SIGUSR1,sgv);
            while(stop) usleep(50);
            int signal_no = SIGRTMIN + (rand()%(SIGRTMAX-SIGRTMIN));
            printf("%s %d %d\n","Sending signal:",signal_no,getpid());
            
            sigqueue(getppid(),signal_no,sgv);
            exit(seconds);

        }
        usleep(50);
    }
    int i = 0;
    while(i<N){
        waitpid(-1,NULL,NULL);
        i++; 
    }
    return 0;
}