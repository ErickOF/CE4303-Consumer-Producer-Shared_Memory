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

#include "constants.h"


int main() {
    // Test data
    char *name = "test buffer";
    int buffer_size = 1024;
    int flags = 0666;

    // Returns a key based on path and id.
    // https://www.mkssoftware.com/docs/man3/ftok.3.asp#:~:text=The%20ftok()%20function%20returns,system%2C%20it%20returns%20different%20keys.
    key_t key = ftok(name, BUFFER_ID);

    // Returns the shared memory identifier associated with key.
    // int shmget(key_t key, size_t size, int shmflg);
    // https://www.mkssoftware.com/docs/man3/shmget.3.asp
    int shmid = shmget(key, buffer_size, flags | IPC_CREAT);

    // Attaches the shared memory segment associated with the shared
    // memory identifier shmid to the data segment of the calling
    // process.
    // https://www.mkssoftware.com/docs/man3/shmat.3.asp
    char *str = (char*) shmat(shmid, (void*) 0, 0);
  
    printf("Data read from memory: %s\n", str); 
      
    //detach from shared memory  
    shmdt(str); 
    
    // destroy the shared memory 
    shmctl(shmid, IPC_RMID, NULL); 
     
    return 0; 
} 