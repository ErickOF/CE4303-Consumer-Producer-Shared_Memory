#ifndef PROYECTO1_CONSUMER_H
#define PROYECTO1_CONSUMER_H

// C program for finding the largest integer 
// among three numbers using command line arguments 
#include <stdio.h> 
#include <termios.h>
#include <unistd.h>

#include "lib/argument_parser.h"
#include "lib/semaphores.h"
#include "lib/random_number_generator.h"
#include "lib/datetime.h"



int usr_input()
{

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

    int pid = getpid();
    int pid_mod = pid % 6;

    // Statistics variables
    float total_locked_time = 0;
    float total_waiting_time = 0;

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
    buffer->total_consumers++;
    // Free the mutex
    sem_post(buffer->semaphores);
    
    
    int a, b, c;
    /*sem_getvalue(buffer->semaphores, &c);
    printf("sem 0 ptr %p and value %i\n", buffer->semaphores, c);
    sem_getvalue(buffer->semaphores + 1 , &b);
    printf("sem 1 ptr %p and value %i\n", buffer->semaphores + 1, b);
    sem_getvalue(buffer->semaphores + 2 , &a);
    printf("sem 2 ptr %p and value %i\n", buffer->semaphores + 2, a);*/

    // Initialize random lib
    srand((unsigned) time(NULL));

    // Time interval
    struct timeval start, stop;

    // Get random time
    double time = poisson_distribution(*(parameters + 1));

    // Only send when the buffer is active
    while (isRecieving)
    {
        // Get waiting time
        int waiting_time = poisson_distribution((double)* (parameters + 1));
        //printf("Esperando: %d\n", waiting_time);

        // Sleep time
        sleep(waiting_time);
        // Total waiting time
        total_waiting_time += (float) waiting_time;

        // Start time
        get_mstime(&start);
        //printf("Esperando available\n");
        // Wait for full spaces
        sem_wait(buffer->semaphores + 2);
        // Wait for mutex
        //printf("Esperando mutex\n");
        sem_wait(buffer->semaphores);
        // End time
        get_mstime(&stop);

        // Total locked time
        float locked_time = ((float)(stop.tv_usec - start.tv_usec)) / 1000000.0 + ((float)(stop.tv_sec - start.tv_sec));
        total_locked_time += locked_time;

        // Set statistics
        buffer->total_waiting_time += (float) waiting_time;
        buffer->total_locked_time += locked_time;
        buffer->total_kernel_time += ((float) waiting_time) + locked_time;

        //Check if the buffer is still active
        isRecieving = buffer->isActive;

        // If it is recieving
        if (isRecieving)
        {

            //printf("Es turno del consumidor: %d y yo soy %d\n", buffer->next_consumer, self_id);
            // If its out turn to recieve
            if (buffer->next_consumer == self_id)
            {
                // Start time
                get_mstime(&start);

                // if we are on manual mode
                if (*(parameters + 2) == FALSE)
                {
                    printf("Press enter to recieve a msg \n");
                    usr_input();
                }

                // End time
                get_mstime(&stop);

                // Total user time
                float user_time = ((float)(stop.tv_usec - start.tv_usec)) / 1000000.0 + ((float)(stop.tv_sec - start.tv_sec));
                total_waiting_time += user_time;

                buffer->total_user_time += user_time;

                // Recieve the msg data
                data = receive_msg(buffer);
                // Update recieved msgs
                ++num_messages;

                // if the data is equal to our id modulus 6
                if (data == pid_mod)
                {
                    // Then stop consuming msgs
                    isRecieving = FALSE;
                    // Decrease the producer counter
                    --(buffer->consumers);
                    // Increase the dead buffer counter
                    ++buffer->deleted_consumers[pid_mod];
                    printf("-----------------------------------------------------------\n");
                    printf("Detected msg equal to consumer id modulus 6, detaching consumer %d...\n", self_id);
                } 
            }
        }
        // If the buffer is down 
        else
        {
            // Decrease the producer counter
            --(buffer->consumers);
            printf("-----------------------------------------------------------\n");
            printf("Detected inactive buffer, detaching consumer %d...\n", self_id);
        }
        
        // Release mutex
        sem_post(buffer->semaphores);
        // Update new empty space available
        sem_post(buffer->semaphores + 1);

        
        /*sem_getvalue(buffer->semaphores, &c);
        printf("sem 0 ptr %p and value %i\n", buffer->semaphores, c);
        sem_getvalue(buffer->semaphores + 1 , &b);
        printf("sem 1 ptr %p and value %i\n", buffer->semaphores + 1, b);
        sem_getvalue(buffer->semaphores + 2 , &a);
        printf("sem 2 ptr %p and value %i\n", buffer->semaphores + 2, a);*/
        
    }

    
    // Detach from shared memory
    shmdt(buffer);


    printf("\033[1m");
    printf("-----------------------------------------------------------\n");
    printf("----------------- Consumidor %d finalizado ----------------\n", self_id);
    printf("-----------------------------------------------------------\n");

    printf("PID: %d\n", pid);
    printf("Mensajes consumidos: %d\n", num_messages);
    printf("Tiempo total esperando: %f\n", total_waiting_time);
    printf("Tiempo total bloqueado: %f\n", total_locked_time);
    printf("Tiempo total en kernel: %f\n", (total_locked_time + total_waiting_time));

    printf("-----------------------------------------------------------\n");
    printf("-----------------------------------------------------------\n");
    printf("-----------------------------------------------------------\n");
    printf("\033[0m");
} 

#endif  // PROYECTO1_CONSUMER_H
