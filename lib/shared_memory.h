#ifndef PROYECTO1_SHARED_MEMORY_H
#define PROYECTO1_SHARED_MEMORY_H

#include <errno.h>
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
#include <time.h>

#include "const.h"
#include "semaphores.h"


extern int errno;

// Types and structures

/**
 * Struct Message is used to as the default communication 
 * structure.
 * 
 * short producer_id - id of the producer that posted the msg.
 * int data - container for the msg.
 * int date - sent date of the msg.
 * int time - time the msg was sent.
 */
typedef struct Message
{
    short producer_id;
    int data;
    int date;
    int time;
} message_t;


/**
 * Struct Buffer is used to represent information needed.
 * 
 * short producers - number of producers using the buffer.
 * short consumers - number of consumers using the buffer.
 * short next_consumer - turn counter for consumer msg reception
 * short isActive - shared memory flag used to control system activity.
 * short available_slots - binary array that indexes available slots in 
 *                         the msg array, has size MAX_MSGS.
 * message_t msg - array that contains the msgs sent by the producers, has size MAX_MSGS.
 */
typedef struct Buffer
{
    short consumers;
    short producers;
    short next_consumer;
    short isActive;
    short* available_slots;
    message_t* msg;
} buffer_t;


// Functions
/**
 * This function return current datetime.
 * 
 * Params:
 *     char date - var to store datetime.
 */
void getDate(char date[25]){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(date, 24, "%Y-%m-%d %H:%M:%S", tm);
}

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
int get_shared_mem_id(const char* buffer_name, int flag){
    // Generate a key based on buffer name
    key_t key = (key_t) generate_uid(buffer_name);
    // Returns the shared memory identifier associated with key.
    // int shmget(key_t key, size_t size, int shmflg);
    // https://www.mkssoftware.com/docs/man3/shmget.3.asp
    int shmid = shmget(key, sizeof(buffer_t), buffer_flags | flag);

    // Check for errors
    if (errno == EEXIST) {
        printf("Error: Buffer '%s' already exist.\n", buffer_name);
        exit(1);
    } else if (shmid == -1) {
        printf("Error: Buffer '%s' doesn't exist.\n", buffer_name);
        exit(1);
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
void attach_shm(int shmid, buffer_t* buffer){
    // Attaches the shared memory segment associated with the shared
    // memory identifier shmid to the data segment of the calling
    // process.
    // https://www.mkssoftware.com/docs/man3/shmat.3.asp
    *buffer = *(buffer_t*) shmat(shmid, NULL, 0);

    // Check for errors
    if ((*(int*)buffer) == -1) {
        printf("Buffer not found.\n"); 
        exit(EXIT_FAILURE);
    }
}


/**
 * This function encapsulates memory attachment, semaphore generation and
 * id extraction for consumers and producers
 *
 * Params:
 *      int shmid - shared memory buffer id to be attached
 *      int isProducer - boolean that defines the kind of client
 *      buffer_t* buffer - ptr to the buffer msg buffer
 *      sem_t* semaphores - ptr to the needed semaphores
 *      short* client_id - ptr to the client id
 */
void start_client(int shmid, int isProducer, buffer_t* buffer, sem_t* semaphores, short* client_id){
    // Attach the shared memory segment 
    attach_shm(shmid, buffer);
    // Get the semaphores used to access the data
    // semaphores[0]: mutex, 
    // semaphores[1]: empty_spaces, 
    // semaphores[2]: available_msgs
    get_semaphores(semaphores);

    // Wait for acces to get the client id
    sem_wait(semaphores);

    // Check if we have consumer or producer
    if (isProducer) {
        // if producer store the id and increase the number of 
        // connected producers
        printf("%d\n", buffer->producers);
        *client_id = (buffer->producers)++;
        printf("%d\n", buffer->producers);
    } else {
        // if consumer store the id and increase the number of 
        // connected consumers
        *client_id = (buffer->consumers)++;
    }

    // Free the mutex
    sem_post(semaphores);

}

/**
 * This function search for a defined value in the available slots array
 *
 * Params:
 *      buffer_t* buffer - ptr to the buffer to analize
 *      int target_value - value we look for in the available slots array
 * 
 */
int search_target(buffer_t* buffer, int target_value){

    // Index of the msg
    int i;    

    // Check availability of all slots
    for (i = 0; i < MAX_MSGS; ++i)
    {
        // Check if the slot is equal to the target
        // TRUE is full and FALSE is empty
        if(buffer->available_slots[i] == target_value){
            return i;
        }
    }

    return -1;

}


/**
 * This function sends a message through the buffer
 *
 * Params:
 *      message_t* msg - ptr to the message to be sent.
 *      buffer_t* buffer - ptr to the buffer that sends the msg
 * 
 */
void send_msg(message_t msg, buffer_t* buffer){
    // Search for an empty space in the available slots
    int idx = search_target(buffer, FALSE);

    // Check for errors
    if (idx == -1) {
        perror("send msg failed");
        exit(EXIT_FAILURE);
    }  
    // If everything is ok then send the message and print the success notification
    else {

        // Assign the new msg
        buffer->msg[idx] = msg;

        // Print notifications
        printf("-----------------------------------------------------------\n");
        printf("Mensaje colocado en la posicion %d satisfactoriamente\n", idx);
        printf("Productores activos: %d", buffer->producers);
        printf("Consumidores activos: %d", buffer->consumers);
        printf("-----------------------------------------------------------\n");
    }
}


int receive_msg(buffer_t* buffer){
    // msg
    message_t* message;
    // Search for an empty space in the available slots
    int idx = search_target(buffer, FALSE);

    // Check for errors
    if (idx == -1) {
        perror("msg reception failed");
        exit(EXIT_FAILURE);
    }  
    // If everything is ok then send the message and print the success notification
    else {

        // Extract the ptr to the msg slot
        message = (buffer->msg + idx);
        buffer->next_consumer = (++buffer->next_consumer) % buffer->consumers;

        // Print notifications
        printf("-----------------------------------------------------------\n");
        printf("Mensaje recibido en la posicion %d con un valor de %d\n", idx, message->data);
        printf("Productores activos: %d", buffer->producers);
        printf("Consumidores activos: %d", buffer->consumers);
        printf("-----------------------------------------------------------\n");

    }

    return message->data;
}

#endif  // PROYECTO1_SHARED_MEMORY_H
