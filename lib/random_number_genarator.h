#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>


double expo_distribution(double lambda) {
    double u = rand() / (RAND_MAX + 1.0);
    return lambda * -log(1.0 - u);
}

/* poisson.c
 *
 * Implementation straight from 
 * http://en.wikipedia.org/wiki/Poisson_distribution#Generating_Poisson-distributed_random_variables
 * which credits Knuth.
 *
 * Time complexity is O(lambda), which is not optimal.
*/
long poisson_distribution(double lambda) {
    long k = 0;
    double L = exp(-lambda), p = 1;

    do {
        ++k;
        p *= random() / (double) INT_MAX;
    } while (p > L);

    return --k;
}