
#ifndef __COMMON_H__
#define __COMMON_H__


#define FAILURE_EXIT(format, ...) {                                            \
    printf(format , ##__VA_ARGS__);                       \
    exit(-1);                                                                  }

#define SEM_NAME "semaph"
#define SHM_NAME "shared_memory" 

#define MAX_QUEUE_SIZE 64

#define SLEEPING 0
#define AWOKEN 1
#define READY 2
#define IDLE 3
#define BUSY 4


#define NEWCOMER 0
#define INVITED 1
#define SHAVED 2


struct Barbershop {
    int barber_status;
    int client_count;
    int waiting_room_size;
    pid_t selected_client;
    pid_t queue[MAX_QUEUE_SIZE];
} *barbershop;

long get_time() {
    struct timespec buffer;
    clock_gettime(CLOCK_MONOTONIC, &buffer);
    return buffer.tv_nsec / 1000;
}

void getsem(sem_t *sem_id) {

    if (sem_wait(sem_id)) // 1 goes for one operation
        FAILURE_EXIT("Could not update semaphore\n");
}

void release_semaphore(sem_t *sem_id) {
    if (sem_post(sem_id)) // 1 goes for one operation
        FAILURE_EXIT("Could not update semaphore\n");
}

int is_queue_full() {
    if (barbershop->client_count < barbershop->waiting_room_size) return 0;
    return 1;
}

int is_queue_empty() {
    if (barbershop->client_count == 0) return 1;
    return 0;
}

void enter_queue(pid_t pid) {
    barbershop->queue[barbershop->client_count] = pid;
    barbershop->client_count += 1;
}

void pop_queue() {
    for (int i = 0; i < barbershop->client_count - 1; ++i) {
        barbershop->queue[i] = barbershop->queue[i + 1];
    }

    barbershop->queue[barbershop->client_count - 1] = 0;
    barbershop->client_count -= 1;
}

#endif