/*Universidade Federal do Rio de Janeiro*/
/*Autor: Thiago de Oliveira Silva 	DRE: 111466197*/
/*Disciplina: Computação concorrente - Laboratório 6 - Introducao a programacao concorrente em Java. */

/*A atividade foi feita usando a ferramenta VsCode, no sistema operacional Windows.*/

/*Sobre o programa:
Cria um vetor de tamanho baseado em NTHREADS. A principio ela tem tamanho fixo = 4.
O vetor é preenchido com valores aleatorios.
As threads contam o numero de elementos pares e armazenam em um contador que é imprimido ao final
Tambem imprime uma lista com todos os pares para melhor visualizacao.

Nota: Pode-se alterar o numero de threads no codigo e com isso, alteramos o tamanho do vetor. 
Esta foi a forma de testar utilizando diferentes numeros de threads e tamanho do vetor.*/

import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;



class P {           // Classe Par
    private V vec;
    private int nItems;
    private int NTHREADS;
    private int count = 0;

    List<Integer> listaPares = new ArrayList<Integer>();

    P(int nItems, int NTHREADS, V vec){
        this.nItems = nItems;
        this.vec = vec;
        this.NTHREADS = NTHREADS;
    }

    public synchronized void Par(int id){
        int inicio, fim;
        int tamBloco = this.nItems/this.NTHREADS;
        inicio = id*tamBloco;
        fim = id == NTHREADS-1? nItems: inicio + tamBloco;

        for(int i = inicio; i<fim; i++){
            if(this.vec.getIndex(i) % 2 == 0){
                listaPares.add(this.vec.getIndex(i));
                count++;
            }
        }
    }

    public int incrementa(){
        return this.count;
    } 

    public List<Integer> imprimepares(){
        return this.listaPares;
    } 
}

class T extends Thread{  //Classe Thread
    private int id;
    private P par;

    public T(int id, P par){
        this.id = id;
        this.par = par;
    }

    public void run(){
       this.par.Par(this.id);
    }
}

class V { // Classe Vector

    // gera numeros aleatorios para o vetor
    Random random = new Random(); 
    private List<Integer> V;

    public V(){
        this.V = new ArrayList<Integer>();
    }

    public V(int N){
        this.V = Arrays.asList(new Integer[N]);

        for(int i = 0; i<N; i++){
            //preenche com valores aleatorios
            int numero = random.nextInt(10); 
            this.V.set(i,numero);
        }
    }

    public int getIndex(int ind){
        return this.V.get(ind);
    }
}

public class Pares {

    public static void main(String[] args){

        int NTHREADS = 4;
        int nItems = 2*NTHREADS;
        V vec = new V(nItems);
        
        System.out.print("Vetor de numeros aleatorios:\n [");

        for(int i = 0; i<nItems; i++){
            System.out.print(vec.getIndex(i));
            if(i<nItems-1){
                System.out.print(", ");
            }
        }
        System.out.print("]\n");

        Thread[] threads = new Thread[NTHREADS];
        P par = new P(nItems, NTHREADS, vec);

        for(int i = 0; i<threads.length; i++){
            threads[i] = new T(i,par);
        }

        for(int i = 0; i<threads.length; i++){
            threads[i].start();
        }

        for(int i = 0; i<threads.length; i++){
            try{
                threads[i].join();
            }catch(InterruptedException IE){
                return;
            }
        }

        System.out.println("\nNumero de elementos Pares: " + par.incrementa() );

        System.out.println("\nOs elementos pares sao:" + par.imprimepares()+"\n");
    }
}