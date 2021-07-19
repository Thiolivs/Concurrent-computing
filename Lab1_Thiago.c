
/*Universidade Federal do Rio de Janeiro*/
/*Autor: Thiago de Oliveira Silva 	DRE: 111466197*/
/*Disciplina: Computação concorrente - Laboratório 1 - Operação com vetor*/


/*A atividade foi feita usando a ferramenta VsCode, no sistema operacional Windows, 
com as devidas instalações do gcc e MingW*/

/*Sobre o programa: O vetor é preenchido com os valores 0, 1, 2, 3, ... 9.999 em sequencia e
pode-se observar na impressão do Vetor Alterado, os valores 0, 1, 4, 9, 16, 25... */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h> 

#define NTHREADS 2
#define SIZE 10000

int* vetor;

typedef struct{
	int inicio;
	int fim;
} t_Args;

// Funcao executada pelas threads
void *CalculaQuadrado (void *arg){
	t_Args *args = (t_Args *)arg;
	for(int i = args->inicio; i < args->fim; i++){
		vetor[i] = vetor[i]*vetor[i]; //Realiza a operacao de elevar ao quadrado
	}
	free(arg);
	pthread_exit(NULL);
}

int main(void){
	
	// Threads do sistema
	pthread_t *tid_sistema = (pthread_t *)malloc(sizeof(pthread_t) *NTHREADS);
	
	// Alocacao de memoria
	vetor = (int *)malloc(sizeof(int) *SIZE);
	
	// Preenchimento do vetor
	for(int i = 0; i<SIZE; i++){
		vetor[i] = i; 
	}
	
	// Imprime o Vetor Inicial
	printf("Valores iniciais:\n");
	for(int i = 0; i<SIZE; i++){
		printf("%d ",vetor[i]); 
	}
	printf("\n");
	
	t_Args *arg;
	for(int i=0;i<NTHREADS;i++){
		arg = malloc(sizeof(t_Args)); // Aloca memoria
		if(arg == NULL) {
			printf("--ERRO: malloc()\n"); exit(-1);
		}
		arg->inicio = floor((double)SIZE/NTHREADS *i); 
		arg->fim = floor((double)SIZE/NTHREADS *(i+1));

		if (pthread_create(&tid_sistema[i], NULL, CalculaQuadrado, (void*) arg)) {
			printf("--ERRO: pthread_create()\n"); exit(-1);
		} 
	}
	
	//Join espera as threads terminarem
	for(int i=0; i<NTHREADS; i++){
		if(pthread_join(tid_sistema[i], NULL)) {
			printf("--ERRO: pthread_join() \n"); exit(-1); 
		} 
	}
	
	//Vetor com valores alterados
	printf("\n Valores alterados:\n");
	for(int i = 0;i<SIZE;i++){
		printf("%d ",vetor[i]); 
	}	
	return(0);
}