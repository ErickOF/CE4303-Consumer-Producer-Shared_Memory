
#include <string.h>

#include "lib/shared_memory.h"


int main() {
    // Test data
    char* buffer_name = "Ericks buffer\0";

    int data_name_size = strlen(buffer_name) + 5;
    char* data_name = (char*) calloc(data_name_size, sizeof(char));
    strcpy(data_name, buffer_name);
    strcat(data_name, "_data\0");

    int buffer_size = 1024;
    short flags = 0666;

    // Generate a key based on buffer name
    key_t key = (key_t) generate_uid(buffer_name);

    // Returns the shared memory identifier associated with key.
    // int shmget(key_t key, size_t size, int shmflg);
    // https://www.mkssoftware.com/docs/man3/shmget.3.asp
    int shmid = shmget(key, sizeof(buffer_t), flags | IPC_CREAT);

    if (shmid == -1) {
        printf("Error: shmget\n");
        exit(1);
    }

    // Attaches the shared memory segment associated with the shared
    // memory identifier shmid to the data segment of the calling
    // process.
    // https://www.mkssoftware.com/docs/man3/shmat.3.asp
    buffer_t* buffer = (buffer_t*) shmat(shmid, NULL, 0);

    if (buffer == (void *) -1) {
        printf("Error: Shared memory attach");
        exit(1);
    }

    printf("Key: %i, ShmID %i\n", key, shmid);

    // Set values
    // Initialize consumer and producer id values
    buffer->consumers = 0;
    buffer->producers = 0;
    // Set system active
    buffer->isActive = 1;
    // Initialize msg related arrays
    buffer->available_slots = (short*) calloc(MAX_MSGS, sizeof(short));
    buffer->msg = (message_t*)malloc(MAX_MSGS * sizeof(message_t));


    printf("Buffer created: %s\n\n", buffer_name);

    // Detach from shared memory
    shmdt(buffer);

    // Free memory after be used
    free(data_name);

    return 0;
}
