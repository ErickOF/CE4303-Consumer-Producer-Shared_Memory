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
    double u;
    u = rand() / (RAND_MAX + 1.0);
    EXPO_SEED = lambda * -log(1.0 - u);

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

    float mean = 0;
    srand((unsigned)time(NULL));

    printf("segundos expo:");

    for (i=0; i<50; i++) {
        double value = expo_distribution(7);
        printf("%f ", value);
        mean += value;
    }

    mean = mean / 50;
    printf("\n%f\n", mean);
    

    //Random number Poisson  generator   
    srand((unsigned)time(NULL));
    POISON_SEED = 7;
    mean = 0;
    printf("segundos poisson:");
    for (i=0; i<50; i++){
        long int value = poisson_distribution(POISON_SEED);
        mean += value;
        printf("%ld ", value);
    }
    mean = mean / 50;
    printf("\n%f\n", mean);
}
// estan aqui llamados en ciclos para pruebas, la idea es
//llamarlos cuando lo necesite. los srand solo se ejecutan 
//cuando se inicia el programa para inicializar
//la semilla con el tiempo. Luego cada valor q da, se guarda
//como nueva semilla