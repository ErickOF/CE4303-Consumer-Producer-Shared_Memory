
#include <string.h>

#include "lib/shared_memory.h"
#include "lib/argument_parser.h"
#include "lib/const.h"


int main(int argc, char* argv[])
{
    char buffer_name[BN_LEN]; 
    int shmid = *parse_initializer(argc, argv, buffer_name);

    // Attaches the shared memory segment associated with the shared
    // memory identifier shmid to the data segment of the calling
    // process.
    // https://www.mkssoftware.com/docs/man3/shmat.3.asp
    buffer_t* buffer = (buffer_t*) shmat(shmid, NULL, 0);

    if (buffer == (void *) -1)
    {
        printf("Error: Shared memory attach");
        exit(1);
    }

    printf("ShmID %i\n", shmid);

    // Set values
    // Set buffer name
    strcpy(buffer->name, buffer_name);

    // Initialize consumer, producer id values and consumer turn 
    buffer->consumers = 0;
    buffer->producers = 0;
    buffer->next_consumer = 0;
    buffer->next_producer = 0;

    // Set system active
    buffer->isActive = 1;
    // Initialize msg related arrays

    for (size_t i = 0; i < MAX_MSGS; i++)
    {
        buffer->available_slots[i] = FALSE;
    }

    // Initialize semaphores
    get_semaphores(buffer->semaphores, buffer_name);

    printf("Buffer created.\n");

    // Detach from shared memory
    shmdt(buffer);

    return 0;
}
