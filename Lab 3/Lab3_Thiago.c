/*Universidade Federal do Rio de Janeiro*/
/*Autor: Thiago de Oliveira Silva 	DRE: 111466197*/
/*Disciplina: Computação concorrente - Laboratório 3 - Maior e Menor elemento do vetor*/

/*A atividade foi feita usando a ferramenta VsCode, no sistema operacional Windows, 
com as devidas instalações do gcc e MingW*/

/*
Compilação:  
gcc NomeDoPrograma.c -o NomeDoPrograma 

execução:
NomeDoPrograma <N> <NTHREADS>
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "timer.h"

typedef struct{
    float menor;
    float maior;
} tArgs;

long long int N;
int NTHREADS;
float* vec;

void* find(void* arg){

    tArgs* args = (tArgs*) arg;
    args = (tArgs*) malloc(sizeof(tArgs));
    long long int id = 0;
    long int div = N/NTHREADS;
    long int inicio = id*div;
    long int fim;
    args->menor=vec[0];
    args->maior=vec[0];
    
    if(id == NTHREADS-1){
        fim = N;
    }
    else{
        fim = inicio + div;
    }
    
    for(long long int i=inicio+1; i < fim; i++){
        
        if( args->maior < vec[i]){
            args->maior = vec[i];
        }
        if(args->menor > vec[i]){
            args->menor = vec[i];
        }
        id++;       
    }
    pthread_exit((void*)args);
}

//função que gera numeros aleatorios para preencher o vetor
float RandomNumber(float Min, float Max){
    return ((((float)(rand())/((float)(RAND_MAX)))*(Max - Min)) + Min);
}

int main(int argc, char* argv[]){

    //tamanho do vetor e numero de threads passadas por argumento
    N = atoll(argv[1]);         
    NTHREADS = atoi(argv[2]);   

    float* sequencial;
    float* concorrente;
    double t_1, t_2, t_seq, t_conc;

    pthread_t* tid;
    tArgs* ret;

    //Inicialização (Comum a ambas as aplicações)

    ret = (tArgs*) malloc(sizeof(tArgs));
    if(ret == NULL){
        puts("ERROR === 'malloc'\n");
        return 1;
    }
    vec = (float*)malloc(sizeof(float) * N);
    if(vec == NULL){
        fprintf(stderr,"ERROR === 'malloc'\n");
        return 1;
    }
    
    sequencial = (float*)malloc(sizeof(float) * 2);
    if(sequencial == NULL){
        fprintf(stderr,"ERROR === 'malloc'\n");
        return 1;
    }
    concorrente = (float*)malloc(sizeof(float) * 2);
    if(concorrente == NULL){
        fprintf(stderr,"ERROR === 'malloc'\n");
        return 1;
    }

    
    tid = (pthread_t*)malloc(sizeof(pthread_t) * NTHREADS);
    if(tid == NULL){
        fprintf(stderr,"ERROR === 'malloc'\n");
        return 1;
    }
    
    //Preenche o vetor com valores aleatorios:
    srand(time(NULL));
    for(long int i=0; i<N; i++){
        vec[i] = RandomNumber(-10.0e15, 10.0e15);
    }
    
    sequencial[0] = vec[0];
    sequencial[1] = vec[0];
    
    // Fim da inicialização

    //Encontrando o maior e menor valor de forma sequencial:
    
    //Inicio da medição do tempo sequencial
    GET_TIME(t_1);

    for(long int i = 0; i<N; i++){
        if(sequencial[0]<vec[i]){
            sequencial[0] = vec[i];
        }
        if(vec[i]<sequencial[1]){
            sequencial[1] = vec[i];
        }
    }
    
    //Fim da medição do tempo sequencial
    GET_TIME(t_2);

    t_seq = t_2 - t_1;
    printf("\nTempo sequencial: %lf",t_seq);


    //Inicio da medição do tempo concorrente
    GET_TIME(t_1);

    //Cria as threads:
    for(long long int i = 0; i<NTHREADS; i++){
        
        if(pthread_create(tid+i, NULL, find, (void*) NULL)){
            fprintf(stderr, "ERROR === 'pthread_create'\n");
            return 2;
        }
    }

    // Join espera as threads terminarem
    for(long long int i = 0; i<NTHREADS; i++){
        pthread_join(*(tid+i), (void**)&ret);
        if(i==0){
            concorrente[0] = ret->maior;
            concorrente[1] = ret->menor;
        }
        else{
            if(concorrente[0]<ret->maior){
                concorrente[0] = ret->maior;
            }
            if(concorrente[1]>ret->menor){
                concorrente[1] = ret->menor;
            }
        }
        free(ret);
        
    }
    // Fim da medição dotempo concorrente
    GET_TIME(t_2);
    t_conc = t_2 - t_1;
    printf("\nTempo concorrente: %lf\n",t_conc);

    printf("\nGanho de desempenho: %lf\n",t_seq/t_conc);

    free(vec);
    free(tid);
    free(sequencial);
    free(concorrente);

    return printf("\nfim do programa\n");
}