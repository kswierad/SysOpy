#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <mqueue.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

#include "POSIX.h"

#define FAILURE_EXIT(code, format, ...) { fprintf(stderr, format, ##__VA_ARGS__);printf("\n"); exit(code);}

#define PID 0
#define QID 1
#define ACTIVE 2

mqd_t privateID;
char* privatePath;
int CID;
mqd_t publicID;

int quit = 0;
int clients[MAX_CLIENTS][3];


void register_client(char* privatePath);

void rq_mirror(msg_buf* message);

void rq_calc(msg_buf* message);
void rq_end(msg_buf* message);

void rq_time(msg_buf* message);
void rq_stop(msg_buf* message);
/*void intHandler(int signum){
    for(int i=0; i < MAX_CLIENTS; i++){
        if(clients[i][ACTIVE]){
            kill(clients[i][PID],SIGINT);
        }
    }
    exit(2);
    
}*/
void deleteQueue(){
    msg_buf message;
    rq_stop(&message);
    mq_close(publicID);
    mq_close(privateID);
    mq_unlink(privatePath);
    printf("Deleted private queue. \n");
    
    
    
}

void intHandler(int signo){
    exit(2);
}

char* generate_path(){
    FILE* random_handle = fopen("/dev/urandom","r");
    unsigned char *result = calloc(15,sizeof(char));
    fread(result,sizeof(unsigned char),15,random_handle);
    for (int j = 0; j < 15; j++){
        result[j] = result[j] % 52;
        if (result[j] < 26) result[j] = result[j] + 65;
        else result[j] = result[j] + 71;
    }
    result[0] = '/';
    result[14] = 0;
    fclose(random_handle);
    return result;
}

int main(){
    if(atexit(deleteQueue) == -1) FAILURE_EXIT(3, "Registering server's atexit failed!");
    if(signal(SIGINT, intHandler) == SIG_ERR) FAILURE_EXIT(3, "Registering INT failed!");

    privatePath = generate_path();
    struct mq_attr posixAttr;
    posixAttr.mq_maxmsg = MAX_MSG;
    posixAttr.mq_msgsize = MSG_SIZE; 
    printf("Connecting with queue name: %s \n",privatePath);
    printf("with pid: %d \n",getpid());
    privateID = mq_open(privatePath,O_RDONLY | O_CREAT | O_EXCL, 0666, &posixAttr);
    if(privateID == -1) FAILURE_EXIT(3, "Creation of private queue failed!");

    publicID = mq_open(SERVER_PATH, O_WRONLY);
    if(publicID == -1) FAILURE_EXIT(3, "Creation of public queue failed!");
    
    register_client(privatePath);
    
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

void register_client(char* privatePath){
    msg_buf message;
    message.mtype = INIT;
    message.sender_pid = getpid();
    sprintf(message.text, "%s", privatePath);

    if(mq_send(publicID,(char*) &message, MSG_SIZE, 1) == -1) FAILURE_EXIT(3, "INIT request failed!");
    if(mq_receive(privateID, (char*) &message, MSG_SIZE, NULL) == -1) FAILURE_EXIT(3, "catching LOGIN response failed!");
    if(sscanf(message.text, "%d", &CID) < 1) FAILURE_EXIT(3, "scanning INIT response failed!");
    if(CID < 0) FAILURE_EXIT(3, "Server cannot have more clients!");
}

void rq_mirror(msg_buf* message){
    
    message->mtype = MIRROR;
    message->sender_pid = getpid();
    printf("Enter string of characters to mirror: ");
    if(fgets(message->text, MAX_MSG_TXT, stdin) == NULL){
        printf("Too many characters!\n");
        return;
    }
    if(mq_send(publicID, (char*) message, MSG_SIZE, 1) == -1) FAILURE_EXIT(3, "MIRROR request failed!");
    if(mq_receive(privateID,(char*) message, MSG_SIZE, NULL) == -1) FAILURE_EXIT(3, "catching MIRROR response failed!");
    printf("%s", message->text);
}

void rq_calc(msg_buf* message){
    message->mtype = CALC;
    message->sender_pid = getpid();
    printf("Enter an expression to calculate: ");
    if(fgets(message->text, MAX_MSG_TXT, stdin) == NULL){
        printf("Too many characters!\n");
        return;
    }
    if(mq_send(publicID,(char*) message, MSG_SIZE, 1) == -1) FAILURE_EXIT(3, "CALC request failed!");
    if(mq_receive(privateID,(char*) message, MSG_SIZE, NULL) == -1) FAILURE_EXIT(3, "catching CALC response failed!");
    printf("%s", message->text);
}

void rq_end(msg_buf* message){
    message->mtype = END;
    message->sender_pid = getpid();
    if(mq_send(publicID,(char*) message, MSG_SIZE, 1) == -1) FAILURE_EXIT(3, "END request failed!");
    exit(2);
}

void rq_stop(msg_buf* message){
    message->mtype = STOP;
    message->sender_pid = getpid();
    if(mq_send(publicID,(char*) message, MSG_SIZE, 1) == -1) FAILURE_EXIT(3, "STOP request failed!");
}

void rq_time(msg_buf* message){
    message->mtype = TIME;
    message->sender_pid = getpid();

    if(mq_send(publicID,(char*) message, MSG_SIZE, 1) == -1) FAILURE_EXIT(3, "TIME request failed!");
    if(mq_receive(privateID,(char*) message, MSG_SIZE, NULL) == -1) FAILURE_EXIT(3, "catching TIME response failed!");
    printf("%s", message->text);
}