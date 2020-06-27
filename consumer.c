#ifndef PROYECTO1_CONSUMER_H
#define PROYECTO1_CONSUMER_H

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
    // [0]:shmid 
    // [1]:avg time sleep in seconds 
    // [2] access mode
    int* parameters = parse_consumer(argc, argv);
    // Stats variables
    unsigned int num_messages = 0;
    double acc_waiting_time = 0;
    double acc_sem_waiting_time = 0;
    double acc_user_time = 0;
    // Consumer control and identification
    short isRecieving = TRUE;
    short self_id;

    // Shared memory buffer
    buffer_t* buffer;
    // Get the semaphores used to access the data
    // semaphores[0]: mutex, 
    // semaphores[1]: empty_spaces, 
    // semaphores[2]: available_msgs
    sem_t* semaphores;

    // Initialize buffer, semaphores and client id
    start_client(*parameters, FALSE, buffer, semaphores, &self_id);

    // Initialize random lib
    srand((unsigned) time(NULL));
    // Get random time
    double time = poisson_distribution(*(parameters + 1));

    // Only send when the buffer is active
    while (isRecieving) {
        // Wait for full spaces
        sem_wait(semaphores + 2);
        // Wait for mutex
        sem_wait(semaphores);

        //Check if the buffer is still active
        isRecieving = buffer->isActive;

        // If it is then send the msg
        if (isRecieving) {
            // TODO : BUILD MESSAGES CORRECTLY
            // Create msg
            message_t msg = {
                .producer_id=self_id,
                .data=self_id, 
                .date=10, .time=11
            };
            // Send the msg
            send_msg(msg, buffer);

        }
        // If the buffer is down 
        else {
            // Decrease the producer counter
            --(buffer->consumers);
            printf("-----------------------------------------------------------\n");
            printf("Detected inactive buffer, detaching consumer %d\n", self_id);

        }

        // Release mutex
        sem_post(semaphores);
        // Update new empty space available
        sem_post(semaphores + 1);

        // Sleep time 
        sleep(*(parameters + 1));
    }

    // Detach from shared memory
    shmdt(buffer);
    // TODO HANDLE PRINTS OVER HERE
    printf("Detached...\n");
} 

#endif  // PROYECTO1_CONSUMER_H