#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <conio.h>
using namespace std;

/* =======================
   NODO DOBLEMENTE ENLAZADO
======================= */
class Nodo {
public:
    int id;
    int idPadre;
    int idMadre;
    string paterno, materno, nombres;
    string telefono, correo, direccion;
    Nodo* next;
    Nodo* prev;

    Nodo(int _id, int _idPadre, int _idMadre,
         string _pat, string _mat, string _nom,
         string _tel, string _cor, string _dir)
        : id(_id), idPadre(_idPadre), idMadre(_idMadre),
          paterno(_pat), materno(_mat), nombres(_nom),
          telefono(_tel), correo(_cor), direccion(_dir),
          next(nullptr), prev(nullptr) {}
};

/* =======================
   CLASE AGENDA
======================= */
class Agenda {
private:
    Nodo* head;
    int idActual;

public:
    Agenda() {
        head = nullptr;
        idActual = 1;
        cargarDesdeArchivo();
    }

    void agregar();
    void mostrar();
    void buscarPorId(int);
    void buscarPorNombre(string);
    void modificar(int);
    void eliminar(int);
    void guardarEnArchivo();
    void cargarDesdeArchivo();
};

/* =======================
   MÉTODOS
======================= */

void Agenda::agregar() {
    string pat, mat, nom, tel, cor, dir;
    int idPadre, idMadre;

    system("cls");
    cout << "=== AGREGAR CONTACTO ===\n\n";
    cout << "Apellido paterno : "; getline(cin, pat);
    cout << "Apellido materno : "; getline(cin, mat);
    cout << "Nombres          : "; getline(cin, nom);
    cout << "Telefono         : "; getline(cin, tel);
    cout << "Correo           : "; getline(cin, cor);
    cout << "Direccion        : "; getline(cin, dir);

    cout << "ID del padre (0 si no tiene): ";
    cin >> idPadre;
    cout << "ID de la madre (0 si no tiene): ";
    cin >> idMadre;
    cin.ignore();

    Nodo* nuevo = new Nodo(idActual++, idPadre, idMadre,
                           pat, mat, nom, tel, cor, dir);

    if (!head) head = nuevo;
    else {
        Nodo* aux = head;
        while (aux->next) aux = aux->next;
        aux->next = nuevo;
        nuevo->prev = aux;
    }

    guardarEnArchivo();
    cout << "\nContacto agregado correctamente.\n";
}

void Agenda::mostrar() {
    system("cls");
    cout << "================= AGENDA =================\n\n";

    if (!head) {
        cout << "Agenda vacia.\n";
        return;
    }

    cout << left
         << setw(5)  << "ID"
         << setw(25) << "NOMBRE COMPLETO"
         << setw(10) << "PADRE"
         << setw(10) << "MADRE"
         << setw(15) << "TELEFONO"
         << setw(30) << "DIRECCION" << endl;

    cout << string(70, '-') << endl;

    Nodo* aux = head;
    while (aux) {
        cout << setw(5) << aux->id
     << setw(25) << (aux->paterno + " " + aux->materno + " " + aux->nombres)
     << setw(8) << aux->idPadre
     << setw(8) << aux->idMadre
     << setw(15) << aux->telefono
     << setw(30) << aux->direccion << endl;



        aux = aux->next;
    }
}

void Agenda::buscarPorId(int idBuscar) {
    system("cls");
    Nodo* aux = head;

    while (aux) {
        if (aux->id == idBuscar) {
            cout << "=== CONTACTO ENCONTRADO ===\n\n";
            cout << "ID        : " << aux->id << endl;
            cout << "Nombre    : " << aux->paterno << " "
                 << aux->materno << " " << aux->nombres << endl;
            cout << "Telefono  : " << aux->telefono << endl;
            cout << "Correo    : " << aux->correo << endl;
            cout << "Direccion : " << aux->direccion << endl;
            cout << "ID Padre  : " << aux->idPadre << endl;
            cout << "ID Madre  : " << aux->idMadre << endl;
            return;
        }
        aux = aux->next;
    }
    cout << "Contacto no encontrado.\n";
}

void Agenda::buscarPorNombre(string nombre) {
    system("cls");
    Nodo* aux = head;
    bool encontrado = false;

    cout << "=== RESULTADOS DE BUSQUEDA ===\n\n";

    while (aux) {
        if (aux->nombres == nombre) {
            cout << "ID: " << aux->id << " | "
                 << aux->paterno << " "
                 << aux->materno << " "
                 << aux->nombres << endl;
            encontrado = true;
        }
        aux = aux->next;
    }

    if (!encontrado)
        cout << "No se encontraron coincidencias.\n";
}

