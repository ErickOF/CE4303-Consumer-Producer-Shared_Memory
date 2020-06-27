
#include <string.h>

#include "lib/shared_memory.h"
#include "lib/argument_parser.h"


int main(int argc, char* argv[]) {
    int shmid = *parse_initializer(argc, argv);

    // Attaches the shared memory segment associated with the shared
    // memory identifier shmid to the data segment of the calling
    // process.
    // https://www.mkssoftware.com/docs/man3/shmat.3.asp
    buffer_t* buffer = (buffer_t*) shmat(shmid, NULL, 0);

    if (buffer == (void *) -1) {
        printf("Error: Shared memory attach");
        exit(1);
    }

    printf("ShmID %i\n", shmid);

    // Set values
    // Initialize consumer and producer id values
    buffer->consumers = 0;
    buffer->producers = 0;
    // Set system active
    buffer->isActive = 1;
    // Initialize msg related arrays
    buffer->available_slots = (short*) calloc(MAX_MSGS, sizeof(short));
    buffer->msg = (message_t*)malloc(MAX_MSGS * sizeof(message_t));

    printf("Buffer created.\n");

    // Detach from shared memory
    shmdt(buffer);

    return 0;
}
