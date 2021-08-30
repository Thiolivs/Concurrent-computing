#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"


int nthreads;   //Nº de threads como uma variável global para podermos usar na função main e na função das threads


struct Data {
    int dim;
    int j;
    double** A;
    double *b;
} tArgs;    //Struct que armazena alguns dados da matriz que será escalonada

//Função que realiza o escalonamento usando as threads
void *escalonamento(void *arg){
    long long int id = (long long int) arg;
    double** A = tArgs.A;
    double* b = tArgs.b;
    int dim = tArgs.dim;
    int j = tArgs.j;
    double m;

    for (int k = j + 1 + id; k<dim; k+=nthreads){
        m = A[k][j]/A[j][j];
        for ( int i = j; i<dim; i++){
            A[k][i] = A[k][i] - (m * A[j][i]);
        }
        b[k] = b[k] - (m*b[j]);
    }
    pthread_exit(NULL);
}

//Função que faz a substituição reversa para resolver a matriz após o escalonamento
void backSUBS(int dim, double** A, double* b, double* x){
	
	for (int i=dim-1; i >= 0; i--){
		x[i] = b[i];
		for (int j=i+1; j<dim; j++){
			x[i] -= A[i][j]*x[j];
		}
		x[i] = x[i] / A[i][i];
	}
}

//Função que realiza a soma do mínimo dos quadrados para verificar se Ax-b = 0
double itsRight(int dim, double** A, double* x, double* b){
    double* answer = (double*)malloc(sizeof(double)*dim);
    double rowSUM;

    for(int i = 0; i<dim; i++){
        rowSUM = 0;
        for(int j = 0; j<dim; j++){
            rowSUM += A[i][j]*x[j];
        }
        answer[i] = rowSUM;
    }

    double sumSQUARES = 0;
    double residual;
    for (int k = 0; k<dim; k++){
        residual = answer[k] - b[k];
        sumSQUARES += residual*residual;
    }
    sumSQUARES = sqrt(sumSQUARES);
}





int main(){
    srand(time(NULL));
    int dim;
    pthread_t *tid;
    double **A;
    double *b, *x;
    double inicio, fim, starting, conc;
    
	//Digitar um valor n para a matriz[n][n]
    puts("Digite a dimensao da matriz:\n");
    scanf("%d", &dim);
    puts("Digite o numero de threads que deseja utilizar:\n");
    scanf("%d", &nthreads);
    
    
    //Alocacao de memoria:
    GET_TIME(inicio);
    A = (double **)calloc(dim, sizeof(double*));
    for(int i = 0; i< dim; i++){
        A[i] = (double*)calloc(dim,sizeof(double*));
    }

    b = (double *)malloc(sizeof(double) * dim);
    x = (double *)malloc(sizeof(double) * dim);
    tid = (pthread_t*)malloc(sizeof(pthread_t)*nthreads);


    //Criando matrizes:
    for (int i=0; i<dim; i++){
        for (int j=0; j<dim; j++){
            A[i][j] = rand()%10 +1;
        }
        b[i] = rand()%100;
    }
    GET_TIME(fim);
    starting = fim-inicio;
    
    puts("\n Deseja ver a matriz inicial e o vetor solucao?\n");
    puts("0 nao \t 1 sim \n");
    int select;
    scanf("%d", &select);
    if (select == 1){

    //Mostrando os valores iniciais:
        puts("\n                STARTING MATRIX     \n");
        for (int i=0; i<dim; i++){
            for (int j=0; j<dim; j++){
                printf("%.4f ", A[i][j]);
            }
            puts("");
        }

        puts("\n                SOLUTION VECTOR     \n");
        for (int j=0; j<dim; j++){
            printf("|%.4f|", b[j]);
        }
    }else{
        goto HERE;
    }
    HERE:

    //Alocando valores da struct:
    GET_TIME(inicio);
    tArgs.A = A;
    tArgs.b = b;
    tArgs.dim = dim;

    //Criando as Threads:
    for( int j = 0; j< dim-1; j++){
        //Originalmente a ideia era introduzir uma função de pivoteamento nesse laço e compartilhar o 'j' para ler a matriz
        tArgs.j = j;
        for(long long int i = 0; i<nthreads; i++){
            if(pthread_create(tid+i, NULL, escalonamento, (void *)i)){
                puts("ERROR === 'thread create'\n");
                exit(-1);
            }
        }
        for(int i = 0; i < nthreads; i++){
            pthread_join(*(tid+i), NULL);
        }
    }
    GET_TIME(fim);
    conc = fim - inicio;
            
    puts("\n=================================================\n");
    puts("                  GAUSSIAN ELIMINATION                      \n");
    puts("=================================================\n");

    puts("\nDeseja verificar os vetores depois da eliminacao?\n");
    puts("0 nao \t 1 sim \n");
    int quero;
    scanf("%d",&quero);
    if(quero == 1){
        puts("\n                MATRIX A     \n");
        for (int i=0; i<dim; i++){
            for (int j=0; j<dim; j++){
                printf("%.4f ", A[i][j]);
            }
            puts("");
        }
        puts("\n                VECTOR B     \n");
        for (int j=0; j<dim; j++){
            printf("|%.4f|", b[j]);
        }
    }else{
        goto THERE;
    }
    THERE:
    backSUBS(dim, A, b, x);
    puts("\nDeseja visualizar o vetor X de solucao?\n");
    puts("0 nao \t 1 sim \n");
    int yes;
    scanf("%d",&yes);
    if(yes == 1){
        puts("\n                X VALUES     \n");
        for (int j=0; j<dim; j++){
            printf("|%.4f|", x[j]);
        }
    }else{
        goto OVER;
    }
    OVER:
    puts("\n****************************************************\n");
    double correct = itsRight(dim, A, x, b);
    printf("\n Ax-b vale: %.4f\n", correct);
    printf("\n A eliminacao gaussiana levou cerca de %lf segundos de forma concorrente",starting+conc);
    free(A);
    free(b);
    free(tid);
    free(x);
    return puts("\nFIM\n");

}
