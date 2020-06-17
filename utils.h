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


// Functions
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
