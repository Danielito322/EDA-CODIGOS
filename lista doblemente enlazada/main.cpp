#include <iostream>
using namespace std;

class Nodo {
public:
    int dato;
    Nodo* siguiente;
    Nodo* anterior;

    Nodo(int valor) {
        dato = valor;
        siguiente = NULL;
        anterior = NULL;
    }
};

class ListaDoble {
private:
    Nodo* inicio;
    Nodo* fin;

public:
    ListaDoble() {
        inicio = NULL;
        fin = NULL;
    }

    // agregar al inicio
    void addInicio(int valor) {
        Nodo* nuevo = new Nodo(valor);

        if (inicio == NULL) {
            inicio = nuevo;
            fin = nuevo;
        } else {
            nuevo->siguiente = inicio;
            inicio->anterior = nuevo;
            inicio = nuevo;
        }
    }

    //agregar al final
    void addFinal(int valor) {
        Nodo* nuevo = new Nodo(valor);

        if (fin == NULL) {
            inicio = nuevo;
            fin = nuevo;
        } else {
            fin->siguiente = nuevo;
            nuevo->anterior = fin;
            fin = nuevo;
        }
    }

    // eliminar el inicio
    void dropInicio() {
        if (inicio == NULL) {
            cout << "Lista vacia" << endl;
            return;
        }

        Nodo* temp = inicio;
        inicio = inicio->siguiente;

        if (inicio != NULL)
            inicio->anterior = NULL;
        else
            fin = NULL;

        delete temp;
    }

    void print() {
        Nodo* aux = inicio;
        while (aux != NULL) {
            cout << aux->dato << " ";
            aux = aux->siguiente;
        }
        cout << endl;
    }

    void printReverso() {
        Nodo* aux = fin;
        while (aux != NULL) {
            cout << aux->dato << " ";
            aux = aux->anterior;
        }
        cout << endl;
    }
};

int main() {
    ListaDoble l;

    l.addInicio(10);
    l.addInicio(20);
    l.addFinal(30);
    l.addFinal(40);

    l.print();          // 20 10 30 40
    l.printReverso();   // 40 30 10 20

    l.dropInicio();
    l.print();          // 10 30 40

    return 0;
}
