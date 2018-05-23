#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <zconf.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "helper.h"


int shm_id;
sem_t *sem_id;

void handle_signal(int _) {
    printf("Closing barbershop\n");
    exit(0);
}

void close_up(){
        munmap(barbershop,sizeof(struct Barbershop));
        if(sem_id){
            sem_close(sem_id);
            sem_unlink(SEM_NAME);
        }

        if(shm_id){
            shm_unlink(SHM_NAME);
        }
}

void invite_client() {
    pid_t client_pid = barbershop->queue[0];
    barbershop->selected_client = client_pid;
    printf("[%lo] barber invited client %i\n", get_time(), client_pid);
}

void serve_client() {
    printf("[%lo] barber started shaving client %i\n", get_time(), barbershop->selected_client);

    printf("[%lo] barber finished shaving client %i\n",get_time(), barbershop->selected_client);

    barbershop->selected_client = 0;
}

void start_up(int N){

    shm_id = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG);
    if(shm_id == -1) FAILURE_EXIT("Failed creating shared memory! \n");
    if(ftruncate(shm_id,sizeof(struct Barbershop))) FAILURE_EXIT("Failed truncating shared memory! \n");
    barbershop = mmap(NULL,sizeof(struct Barbershop),PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
    if(barbershop == (void *) -1) FAILURE_EXIT("Failed getting shared memory's addres! \n");

    sem_id = sem_open(SEM_NAME,O_WRONLY | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG, 0);
    if(sem_id == SEM_FAILED) FAILURE_EXIT("Failed creating semaphore! \n");

    printf("Barber Fell asleep. \n");
    barbershop->barber_status = SLEEPING;
    barbershop->waiting_room_size = N;
    barbershop->client_count = 0;
    barbershop->selected_client = 0;

    for(int i=0;i<MAX_QUEUE_SIZE;i++) barbershop->queue[i]=0;
}

int main(int argc, char* argv[]){
    
    signal(SIGTERM,handle_signal);
    signal(SIGKILL,handle_signal);
    signal(SIGINT,handle_signal);
    atexit(close_up);

    if(argc!= 2) FAILURE_EXIT("Wrong number of arguments! \n");
    int number_of_seats = strtol(argv[1],0,10);
    if(number_of_seats > MAX_QUEUE_SIZE) FAILURE_EXIT("Exceeded max number of seats! \n");

    start_up(number_of_seats);

    release_semaphore(sem_id);

    while(1){
        getsem(sem_id);

        switch(barbershop->barber_status){
            case IDLE:
                if(is_queue_empty()){
                    printf("[%lo] barber fell asleep \n ",get_time());
                    barbershop->barber_status=SLEEPING;
                } else {
                    invite_client();
                    barbershop->barber_status=READY;
                }
                break;
            case AWOKEN:
                printf("[%lo] barber woke up \n", get_time());
                barbershop->barber_status=READY;
                break;
            case BUSY:
                serve_client();
                barbershop->barber_status=READY;
                break;
            default:
                break;
        }

        release_semaphore(sem_id);
    }


}