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


#include "const.h"


// Types and structures

/**
 * Struct Message is used to as the default communication 
 * structure.
 * 
 * short producer_id - id of the producer that posted the msg.
 * short data - container for the msg.
 * int date - sent date of the msg.
 * int time - time the msg was sent.
 */
typedef struct Message
{
    short producer_id;
    short data;
    int date;
    int time;

} message_t;


/**
 * Struct Buffer is used to represent information needed.
 * 
 * short producers - number of producers using the buffer.
 * short consumers - number of consumers using the buffer.
 * short isActive - shared memory flag used to control system activity.
 * short available_slots - binary array that indexes available slots in 
 *                         the msg array, has size MAX_MSGS.
 * message_t msg - array that contains the msgs sent by the producers, has size MAX_MSGS.
 */
typedef struct Buffer
{
    short consumers;
    short producers;
    short isActive;
    short* available_slots;
    message_t* msg;

} buffer_t;



// Functions
/**
 * This functions generates an unique id.
 * 
 * Params:
 *     const char* s - string used to generate unique id.
 * 
 * Returns
 *     int uid.
 */
int generate_uid(const char* s) {
    int h = 0;
    const int size = sizeof(s) / sizeof(char);

    for (int i = 0; i < size; i++) {
        h = 31 * h + *(s + i);
    }

    return h;
}
/**
 * This function gets the id of a shred memory location
 * based on a buffer name
 *
 * Params:
 *      const char* buffer_name - string used to generate unique id.
 *
 * Returns
 *      int id of the shm.
 */
int get_shared_mem_id(const char* buffer_name){

    // Generate a key based on buffer name
    key_t key = (key_t) generate_uid(buffer_name);
    // Returns the shared memory identifier associated with key.
    // int shmget(key_t key, size_t size, int shmflg);
    // https://www.mkssoftware.com/docs/man3/shmget.3.asp
    int shmid = shmget(key, sizeof(buffer_t), buffer_flags | IPC_CREAT);
    // Check for errors
    if (shmid == -1) {
        printf("***ERROR: shmid is %d ***", shmid);
        perror("shmget failed"); 
        exit(EXIT_FAILURE);
    }

    return shmid;
}


/**
 * This function attaches to a shared memory segment and handles
 * attachment errors
 *
 * Params:
 *      int shmid - shared memory segment id.
 *
 * Returns
 *      buffer_t* shared memory buffer structure.
 */
buffer_t* attach_shm(int shmid){

    // Attaches the shared memory segment associated with the shared
    // memory identifier shmid to the data segment of the calling
    // process.
    // https://www.mkssoftware.com/docs/man3/shmat.3.asp
    buffer_t* buffer = (buffer_t*) shmat(shmid, NULL, 0);
    // Check for errors
    if (((int)buffer) == -1) {
        perror("shmat failed"); 
        exit(EXIT_FAILURE);
    }

    return buffer;
}

/**
 * This function sends a message through the buffer
 *
 * Params:
 *      message_t* msg - ptr to the message to be sent.
 *      buffer_t* buffer - ptr to the buffer that sends the msg
 * 
 * Returns
 *      int 0 for success 1 for error.
 */
int send_msg(message_t msg, buffer_t* buffer){

    int r = 1;
    short* isFull;

    for (size_t i = 0; i < MAX_MSGS; ++i)
    {
        isFull = buffer->available_slots + i;
        // Check if the slot is not full
        if(*isFull == FALSE){
            // Extract the ptr to the msg slot
            message_t* slt = (buffer->msg + i);
            // Assign the new msg
            *slt = msg;
            // Set the slot as full
            *isFull = TRUE;
            // Update the return value
            r = 0;
            // Stop the loop 
            break;
        }
    }

    return r;
}