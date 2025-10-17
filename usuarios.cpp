#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <sys/stat.h>
#include <limits>
#include "decodificar.h"
#include "codificar.h"
#include "usuario.h"

using namespace std;

static string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

bool existeArchivo1(const string &ruta) {
    struct stat buffer;
    return (stat(ruta.c_str(), &buffer) == 0);
}

bool validarUsuario(Usuario &usuarioEncontrado) {
    const string archivoCodificado = "usuariosCodi.txt";
    const string archivoDescifrado = "usuariosDeco.txt";
    const int semilla = 4;
    const int metodo = 1;

    if (!existeArchivo1(archivoCodificado)) {
        cout << "No existe base de datos de usuarios.\n";
        return false;
    }

    decodificar(archivoCodificado, archivoDescifrado, semilla, metodo);

    ifstream in(archivoDescifrado);
    if (!in.is_open()) {
        cout << "Error al abrir el archivo descifrado.\n";
        remove(archivoDescifrado.c_str());
        return false;
    }

    Usuario* usuarios = nullptr;
    size_t numUsuarios = 0;

    string linea;
    while (getline(in, linea)) {
        linea = trim(linea);
        if (linea.empty()) continue;

        stringstream ss(linea);
        string cedulaStr, clave, saldoStr;
        if (!getline(ss, cedulaStr, '-')) continue;
        if (!getline(ss, clave, '-')) continue;
        if (!getline(ss, saldoStr, '-')) continue;

        try {
            Usuario nuevo;
            nuevo.cedula = stoull(cedulaStr);
            nuevo.clave = trim(clave);
            nuevo.saldo = stoull(saldoStr);

            // --- aumentar el arreglo dinámico ---
            Usuario* temp = new Usuario[numUsuarios + 1];
            for (size_t i = 0; i < numUsuarios; ++i)
                temp[i] = usuarios[i];
            temp[numUsuarios] = nuevo;

            delete[] usuarios;
            usuarios = temp;
            numUsuarios++;
        } catch (...) {
            continue;
        }
    }

    in.close();
    remove(archivoDescifrado.c_str());

    if (numUsuarios == 0) {
        cout << "No hay usuarios registrados.\n";
        return false;
    }

    size_t cedulaIngresada;
    string claveIngresada;

    cout << "Ingrese su cedula: ";
    cin >> cedulaIngresada;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Ingrese su clave: ";
    getline(cin, claveIngresada);

    bool encontrado = false;
    for (size_t i = 0; i < numUsuarios; ++i) {
        if (usuarios[i].cedula == cedulaIngresada && usuarios[i].clave == claveIngresada) {
            usuarioEncontrado = usuarios[i];
            cout << "Ingreso exitoso. Bienvenido, usuario " << usuarios[i].cedula << ".\n";
            encontrado = true;
            break;
        }
    }
    delete[] usuarios;
    if (!encontrado)
        cout << "Credenciales incorrectas.\n";

    return encontrado;
}

void actualizarSaldoEnArchivo(const Usuario &usuarioActual) {
    const string archivoCodificado = "usuariosCodi.txt";
    const string archivoDescifrado = "usuariosDeco.txt";
    const int semilla = 4;
    const int metodo = 1;

    decodificar(archivoCodificado, archivoDescifrado, semilla, metodo);

    ifstream in(archivoDescifrado);
    if (!in.is_open()) {
        cout << "Error al abrir el archivo descifrado.\n";
        return;
    }

    Usuario* usuarios = nullptr;
    size_t numUsuarios = 0;

    string linea;
    while (getline(in, linea)) {
        linea = trim(linea);
        if (linea.empty()) continue;

        stringstream ss(linea);
        string cedulaStr, clave, saldoStr;
        if (!getline(ss, cedulaStr, '-')) continue;
        if (!getline(ss, clave, '-')) continue;
        if (!getline(ss, saldoStr, '-')) continue;

        try {
            Usuario nuevo;
            nuevo.cedula = stoull(cedulaStr);
            nuevo.clave = trim(clave);
            nuevo.saldo = stoull(saldoStr);

            Usuario* temp = new Usuario[numUsuarios + 1];
            for (size_t i = 0; i < numUsuarios; ++i)
                temp[i] = usuarios[i];
            temp[numUsuarios] = nuevo;

            delete[] usuarios;
            usuarios = temp;
            numUsuarios++;
        } catch (...) {
            continue;
        }
    }
    in.close();

    // actualizar saldo
    for (size_t i = 0; i < numUsuarios; ++i) {
        if (usuarios[i].cedula == usuarioActual.cedula) {
            usuarios[i].saldo = usuarioActual.saldo;
            break;
        }
    }

    ofstream out(archivoDescifrado, ios::trunc);
    for (size_t i = 0; i < numUsuarios; ++i) {
        out << usuarios[i].cedula << "-" << usuarios[i].clave << "-" << usuarios[i].saldo << "\n";
    }
    out.close();

    codificar(archivoDescifrado, archivoCodificado, semilla, metodo);
    remove(archivoDescifrado.c_str());

    delete[] usuarios; // liberar memoria
}

void consultarSaldo(Usuario &usuarioActual) {
    const int costo = 1000;
    if (usuarioActual.saldo < costo) {
        cout << "Saldo insuficiente para realizar la consulta. Debe tener al menos " << costo << ".\n";
        return;
    }
    usuarioActual.saldo -= costo;
    cout << "Saldo actual (despues del cobro de " << costo << "): " << usuarioActual.saldo << endl;
    actualizarSaldoEnArchivo(usuarioActual);
}

void retirarDinero(Usuario &usuarioActual) {
    const int costo = 1000;
    size_t monto;
    cout << "Ingrese el monto a retirar: ";
    cin >> monto;
    if (usuarioActual.saldo < (monto + costo)) {
        cout << "Saldo insuficiente. Debe tener al menos " << (monto + costo) << " para retirar " << monto << ".\n";
        return;
    }
    usuarioActual.saldo -= (monto + costo);
    cout << "Retiro exitoso. Nuevo saldo: " << usuarioActual.saldo << endl;
    actualizarSaldoEnArchivo(usuarioActual);
}

