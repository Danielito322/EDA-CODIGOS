#include <iostream>
using namespace std;


class Pila {
private:
    int elementos[6];
    int indice;

public:
    Pila() {
        indice = 0;
    }

    bool estaVacia() {
        if (indice == 0)
            return true;
        else
            return false;
    }

    bool estaLlena() {
        if (indice == 6)
            return true;
        else
            return false;
    }

    void push(int valor) {
        if (estaLlena()) {
            cout << "No puedo entrar la pila esta pila llena" << endl;
            return;
        }
        elementos[indice] = valor;
        indice++;
    }

    int pop() {
        if (estaVacia()) {
            cout << "la pila esta vacia" << endl;
            return -1;
        }
        indice--;
        return elementos[indice];
    }

    void clean() {
        indice = 0;
    }

    void imprimir() {
        if (estaVacia()) {
            cout << "Pila vacia" << endl;
            return;
        }
        for (int i = 0; i < indice; i++) {
            cout << elementos[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    Pila p;

    p.push(10);
    p.push(20);
    p.push(30);
    p.push(100);

    p.imprimir();

    cout << "se elimina: " << p.pop() << endl;

    p.imprimir();

    p.clean();
    p.imprimir();

    return 0;
}
