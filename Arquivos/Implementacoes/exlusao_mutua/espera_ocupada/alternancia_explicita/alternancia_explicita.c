#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define THREADS 2 // Numero de threads desejadas
#define NUMREP 1000000

long compartilhada = 0; // memória compartilhada
int turn = 0;
//Função executada em paralelo
void *threadFuncIncrement(void *arg){
	int a, b, condicao=1;

	for(long j=0; j<NUMREP; j++){
		a = compartilhada;
		b = 1;
		while(turn != 0);//Loope até que a trava seja igual a 1
		compartilhada = a + b; //incrementando em 1(condicao de corrida)
		turn = 1;
	}

}

void *threadFuncDecrement(void *arg){
	int a, b;
	for(long j=NUMREP; j>0; j--){
		a = compartilhada;
		b = 1;
		while(turn != 1);//Loop até que trava seja igual a 1
		compartilhada = a - b; //incrementando em 1(condicao de corrida)
		turn = 0;
	}

}

int main(int argc, char *argv[]){
	pthread_t t[THREADS];
	int s;

	s = pthread_create(&t[0], NULL, threadFuncIncrement, NULL); //criado
	s = pthread_create(&t[1], NULL, threadFuncDecrement, NULL); //criado

	s = pthread_join(t[0], NULL); //esperando as threads terminarem
	s = pthread_join(t[1], NULL); //esperando as threads terminarem

	printf("compartilhada = %1ld\n", compartilhada);
}
