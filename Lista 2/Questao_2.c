/*Universidade Federal do Rio de Janeiro*/
/*Autor: Thiago de Oliveira Silva 	DRE: 111466197*/
/*Disciplina: Computação concorrente*/

/* Questão 2*/

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#define nthreads 2

long long int contador=0;
bool aux = false;                       //Variavel auxiliar para conduzir o fluxo da execução

pthread_cond_t cond;                    //usado para condicionar as chamadas de funcoes 
pthread_mutex_t mutex;                  //usado para exclusao mutua

int FazAlgo(long long int cont){        //Função FazAlgo ocupa as threads
    while(cont<999999){ cont++; }
}

void *T1 (void *arg) {
    while(1) {                          //loop infinito
        FazAlgo(contador);
        
        pthread_mutex_lock(&mutex);     //usado para garantir a exclusão mútua
        contador++;

        if(contador%100==0){            // verificacao da condicao do problema para que T2 imprima
            pthread_cond_signal(&cond); // desbloqueia thread na condição cond
            aux=true;                   // altera aux para que T2 desbloqueie
        }
        pthread_mutex_unlock(&mutex);   //usado para garantir a exclusão mútua
    }
}

void *T2 (void *arg){
    while(1){                           //loop infinito
        pthread_mutex_lock(&mutex);     //usado para garantir a exclusão mútua
           
        while(aux==false){              //condicao alterada em T1
            pthread_cond_wait(&cond, &mutex);   // aguarda sob a condicao de cond
        }
    
        printf("\n%lld", contador);         //imprime valor multiplo de 10
        aux = false;        //após imprimir, altera aux para false para continuar aguardando sinal
        pthread_mutex_unlock(&mutex);       //usado para garantir a exclusão mútua
    }
}

int main(){
    pthread_t threads[nthreads];    
    
    pthread_mutex_init(&mutex, NULL);       //inicializa mutex
    pthread_cond_init(&cond, NULL);         //inicializa variavel de condicao

    pthread_create(&threads[0], NULL, T1, NULL);    //cria thread e atribui funcao T1
    pthread_create(&threads[1], NULL, T2, NULL);    //cria thread e atribui funcao T2
    
    for (int i=0; i<nthreads; i++) {                //aguarda as threads terminarem
    pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);                  //inutiliza mutex
    pthread_cond_destroy(&cond);                    //inutiliza variavel de condicao
    return 0;
}