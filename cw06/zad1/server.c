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

#define FAILURE_EXIT(code, format, ...) { fprintf(stderr, format, ##__VA_ARGS__);printf("\n"); exit(code);}

void readMsg(msg_buf* message);
void mirror(msg_buf* message);
void calc(msg_buf* message);
void init(msg_buf* message);
void end(msg_buf* message);
void stop(msg_buf* message);
void time_rcv(msg_buf* message);
void rply(msg_buf* message);
int prepareMsg(msg_buf* message);
int findQID(pid_t client_pid);
int getEmptyCID();


#define PID 0
#define QID 1
#define ACTIVE 2

int publicID;

int quit = 0;
int clients[MAX_CLIENTS][3];

void deleteQueue(){
    msgctl(publicID, IPC_RMID, NULL);
    printf("Deleted public queue. \n");
}

void intHandler(int signum){
    /*for(int i=0; i < MAX_CLIENTS; i++){
        if(clients[i][ACTIVE]){
            kill(clients[i][PID],SIGINT);
        }
    }*/
    exit(2);
    
}

int main(){
    if(atexit(deleteQueue) == -1) FAILURE_EXIT(3, "Registering server's atexit failed!");
    if(signal(SIGINT, intHandler) == SIG_ERR) FAILURE_EXIT(3, "Registering INT failed!");

    struct msqid_ds public_state;
    char* home = getenv("HOME");
    if(home == NULL) FAILURE_EXIT(3, "Getting enviromental variable 'HOME' failed!");

    key_t publicKey = ftok(home, ID_SEED);
    if(publicKey == -1) FAILURE_EXIT(3, "Generation of publicKey failed!");

    publicID = msgget(publicKey, IPC_CREAT | IPC_EXCL | 0666);
    if(publicID == -1) FAILURE_EXIT(3, "Creation of public queue failed!");

    msg_buf buffer;
    while(1){
        if(quit){
            if(msgctl(publicID, IPC_STAT, &public_state) == -1) FAILURE_EXIT(3, "Getting current state of public queue failed!\n");
            if(public_state.msg_qnum == 0) break;
        }

        if(msgrcv(publicID, &buffer, MSG_SIZE, 0, 0) < 0) FAILURE_EXIT(3, "Receiving message failed!");
        readMsg(&buffer);
    }
}


void readMsg(msg_buf* message){
    if(message ==NULL) return;
    switch(message->mtype){
        case MIRROR:
            mirror(message);
            break;
        case CALC:
            calc(message);
            break;
        case TIME:
            time_rcv(message);
            break;
        case END:
            end(message);
            break;
        case INIT:
            init(message);
            break;
        case STOP:
            stop(message);
            break;
 /*       case RPLY:
            rply(message);
            break;*/
        default:
            FAILURE_EXIT(3, "Received uknown message!");
    }
}

void mirror(msg_buf* message){
    
    int clientQID = prepareMsg(message);
    if(clientQID==-1) return;
    
    int msgLen = (int) strlen(message->text);
    if(message->text[msgLen-1] == '\n') msgLen--;
    
    for(int i=0; i < msgLen / 2; i++){
        char buff = message->text[i];
        message->text[i] = message->text[msgLen - i - 1];
        message->text[msgLen - i - 1] = buff;
    }

    if(msgsnd(clientQID, message, MSG_SIZE, 0) == -1) FAILURE_EXIT(3, "MIRROR response failed!");
}

void calc(msg_buf* message){
    int clientQID = prepareMsg(message);
    if(clientQID == -1) return;

    char cmd[MAX_MSG_TXT+10];
    sprintf(cmd, "echo '%s' | bc", message->text);
    FILE* calc = popen(cmd, "r");
    fgets(message->text, MAX_MSG_TXT, calc);
    pclose(calc);

     if(msgsnd(clientQID, message, MSG_SIZE, 0) == -1) FAILURE_EXIT(3, "CALC response failed!");
}

void init(msg_buf* message){
    int CID = getEmptyCID();
    
    key_t clientQKey;
    if(sscanf(message->text, "%d", &clientQKey) < 0) FAILURE_EXIT(3, "Reading clientKey failed!");
    
    int clientQID = msgget(clientQKey, 0);
    
    if(clientQID == -1) FAILURE_EXIT(3, "Reading clientQID failed!");

    if(CID == -1) {
        sscanf(message->text, "%d", &CID);
        if(msgsnd(clientQID, message, MSG_SIZE, 0) == -1) FAILURE_EXIT(3, "INIT response failed!");
        return;
    }

    clients[CID][PID] = message->sender_pid;
    clients[CID][QID] = clientQID;
    message->mtype = INIT;
    message->sender_pid = getpid();
    sprintf(message->text, "%d", CID);

    if(msgsnd(clients[CID][QID], message, MSG_SIZE, 0) == -1) FAILURE_EXIT(3, "INIT response failed!");

}

void end(msg_buf* message){
    quit = 1;
}

void time_rcv(msg_buf* message){
    int clientQID = prepareMsg(message);
    if(clientQID == -1) return;

    FILE* date = popen("date", "r");
    fgets(message->text, MAX_MSG_TXT, date);
    fgets(message->text, MAX_MSG_TXT, date);
    pclose(date);

    if(msgsnd(clientQID, message, MSG_SIZE, 0) == -1) FAILURE_EXIT(3, "TIME response failed!");
}

void stop(msg_buf* message){
    int clientQID;
    int i;
    for(i = 0; i < MAX_CLIENTS; i++){
        if(clients[i][PID] == message->sender_pid){
            clientQID = clients[i][QID];
            break;       
        }
    }
    msgctl(clientQID, IPC_RMID, NULL);
    printf("Deleted clients queue. \n");
    clients[i][ACTIVE] = 0;
}

int prepareMsg(struct msg_buf* message){
    int clientQID = findQID(message->sender_pid);
    if(clientQID == -1){
        printf("Client Not Found!\n");
        return -1;
    }

    message->mtype = message->sender_pid;
    message->sender_pid = getpid();

    return clientQID;
}

int findQID(pid_t client_pid){
    for(int i=0; i<MAX_CLIENTS; i++){
        if(clients[i][PID] == client_pid) return clients[i][QID];
    }
    return -1;
}

int getEmptyCID(){
    int i=0;
    while(i<MAX_CLIENTS && clients[i][ACTIVE]) i++;
    if(i==MAX_CLIENTS) return -1;
    clients[i][ACTIVE] = 1;
    return i;

}