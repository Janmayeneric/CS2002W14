#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>

#define MAX_CYCLES 1000
#define BUFFER_SIZE 10
#define EMPTY -1

int buffer[BUFFER_SIZE];
sem_t *mutex, *num_elements, *num_free;


void delete_semaphores() {
    sem_unlink("mutex");
    sem_unlink("num_elements");
    sem_unlink("num_free");
}


void cleanup() {
    if (mutex) sem_close(mutex);
    if (num_elements) sem_close(num_elements);
    if (num_free) sem_close(num_free);
    delete_semaphores();
}


void exit_err(char *msg){
    perror(msg);
    fprintf(stderr, "errno = %i\n", errno);
    cleanup();
    exit(EXIT_FAILURE);
}


void *consumer(void *arg){
    int cycles = 0;
    int total = 0;
    while (cycles++ < MAX_CYCLES) {
        sem_wait(num_elements); // wait/block until elements are available and then decrement elements
        sem_wait(mutex);        // lock mutex (implemented here using semaphore)

        //pick a random element until we find a full one
        int index;
        do {
            index = rand() % BUFFER_SIZE;
        } while (buffer[index] == EMPTY);
        total += buffer[index];
        printf("consumer gets: %i\n", buffer[index]);
        buffer[index] = EMPTY;

        sem_post(mutex);        // unlock mutex
        sem_post(num_free);     // increment number of free spaces in buffer - will wake up a producer who could be waiting
    }
    printf("consumer got total: %i\n", total);
    return NULL;
}

void *producer(void *arg){
    int cycles = 0;
    while (cycles++ < MAX_CYCLES) {
        sem_wait(num_free);     // wait/block until there is space in buffer (i.e. num_free > 0) and then decrement free space
        sem_wait(mutex);        // lock mutex (implemented here using semaphore)

        int index = 0;
        while (buffer[index] != EMPTY) { //we know one of the elements is empty
            index++;
        }
        buffer[index] = cycles;
        printf("producer puts: %i\n", buffer[index]);
        sem_post(mutex);        // unlock mutex
        sem_post(num_elements); // increment number of elements in buffer - will wake up a consumer who could be waiting
    }
    return NULL;
}

int main(int argc, char *argv[]){
    pthread_t cons_thread, prod_thread;

    if (argc == 2 && strcmp(argv[1], "cleanup") == 0) {
        delete_semaphores();
        return 0;
    }

    for (int i = 0; i < BUFFER_SIZE; i++) buffer[i] = EMPTY;

    if ((mutex = sem_open("mutex", O_CREAT, 0600, 1)) == SEM_FAILED) exit_err("mutex");
    if ((num_elements = sem_open("num_elements", O_CREAT, 0600, 0)) == SEM_FAILED) exit_err("num_elements");
    if ((num_free = sem_open("num_free", O_CREAT, 0600, BUFFER_SIZE)) == SEM_FAILED) exit_err("num_free");

    pthread_create(&cons_thread, NULL, consumer, NULL);
    pthread_create(&prod_thread, NULL, producer, NULL);

    pthread_join(cons_thread, NULL);
    pthread_join(prod_thread, NULL);

    cleanup();

    return 0;
}

