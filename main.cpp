#include <iostream>
#include <string>
#include "codificar.h"  // incluir el encabezado
#include "decodificar.h"

using namespace std;

int main() {
    string nombreArchivo, nombreSalida;
    int semilla, metodo, opcion;
    while(1){
        cout << "Ingrese la opcion a realizar."<<endl;
        cout << "1. Codificar"<<endl;
        cout << "2. Decodificar"<<endl;
        cout << "3. Aplicacion"<<endl;
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion){
        case 1:
            // Codificacion
            cout << "Ingrese el nombre del archivo a codificar: ";
            cin >> nombreArchivo;

            cout << "Ingrese el nombre del archivo a salida: ";
            cin >> nombreSalida;

            cout << "Ingrese la semilla (n): ";
            cin >> semilla;

            cout << "Seleccione el metodo (1 o 2): ";
            cin >> metodo;
            codificar(nombreArchivo,nombreSalida,semilla,metodo);
            break;
        case 2:
            // Decodificacion
            cout << "Ingrese el nombre del archivo a decodificar: ";
            cin >> nombreArchivo;

            cout << "Ingrese el nombre del archivo a salida: ";
            cin >> nombreSalida;

            cout << "Ingrese la semilla (n): ";
            cin >> semilla;

            cout << "Seleccione el metodo (1 o 2): ";
            cin >> metodo;
            decodificar(nombreArchivo,nombreSalida,semilla,metodo);
            break;
        case 3:
            break;
        default:
            cout << "Opcion no implementada. Seleccione una disponible.";
            break;
        }
    }
    cout << "\nProceso finalizado.\n";

    return 0;
}

