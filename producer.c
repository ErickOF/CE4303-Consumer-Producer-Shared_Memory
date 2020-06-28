#ifndef PROYECTO1_PRODUCER_H
#define PROYECTO1_PRODUCER_H


// C program for finding the largest integer 
// among three numbers using command line arguments 
#include <stdio.h> 
#include <unistd.h>

#include "lib/argument_parser.h"
#include "lib/semaphores.h"
#include "lib/random_number_genarator.h"


// Taking argument as command line 
int main(int argc, char *argv[]) 
{ 
    // Get the attachment parametters
    // [0]:shmid [1]:avg time sleep in seconds
    int* parameters = parse_producer(argc, argv);
    // Stats variables
    unsigned int num_messages = 0;
    double acc_waiting_time = 0;
    double acc_sem_waiting_time = 0;
    double acc_kernel_time = 0;
    // Producer control and identification
    short isSending = TRUE;
    short self_id;

    // Shared memory buffer
    buffer_t* buffer = attach_shm(*parameters);
    // Semaphores used to access the data
    // semaphores[0]: mutex, 
    // semaphores[1]: empty_spaces, 
    // semaphores[2]: available_msgs

    // Acces number of producers
    sem_wait(buffer->semaphores);
    self_id = buffer->producers++;
    // Free the mutex
    sem_post(buffer->semaphores);


    // Initialize random lib
    srand((unsigned) time(NULL));
    // Get random time
    double time = poisson_distribution(*(parameters + 1));

    // Only send when the buffer is active
    while(isSending){

        int a = -8;
        sem_getvalue(buffer->semaphores, &a);
        printf("sem 0 ptr %p and value %d\n", buffer->semaphores, a);
        a = -8;
        sem_getvalue(buffer->semaphores + 1 , &a);
        printf("sem 1 ptr %p and value %d\n", buffer->semaphores + 1, a);
        a = -8;
        sem_getvalue(buffer->semaphores + 2, &a);
        printf("sem 2 ptr %p and value %d\n", buffer->semaphores + 2, a);


        // Wait for empty spaces
        sem_wait(buffer->semaphores + 1);
        // Wait for mutex
        sem_wait(buffer->semaphores);

        //Check if the buffer is still active
        isSending = buffer->isActive;

        // If it is then send the msg
        if(isSending){
    
            // TODO : BUILD MESSAGES CORRECTLY
            // Create msg
            message_t msg = {.producer_id=self_id, .data=(rand() % 6), 
                            .date=10, .time=11};
            // Send the msg
            send_msg(msg, buffer);
            // Increment the counter
            ++num_messages;

        }
        // If the buffer is down 
        else{
            // Decrease the producer counter
            --buffer->producers;
            isSending = FALSE;
        }

        // Release mutex
        sem_post(buffer->semaphores);
        // Update available
        sem_post(buffer->semaphores + 2);

        // Sleep time 
        //sleep(*(parameters + 1));
        sleep(self_id + 5);

    }
    
    // Detach from shared memory
    shmdt(buffer);
    // TODO HANDLE PRINTS OVER HERE
    printf("Detached...\n");
}

#endif  // PROYECTO1_PRODUCER_H
