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

#include "utils.h"


 // Test data
char* buffer_name = "Ericks buffer\0";

int data_name_size;
char* data_name;

int buffer_size = 1024;
short flags = 0666;


int write_example() {
    // Generate a key based on buffer name
    key_t key = (key_t) generate_uid(buffer_name);

    // Returns the shared memory identifier associated with key.
    // int shmget(key_t key, size_t size, int shmflg);
    // https://www.mkssoftware.com/docs/man3/shmget.3.asp
    int shmid = shmget(key, sizeof(buffer_t), flags | IPC_CREAT);

    if (shmid == -1) {
        printf("Error: shmget\n");
        return -1;
    }

    // Attaches the shared memory segment associated with the shared
    // memory identifier shmid to the data segment of the calling
    // process.
    // https://www.mkssoftware.com/docs/man3/shmat.3.asp
    buffer_t* buffer = (buffer_t*) shmat(shmid, NULL, 0);

    if (buffer == (void *) -1) {
        printf("Error: Shared memory attach");
        return -1;
    }

    // Asking for shared data memory
    buffer->id_data = shmget(ftok(data_name, BUFFER_ID), buffer_size * sizeof(char), IPC_CREAT | flags);
    char* data = (char*) shmat(shmid, NULL, 0);

    // Ask for data
    printf("Write Data: ");
    gets(data);

    // Detach from shared memory
    shmdt(data);
    shmdt(buffer);

    return 0;
}


int read_example() {
    // Generate a key based on buffer name
    key_t key = (key_t) generate_uid(buffer_name);

    // Returns the shared memory identifier associated with key.
    // int shmget(key_t key, size_t size, int shmflg);
    // https://www.mkssoftware.com/docs/man3/shmget.3.asp
    int shmid = shmget(key, sizeof(buffer_t), flags | IPC_CREAT);

    if (shmid == -1) {
        printf("Error: shmget\n");
        return -1;
    }

    // Attaches the shared memory segment associated with the shared
    // memory identifier shmid to the data segment of the calling
    // process.
    // https://www.mkssoftware.com/docs/man3/shmat.3.asp
    buffer_t* buffer = (buffer_t*) shmat(shmid, NULL, 0);

    if (buffer == (void *) -1) {
      printf("Error: Shared memory attach");
      return -1;
    }

    char* data = (char*) shmat(shmid, NULL, 0);

    printf("Producers: %i\n", buffer->producers);
    printf("Consumers: %i\n", buffer->consumers);
    printf("Flags: %i\n", buffer->flags);
    printf("Data read from memory: %s\n", data);

    // Detach from shared memory
    shmdt(data);
    shmdt(buffer);

    return 0;
}

int main() {
    data_name_size = strlen(buffer_name) + 5;
    data_name = (char*) calloc(data_name_size, sizeof(char));
    strcpy(data_name, buffer_name);
    strcat(data_name, "_data\0");

    printf("Writing...\n");
    write_example();

    printf("\nReading...\n");
    read_example();

    printf("\n");

    return 0;
}
