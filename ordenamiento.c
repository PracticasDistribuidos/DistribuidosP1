#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define ELEMS 20000000 // Elementos a ordenar

int errors(int *numbers,int elems);
void initArr(int *numbers,int elems);
void SortArr(int *numbers);

int arr[ELEMS];
int firstBucket[ELEMS/4];
int secondBucket[ELEMS/4];
int thirdBucket[ELEMS/4];
int fourthBucket[ELEMS/4];

int main()
{
	int i;
	int n;

	long long start_ts;
	long long stop_ts;
	float elapsed_time;
	long lElapsedTime;
	struct timeval ts;
	
	initArr(arr,ELEMS);

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

	SortArr(arr);

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final

	elapsed_time = (float) (stop_ts - start_ts)/1000000.0;
	
	if((n=errors(arr,ELEMS)))
		printf("Se encontraron %d errores\n",n);
	else
		printf("%d elementos ordenados en %1.2f segundos\n",ELEMS,elapsed_time);
}

void initArr(int *numbers,int elems)
{
	int i;
	for(i=0;i<elems;i++) {
		numbers[i]=rand()*rand()%ELEMS;
	}
}

int errors(int *numbers,int elems)
{
	int i;
	int errs=0;
	for(i=0;i<elems-1;i++)
		if(numbers[i]>numbers[i+1])
			errs++;
	return(errs);
}

/* Estrategia 1:
 * Dividir el algoritmo en 4 partes. Cada una está encargada de ordenar
 * números de cierto rango en un arreglo temporal. Al final sustituimos
 * esos arreglos temporales en el arreglo original.
 */

 /* Estrategia 2:
 * Dividir el arreglo en 4 partes. Ordenar esas 4 partes.
 * Posteriormente ordenar los 4 arreglos.
 */

void SortArr(int *numbers)
{
	int fb = 0;
	int sb = 0;
	int tb = 0;
	int fob = 0;
	for(int i = 0; i<ELEMS; i++) {
		if(numbers[i]<(ELEMS/2)*-1) {
			firstBucket[fb] = numbers[i];
		} else if (numbers[i]<0) {
			secondBucket[sb] = numbers[i];
		} else if (numbers[i]<ELEMS/2) {
			thirdBucket[tb] = numbers[i];
		} else {
			fourthBucket[fob] = numbers[i];
		}
	}
	
}
