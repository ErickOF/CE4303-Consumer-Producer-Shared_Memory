// The <sys/ipc.h> header is used by three mechanisms for interprocess
// communication (IPC): messages, semaphores and shared memory. All use
// a common structure type, ipc_perm to pass information used in
// determining permission to perform an IPC operation.
// https://pubs.opengroup.org/onlinepubs/007908799/xsh/sysipc.h.html
#include <sys/ipc.h>
// XSI shared memory facility.
// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/shm.h.html
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib/shared_memory.h"
#include "lib/argument_parser.h"
#include "lib/const.h"
#include "lib/datetime.h"


int main(int argc, char *argv[])
{
    int shmid = *parse_killer(argc, argv);

    // Attaches the shared memory segment associated with the shared
    // memory identifier shmid to the data segment of the calling
    // process.
    // https://www.mkssoftware.com/docs/man3/shmat.3.asp
    buffer_t* buffer = attach_shm(shmid);


    // Acces number of producers
    sem_wait(buffer->semaphores);
    // Send the kill signal 
    buffer->isActive = FALSE;

    // Post in case there are consumers waiting 
    for (size_t i = 0; i < buffer->consumers; i++)
    {
        sem_post(buffer->semaphores + 2);
    }

    // Post in case there are producers waiting 
    for (size_t i = 0; i < buffer->producers; i++)
    {
        sem_post(buffer->semaphores + 1);
    }

    // Allocate memory to make names uniques to each buffer
    char * s0 = (char *) malloc(1 + strlen(mutex) + BN_LEN);
    char * s1 = (char *) malloc(1 + strlen(empty) + BN_LEN);
    char * s2 = (char *) malloc(1 + strlen(available) + BN_LEN);

    // Concat the names
    strcpy(s0, buffer->name);
    strcat(s0, mutex);
    strcpy(s1, buffer->name);
    strcat(s1, empty);
    strcpy(s2, buffer->name);
    strcat(s2, available);
    
    // Free the mutex
    sem_post(buffer->semaphores);

	sleep(5);


    // Kill semaphores
    sem_close(buffer->semaphores);
    sem_close(buffer->semaphores + 1);
    sem_close(buffer->semaphores + 2);
    sem_unlink(s0);
    sem_unlink(s1);
    sem_unlink(s2);

    // Free the memory 
    free(s0);
    free(s1);
    free(s2);

    // Unread messages
    int unread = 0;

    for (int i = 0; i < MAX_MSGS; i++)
    {
        unread += buffer->available_slots[i];
    }
    

    // Statistics
    printf("\033[1m");
    printf("-----------------------------------------------------------\n");
    printf("-------------------- Buffer was deleted -------------------\n");
    printf("-----------------------------------------------------------\n");

    printf("Mensajes totales: %d\n", buffer->total_msgs);
    printf("Mensajes en el buffer: %d\n", unread);
    printf("Total de productores: %d\n", buffer->total_producers);
    printf("Total de consumidores: %d\n", buffer->total_consumers);
    printf("Consumidores eliminados por llave:\n");

    for (int i = 0; i < 6; i++)
    {
        printf("----> Llave [%d]: %d\n", i, buffer->deleted_consumers[i]);
    }

    printf("Tiempo total esperando: %f\n", buffer->total_waiting_time);
    printf("Tiempo total bloqueado: %f\n", buffer->total_locked_time);
    printf("Tiempo total de usuario: %f\n", buffer->total_user_time);
    printf("Tiempo total en kernel: %f\n", buffer->total_kernel_time);

    printf("-----------------------------------------------------------\n");
    printf("-----------------------------------------------------------\n");
    printf("-----------------------------------------------------------\n");
    printf("\033[0m");

    // Detach and destroy shared memory for buffer
    shmdt(buffer);
    shmctl(shmid, IPC_RMID, NULL);

    return 0; 
}
