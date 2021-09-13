/*Universidade Federal do Rio de Janeiro*/
/*Autor: Thiago de Oliveira Silva 	DRE: 111466197*/
/*Disciplina: Computação concorrente - Laboratório 7
Implementacao de uma aplicacao que usa o padrao leitores/escritores usando o conceito de monitores */

/*A atividade foi feita usando a ferramenta VsCode, no sistema operacional Windows.*/

/*A explicacao sobre o programa encontra-se no README do github*/

import java.util.List;
import java.util.Arrays;
import java.util.ArrayList;


class Leitor extends Thread {
    private N num;
    private Sync monitor;
    private int id, espera;

    static boolean ehPrimo(int n) {
        if (n == 0) {
            return false;
        }

        for (int j = 2; j < n; j++) {
            if (n % j == 0)
                return false;
        }
        return true;
    }

    Leitor(int id, int espera, Sync m, N num) {
        this.id = id;
        this.num = num;
        this.monitor = m;
        this.espera = espera;
    }

    public void run() {

        try {
            while (true) {
                this.monitor.entraLeitor(this.id);
                if (ehPrimo(this.num.index(0))) {
                    System.out.println("   O numero " + this.num.index(0) + " e primo");
                } else {
                    System.out.println("   O numero " + this.num.index(0) + " NAO e primo");
                }
                this.monitor.saiLeitor(this.id);
                sleep(this.espera);
            }
        } catch (InterruptedException e) {
            return;
        }
    }
}

class Escritor extends Thread {
    private N num;
    private Sync monitor;
    private int id, espera;


    Escritor(int id, int espera, Sync m, N num) {
        this.id = id;
        this.num = num;
        this.monitor = m;
        this.espera = espera;
    }

    public void run() {
        try {
            while (true) {

                this.monitor.entraEscritor(this.id);
                num.setIndex(0, id);
                System.out.println("------------------------- \nReescrevendo o numero: " + num.index(0));

                this.monitor.saiEscritor(this.id);
                sleep(this.espera);
            }
        } catch (InterruptedException e) {
            return;
        }
    }
}

class LeEscreve extends Thread {
    private N num;
    private Sync monitor;
    private int id, espera;

    LeEscreve(int id, int espera, Sync m, N num) {
        this.id = id;
        this.num = num;
        this.monitor = m;
        this.espera = espera;
    }

    public void run() {
        try {
            while (true) {
                this.monitor.entraLeitor(this.id);

                if (this.num.index(0) % 2 == 0) {
                    System.out.println("   O numero " + this.num.index(0) + " e PAR");
                } else {
                    System.out.println("   O numero " + this.num.index(0) + " e IMPAR");
                }

                this.monitor.saiLeitor(this.id);
                sleep(this.espera);
                this.monitor.entraEscritor(this.id);

                int aux = num.index(0);
                num.setIndex(0, 2 * num.index(0));
                System.out.println("   Numero " + aux + " multiplicado (X2): " + num.index(0));

                this.monitor.saiEscritor(this.id);
                sleep(this.espera);
            }
        } catch (InterruptedException e) {
            return;
        }
    }
}

class Sync {
    private int escritor, leitor;

    public Sync() {
        this.leitor = 0;
        this.escritor = 0;
    }

    public synchronized void entraLeitor(int id) {
        try {
            while (this.escritor > 0) {
                wait();
            }
            this.leitor++;
        } catch (InterruptedException e) {
        }
    }

    public synchronized void saiLeitor(int id) {
        this.leitor--;
        if (this.leitor == 0) {
            this.notify();
        }
    }

    public synchronized void entraEscritor(int id) {
        try {
            while ((this.escritor > 0) || (this.leitor > 0)) {
                wait();
            }
            this.escritor++;
        } catch (InterruptedException e) {
        }
    }

    public synchronized void saiEscritor(int id) {
        this.escritor--;
        notifyAll();
    }

}

class N {
    private List<Integer> N;

    public N() {
        this.N = new ArrayList<Integer>();
    }

    public N(int n) {
        this.N = Arrays.asList(new Integer[n]);
        this.N.set(0, 0);
    }

    public int index(int ind) {
        return this.N.get(ind);
    }

    public void setIndex(int ind, int item) {
        this.N.set(ind, item);
    }

}


class LeitoresEscritores {

    static final int E = 5, L= 5, LE = 5;

    public static void main(String[] args) {

        int i;
        N num = new N(1);
        Sync monitor = new Sync();

        Leitor[] l = new Leitor[L];
        Escritor[] e = new Escritor[E];
        LeEscreve[] le = new LeEscreve[LE];

        for (i = 0; i < L; i++) {
            l[i] = new Leitor(i, 1000, monitor, num);
            l[i].start();

        }
        for (i = 0; i < E; i++) {
            e[i] = new Escritor(i, 1000, monitor, num);
            e[i].start();
        }
        for (i = 0; i < LE; i++) {
            le[i] = new LeEscreve(i, 1000, monitor, num);
            le[i].start();
        }
    }

}
