/*Universidade Federal do Rio de Janeiro*/
/*Autor: Thiago de Oliveira Silva 	DRE: 111466197*/
/*Disciplina: Computação concorrente - Laboratório 5 - 
Problemas classicos de concorrencia usando locks e variaveis de condicao (barreira)*/

/*A atividade foi feita usando a ferramenta VsCode, no sistema operacional Windows, 
com as devidas instalações do gcc e MingW*/

/*Compilação:  
gcc NomeDoPrograma.c -o NomeDoPrograma (para compilação no windows não tem sido necessario o uso de -lpthread)
execução:
NomeDoPrograma*/

/*Sobre o programa:
Inicia criando um vetor (de tamanho NTHREADS) e preenchendo com zeros. 
Cada posicao recebe um numero aleatorio designado por uma thread.
Os valores são somados, a barreira faz com que as threads aguardem o processo terminar 
para que seja feito novamente em um total de vezes igual a NTHREADS
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>

pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
int NTHREADS, lock = 0, randomList[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
int* vetor;


//funcao que implementa a barreira para as threads
void espera (int NTHREADS){         
    pthread_mutex_lock(&x_mutex);
    if(lock == (NTHREADS-1)){
        pthread_cond_broadcast(&x_cond);
        lock = 0;
    }else{
        lock++;
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex);
}

//funcao executada pelas threads
void* incrementa(void* arg){        
    int id = *(int*) arg, soma = 0, N=0;

    while(N<NTHREADS){

        //substitui as posicoes com valores aleatorios
        vetor[id] = (id*3)*(randomList[rand()%10])%10;  

        printf("vetor[%d] foi substituido por: %d (pela thread: %d)\n", id, vetor[id], id);
        espera(NTHREADS);
        sleep(1);
        N++;
        printf(".\n");
        espera(NTHREADS);
        
        for(int i = 0; i<NTHREADS; i++){
            soma += vetor[i];
            printf("Somatorio: %d --- sou a Thread: %d\n", soma, id);
            espera(NTHREADS);
            sleep(1);
            printf(".\n");
            espera(NTHREADS);
        }
    }
    pthread_exit(NULL);
}


int main(){
    time_t t;                    //usado para a funcao rand()
    srand((unsigned)time(&t));   //usado para a funcao rand()
    
    printf("Defina o tamanho do vetor:\n");
    scanf("%d", &NTHREADS);
    
    pthread_t threads[NTHREADS];
    int id[NTHREADS];
    
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    vetor = (int*)malloc(sizeof(int)*NTHREADS); //Aloca memoria
   
    for(int i = 0; i<NTHREADS; i++){
        vetor[i] = 0;       //preenche-se o vetor inicial com zeros
    }

    for(int i = 0; i<NTHREADS; i++){
        id[i] = i;
        pthread_create(&threads[i], NULL, incrementa, (void*)&id[i]);
    }

    for(int i = 0; i<NTHREADS; i++){
        pthread_join(threads[i], NULL);
    }
    
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    
    free(vetor);

}
