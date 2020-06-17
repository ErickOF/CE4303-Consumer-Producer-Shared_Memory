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

#include "utils.h"


int main() {
    // Test data
    char* name = "README.md\0";
    short buffer_size = 1024;
    short flags = 0644;

    // Returns a key based on path and id.
    // https://www.mkssoftware.com/docs/man3/ftok.3.asp#:~:text=The%20ftok()%20function%20returns,system%2C%20it%20returns%20different%20keys.
    key_t key;

    if ((key = ftok(name, BUFFER_ID)) == (key_t) -1) {
        printf("IPC error: ftok\n");
        exit(1);
    }

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

    char* data = (char*) shmat(shmid, NULL, 0);

    // Detach and destroy shared memory for data
    shmdt(data);
    shmctl(buffer->id_data, IPC_RMID, NULL);

    // Detach and destroy shared memory for data
    shmdt(buffer);
    shmctl(shmid, IPC_RMID, NULL);
     
    return 0; 
} 