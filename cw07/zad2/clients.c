#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <zconf.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "helper.h"

int status;
int shm_id;
sem_t *sem_id;
int shaves;

void close_up(){
    sem_close(sem_id);
    munmap(barbershop,sizeof(struct Barbershop));
}

void init() {
    shm_id = shm_open(SHM_NAME, O_RDWR, S_IRWXU | S_IRWXG);
    if (shm_id == -1) FAILURE_EXIT("Couldn't create shared memory\n");
    if(ftruncate(shm_id,sizeof(struct Barbershop))) FAILURE_EXIT("Failed truncating shared memory! \n");

    barbershop = mmap(NULL, sizeof(struct Barbershop), PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
    if (barbershop == (void*) -1) FAILURE_EXIT("Couldn't access shared memory\n");

    sem_id = sem_open(SEM_NAME, O_WRONLY, S_IRWXU | S_IRWXG, 0);
    if (sem_id == SEM_FAILED) FAILURE_EXIT("Couldn't create semaphore\n");
}

void claim_chair() {
    pid_t pid = getpid();

    if (status == INVITED) {
        pop_queue();
    } else if (status == NEWCOMER) {
        while (1) {
            release_semaphore(sem_id);
            getsem(sem_id);
            if (barbershop->barber_status == READY) break;
        }
        status = INVITED;
    }
    barbershop->selected_client = pid;
    printf("[%lo] client#%i took place in the chair\n", get_time(), pid);
}

void run_client() {
    pid_t pid = getpid();
    int cuts = 0;

    while (cuts < shaves) {
        status = NEWCOMER;

        getsem(sem_id);

        if (barbershop->barber_status == SLEEPING) {
            printf("[%lo] client#%i woke up the barber\n", get_time(), pid);
            barbershop->barber_status = AWOKEN;
            claim_chair();
            barbershop->barber_status = BUSY;
        } else if (!is_queue_full()) {
            enter_queue(pid);
            printf("[%lo] client#%i entered the queue\n", get_time(), pid);
        } else {
            printf("[%lo] client#%i couldn't find place in the queue\n", get_time(), pid);
            release_semaphore(sem_id);
            return;
        }

        release_semaphore(sem_id);

        while(status != INVITED) {
            getsem(sem_id);
            if (barbershop->selected_client == pid) {
                status = INVITED;
                claim_chair();
                barbershop->barber_status = BUSY;
            }
            release_semaphore(sem_id);
        }

        while(status != SHAVED) {
            getsem(sem_id);
            if (barbershop->selected_client != pid) {
                status = SHAVED;
                printf("[%lo] client#%i is shaved\n", get_time(), pid);
                barbershop->barber_status = IDLE;
                cuts++;
            }
            release_semaphore(sem_id);
        }
    }
    printf("[%lo] client#%i left barbershop after %i cuts\n", get_time(), pid, cuts);
}

int main(int argc, char** argv) {
    if (argc != 3) FAILURE_EXIT("Wrong number of arguments provided!\n");
    int clients_number = (int) strtol(argv[1], 0, 10);
    shaves = (int) strtol(argv[2], 0, 10);
    init();

    for (int i = 0; i < clients_number; ++i) {
        if (!fork()) {
            atexit(close_up);
            run_client();
            exit(0);
        }
    }
    while (wait(0)) if (errno = ECHILD) break;
}
