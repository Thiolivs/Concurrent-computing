#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"


void backSUBS(int dim, double* A, double* b, double* x){
	
	for (int i=dim-1; i >= 0; i--){
		x[i] = b[i];
		for (int j=i+1; j<dim; j++){
			x[i] -= A[i*dim+j]*x[j];
		}
		x[i] = x[i] / A[i*dim+i];
	}
}

double itsRight(int dim, double* A, double* x, double* b){
    double* answer = (double*)malloc(sizeof(double)*dim);
    double rowSUM;

    for(int i = 0; i<dim; i++){
        rowSUM = 0;
        for(int j = 0; j<dim; j++){
            rowSUM += A[i*dim+j]*x[j];
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
    double m, inicio, fim, starting, seq;
    int dim;
    puts("Digite a dimensao da matriz:\n");
    scanf("%d", &dim);
    double* A;
    double* b;
    double* x;
    
    GET_TIME(inicio);
    //Alocando memoria:
    A = (double*)malloc(sizeof(double)*dim*dim);
    b = (double*)malloc(sizeof(double)*dim);
    x = (double*)malloc(sizeof(double)*dim);
    //Criando matrizes:
    for (int i=0; i<dim; i++){
		for (int j=0; j<dim; j++){
			A[i*dim+j] = rand()%10 +1;
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
                printf("%.4f ", A[i*dim+j]);
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

    GET_TIME(inicio);
    //Gaussian
    for(int i = 0; i<dim-1; i++){
        for(int j = i+1; j<dim; j++){
            m = A[j*dim+i]/A[i*dim+i];
            for(int k = i; k<dim; k++){
                A[j*dim+k] = A[j*dim+k] - (m * A[i*dim+k]);
            }
            b[j] = b[j] - (m*b[i]);
        }
    }
    GET_TIME(fim);
    seq = fim - inicio;
    puts("\n=================================================\n");
    puts("                  GAUSSIAN ELIMINATION                      \n");
    puts("=================================================\n");
    puts("\nDeseja verificar os vetores depois da eliminacao?\n");
    puts("0 nao \t 1 sim \n");
    int ok;
    scanf("%d",&ok);
    if(ok == 1){
        puts("\n                MATRIX A     \n");
        for (int i=0; i<dim; i++){
            for (int j=0; j<dim; j++){
                printf("%.4f ", A[i*dim+j]);
            }
            puts("");
                        
        }
        puts("\n                VECTOR B     \n");
        for (int j=0; j<dim; j++){
            printf("|%.4f|", b[j]);
        }
    }else{
        goto END;
    }
    END:
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
    printf("\n A eliminacao gaussiana levou cerca de %lf segundos de forma sequencial.",starting+seq);
    free(A);
    free(b);
    free(x);
    return puts("\nFIM\n");
}
