#include <iostream>
using namespace std;

/* ======================
   NODO
====================== */
class Nodo {
public:
    int dato;
    Nodo* siguiente;

    Nodo(int valor) {
        dato = valor;
        siguiente = NULL;
    }
};

/* ======================
   LISTA SIMPLE
====================== */
class Lista {
private:
    Nodo* inicio;

public:
    Lista() {
        inicio = NULL;
    }

    bool isEmpty() {
        return inicio == NULL;
    }

    // Agregar al INICIO
    void addStart(int valor) {
        Nodo* nuevo = new Nodo(valor);
        nuevo->siguiente = inicio;
        inicio = nuevo;
    }

    // Agregar al FINAL
    void addEnd(int valor) {
        Nodo* nuevo = new Nodo(valor);

        if (isEmpty()) {
            inicio = nuevo;
            return;
        }

        Nodo* aux = inicio;
        while (aux->siguiente != NULL) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo;
    }

    // Eliminar del INICIO
    int dropStart() {
        if (isEmpty()) {
            cout << "Lista vacia\n";
            return -1;
        }

        Nodo* temp = inicio;
        int valor = temp->dato;
        inicio = inicio->siguiente;
        delete temp;
        return valor;
    }

    // Eliminar del FINAL
    int dropEnd() {
        if (isEmpty()) {
            cout << "Lista vacia\n";
            return -1;
        }

        if (inicio->siguiente == NULL) {
            int valor = inicio->dato;
            delete inicio;
            inicio = NULL;
            return valor;
        }

        Nodo* aux = inicio;
        while (aux->siguiente->siguiente != NULL) {
            aux = aux->siguiente;
        }

        int valor = aux->siguiente->dato;
        delete aux->siguiente;
        aux->siguiente = NULL;
        return valor;
    }

    void print() {
        Nodo* aux = inicio;
        while (aux != NULL) {
            cout << aux->dato << " ";
            aux = aux->siguiente;
        }
        cout << endl;
    }
};

/* ======================
   PILA (STACK)
====================== */
class Pila {
private:
    Lista elementos;

public:
    void push(int valor) {
        elementos.addEnd(valor);
    }

    int pop() {
        return elementos.dropEnd();
    }

    void print() {
        elementos.print();
    }
};

/* ======================
   COLA (QUEUE)
====================== */
class Cola {
private:
    Lista elementos;

public:
    void enqueue(int valor) {
        elementos.addEnd(valor);
    }

    int dequeue() {
        return elementos.dropStart();
    }

    void print() {
        elementos.print();
    }
};

/* ======================
   MAIN
====================== */
int main() {

    cout << "=== PILA ===\n";
    Pila p;
    p.push(10);
    p.push(20);
    p.push(30);
    p.print();

    cout << "Pop: " << p.pop() << endl;
    p.print();

    cout << "\n=== COLA ===\n";
    Cola c;
    c.enqueue(1);
    c.enqueue(2);
    c.enqueue(3);
    c.print();

    cout << "Dequeue: " << c.dequeue() << endl;
    c.print();

    return 0;
}
