/*Universidade Federal do Rio de Janeiro*/
/*Autor: Thiago de Oliveira Silva 	DRE: 111466197*/
/*Disciplina: Computação concorrente - Laboratório 8 - Exercício com semáforos*/

/*A atividade foi feita usando a ferramenta VsCode, no sistema operacional Windows, 
com as devidas instalações do gcc e MingW*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

sem_t s1, s2;
int x = 0;

void* T1(void* t){
    int* id = (int*) t;
    sem_wait(&s1);
    printf("Fique a vontade.\n");
    x++;
    sem_post(&s1);
    if(x==2){
        sem_post(&s2);
    }
    pthread_exit(NULL);
}

void* T2(void* t){
    int* id = (int*) t;
    printf("\nSeja bem vindo!\n");
    x=0;
    sem_post(&s1);
    pthread_exit(NULL);
}

void* T3(void* t){
    int* id = (int*)t;
    sem_wait(&s2);
    printf("Volte sempre!\n\n");
    pthread_exit(NULL);
}

void* T4(void* t){
    int* id = (int*)t;
    sem_wait(&s1);
    printf("Sente-se por favor.\n");
    x++;
    sem_post(&s1);
    if(x==2){
        sem_post(&s2);
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t tid[NTHREADS];
    int t;
    int* id[3];

    for(t=0; t<NTHREADS; t++){
        if((id[t] = malloc(sizeof(int))) == NULL){
            pthread_exit(NULL);
            return 1;
        }
        *id[t] = t;
    }

    //--inicia os semaforos
    sem_init(&s1, 0, 0);
    sem_init(&s2, 0, 0);

    
    //--Cria as threads 
    if (pthread_create(&tid[0], NULL, T1, (void *)id[1])) { 
        printf("--ERRO: pthread_create()\n"); exit(-1); 
    }

    if (pthread_create(&tid[1], NULL, T2, (void *)id[0])) { 
        printf("--ERRO: pthread_create()\n"); exit(-1); 
    }

    if (pthread_create(&tid[2], NULL, T3, (void *)id[2])) { 
        printf("--ERRO: pthread_create()\n"); exit(-1); 
    }

    if (pthread_create(&tid[3], NULL, T4, (void *)id[1])) { 
        printf("--ERRO: pthread_create()\n"); exit(-1); 
    }

    //--espera todas as threads terminarem
    for( t=0; t<NTHREADS; t++){
        if(pthread_join(tid[t],NULL)){
            printf("--ERRO: pthread join()\n");
            exit(-1);
        }
        free(id[t]);
    }
    pthread_exit(NULL);
    
}