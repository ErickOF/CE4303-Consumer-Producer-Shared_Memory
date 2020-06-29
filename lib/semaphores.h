#ifndef PROYECTO1_SEMAPHORES_H
#define PROYECTO1_SEMAPHORES_H


#include <stdlib.h>
#include <fcntl.h>
// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/semaphore.h.html
#include <semaphore.h>
#include "const.h"


void get_semaphores(sem_t* semaphores, char* buffer_name)
{    
    // Allocate memory to make names uniques to each buffer
    char * s0 = (char *) malloc(1 + strlen(mutex) + BN_LEN);
    char * s1 = (char *) malloc(1 + strlen(empty) + BN_LEN);
    char * s2 = (char *) malloc(1 + strlen(available) + BN_LEN);

    // Concat the names
    strcpy(s0, buffer_name);
    strcat(s0, mutex);
    strcpy(s1, buffer_name);
    strcat(s1, empty);
    strcpy(s2, buffer_name);
    strcat(s2, available);

    // Sets the mutual execution control, starts at 1 since its a binary sem
    semaphores[0] = *sem_open(s0, O_CREAT, semaphore_flags, 1);
    // Producer control, wait for it to send msgs and post from
    // consumers to make spaces available, starts with MX_MSGS empty slots
    semaphores[1] = *sem_open(s1, O_CREAT, semaphore_flags, MAX_MSGS);
    // Consumer control, post from producers to allow consumer access
    // and wait from consumers to read, starts with 0 available msgs
    semaphores[2] = *sem_open(s2, O_CREAT, semaphore_flags, 0);

    // Error handling
    if (semaphores == SEM_FAILED)
    {
        sem_close(semaphores);
        sem_close(semaphores + 1);
        sem_close(semaphores + 2);
        perror("Failed to open semphore for mutex");
        exit(EXIT_FAILURE);
    }

    if ((semaphores + 1) == SEM_FAILED)
    {
        sem_close(semaphores);
        sem_close(semaphores + 1);
        sem_close(semaphores + 2);
        perror("Failed to open semphore for empty");
        exit(EXIT_FAILURE);
    }

    if ((semaphores + 2) == SEM_FAILED)
    {
        sem_close(semaphores);
        sem_close(semaphores + 1);
        sem_close(semaphores + 2);
        perror("Failed to open semphore for available");
        exit(EXIT_FAILURE);
    }

    // Free the memory 
    free(s0);
    free(s1);
    free(s2);
}



#endif  // PROYECTO1_SEMAPHORES_H
