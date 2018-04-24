#ifndef __SYSTEMV
#define __SYSTEMV

#define MIRROR  1
#define CALC    2
#define TIME    3
#define END     4
#define INIT    5
#define STOP    6
#define RPLY    7
#define ERR     8
#define UNDEF   9

#define ID_SEED 123

#define MAX_MSG 10
#define MAX_MSG_TXT 4096
#define MSG_SIZE     sizeof(struct msg_buf)-sizeof(long)
#define MAX_CLIENTS 10
#define SERVER_PATH "/server1"
typedef struct msg_buf 
{
    long mtype;
    char text[MAX_MSG_TXT];
    pid_t sender_pid;
}msg_buf;

#endif