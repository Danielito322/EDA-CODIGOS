#include <iostream>
using namespace std;

class Nodo {
public:
    int dato;
    Nodo* siguiente;

    Nodo(int valor) {
        dato = valor;
        siguiente = NULL;
    }
};

class Lista {
private:
    Nodo* inicio;

public:
    // Constructor
    Lista() {
        inicio = NULL;
    }

    // Agregar al INICIO
    void add(int valor) {
        Nodo* nuevo = new Nodo(valor);
        nuevo->siguiente = inicio;
        inicio = nuevo;
    }

    // Agregar al FINAL
    void addEnd(int valor) {
        Nodo* nuevo = new Nodo(valor);

        if (inicio == NULL) {
            inicio = nuevo;
            return;
        }

        Nodo* aux = inicio;
        while (aux->siguiente != NULL) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo;
    }

    // Agregar en una POSICIÓN
    void addPosicion(int valor, int posicion) {
        if (posicion == 0) {
            add(valor);
            return;
        }

        Nodo* nuevo = new Nodo(valor);
        Nodo* aux = inicio;

        for (int i = 0; i < posicion - 1; i++) {
            if (aux == NULL) {
                cout << "Posicion invalida" << endl;
                delete nuevo;
                return;
            }
            aux = aux->siguiente;
        }

        nuevo->siguiente = aux->siguiente;
        aux->siguiente = nuevo;
    }

    // Eliminar del INICIO
    void drop() {
        if (inicio == NULL) {
            cout << "Lista vacia" << endl;
            return;
        }

        Nodo* temp = inicio;
        inicio = inicio->siguiente;
        delete temp;
    }

    // Imprimir lista
    void print() {
        if (inicio == NULL) {
            cout << "Lista vacia" << endl;
            return;
        }

        Nodo* aux = inicio;
        while (aux != NULL) {
            cout << aux->dato << " ";
            aux = aux->siguiente;
        }
        cout << endl;
    }
};

int main() {
    Lista l;

    // Agregar al inicio
    l.add(1);
    l.add(2);
    l.add(3);
    // Lista: 3 2 1

    // Agregar al final
    l.addEnd(9);
    // Lista: 3 2 1 9

    // Agregar en posición
    l.addPosicion(5, 2);
    // Lista: 3 2 5 1 9

    l.print();

    // Eliminar inicio
    l.drop();
    // Lista: 2 5 1 9

    l.print();

    return 0;
}
