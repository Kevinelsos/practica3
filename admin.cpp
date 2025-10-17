#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <vector>
#include <sys/stat.h>
#include "decodificar.h"
#include "codificar.h"
#include "admin.h"
#include "usuario.h"

using namespace std;

static string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

bool existeArchivo(const string &ruta) {
    struct stat buffer;
    return (stat(ruta.c_str(), &buffer) == 0);
}

bool validar_admin() {
    string archivoEncriptado = "sudo.txt";
    string archivoDescifrado = "sudo_dec.txt";
    const int n = 4;
    const int metodo = 1;

    decodificar(archivoEncriptado, archivoDescifrado, n, metodo);

    ifstream in(archivoDescifrado);
    if (!in.is_open()) {
        cout << "No se pudo abrir el archivo descifrado '" << archivoDescifrado << "'.\n";
        remove(archivoDescifrado.c_str());
        return false;
    }

    // --- reemplazo de vector<string> listaAdmins ---
    string *listaAdmins = nullptr;
    int numAdmins = 0;
    int capacidad = 0;
    string linea;

    while (getline(in, linea)) {
        string t = trim(linea);
        if (t.empty() || t.find('-') == string::npos) continue;

        if (numAdmins == capacidad) {
            int nuevaCapacidad = (capacidad == 0) ? 4 : capacidad * 2;
            string *temp = new string[nuevaCapacidad];
            for (int i = 0; i < numAdmins; i++)
                temp[i] = listaAdmins[i];

            delete[] listaAdmins;
            listaAdmins = temp;
            capacidad = nuevaCapacidad;
        }

        listaAdmins[numAdmins++] = t;
    }

    in.close();
    remove(archivoDescifrado.c_str());

    if (numAdmins == 0) {
        cout << "No se encontraron credenciales válidas.\n";
        delete[] listaAdmins;
        return false;
    }

    const int MAX_INTENTOS = 3;
    int contador = 0;

    while (contador < MAX_INTENTOS) {
        string nick_ingresado, clave_ingresada;
        cout << "Ingrese el nickname: ";
        getline(cin >> ws, nick_ingresado);
        cout << "Ingrese la contrasena: ";
        getline(cin, clave_ingresada);

        string candidato = trim(nick_ingresado) + "-" + trim(clave_ingresada);
        bool encontrado = false;

        for (int i = 0; i < numAdmins; i++) {
            if (listaAdmins[i] == candidato) {
                encontrado = true;
                break;
            }
        }

        if (encontrado) {
            cout << "Contrasena correcta. Acceso concedido.\n";
            delete[] listaAdmins;
            return true;
        } else {
            contador++;
            cout << "Contrasena incorrecta. Intento " << contador << " de " << MAX_INTENTOS << ".\n";
        }
    }

    cout << "Usuario bloqueado tras " << MAX_INTENTOS << " intentos.\n";
    delete[] listaAdmins;
    return false;
}

void agregarUsuarios() {
    const string archivoCodificado = "usuariosCodi.txt";
    const string archivoDescifrado = "usuariosDeco.txt";
    const int semilla = 4;
    const int metodo = 1;

    if (existeArchivo(archivoCodificado)) {
        decodificar(archivoCodificado, archivoDescifrado, semilla, metodo);
    } else {
        ofstream nuevo(archivoDescifrado);
        nuevo.close();
    }

    ofstream out(archivoDescifrado, ios::app);
    if (!out.is_open()) {
        cout << "Error: no se pudo abrir el archivo descifrado para escribir.\n";
        return;
    }

    char opcion = '1';
    while (toupper(opcion) == '1') {
        Usuario u;

        cout << "\nIngrese la cedula del usuario: ";
        cin >> u.cedula;
        cin.ignore();

        cout << "Ingrese la clave del usuario: ";
        getline(cin, u.clave);
        cout << "Ingrese el saldo inicial: ";
        cin >> u.saldo;
        out << u.cedula << "-" << u.clave << "-" << u.saldo << "\n";
        cout << "Usuario agregado exitosamente.\n";

        cout << "¿Desea agregar otro usuario? (1/2): ";
        cin >> opcion;
        cin.ignore();
    }

    out.close();
    codificar(archivoDescifrado, archivoCodificado, semilla, metodo);
    remove(archivoDescifrado.c_str());

    cout << "Base de datos actualizada y cifrada correctamente.\n";
}
