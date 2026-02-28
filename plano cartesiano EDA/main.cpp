#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
using namespace std;

const string RESET  = "\033[0m";
const string COLORES[] = {
    "\033[31m",
    "\033[32m",
    "\033[33m",
    "\033[34m",
    "\033[35m",
    "\033[36m",
    "\033[91m",
    "\033[92m",
};
const int NUM_COLORES = 8;

struct Punto {
    string nombre;
    int x, y;
    int grupo;
};

struct Grupo {
    string nombre;
};

class plano_cartesiano {
private:
    int t;
    Punto puntos[100];
    int n;
    Grupo grupos[50];
    int numGrupos;

    string colorGrupo(int g) {
        if (g < 0) return RESET;
        return COLORES[g % NUM_COLORES];
    }

public:
    void pedir_datos() {
        cout << "Colocar dimension del plano cartesiano: ";
        cin >> t;

        cout << "Cuantos puntos desea ingresar? ";
        cin >> n;
        for (int i = 0; i < n; i++) {
            cout << "\n--- Punto " << i + 1 << " ---" << endl;
            cout << "Nombre del punto: ";
            cin >> puntos[i].nombre;
            cout << "Coordenada x: ";
            cin >> puntos[i].x;
            cout << "Coordenada y: ";
            cin >> puntos[i].y;
            puntos[i].grupo = -1;
        }

        cout << "\nCuantos grupos desea crear? ";
        cin >> numGrupos;
        for (int i = 0; i < numGrupos; i++) {
            cout << "Nombre del grupo " << i + 1 << ": ";
            cin >> grupos[i].nombre;
        }

        asignar_inicial();
    }

    double distancia_pts(int i, int j) {
        return sqrt(pow(puntos[i].x - puntos[j].x, 2.0) +
                    pow(puntos[i].y - puntos[j].y, 2.0));
    }

