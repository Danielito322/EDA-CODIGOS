#include <iostream>
using namespace std;

class Cola {
private:
    int elementos[6];
    int frente;
    int final;

public:
    Cola() {
        frente = 0;
        final = 0;
    }

    bool estaVacia() {
        if (frente == final)
            return true;
        else
            return false;
    }

    bool estaLlena() {
    if ((final + 1) % 6 == frente)
            return true;
        else
            return false;
    }

    void enqueue(int valor) {
        if (estaLlena()) {
            cout << "No puedo entrar la cola esta llena" << endl;
            return;
        }
        elementos[final] = valor;
        final = (final + 1) % 6;


    }

    int dequeue() {
        if (estaVacia()) {
            cout << "La cola esta vacia" << endl;
            return -1;
        }
        int valor = elementos[frente];
        frente = (frente + 1) % 6;
        return valor;
    }

    void clean() {
        frente = 0;
        final = 0;
    }

    void imprimir() {
        if (estaVacia()) {
            cout << "Cola vacia" << endl;
            return;
        }

        int i = frente;
    while (i != final) {
        cout << elementos[i] << " ";
        i = (i + 1) % 6;
    }
    cout << endl;
}
};


int main() {
    Cola c;

    c.enqueue(10);
    c.enqueue(20);
    c.enqueue(30);
    c.enqueue(100);

    c.imprimir();

    cout << "Sale: " << c.dequeue() << endl;

    c.imprimir();

    return 0;
}
