/*Universidade Federal do Rio de Janeiro*/
/*Autor: Thiago de Oliveira Silva 	DRE: 111466197*/
/*Disciplina: Computação concorrente*/

/* Questão 1 - Sequencial*/

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int n;
int* vetor;
int main(){
    printf("Digite o tamanho do vetor:");
    scanf("%d", &n);

/* Inicia as variaveis globais e carrega o vetor de entrada */
    vetor = (int*)malloc(sizeof(int)*n);

    printf("\nVetor de numeros aleatorios:\n [");

    srand(time(NULL));
    for(int i=0; i<n; i++){
        int r = rand()%10;
        vetor[i] = r;
        printf("%d", vetor[i]);

        if(i<n-1){
            printf(", ");
        }
    }
    printf("]\n\n");
    
    for(int i = 1; i<n; i++){
        vetor[i] = vetor[i]+vetor[i-1];
    }

    printf("Vetor modificado:\n [");

    for(int i=0; i<n; i++){
        printf("%d", vetor[i]);

        if(i<n-1){
            printf(", ");
        }
    }
    printf("]\n\n");
    free(vetor);
}