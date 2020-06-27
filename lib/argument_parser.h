#ifndef PROYECTO1_ARGUMENT_PARSER_H
#define PROYECTO1_ARGUMENT_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared_memory.h"

int* parse_initializer(int argc, char *argv[]){

    //                      buffer name, buffer size, 
    int valid_arguments[2] =    {0,         0};
    // [0] is shared memory id and [1] is buffer size
    int* values = (int*)malloc(2 * sizeof(int)); 
    *values = -1;
    *(values + 1) = -1;

	// Checking if number of argument is 
	// equal to 4 or not. 
    
	if (argc != 5) 
	{ 
		printf("Please enter only the buffer name and buffer size. \nExample: initializer -bn <buffer_name> -bs <buffer_size>\n"); 
		return 0; 
	} 

    for (int i = 0; i < argc; ++i)
    {
        // If we recive the buffer name
        if(strcmp(argv[i], "-bn") == 0){
            // And we havent recieved it
            if(valid_arguments[0] == 0){
                // Get the associated shared memory id
                *values = get_shared_mem_id(argv[i+1], IPC_CREAT | IPC_EXCL);
                valid_arguments[0] = 1;
            }
            else
            {
                printf("Entered buffer name twice\n"); 
                exit(EXIT_FAILURE);
            }
            
        }
        // If we recive the avg message time
        else if(strcmp(argv[i], "-bs") == 0){
            // And we havent recieved it
            if(valid_arguments[1] == 0){
                // Store it
                *(values + 1) = atoi(argv[i+1]);
                valid_arguments[1] = 1;
            }
            else
            {
                printf("Entered buffer size twice\n"); 
                exit(EXIT_FAILURE);
            }
            
        }
    }

    // Check we read all the values 
    if(*values == -1){
        printf("Missing buffer name\n"); 
        exit(EXIT_FAILURE);
    } 
    if(*(values + 1) == -1){
        printf("Missing buffer size\n"); 
        exit(EXIT_FAILURE);
    }

    return values;

}


int* parse_producer(int argc, char *argv[]){

    //                      buffer name, avg seconds
    int valid_arguments[2] = {0         ,      0};
    // [0] is buffer id and [1] is avg seconds
    int* values = (int*)malloc(2 * sizeof(int)); 
    *values = -1;
    *(values + 1) = -1;

	// Checking if number of argument is 
	// equal to 5 or not. 
    printf("%d\n", argc);
	if (argc != 5) 
	{ 
		printf("Please enter only the buffer name and avg send time. \nExample: producer -bn <buffer_name> -ti <avg_time(seconds)>\n"); 
		return 0; 
	} 

    for (int i = 0; i < argc; ++i)
    {
        // If we recive the buffer name
        if(strcmp(argv[i], "-bn") == 0){
            // And we havent recieved it
            if(valid_arguments[0] == 0){
                // Get the associated shared memory id
                *values = get_shared_mem_id(argv[i+1], 0);
                valid_arguments[0] = 1;
            }
            else
            {
                printf("Entered buffer name twice\n"); 
                exit(EXIT_FAILURE);
            }
            
        }
        // If we recive the avg message time
        else if(strcmp(argv[i], "-ti") == 0){
            // And we havent recieved it
            if(valid_arguments[1] == 0){
                // Store it
                *(values + 1) = atoi(argv[i+1]);
                valid_arguments[1] = 1;
            }
            else
            {
                printf("Entered time twice\n"); 
                exit(EXIT_FAILURE);
            }
            
        }
    }

    // Check we read all the values 
    if(*values == -1){
        printf("Missing buffer name\n"); 
        exit(EXIT_FAILURE);
    } 
    if(*(values + 1) == -1){
        printf("Missing average time\n"); 
        exit(EXIT_FAILURE);
    }

    return values;

}


int* parse_consumer(int argc, char *argv[]){

    //                      buffer name, avg seconds, access mode
    int valid_arguments[3] = {0         ,      0,       0};
    // [0] is buffer id, [1] is avg seconds, [2] access mode
    int* values = (int*)malloc(3 * sizeof(int)); 
    *values = -1;
    *(values + 1) = -1;
    *(values + 2) = -1;

	// Checking if number of argument is 
	// equal to 5 or not. 
    printf("%d\n", argc);
	if (argc != 5) 
	{ 
		printf("Please enter only the buffer name, avg send time and access mode. \nExample: producer -bn <buffer_name> -ti <avg_time(seconds)> -am <0 for manual, 1 for automatic>\n"); 
		return 0; 
	} 

    for (int i = 0; i < argc; ++i)
    {
        // If we recive the buffer name
        if(strcmp(argv[i], "-bn") == 0){
            // And we havent recieved it
            if(valid_arguments[0] == 0){
                // Get the associated shared memory id
                *values = get_shared_mem_id(argv[i+1], 0);
                valid_arguments[0] = 1;
            }
            else
            {
                printf("Entered buffer name twice\n"); 
                exit(EXIT_FAILURE);
            }
            
        }
        // If we recive the avg message time
        else if(strcmp(argv[i], "-ti") == 0){
            // And we havent recieved it
            if(valid_arguments[1] == 0){
                // Store it
                *(values + 1) = atoi(argv[i+1]);
                valid_arguments[1] = 1;
            }
            else
            {
                printf("Entered time twice\n"); 
                exit(EXIT_FAILURE);
            }
            
        }
        // If we recive the access mode
        else if(strcmp(argv[i], "-am") == 0){
            // And we havent recieved it
            if(valid_arguments[2] == 0){
                // Store it
                *(values + 2) = atoi(argv[i+1]);
                valid_arguments[2] = 1;
            }
            else
            {
                printf("Entered time twice\n"); 
                exit(EXIT_FAILURE);
            }
            
        }
    }

    // Check we read all the values 
    if(*values == -1){
        printf("Missing buffer name\n"); 
        exit(EXIT_FAILURE);
    } 
    if(*(values + 1) == -1){
        printf("Missing average time\n"); 
        exit(EXIT_FAILURE);
    }
    if(*(values + 2) == -1){
        printf("Missing accses mode\n"); 
        exit(EXIT_FAILURE);
    }

    return values;

}

int* parse_killer(int argc, char *argv[]){
    // buffer name
    int valid_arguments = 0;
    // [0] is buffer id 
    int* values = (int*)malloc(1 * sizeof(int)); 
    *values = -1;

	// Checking if number of argument is 
	// equal to 3 or not. 
    
	if (argc != 3)  { 
		printf("Please enter only the buffer name. \nExample: killer -bn <buffer_name>\n");
		return 0; 
	} 

    for (int i = 0; i < argc; ++i) {
        // If we recive the buffer name
        if(strcmp(argv[i], "-bn") == 0){
            // And we havent recieved it
            if(valid_arguments == 0){
                // Get the associated shared memory id
                *values = get_shared_mem_id(argv[i + 1], 0);
                valid_arguments = 1;
            }
            else
            {
                printf("Entered buffer name twice\n"); 
                exit(EXIT_FAILURE);
            }
            
        }
    }

    // Check we read all the values 
    if (*values == -1){
        printf("Missing buffer name\n"); 
        exit(EXIT_FAILURE);
    }

    return values;
}

#endif  // PROYECTO1_ARGUMENT_PARSER_H
