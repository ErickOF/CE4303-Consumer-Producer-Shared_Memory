#ifndef PROYECTO1_CONSUMER_H
#define PROYECTO1_CONSUMER_H

// C program for finding the largest integer 
// among three numbers using command line arguments 
#include <stdio.h> 
#include <termios.h>
#include <unistd.h>

#include "lib/argument_parser.h"
#include "lib/semaphores.h"
#include "lib/random_number_genarator.h"




int usr_input(){

    int ch;
    struct termios oldt, newt;

    tcgetattr ( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );

    return ch;
}

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
    int data;
    
    // Shared memory buffer
    buffer_t* buffer = attach_shm(*parameters);
    // Semaphores used to access the data
    // semaphores[0]: mutex, 
    // semaphores[1]: empty_spaces, 
    // semaphores[2]: available_msgs

    // Acces number of producers
    sem_wait(buffer->semaphores);
    self_id = buffer->consumers++;
    // Free the mutex
    sem_post(buffer->semaphores);
    printf("Productores activos %d\n", buffer->producers);


    // Initialize random lib
    srand((unsigned) time(NULL));

    // Get random time
    double time = poisson_distribution(*(parameters + 1));

    // Only send when the buffer is active
    while (isRecieving) {
        
        // Wait for full spaces
        sem_wait(buffer->semaphores + 2);
        // Wait for mutex
        sem_wait(buffer->semaphores);

        //Check if the buffer is still active
        isRecieving = buffer->isActive;

        // If it is recieving
        if (isRecieving) {
            // If its out turn to recieve
            if(buffer->next_consumer == self_id){

                // if we are on manual mode
                if(*(parameters + 2) == FALSE){
                    printf("Press enter to recieve a msg ");
                    usr_input();
                }
                

                // Recieve the msg data
                data = receive_msg(buffer);
                // Update recieved msgs
                ++num_messages;

                // if the data is equal to our id modulus 6
                if(data == self_id%6){
                    // Then stop consuming msgs
                    isRecieving = FALSE;
                    // Decrease the producer counter
                    --(buffer->consumers);
                    printf("-----------------------------------------------------------\n");
                    printf("Detected msg equal to consumer id modulus 6, detaching consumer %d...\n", self_id);
                

                } 
            }
        }
        // If the buffer is down 
        else {
            // Decrease the producer counter
            --(buffer->consumers);
            printf("-----------------------------------------------------------\n");
            printf("Detected inactive buffer, detaching consumer %d...\n", self_id);
        }
        

        // Release mutex
        sem_post(buffer->semaphores);
        // Update new empty space available
        sem_post(buffer->semaphores + 1);

        // Sleep time 
        sleep(poisson_distribution( (double)*(parameters + 1) ));
        

    }
    
    // Detach from shared memory
    shmdt(buffer);
    // TODO HANDLE PRINTS OVER HERE
    printf("Detached...\n");
} 

#endif  // PROYECTO1_CONSUMER_H