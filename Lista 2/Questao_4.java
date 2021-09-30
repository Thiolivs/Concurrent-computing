/*Universidade Federal do Rio de Janeiro*/
/*Autor: Thiago de Oliveira Silva 	DRE: 111466197*/
/*Disciplina: Computação concorrente*/

/* Questão 4 */

// Monitor que implementa a logica do padrao leitores/escritores
class LE {
  private int leit, escr, countEscr;
  
  // Construtor
  LE() { 
    this.leit = 0;        //leitores lendo (0 ou mais)
    this.escr = 0;        //escritor escrevendo (0 ou 1)
    this.countEscr = 0;   //contador para escritores
  } 
  
  // Entrada para leitores
  public synchronized void EntraLeitor (int id) {
    try { 
      while (this.escr > 0 || this.countEscr>0) { 
        System.out.println ("Leitor ("+id+") Bloqueado" );
        wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.leit++;  //registra que ha mais um leitor lendo
      System.out.println ("Leitor ("+id+") Lendo" );
    } catch (InterruptedException e) { }
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor (int id) {
    this.leit--; //registra que um leitor saiu
    if (this.leit == 0)
        this.notifyAll(); //usado para liberar todos os escritores
    System.out.println ("Leitor ("+id+") Saindo");
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor (int id) {
    try {
      this.countEscr++; 
      while (this.leit > 0 || this.escr > 0) {
        System.out.println ("Escritor ("+id+") Bloqueado"); 
        wait();  //bloqueia pela condicao logica da aplicacao 
      }

      //registra que ha um escritor escrevendo
      System.out.println ("=== Escritor ("+id+") Escrevendo ");

      this.escr++;
      this.countEscr--;
    } catch (InterruptedException e) { }
    
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (int id) {
    this.escr--; //registra que o escritor saiu
    notify();  
    System.out.println ("=== Escritor ("+id+") Saindo ");
  }
}
  
// Leitor
class Leitor extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo
  LE monitor;//objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Leitor (int id, int delayTime, LE m) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {
    double j=777777777.7, i;
    try {
      for (;;) {
        this.monitor.EntraLeitor(this.id);
        for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de leitura
        this.monitor.SaiLeitor(this.id);
        sleep(this.delay); 
      }
    } catch (InterruptedException e) { return; }
  }
}

//--------------------------------------------------------
// Escritor
class Escritor extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo...
  LE monitor; //objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Escritor (int id, int delayTime, LE m) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {
    double j=777777777.7, i;
    try {
      for(;;) {
        this.monitor.EntraEscritor(this.id); 
        for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de escrita  
        this.monitor.SaiEscritor(this.id); 
        sleep(this.delay); //atraso bobo...
      }
    } catch (InterruptedException e) { return; }
  }
}

//--------------------------------------------------------
// Classe principal
class Questao_4 {
  static final int L = 3;
  static final int E = 3;

  public static void main (String[] args) {
    int i;
    LE monitor = new LE();            // Monitor (objeto compartilhado entre leitores e escritores)
    Leitor[] l = new Leitor[L];       // Threads leitores
    Escritor[] e = new Escritor[E];   // Threads escritores
    
    for (i=0; i<L; i++) {
      l[i] = new Leitor(i+1, (i+1)*500, monitor);
      l[i].start(); 
    }
    for (i=0; i<E; i++) {
      e[i] = new Escritor(i+1, (i+1)*500, monitor);
      e[i].start(); 
    }  
  }
}