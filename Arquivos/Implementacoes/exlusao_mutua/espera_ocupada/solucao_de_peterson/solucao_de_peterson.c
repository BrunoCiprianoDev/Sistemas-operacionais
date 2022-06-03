#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define THREADS 2 // Numero de threads desejadas
#define NUMREP 100000
#define TRUE 1
#define FALSE 0
#define N 2

long compartilhada = 0; // memória compartilhada
int turn = 0;
int interested[N];

void enter_region(int process){
	int other; 
	other = 1 - process; //Identifica qual é o outro processo
	interested[process] = TRUE; //Indica que o processo atual 1 ou zero quer entrar na RC
	turn = process;// Turn recebe o processo que chamou essa função
	while(turn == process && interested[other] == TRUE);
	//Enquanto turn == processo e o outro processo for true permance no looping
	//Se o interested[other] trocar para false ele entra na região critica
}

void leave_region(int process){
	interested[process] = FALSE;
}


//Função executada em paralelo
void *threadFuncIncrement(void *arg){
	int a, b, condicao=1;

	for(long j=0; j<NUMREP; j++){
		a = compartilhada;
		b = 1;
		enter_region(0);
		compartilhada = a + b; //incrementando em 1(condicao de corrida)
		leave_region(0);
	}

}

void *threadFuncDecrement(void *arg){
	int a, b;
	for(long j=NUMREP; j>0; j--){
		a = compartilhada;
		b = 1;
		enter_region(1);
		compartilhada = a - b; //incrementando em 1(condicao de corrida)
		leave_region(1);
	}

}

int main(int argc, char *argv[]){
	interested[0] = 0;
	interested[1] = 0;
	pthread_t t[THREADS];
	int s;

	s = pthread_create(&t[0], NULL, threadFuncIncrement, NULL); //criado
	s = pthread_create(&t[1], NULL, threadFuncDecrement, NULL); //criado

	s = pthread_join(t[0], NULL); //esperando as threads terminarem
	s = pthread_join(t[1], NULL); //esperando as threads terminarem

	printf("compartilhada = %1ld\n", compartilhada);
}


