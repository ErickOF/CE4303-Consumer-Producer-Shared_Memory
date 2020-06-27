#ifndef PROYECTO1_SEMAPHORES_H
#define PROYECTO1_SEMAPHORES_H


#include <stdlib.h>
#include <fcntl.h>
// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/semaphore.h.html
#include <semaphore.h>
#include "const.h"

sem_t* get_semaphores(){
    // Allocate memory
    // semaphores[0]: mutex, semaphores[1]: empty_spaces, semaphores[2]: available_msgs
    sem_t* semaphores = (sem_t*)malloc(3 * sizeof(sem_t));
    sem_t* aux;

    // Sets the mutual execution control, starts at 1 since its a binary sem
    semaphores = sem_open(mutex, O_CREAT, semaphore_flags, 1);
    // Producer control, wait for it to send msgs and post from
    // consumers to make spaces available, starts with MX_MSGS empty slots
    aux = (semaphores + 1);
    aux = sem_open(empty, O_CREAT, semaphore_flags, MAX_MSGS);
    // Consumer control, post from producers to allow consumer access
    // and wait from consumers to read, starts with 0 available msgs
    aux = (semaphores + 2);
    aux = sem_open(available, O_CREAT, semaphore_flags, 0);

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

    // Return the array of semaphores
    return semaphores;

}



#endif  // PROYECTO1_SEMAPHORES_H