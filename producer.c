// C program for finding the largest integer 
// among three numbers using command line arguments 
#include <stdio.h> 

#include "lib/argument_parser.h"
#include "lib/semaphores.h"
#include "lib/const.h"

// Taking argument as command line 
int main(int argc, char *argv[]) 
{ 
    // Get the attachment parametters
    // [0]:shmid [1]:buffer size
    int* parameters = parse_producer(argc, argv);
    int failed_send;
    short self_id;

    // Attach the shared memory segment 
    buffer_t* buffer = attach_shm(*parameters);
    // Get the semaphores used to access the data
    // semaphores[0]: available, semaphores[1]: accessing
    sem_t* semaphores = get_semaphores();

    // Wait for acces to get the producer id
    sem_wait(semaphores);
    // Once we have it store the id and increase the number of 
    // connected producers
    self_id = (buffer->producers)++;
    sem_post(semaphores);

    while(TRUE){
        // Wait for empty spaces
        sem_wait(semaphores + 1);
        // Wait for mutex
        sem_wait(semaphores);

        //TODO CHECK FOR isActive flag and handle it

        // Create msg
        message_t msg = {.producer_id=self_id, .data=self_id, 
                        .date=10, .time=11};
        // Send the msg
        failed_send = send_msg(msg, buffer);
        // Cr=heck for errors
        if(failed_send == TRUE){
            printf("Error when sending msg from producer %d", self_id);
        }        

        // Release mutex
        sem_post(semaphores);
        // Update available
        sem_post(semaphores + 2);

    }


    // Detach from shared memory
    shmdt(buffer);

} 
