#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define ELEMS 20000000 // Elementos a ordenar

int errors(int *numbers,int elems);
void initArr(int *numbers,int elems);
void SortArr(int *numbers);
int cmpfunc (const void * a, const void * b);
void makeNewArray(int* source, int* target, int start, int end);

int arr[ELEMS];
int firstBucket[ELEMS/2];
int secondBucket[ELEMS/2];
int thirdBucket[ELEMS/2];
int fourthBucket[ELEMS/2];

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
		if(numbers[i]>numbers[i+1]) {
			errs++;
			printf("Error encontrado en la posición %d\n",i);
			printf("Valor de number[i] = %d\n",numbers[i]);
			printf("Valor de number[i+1] = %d\n",numbers[i+1]);
		}
	return(errs);
}

 /* Estrategia:
 * Dividir el arreglo en 4 arreglos temporales acorde a rangos. 
 * Ordenar esos 4 arreglos.
 * Posteriormente sustituir los valores de los 4 arreglos temporales
 * en los espacios de memoria del arreglo original.
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
			fb++;
		} else if (numbers[i]<0) {
			secondBucket[sb] = numbers[i];
			sb++;
		} else if (numbers[i]<ELEMS/2) {
			thirdBucket[tb] = numbers[i];
			tb++;
		} else {
			fourthBucket[fob] = numbers[i];
			fob++;
		}
	}

	#pragma omp parallel num_threads(4)
	{
		switch(omp_get_thread_num()) {
			case(0):
				qsort(firstBucket, fb, sizeof(int), cmpfunc);
				makeNewArray(firstBucket,numbers,0,fb);
				break;
			case(1):
				qsort(secondBucket, sb, sizeof(int), cmpfunc);
				makeNewArray(secondBucket,numbers,fb,fb+sb);
				break;
			case(2):
				qsort(thirdBucket, tb, sizeof(int), cmpfunc);
				makeNewArray(thirdBucket,numbers,fb+sb,fb+sb+tb);
				break;
			case(3):
				qsort(fourthBucket, fob, sizeof(int), cmpfunc);
				makeNewArray(fourthBucket,numbers,fb+sb+tb,fb+sb+tb+fob);
				break;
		}
	}
}

void makeNewArray(int* source, int* target, int start, int end){
	for (int i = start; i<end;i++){
		target[i] = source[i-start];
	}
}

/* Función sacada de:
 * https://stackoverflow.com/questions/49834742/how-to-write-qsort-comparison-function-for-integers
 */
int cmpfunc (const void *a, const void *b) {
    const int *ia = (const int *)a; // casting pointer types 
    const int *ib = (const int *)b;
    if(*ia > *ib) {
        return 1;
    } else if (*ia == *ib) {
        return 0;
    }
    return -1;
}

