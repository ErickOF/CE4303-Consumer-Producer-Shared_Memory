#ifndef PROYECTO1_CONST_H
#define PROYECTO1_CONST_H

/*      BOOLEAN CONSTANTS      */
#define TRUE 1
#define FALSE 0

/*   DATE AND TIME CONSTANTS   */
// Date and time len 
#define DNT_LEN 25
// Buffer name max len
#define BN_LEN 64


/*   SHARED MEMORY CONSTANTS   */
#define MAX_MSGS 30
const short buffer_flags = 0666;


/*     SEMAPHORE CONSTANTS     */
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