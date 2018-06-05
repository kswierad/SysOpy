#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <semaphore.h>



sem_t buffer_sem;
sem_t empty_buffer_sem;
sem_t full_buffer_sem;
pthread_t *prods;
pthread_t *cons;

int P;
int K;
int N;
char *src_file = NULL;
FILE *file_handle;
int L;
int operand;
int mode;
int nk;
char **buffer;
int in_buffer = 0;
int last_read = 0;
int last_written = 0;
#define MAX_LINE_SIZE 512
#define FAILURE_EXIT(format, ...) {                                            \
    printf(format , ##__VA_ARGS__);                       \
    exit(-1);                                                                  }



void *consumer(void *argument){
    while(1) {
        sem_wait(&empty_buffer_sem);
        sem_wait(&buffer_sem);

        char *line = malloc(sizeof(char)*MAX_LINE_SIZE);
        strcpy(line,buffer[last_read]);
       
        

        if(mode == 1) {
            printf("Consumer gets line from %i, is the %i line in buff. \n", last_read, in_buffer);
        }

        if (line[strlen(line)-1] == '\n') line[strlen(line)-1] = '\0';
        int flag;
        switch (operand){
            case 0:     flag = (strlen(line) == L); break;
            case 1:     flag = (strlen(line) > L);  break;
            case -1:    flag = (strlen(line) < L);  break;
        }
        if(flag) printf("Index: %i, line: %s \n", last_read, line);
        if(line) free(line);
        if(buffer[last_read]) free(buffer[last_read]);
        last_read = (last_read + 1) % N;
        in_buffer--;
        sem_post(&full_buffer_sem);
        sem_post(&buffer_sem);
        //printf("cons2\n");
    }
}

void *producer(void *argument){
    size_t line_size = 0;
    char* line = NULL;
    while(1){
        sem_wait(&full_buffer_sem);
        sem_wait(&buffer_sem);

        if(getline(&line,&line_size,file_handle) <= 0){
            sem_post(&buffer_sem);
            printf("Stopped reading file! \n");
            break;
        }
        //printf("%s \n",line);
        buffer[last_written] = malloc(sizeof(char)*line_size);
        strcpy(buffer[last_written],line);
        if(line) {
            free(line);
            line = NULL;
            
        }
        
        if(mode == 1) {
            printf("Producer puts line at %i, of length %li, is the %i line in buff. \n", last_written, line_size, in_buffer);
            printf("Contents:\n %s",buffer[last_written]);
        }
        last_written = (last_written+1)%N;
        in_buffer++;
        sem_post(&empty_buffer_sem);
        sem_post(&buffer_sem);
    }
}

void close_up(){
    for(int i=0; i < K; i++) pthread_cancel(cons[i]);
    for(int i=0; i < P; i++) pthread_cancel(prods[i]);
    fclose(file_handle);
    if(buffer) free(buffer);
    sem_destroy(&buffer_sem);
    sem_destroy(&full_buffer_sem);
    sem_destroy(&empty_buffer_sem);
}

void sig_func(int signum){
    if(signum == SIGINT){
        printf("Caught SIGINT \n");
    }
    exit(0);
}



int main(int argc, char* argv[]){

    atexit(&close_up);
    signal(SIGINT,sig_func);
    signal(SIGALRM,sig_func);
    src_file = malloc(sizeof(char)*MAX_LINE_SIZE);
    if(argc < 2) FAILURE_EXIT("Not enough arguments!\n");

    FILE *conf_file = fopen(argv[1],"r");
    if(!conf_file) FAILURE_EXIT("Couldn't open configuration file!\n");
    if(fscanf(conf_file,"%d %d %d %s %d %d %d %d", &P, &K, &N, src_file, &L, &operand, &mode, &nk) < 0) 
        FAILURE_EXIT("Couldn't read from configuration file!\n");
    printf("%d %d %d %s %d %d %d %d \n", P, K, N, src_file, L, operand, mode, nk);
    fclose(conf_file);

    sem_init(&buffer_sem, 0, 1);
    sem_init(&full_buffer_sem, 0, N);
    sem_init(&empty_buffer_sem, 0, 0);
    
    prods = malloc(sizeof(pthread_t)*P);
    cons = malloc(sizeof(pthread_t)*K);
    if(nk>0) alarm(nk);
    file_handle = fopen(src_file,"r");
    free(src_file);
    buffer = malloc(sizeof(char*)*N);
    for(int i = 0; i < N; i++) buffer[i] = NULL;

    for(int i = 0; i < P; ++i) {
        pthread_create(&prods[i], NULL , &producer, NULL);
    }

    for(int i = 0; i < K; ++i) {
        pthread_create(&cons[i], NULL , &consumer, NULL);
    }

    for(int i = 0; i < P; i++) pthread_join(prods[i], NULL);    

    while (1){
        sem_wait(&buffer_sem);
        if (in_buffer == 0) break;
        sem_post(&buffer_sem);
    }
    while(nk);

    return 0;
}