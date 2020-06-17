// Constants
const int BUFFER_ID = 256;


// Types and structures

/**
 * Struct Buffer is used to represent information needed.
 * 
 * char* name - buffer name.
 * int producers - number of producers using the buffer.
 * int consumers - number of consumers using the buffer.
 * int flags - flags used to create the buffer.
 */
typedef struct Buffer
{
    int id_data;
    int size;
    short consumers;
    short producers;
    short flags;
} buffer_t;

