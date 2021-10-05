/*Universidade Federal do Rio de Janeiro*/
/*Autor: Thiago de Oliveira Silva 	DRE: 111466197*/
/*Disciplina: Computação concorrente - Laboratório 4 - Sincronização condicional com variaveis de condição*/

/*A atividade foi feita usando a ferramenta VsCode, no sistema operacional Windows, 
com as devidas instalações do gcc e MingW*/

/*
Compilação:  
gcc NomeDoPrograma.c -o NomeDoPrograma (para compilação no windows não tem sido necessario o uso de -lpthread)

execução:
NomeDoPrograma 
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NTHREADS 4

/* Variaveis globais */
pthread_cond_t x_cond, y_cond;
pthread_mutex_t mutex;

int x = 0;

/*funções executadas pelas threads*/
void* T1 (void* t){
    pthread_mutex_lock(&mutex);
    while(x < 1)
        pthread_cond_wait(&x_cond, &mutex);
    
    x++;
    
    printf("Fique a vontade!\n");

    pthread_cond_signal(&y_cond);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* T2 (void* t){
    pthread_mutex_lock(&mutex);
    
    printf("Seja bem-vindo!\n");

    x++;
    
    if(x == 1)
        pthread_cond_broadcast(&x_cond);
    
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}


void* T3 (void* t){
    pthread_mutex_lock(&mutex);

    while(x < 3)
        pthread_cond_wait(&y_cond, &mutex);
    
    printf("Volte sempre!\n");

    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* T4 (void* t){
    pthread_mutex_lock(&mutex);

    while(x < 1)
        pthread_cond_wait(&x_cond, &mutex);

    x++;

    printf("Sente-se por favor.\n");

    pthread_cond_signal(&y_cond);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}


int main(){

    pthread_t threads[NTHREADS];

    /* Inicilaização do mutex e das variáveis de condição */
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&x_cond, NULL);
    pthread_cond_init(&y_cond, NULL);

    /* Criação das threads */
    pthread_create(&threads[0], NULL, T1, NULL);
    pthread_create(&threads[1], NULL, T2, NULL);
    pthread_create(&threads[2], NULL, T3, NULL);
    pthread_create(&threads[3], NULL, T4, NULL);

    /* Espera todas as threads completarem */
    for(int i = 0; i<NTHREADS; i++)
        pthread_join(threads[i], NULL);

    /* Desalocação de variaveis */
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&x_cond);
    pthread_cond_destroy(&y_cond);

}