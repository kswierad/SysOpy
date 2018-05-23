
#ifndef __COMMON_H__
#define __COMMON_H__


#define FAILURE_EXIT(format, ...) {                                            \
    printf(format , ##__VA_ARGS__);                       \
    exit(-1);                                                                  }

#define PROJECT_PATH getenv("HOME")
#define PROJECT_ID 1235 

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

void getsem(int semaphore_id) {
    struct sembuf semaphore_request;
    semaphore_request.sem_num = 0;
    semaphore_request.sem_op = -1;
    semaphore_request.sem_flg = 0;

    if (semop(semaphore_id, &semaphore_request, 1)) // 1 goes for one operation
        FAILURE_EXIT("Could not update semaphore\n");
}

void release_semaphore(int semaphore_id) {
    struct sembuf semaphore_request;
    semaphore_request.sem_num = 0;
    semaphore_request.sem_op = 1;
    semaphore_request.sem_flg = 0;

    if (semop(semaphore_id, &semaphore_request, 1)) // 1 goes for one operation
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