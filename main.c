#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#define N 10

//Execução use: ./main threads rodadas

float *novoVetor;	
float *vetorAntigo;
int rodadas;
int threads;

int *inicio;
int *final;

pthread_barrier_t barrier1;
pthread_barrier_t barrier2;

void *funcaoManipulaThread(void*);

void imprimeMatriz(float *array){
	for (int i = 0; i < N; i++){
		printf("|%0.7f|", array[i]);
	}
	printf("|\n");
}

int main(int argc, char *argv[]){
	int s, divideTamanhos;
	pthread_t * thId;

	threads = strtol(argv[1], NULL, 10);
	rodadas = strtol(argv[2], NULL, 10);

	vetorAntigo = calloc(sizeof(float), N);
	vetorAntigo[N - 1] = 1;
	novoVetor = malloc(sizeof(float) *N);
	novoVetor[N - 1] = 1;
	novoVetor[0] = 0;

	divideTamanhos = ceil((N - 2) / threads);

	inicio = malloc(sizeof(int) *threads);
	final = malloc(sizeof(int) *threads);

	for (int j = 0; j < threads; j++){
		inicio[j] = divideTamanhos *j + 1;
		final[j] = (divideTamanhos *j) + divideTamanhos;
	}
	final[threads - 1] = N - 2;

	pthread_barrier_init(&barrier1, NULL, threads);	//inicializando barreiras.
	pthread_barrier_init(&barrier2, NULL, threads);	//inicializando barreiras.

	thId = calloc(sizeof(pthread_t), threads);
	if (thId == NULL)
		printf("calloc error");
	//criando as threads.
	for (long threadId = 0; threadId < threads; threadId++){
		pthread_create(&thId[threadId], NULL, funcaoManipulaThread, (void*) threadId);
	}
	//juntando as threads.
	for (int threadId = 0; threadId < threads; threadId++){
		pthread_join(thId[threadId], NULL);
	}
	return 0;
}

void *funcaoManipulaThread(void *aux){
	int s;
	long threadId = (long) aux;
	
	for (int i = 0; i < rodadas; i++){
		for (int j = inicio[threadId]; j <= final[threadId]; j++){
			novoVetor[j] = (vetorAntigo[j - 1] + vetorAntigo[j + 1]) / 2;
		}

		s = pthread_barrier_wait(&barrier1);

		if (s == 0){ //caso suceda 
			pthread_barrier_wait(&barrier2);
		}else{
			printf("\n|Vetor [%d]:|\n", i);
			imprimeMatriz(novoVetor);

			float *altera = vetorAntigo;
			vetorAntigo = novoVetor;
			novoVetor = altera;

			pthread_barrier_wait(&barrier2);
		}
	}
	return NULL;
}