#include <iostream>
#include <string>
#include "codificar.h"  // incluir el encabezado
#include "decodificar.h"

using namespace std;

int main() {
    string nombreArchivo, nombreSalida;
    int semilla;
    int metodo;
    int tipo;

    cout << "===== CODIFICADOR DE ARCHIVOS =====" << endl;

    // Solicitar datos al usuario
    cout << "Ingrese el nombre del archivo a decodificar: ";
    cin >> nombreArchivo;

    cout << "Ingrese el nombre del archivo a salida: ";
    cin >> nombreSalida;

    cout << "Ingrese la semilla (n): ";
    cin >> semilla;

    cout << "Seleccione el metodo (1 o 2): ";
    cin >> metodo;

    cout << "Seleccione el tipo de manejo:\n";
    cout << " 1. String\n";
    cout << " 2. Arreglo de char\n";
    cout << "Opcion: ";
    cin >> tipo;

    // Llamar a la función codificar
    //codificar(nombreArchivo,semilla, metodo, tipo);
    decodificar(nombreArchivo,nombreSalida, semilla, metodo);



    cout << "\nProceso finalizado.\n";

    return 0;
}

