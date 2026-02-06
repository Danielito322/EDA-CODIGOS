#include <iostream>
#include <chrono>
#include <cstdlib> // rand()
using namespace std;
using namespace chrono;

// Metodo burbuja para float
void burbuja(float arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                float temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n = 5000;               // cantidad inicial
    const int LIMITE_TIEMPO = 60; // segundos

    while (true) {
        float* arr = new float[n];

        // Llenar arreglo con numeros flotantes aleatorios
        for (int i = 0; i < n; i++) {
            arr[i] = static_cast<float>(rand()) / RAND_MAX * 100000;
        }

        auto inicio = high_resolution_clock::now();
        burbuja(arr, n);
        auto fin = high_resolution_clock::now();

        duration<double> tiempo = fin - inicio;

        cout << "Elementos: " << n
             << " | Tiempo: " << tiempo.count() << " segundos" << endl;

        delete[] arr;

        // Si pasa de 1 minuto, se detiene
        if (tiempo.count() >= LIMITE_TIEMPO) {
            cout << "\nMaximo aproximado de elementos ordenados en menos de 1 minuto: "
                 << n << endl;
            break;
        }

        n += 5000; // aumenta de 1000 en 1000
    }

    return 0;
}
