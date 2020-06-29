#ifndef PROYECTO1_PRODUCER_H
#define PROYECTO1_PRODUCER_H


// C program for finding the largest integer 
// among three numbers using command line arguments 
#include <stdio.h> 
#include <unistd.h>

#include "lib/argument_parser.h"
#include "lib/semaphores.h"
#include "lib/random_number_generator.h"
#include "lib/datetime.h"


// Taking argument as command line 
int main(int argc, char *argv[])
{
    // Get the attachment parametters
    // [0]:shmid [1]:avg time sleep in seconds
    int* parameters = parse_producer(argc, argv);

    // Statistics variables
    float total_locked_time = 0;
    float total_waiting_time = 0;
    unsigned int num_messages = 0;

    // Stats variables
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
    buffer->total_producers++;

    // Free the mutex
    sem_post(buffer->semaphores);

    // Initialize random lib
    srand((unsigned) time(NULL));

    // Time interval
    struct timeval start, stop;

    // Only send when the buffer is active
    while (isSending)
    {
        // Get waiting time
        int waiting_time = expo_distribution((double)* (parameters + 1));
        //printf("Esperando: %d\n", waiting_time);

        // Sleep time
        sleep(waiting_time);
        // Total waiting time
        total_waiting_time += (float) waiting_time;

        // Start time
        get_mstime(&start);
        //printf("Esperando empty\n");
        // Wait for empty spaces
        sem_wait(buffer->semaphores + 1);
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
        isSending = buffer->isActive;

        // If it is then send the msg
        if (isSending)
        {
            //printf("Es turno del productor: %d y yo soy %d\n", buffer->next_producer, self_id);
            // If its out turn to send
            if (buffer->next_producer == self_id)
            {
                // Get the date and time 
                char date_n_time[DNT_LEN];
                get_date(date_n_time);

                // Create msg
                message_t msg = {
                    .producer_id=self_id, 
                    .data=(rand() % 7)
                };

                // Set datetime
                strcpy(msg.date_n_time, date_n_time);

                // Send the msg
                send_msg(msg, buffer);
                buffer->total_msgs++;
                // Increment the counter
                ++num_messages;
            }

        }
        // If the buffer is down 
        else
        {
            // Decrease the producer counter
            --buffer->producers;
            isSending = FALSE;
        }

        // Release mutex
        sem_post(buffer->semaphores);
        // Update available
        sem_post(buffer->semaphores + 2);

        /*int a, b, c;
        sem_getvalue(buffer->semaphores, &c);
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
    printf("----------------- Productor %d finalizado -----------------\n", self_id);
    printf("-----------------------------------------------------------\n");

    printf("Mensajes producidos: %d\n", num_messages);
    printf("Tiempo total esperando: %f\n", total_waiting_time);
    printf("Tiempo total bloqueado: %f\n", total_locked_time);
    printf("Tiempo total en kernel: %f\n", (total_locked_time + total_waiting_time));
  
    printf("-----------------------------------------------------------\n");
    printf("-----------------------------------------------------------\n");
    printf("-----------------------------------------------------------\n");
    printf("\033[0m");

}


#endif  // PROYECTO1_PRODUCER_H