void Agenda::modificar(int idBuscar) {
    system("cls");
    Nodo* aux = head;

    while (aux) {
        if (aux->id == idBuscar) {
            cout << "=== MODIFICAR CONTACTO ===\n\n";
            cout << "Nuevo telefono : "; getline(cin, aux->telefono);
            cout << "Nuevo correo   : "; getline(cin, aux->correo);
            cout << "Nueva direccion: "; getline(cin, aux->direccion);
            guardarEnArchivo();
            cout << "\nContacto modificado correctamente.\n";
            return;
        }
        aux = aux->next;
    }
    cout << "ID no encontrado.\n";
}

void Agenda::eliminar(int idEliminar) {
    system("cls");
    Nodo* aux = head;

    while (aux) {
        if (aux->id == idEliminar) {
            if (aux->prev) aux->prev->next = aux->next;
            else head = aux->next;

            if (aux->next) aux->next->prev = aux->prev;

            delete aux;
            guardarEnArchivo();
            cout << "Contacto eliminado correctamente.\n";
            return;
        }
        aux = aux->next;
    }
    cout << "ID no encontrado.\n";
}

void Agenda::guardarEnArchivo() {
    ofstream file("agenda.txt");
    Nodo* aux = head;

    while (aux) {
        file << aux->id << ";"
             << aux->idPadre << ";"
             << aux->idMadre << ";"
             << aux->paterno << ";"
             << aux->materno << ";"
             << aux->nombres << ";"
             << aux->telefono << ";"
             << aux->correo << ";"
             << aux->direccion << "\n";
        aux = aux->next;
    }
    file.close();
}

void Agenda::cargarDesdeArchivo() {
    ifstream file("agenda.txt");
    string linea;

    while (getline(file, linea)) {
        stringstream ss(linea);
        string campo;
        int id, idPadre, idMadre;
        string pat, mat, nom, tel, cor, dir;

        getline(ss, campo, ';'); id = stoi(campo);
        getline(ss, campo, ';'); idPadre = stoi(campo);
        getline(ss, campo, ';'); idMadre = stoi(campo);
        getline(ss, pat, ';');
        getline(ss, mat, ';');
        getline(ss, nom, ';');
        getline(ss, tel, ';');
        getline(ss, cor, ';');
        getline(ss, dir, ';');

        Nodo* nuevo = new Nodo(id, idPadre, idMadre,
                               pat, mat, nom, tel, cor, dir);

        if (!head) head = nuevo;
        else {
            Nodo* aux = head;
            while (aux->next) aux = aux->next;
            aux->next = nuevo;
            nuevo->prev = aux;
        }

        if (id >= idActual)
            idActual = id + 1;
    }
    file.close();
}

/* =======================
   MAIN
======================= */
int main() {
    Agenda agenda;
    int opcion;

    do {
        system("cls");
        cout << "========= AGENDA DE CONTACTOS =========\n\n";
        cout << "1. Agregar contacto\n";
        cout << "2. Mostrar agenda\n";
        cout << "3. Buscar por ID\n";
        cout << "4. Buscar por nombre\n";
        cout << "5. Editar contacto\n";
        cout << "6. Eliminar contacto\n";
        cout << "7. Salir\n\n";
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();

        if (opcion == 1) agenda.agregar();
        else if (opcion == 2) agenda.mostrar();
        else if (opcion == 3) {
            int id;
            cout << "Ingrese ID: ";
            cin >> id;
            cin.ignore();
            agenda.buscarPorId(id);
        }
        else if (opcion == 4) {
            string nombre;
            cout << "Ingrese nombre: ";
            getline(cin, nombre);
            agenda.buscarPorNombre(nombre);
        }
        else if (opcion == 5) {
            int id;
            cout << "ID a modificar: ";
            cin >> id;
            cin.ignore();
            agenda.modificar(id);
        }
        else if (opcion == 6) {
            int id;
            cout << "ID a eliminar: ";
            cin >> id;
            cin.ignore();
            agenda.eliminar(id);
        }

        cout << "\nPresione una tecla para continuar...";
        getch();

    } while (opcion != 7);

    return 0;
}
