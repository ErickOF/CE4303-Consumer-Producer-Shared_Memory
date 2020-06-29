#ifndef PROYECTO1_RANDOM_NUMBER_GENERATOR_H
#define PROYECTO1_RANDOM_NUMBER_GENERATOR_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>


int expo_distribution(double lambda) {
    double u = rand() / (RAND_MAX + 1.0);
    return (int)(lambda * -log(1.0 - u));
}

/* poisson.c
 *
 * Implementation straight from 
 * http://en.wikipedia.org/wiki/Poisson_distribution#Generating_Poisson-distributed_random_variables
 * which credits Knuth.
 *
 * Time complexity is O(lambda), which is not optimal.
*/
int poisson_distribution(double lambda) {
    int k = 0;
    double L = exp(-lambda), p = 1;

    do {
        ++k;
        p *= random() / (double) INT_MAX;
    } while (p > L);

    return --k;
}

#endif  // PROYECTO1_RANDOM_NUMBER_GENERATOR_H