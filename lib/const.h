#ifndef PROYECTO1_CONST_H
#define PROYECTO1_CONST_H

/*      BOOLEAN CONSTANTS      */
#define TRUE 1
#define FALSE 0

/*      SHARED MEMORY CONSTANTS      */
#define MAX_MSGS 10

const int BUFFER_ID = 256;
const int buffer_size = 1024;
const short buffer_flags = 0666;


/*      SEMAPHORE CONSTANTS      */
// Semaphore names
// Mutual excecution control 
const char* mutex = "mutex";
// Producer control, wait for it to send msgs and post from
// consumers to make spaces available
const char* empty = "empty";
// Consumer control, post from producers to allow consumer access
// and wait from consumers to read
const char* available = "available";
// Semaphore flags
const short semaphore_flags = 0666;

#endif  // PROYECTO1_CONST_H