#ifndef PROYECTO1_DATETIME_H
#define PROYECTO1_DATETIME_H

#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "const.h"


/**
 * This function returns current datetime.
 * 
 * Params:
 *     char date - var to store datetime.
 */
void get_date(char date_n_time[DNT_LEN])
{
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
    strftime(date_n_time, DNT_LEN - 1, "%Y-%m-%d %H:%M:%S", tm);
}

/**
 * This function returns current time of date.
 * 
 * Params:
 *     struct timeval vale - var to store time of date.
 */
void get_mstime(struct timeval *value)
{
	gettimeofday(value, NULL);
}

#endif  // PROYECTO1_DATETIME_H
