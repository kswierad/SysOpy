#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>

#include "systemV.h"

#define FAILURE_EXIT(code, format, ...) { fprintf(stderr, format, ##__VA_ARGS__); exit(code);}

#define PID 0
#define QID 1
#define ACTIVE 2

int privateID;
int CID;
int publicID;

int quit = 0;
int clients[MAX_CLIENTS][3];

void deleteQueue(){
    msgctl(privateID, IPC_RMID, NULL);
    printf("Deleted private queue. \n");
}

void register_client(int privateKey);

void rq_mirror(msg_buf* message);

void rq_calc(msg_buf* message);
void rq_end(msg_buf* message);

void rq_time(msg_buf* message);
/*void intHandler(int signum){
    for(int i=0; i < MAX_CLIENTS; i++){
        if(clients[i][ACTIVE]){
            kill(clients[i][PID],SIGINT);
        }
    }
    exit(2);
    
}*/

void intHandler(int signo){
    exit(2);
}



int main(){
    if(atexit(deleteQueue) == -1) FAILURE_EXIT(3, "Registering server's atexit failed!");
    if(signal(SIGINT, intHandler) == SIG_ERR) FAILURE_EXIT(3, "Registering INT failed!");

    char* home = getenv("HOME");
    if(home == NULL) FAILURE_EXIT(3, "Getting enviromental variable 'HOME' failed!");

    key_t privateKey = ftok(home, getpid());
    if(privateKey == -1) FAILURE_EXIT(3, "Generation of publicKey failed!");

    privateID = msgget(privateKey, IPC_CREAT | IPC_EXCL | 0666);
    if(privateID == -1) FAILURE_EXIT(3, "Creation of public queue failed!");

    key_t publicKey = ftok(home, ID_SEED);
    if(publicKey == -1) FAILURE_EXIT(3, "Generation of publicKey failed!");

    publicID = msgget(publicKey, 0);
    if(publicID == -1) FAILURE_EXIT(3, "Creation of public queue failed!");
    
    register_client(privateKey);
    
    msg_buf message;
    char command[20];
    
    while(1){

        
        message.sender_pid = getpid();
        printf("Enter your request: ");
        if(fgets(command, 20, stdin) == NULL){
            printf("Error reading your command!\n");
            continue;
        }
        int n = strlen(command);
        if(command[n-1] == '\n') command[n-1] = 0;


        if(strcmp(command, "mirror") == 0){
            rq_mirror(&message);
        }else if(strcmp(command, "calc") == 0){
            rq_calc(&message);
        }else if(strcmp(command, "time") == 0){
            rq_time(&message);
        }else if(strcmp(command, "end") == 0){
            rq_end(&message);
        }else if(strcmp(command, "q") == 0){
            exit(0);
        }else printf("Wrong command!\n");
    }
    return 0;
}

void register_client(int privateKey){
    msg_buf message;
    message.mtype = INIT;
    message.sender_pid = getpid();
    sprintf(message.text, "%d", privateKey);

    if(msgsnd(publicID, &message, MSG_SIZE, 0) == -1) FAILURE_EXIT(3, "INIT request failed!");
    printf("Waiting for response \n");
    if(msgrcv(privateID, &message, MSG_SIZE, 0, 0) == -1) FAILURE_EXIT(3, "catching LOGIN response failed!");
    printf("got response \n");
    if(sscanf(message.text, "%d", &CID) < 1) FAILURE_EXIT(3, "scanning INIT response failed!");
    if(CID < 0) FAILURE_EXIT(3, "Server cannot have more clients!");
}

void rq_mirror(msg_buf* message){
    
    message->mtype = MIRROR;
    printf("Enter string of characters to mirror: ");
    if(fgets(message->text, MAX_MSG_TXT, stdin) == NULL){
        printf("Too many characters!\n");
        return;
    }
    if(msgsnd(publicID, message, MSG_SIZE, 0) == -1) FAILURE_EXIT(3, "MIRROR request failed!");
    if(msgrcv(privateID, message, MSG_SIZE, 0, 0) == -1) FAILURE_EXIT(3, "catching MIRROR response failed!");
    printf("%s", message->text);
}

void rq_calc(msg_buf* message){
    message->mtype = CALC;
    printf("Enter an expression to calculate: ");
    if(fgets(message->text, MAX_MSG_TXT, stdin) == NULL){
        printf("Too many characters!\n");
        return;
    }
    if(msgsnd(publicID, message, MSG_SIZE, 0) == -1) FAILURE_EXIT(3, "CALC request failed!");
    if(msgrcv(privateID, message, MSG_SIZE, 0, 0) == -1) FAILURE_EXIT(3, "catching CALC response failed!");
    printf("%s", message->text);
}

void rq_end(msg_buf* message){
    message->mtype = END;
    if(msgsnd(publicID, message, MSG_SIZE, 0) == -1) FAILURE_EXIT(3, "END request failed!");
}

void rq_time(msg_buf* message){
    message->mtype = TIME;

    if(msgsnd(publicID, message, MSG_SIZE, 0) == -1) FAILURE_EXIT(3, "TIME request failed!");
    if(msgrcv(privateID, message, MSG_SIZE, 0, 0) == -1) FAILURE_EXIT(3, "catching TIME response failed!");
    printf("%s", message->text);
}