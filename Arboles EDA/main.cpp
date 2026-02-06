#include <iostream>
#include <string>
#include <cmath>
using namespace std;


class Nodo {
public:
    char valor;
    int contador;
    Nodo* izq;
    Nodo* der;
    Nodo(char v) {
        valor = v;
        contador = 1;
        izq = NULL;
        der = NULL;
    }
};


class Arbol {
public:
    Nodo* raiz;
    Arbol() {
        raiz = NULL;
    }
    void Insertar(char);
    void ImprimirGrafico();
    void Preorden();
    void Inorden();
    void Posorden();
    bool Buscar(char);
    void Eliminar(char);

private:
    Nodo* insertar(Nodo*, char);
    void imprimirArbol(Nodo*, int, int, int);
    int altura(Nodo*);
    void preorden(Nodo*);
    void inorden(Nodo*);
    void posorden(Nodo*);
    bool buscar(Nodo*, char);
    Nodo* eliminar(Nodo*, char);
    Nodo* encontrarMinimo(Nodo*);
};

// insertar
void Arbol::Insertar(char v) {
    raiz = insertar(raiz, v);
}

Nodo* Arbol::insertar(Nodo* r, char v) {
    if (r == NULL)
        return new Nodo(v);
    if (v < r->valor)
        r->izq = insertar(r->izq, v);
    else if (v >= r->valor)
        r->der = insertar(r->der, v);
    else
        r->contador++;
    return r;
}

//altura
int Arbol::altura(Nodo* r) {
    if (r == NULL)
        return 0;
    int altIzq = altura(r->izq);
    int altDer = altura(r->der);
    return 1 + (altIzq > altDer ? altIzq : altDer);
}

//imprimir
void Arbol::ImprimirGrafico() {
    if (raiz == NULL) {
        cout << "Arbol vacio\n";
        return;
    }

    int h = altura(raiz);
    int espacioTotal = pow(2, h) * 3;

    cout << "\n";
    imprimirArbol(raiz, espacioTotal / 2, 0, espacioTotal / 4);
    cout << "\n";
}

void Arbol::imprimirArbol(Nodo* r, int pos, int nivel, int separacion) {
    if (r == NULL)
        return;

    for (int i = 0; i < pos; i++)
        cout << " ";

    cout << r->valor;
    if (r->contador > 1)
        cout << "(" << r->contador << ")";
    cout << "\n";

    if (r->izq != NULL || r->der != NULL) {
        for (int i = 0; i < pos - separacion; i++)
            cout << " ";

        if (r->izq != NULL) {
            cout << "/";
        } else {
            cout << " ";
        }

        for (int i = 0; i < separacion * 2 - 1; i++)
            cout << " ";

        if (r->der != NULL) {
            cout << "\\";
        }
        cout << "\n";
    }

    // Imprimir subárboles
    if (r->izq != NULL)
        imprimirArbol(r->izq, pos - separacion, nivel + 1, separacion / 2);

    if (r->der != NULL)
        imprimirArbol(r->der, pos + separacion, nivel + 1, separacion / 2);
}

// buscar
bool Arbol::Buscar(char v) {
    return buscar(raiz, v);
}

bool Arbol::buscar(Nodo* r, char v) {
    if (r == NULL)
        return false;
    if (v == r->valor)
        return true;
    if (v < r->valor)
        return buscar(r->izq, v);
    else
        return buscar(r->der, v);
}

//eliminar
void Arbol::Eliminar(char v) {
    raiz = eliminar(raiz, v);
}

Nodo* Arbol::encontrarMinimo(Nodo* r) {
    while (r->izq != NULL)
        r = r->izq;
    return r;
}

Nodo* Arbol::eliminar(Nodo* r, char v) {
    if (r == NULL) {
        return NULL;
    }

    if (v < r->valor) {
        r->izq = eliminar(r->izq, v);
    }
    else if (v > r->valor) {
        r->der = eliminar(r->der, v);
    }
    else {
        if (r->contador > 1) {
            r->contador--;
            return r;
        }

        if (r->izq == NULL && r->der == NULL) {
            delete r;
            return NULL;
        }
        else if (r->izq == NULL) {
            Nodo* temp = r->der;
            delete r;
            return temp;
        }
        else if (r->der == NULL) {
            Nodo* temp = r->izq;
            delete r;
            return temp;
        }
        else {
            Nodo* temp = encontrarMinimo(r->der);
            r->valor = temp->valor;
            r->contador = temp->contador;
            temp->contador = 1;
            r->der = eliminar(r->der, temp->valor);
        }
    }
    return r;
}


void Arbol::Preorden() {
    cout << "PREORDEN: ";
    preorden(raiz);
    cout << endl;
}

void Arbol::Inorden() {
    cout << "INORDEN: ";
    inorden(raiz);
    cout << endl;
}

void Arbol::Posorden() {
    cout << "POSORDEN: ";
    posorden(raiz);
    cout << endl;
}

void Arbol::preorden(Nodo* r) {
    if (r != NULL) {
        for (int i = 0; i < r->contador; i++)
            cout << r->valor << " ";
        preorden(r->izq);
        preorden(r->der);
    }
}

void Arbol::inorden(Nodo* r) {
    if (r != NULL) {
        inorden(r->izq);
        for (int i = 0; i < r->contador; i++)
            cout << r->valor << " ";
        inorden(r->der);
    }
}

void Arbol::posorden(Nodo* r) {
    if (r != NULL) {
        posorden(r->izq);
        posorden(r->der);
        for (int i = 0; i < r->contador; i++)
            cout << r->valor << " ";
    }
}


int main() {
    Arbol a;

    cout << "===== ARBOL BINARIO DE BUSQUEDA =====\n\n";

    string texto;
    cout << "Ingresa un texto: ";
    getline(cin, texto);

    cout << "\nInsertando letras...\n";
    for (int i = 0; i < texto.length(); i++) {
        if (texto[i] != ' ') {
            a.Insertar(texto[i]);
        }
    }

    cout << "\n===== ARBOL INICIAL =====";
    a.ImprimirGrafico();

    cout << "\n===== RECORRIDOS =====\n";
    a.Preorden();
    a.Inorden();
    a.Posorden();

    char buscarLetra;
    cout << "\nIngresa una letra a buscar: ";
    cin >> buscarLetra;

    if (a.Buscar(buscarLetra))
        cout << "La letra '" << buscarLetra << "' se encuentra en el arbol.\n";
    else
        cout << "La letra '" << buscarLetra << "' no se encuentra en el arbol.\n";

    char eliminarLetra;
    cout << "\nIngresa una letra a eliminar: ";
    cin >> eliminarLetra;
    a.Eliminar(eliminarLetra);

    cout << "\n===== ARBOL DESPUES DE ELIMINAR =====";
    a.ImprimirGrafico();

    cout << "\n===== RECORRIDOS =====\n";
    a.Preorden();
    a.Inorden();
    a.Posorden();

    return 0;
}
