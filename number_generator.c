#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#define GOODRAND random()
#define GOODRANDMAX INT_MAX
#define RANDTYPE long

long int POISON_SEED;
double EXPO_SEED;

double expo_distribution(double lambda){
    double u, seconds;
    u = rand() / (RAND_MAX + 1.0);
    EXPO_SEED = (-log(1-u)/lambda);
    return EXPO_SEED;
}

//esta referencia tiene q estar con el codigo de poisson siempre
/* poisson.c
 *
 * Implementation straight from 
 * http://en.wikipedia.org/wiki/Poisson_distribution#Generating_Poisson-distributed_random_variables
 * which credits Knuth.
 *
 * Time complexity is O(lambda), which is not optimal.
*/
RANDTYPE poisson_distribution(double lambda){
  RANDTYPE k=0;  
  double L=exp(-lambda), p=1;
  do {
    ++k;
    p *= GOODRAND/(double)GOODRANDMAX;
  } while (p > L);
  return --k;
}

void main(int argc, char**argv){
//Random number exponential generator 
	int i;
    srand((unsigned)time(NULL));
    for (i=0; i<5; i++)
        printf("segundos expo: %f\n", expo_distribution(0.5));
    

//Random number Poisson  generator   
    srand((unsigned)time(NULL));
    POISON_SEED = rand() / (RAND_MAX + 1.0) * 10;
    for (i=0; i<5; i++){       
        printf("segundos poisson: %ld\n",poisson_distribution(POISON_SEED));
        }
}
// estan aqui llamados en ciclos para pruebas, la idea es
//llamarlos cuando lo necesite. los srand solo se ejecutan 
//cuando se inicia el programa para inicializar
//la semilla con el tiempo. Luego cada valor q da, se guarda
//como nueva semilla