    double distancia_xy(double x1, double y1, double x2, double y2) {
        return sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0));
    }

    void asignar_inicial() {
        // si hay menos puntos que grupos, cada punto a un grupo distinto
        if (n <= numGrupos) {
            for (int i = 0; i < n; i++)
                puntos[i].grupo = i;
            return;
        }

        // primer punto al grupo 0
        puntos[0].grupo = 0;

        // asegurar que cada grupo tenga al menos un punto semilla
        // usando los puntos mas alejados entre si
        bool grupoOcupado[50] = {false};
        grupoOcupado[0] = true;

        for (int g = 1; g < numGrupos; g++) {
            double maxDist = -1;
            int candidato = 1;
            for (int i = 1; i < n; i++) {
                double minD = 1e9;
                for (int j = 0; j < i; j++) {
                    if (puntos[j].grupo >= 0) {
                        double d = distancia_pts(i, j);
                        if (d < minD) minD = d;
                    }
                }
                if (minD > maxDist) {
                    maxDist = minD;
                    candidato = i;
                }
            }
            puntos[candidato].grupo = g;
            grupoOcupado[g] = true;
        }

        // asignar el resto por knn con los ya asignados
        for (int i = 0; i < n; i++) {
            if (puntos[i].grupo != -1) continue;

            double dists[100];
            int indices[100];
            int count = 0;
            for (int j = 0; j < n; j++) {
                if (puntos[j].grupo == -1) continue;
                dists[count] = distancia_pts(i, j);
                indices[count] = j;
                count++;
            }

            for (int a = 0; a < count - 1; a++)
                for (int b = a + 1; b < count; b++)
                    if (dists[b] < dists[a]) {
                        swap(dists[a], dists[b]);
                        swap(indices[a], indices[b]);
                    }

            int k = (int)sqrt((double)count);
            if (k < 1) k = 1;
            int limite = (k < count) ? k : count;

            int votos[50] = {0};
            for (int j = 0; j < limite; j++)
                votos[puntos[indices[j]].grupo]++;

            int ganador = 0;
            for (int g = 1; g < numGrupos; g++)
                if (votos[g] > votos[ganador]) ganador = g;

            puntos[i].grupo = ganador;
        }
    }

    int knn(double px, double py, int k) {
        double dists[100];
        int indices[100];
        for (int i = 0; i < n; i++) {
            dists[i] = distancia_xy(px, py, puntos[i].x, puntos[i].y);
            indices[i] = i;
        }

        for (int i = 0; i < n - 1; i++)
            for (int j = i + 1; j < n; j++)
                if (dists[j] < dists[i]) {
                    swap(dists[i], dists[j]);
                    swap(indices[i], indices[j]);
                }

        int votos[50] = {0};
        int limite = (k < n) ? k : n;
        cout << "\n  vecinos mas cercanos considerados:" << endl;
        for (int i = 0; i < limite; i++) {
            int idx = indices[i];
            votos[puntos[idx].grupo]++;
            cout << "   " << colorGrupo(puntos[idx].grupo)
                 << puntos[idx].nombre
                 << " (dist: " << fixed << setprecision(2) << dists[i]
                 << ", grupo: " << grupos[puntos[idx].grupo].nombre << ")"
                 << RESET << endl;
        }

        int ganador = 0;
        for (int g = 1; g < numGrupos; g++)
            if (votos[g] > votos[ganador]) ganador = g;
        return ganador;
    }

    void mostrar_grupos() {
        cout << "\n--- grupos formados ---" << endl;
        for (int g = 0; g < numGrupos; g++) {
            cout << colorGrupo(g) << "\ngrupo [" << grupos[g].nombre << "]:" << RESET << endl;
            bool alguno = false;
            for (int i = 0; i < n; i++) {
                if (puntos[i].grupo == g) {
                    cout << colorGrupo(g)
                         << "   " << puntos[i].nombre
                         << " (" << puntos[i].x << ", " << puntos[i].y << ")"
                         << RESET << endl;
                    alguno = true;
                }
            }
            if (!alguno) cout << "   (sin puntos)" << endl;
        }
    }

    void vecino_mas_cercano() {
        if (n < 2) { cout << "se necesitan al menos 2 puntos." << endl; return; }
        cout << "\n--- vecino mas cercano por punto ---" << endl;
        for (int i = 0; i < n; i++) {
            int vecino = -1;
            double minDist = 1e9;
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                double d = distancia_pts(i, j);
                if (d < minDist) { minDist = d; vecino = j; }
            }
            cout << colorGrupo(puntos[i].grupo) << puntos[i].nombre << RESET
                 << " -> vecino mas cercano: "
                 << colorGrupo(puntos[vecino].grupo) << puntos[vecino].nombre << RESET
                 << " (dist: " << fixed << setprecision(2) << minDist << ")" << endl;
        }
    }

    void agregar_nuevo_punto() {
        if (n >= 100) { cout << "limite de puntos alcanzado." << endl; return; }

        cout << "\n--- agregar nuevo punto ---" << endl;
        cout << "nombre del punto: ";
        cin >> puntos[n].nombre;
        cout << "coordenada x: ";
        cin >> puntos[n].x;
        cout << "coordenada y: ";
        cin >> puntos[n].y;

        int k = (int)sqrt((double)n);
        if (k < 1) k = 1;

        int grupoAsignado = knn(puntos[n].x, puntos[n].y, k);
        puntos[n].grupo = grupoAsignado;
        n++;

        cout << "\n=> " << colorGrupo(grupoAsignado)
             << puntos[n-1].nombre
             << " fue asignado al grupo [" << grupos[grupoAsignado].nombre << "]"
             << RESET << endl;
    }

    void dibujar() {
        cout << "\n--- plano cartesiano ---" << endl;
        for (int i = t; i >= 0; i--) {
            cout << setw(3) << i;
            for (int j = 0; j <= t; j++) {
                bool encontrado = false;
                for (int k = 0; k < n; k++) {
                    if (puntos[k].x == j && puntos[k].y == i) {
                        cout << colorGrupo(puntos[k].grupo)
                             << setw(3) << puntos[k].nombre[0]
                             << RESET;
                        encontrado = true;
                        break;
                    }
                }
                if (!encontrado) {
                    if (i == 0)      cout << setw(3) << "-";
                    else if (j == 0) cout << setw(3) << "|";
                    else             cout << setw(3) << ".";
                }
            }
            cout << endl;
        }
        cout << setw(3) << " ";
        for (int j = 0; j <= t; j++) cout << setw(3) << j;
        cout << endl;

        cout << "\n--- leyenda ---" << endl;
        for (int g = 0; g < numGrupos; g++) {
            cout << colorGrupo(g) << "grupo [" << grupos[g].nombre << "]" << RESET << ": ";
            for (int i = 0; i < n; i++)
                if (puntos[i].grupo == g)
                    cout << colorGrupo(g) << puntos[i].nombre[0]
                         << "=" << puntos[i].nombre << RESET << "  ";
            cout << endl;
        }
    }
};

int main() {
    plano_cartesiano plano;
    plano.pedir_datos();
    plano.dibujar();
    plano.mostrar_grupos();
    plano.vecino_mas_cercano();

    while (true) {
        cout << "\n+---------------------------+" << endl;
        cout << "|  1. agregar nuevo punto   |" << endl;
        cout << "|  2. salir                 |" << endl;
        cout << "+---------------------------+" << endl;
        cout << "opcion: ";
        int op;
        cin >> op;
        if (op == 2) {
            cout << "saliendo..." << endl;
            break;
        }
        if (op == 1) {
            plano.agregar_nuevo_punto();
            plano.dibujar();
            plano.mostrar_grupos();
            plano.vecino_mas_cercano();
        }
    }

    return 0;
}
