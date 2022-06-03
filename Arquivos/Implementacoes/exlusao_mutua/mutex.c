#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define THREADS 40 //Número de threads desejadas

long compartilhada = 0; // memória compartilhada
pthread_mutex_t mutex;  // declaração do mutex



//Função executada em paralelo
void *threadFunc(void *arg){
	int a, b;
	for(long j=0; j<100000; j++){
		pthread_mutex_lock(&mutex);//entrar na sessão crítica
		a = compartilhada;
		b = 1;
		compartilhada = a + b; //incrementando em 1(condicao de corrida)
		pthread_mutex_unlock(&mutex);
	}

}

int main(int argc, char *argv[]){
	pthread_t t[THREADS];
	int s;
	
	for(int i = 0; i<THREADS; ++i){
		s = pthread_create(&t[i], NULL, threadFunc, NULL); //criado
	}

	for(int i = 0; i<THREADS; ++i){
		s = pthread_join(t[i], NULL); //esperando as threads terminarem
	}

	printf("compartilhada = %1ld\n", compartilhada);
	// O resultado esperado aqui era que cada uma das 40 threads acrescentassem 100000. No entanto
	// o que resultaria em 4000000 mil na variavel compartilhada. O que não ocorrre.
	// Use gcc mutex.c -o executavel -lpthread para compilar esse código
}
