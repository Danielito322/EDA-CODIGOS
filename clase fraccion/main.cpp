#include <iostream>
using namespace std;

class Fraccion {
public:
    int numerador;
    int denominador;

    Fraccion(int n, int d) {
        numerador = n;
        if (d == 0)
            denominador = 1;
        else
            denominador = d;
    }

    Fraccion sumar(Fraccion f) {
        int n = numerador * f.denominador + f.numerador * denominador;
        int d = denominador * f.denominador;
        return Fraccion(n, d);
    }

    Fraccion restar(Fraccion f) {
        int n = numerador * f.denominador - f.numerador * denominador;
        int d = denominador * f.denominador;
        return Fraccion(n, d);
    }

    Fraccion multiplicar(Fraccion f) {
        int n = numerador * f.numerador;
        int d = denominador * f.denominador;
        return Fraccion(n, d);
    }

    Fraccion dividir(Fraccion f) {
        int n = numerador * f.denominador;
        int d = denominador * f.numerador;
        return Fraccion(n, d);
    }

    void mostrar() {
        cout << numerador << "/" << denominador << endl;
    }
};




int main() {

    Fraccion f1(2, 1);
    Fraccion f2(3, 4);

    Fraccion suma = f1.sumar(f2);
    Fraccion resta = f1.restar(f2);
    Fraccion multiplicacion = f1.multiplicar(f2);
    Fraccion division  = f1.dividir(f2);

    cout << "Suma: ";
    suma.mostrar();

    cout << "Resta: ";
    resta.mostrar();

    cout << "Multiplicacion: ";
    multiplicacion.mostrar();

    cout << "Division: ";
    division.mostrar();

    return 0;
}














