/*Universidade Federal do Rio de Janeiro*/
/*Autor: Thiago de Oliveira Silva 	DRE: 111466197*/
/*Disciplina: Computação concorrente - Laboratorio 5 - Exericios com semaforos*/

/*A atividade foi feita usando a ferramenta VsCode, no sistema operacional Windows, 
com as devidas instalações do gcc e MingW*/

/*Compilação:  
gcc NomeDoPrograma.c -o NomeDoPrograma (para compilação no windows não tem sido necessario o uso de -lpthread)
execução:
NomeDoPrograma*/

/*Sobre o programa:
Ao executar o programa solicita o tamanho do bufer, numero de produtores e numero de consumidores
Inicialmente cria o buffer e preenche com '-'. As threads produtoras o preenchem e somente após isso,
uma unica thread consumidora o esvazia. A sincronização é feita usando dois semaforos.
O provesso se repete até que o usuário pare. */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem_prod, sem_cons; // -- Semáforos

// -- Numero de produtores, consumidores, tamanho do buffer e auxiliares
int prod, cons, sizeB, items=0, i=0; 
int* buffer; 


// -- Imprime o buffer no estado atual
void state_buffer(){
    printf("Estado do Buffer: [");
    for(int i=0 ; i<sizeB ; i++){
        if (buffer[i]=='-'){
            printf("  %c  " , buffer[i]);
        }
        else{
            printf("  %d  " , buffer[i]);
        }        
    }
    printf("]\n");
}

// -- Produz os itens do buffer
void producer(){
    buffer[i]=(i+1); // i inicia em 0 (variavel global auxiliar)
    items++;
    state_buffer(); // -- imprime o estado atual do buffer
    i++;

}

// -- Consome os itens do buffer
void consumer(int n){
   int id = n;
   printf("\nConsumidor %d consumindo todos os recursos: \n", (id+1) );
   for(int i=0 ; i<sizeB ; i++){
       printf("item %d\n", buffer[i]);
       buffer[i] = '-' ;
   }
   state_buffer();
   printf("\n---------------------------------------------\n\n");
   sleep(3);
   items = 0; 
   sem_post(&sem_prod); // -- Libera threads produtoras
}

// -- Função das threads produtoras
void * t_producer(void *arg){
    int id = (int)arg;
    printf("Produtor %d criado\n", (id+1));
    for(;;){

        sem_wait(&sem_prod);     // -- bloqueia o acesso de novas threads 

        if(items==sizeB){        // -- verifica se o buffer está cheio
            i=0;                 // -- reinicia variavel para funcao produtora
            sem_post(&sem_cons); // -- Produtor permite consumidor esvaziar o buffer
            sem_wait(&sem_prod); // -- Produtor espera até que o buffer esvazie
        }
        
        sleep(1);
        printf("Produtor %d produzindo\n",(id+1));
        producer();

        sem_post(&sem_prod);     // -- libera o acesso para novas threads
    }
    pthread_exit(NULL);
}

// -- Função das threads consumidoras
void * t_consumer(void *arg){
    int id = (int) arg;
    printf("Consumidor %d criado\n",(id+1));
    for(;;){
        sem_wait(&sem_cons);     // -- Aguarda o preenchimento do buffer
        consumer(id);
    }
}

// -- Preenche o buffer inicialmente com '-'
void fill_buffer(int n){
    for(int i=0; i<n; i++){
        buffer[i]='-';
    }
}

int main(){
    
    // -- Solicita o tamanho do buffer e numero de threads (produtoras e consumidoras)
    printf("\nDefina o tamanho do buffer: ");
    scanf("%d", &sizeB);

    printf("Defina o numero de Produtores: ");
    scanf("%d", &prod);

    printf("Defina o numero de Consumidores: ");
    scanf("%d", &cons);

    printf("\n");


    // -- Inicialização dos semáforos
    sem_init(&sem_prod,0,1);
    sem_init(&sem_cons,0,0);
   
    pthread_t tid[prod+cons];
    buffer = malloc(sizeof(int)*sizeB);

    fill_buffer(sizeB); // -- preenche o buffer

    // -- Cria as threads produtoras
    for(int i=0;i<prod;i++){
        if(pthread_create(&tid[i], NULL, t_producer, (void*) i)){
            return 2;
        }
    }
    // -- Cria as thhreads consumidoras
    for(int i=0;i<cons;i++){
        if(pthread_create(&tid[prod+i], NULL, t_consumer, (void*) i)){
            return 2;
        }
    }

    pthread_exit(NULL);

    // -- Inutiliza os semáfaros e libera memória
    sem_destroy(&sem_prod);
    sem_destroy(&sem_cons);
    free(buffer);

    return 0;
}