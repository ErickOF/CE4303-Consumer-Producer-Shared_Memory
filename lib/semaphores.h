#ifndef PROYECTO1_SEMAPHORES_H
#define PROYECTO1_SEMAPHORES_H


#include <stdlib.h>
#include <fcntl.h>
// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/semaphore.h.html
#include <semaphore.h>
#include "const.h"


void get_semaphores(sem_t* semaphores){
    // Allocate memory
    // semaphores[0]: mutex, semaphores[1]: empty_spaces, semaphores[2]: available_msgs
    //semaphores = (sem_t*)malloc(3 * sizeof(sem_t));

    // Sets the mutual execution control, starts at 1 since its a binary sem
    semaphores[0] = *sem_open(mutex, O_CREAT, semaphore_flags, 1);
    
    // Producer control, wait for it to send msgs and post from
    // consumers to make spaces available, starts with MX_MSGS empty slots
    semaphores[1] = *sem_open(empty, O_CREAT, semaphore_flags, MAX_MSGS);
    // Consumer control, post from producers to allow consumer access
    // and wait from consumers to read, starts with 0 available msgs
    semaphores[2] = *sem_open(available, O_CREAT, semaphore_flags, 0);

    // Error handling
    if (semaphores == SEM_FAILED) {
        sem_close(semaphores);
        sem_close(semaphores + 1);
        sem_close(semaphores + 2);
        perror("Failed to open semphore for mutex");
        exit(EXIT_FAILURE);
    }
    if ((semaphores + 1) == SEM_FAILED) {
        sem_close(semaphores);
        sem_close(semaphores + 1);
        sem_close(semaphores + 2);
        perror("Failed to open semphore for empty");
        exit(EXIT_FAILURE);
    }
    if ((semaphores + 2) == SEM_FAILED) {
        sem_close(semaphores);
        sem_close(semaphores + 1);
        sem_close(semaphores + 2);
        perror("Failed to open semphore for available");
        exit(EXIT_FAILURE);
    }

}


int isValidSemaphore(sem_t* semaphores){

    int a = -8;
    int b = -8;
    int c = -8;
    int e;
    e = sem_getvalue(semaphores, &a);
    if(e ==-1){
        perror("Error on sem 0");
    }
    printf("sem 0 ptr %p and value %i\n", semaphores, a);
    e = sem_getvalue(semaphores + 1 , &b);
    if(e ==-1){
        perror("Error on sem 0");
    }
    printf("sem 1 ptr %p and value %i\n", semaphores + 1, b);
    e = sem_getvalue(semaphores + 2, &c);
    if(e ==-1){
        perror("Error on sem 0");
    }
    printf("sem 2 ptr %p and value %i\n", semaphores + 2, c);

    return a + b + c;
}





#endif  // PROYECTO1_SEMAPHORES_H