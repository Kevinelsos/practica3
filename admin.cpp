#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <vector>
#include <sys/stat.h>
#include "decodificar.h"
#include "codificar.h"
#include "admin.h"


using namespace std;

struct Usuario {
    size_t cedula; // La cédula del usuario
    string clave; // La clave del usuario
    size_t saldo; // El saldo del usuario
};

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
    int n;
    int metodo;
    n = 4;
    metodo = 1;
    decodificar(archivoEncriptado, archivoDescifrado, n, metodo);

    ifstream in(archivoDescifrado);
    if (!in.is_open()) {
        cout << "No se pudo abrir el archivo descifrado '" << archivoDescifrado << "'.\n";
        std::remove(archivoDescifrado.c_str());
        return false;
    }

    vector<string> listaAdmins;
    string linea;
    while (getline(in, linea)) {
        string t = trim(linea);
        if (t.empty()) continue;
        // aceptar líneas del tipo "nick-clave", si la línea no contiene '-', la ignoramos
        if (t.find('-') == string::npos) continue;
        listaAdmins.push_back(t);
    }
    in.close();

    // borramos el archivo temporal (ya no lo necesitamos)
    std::remove(archivoDescifrado.c_str());

    if (listaAdmins.empty()) {
        cout << "No se encontraron credenciales válidas en el archivo descifrado.\n";
        return false;
    }
    // 3) Pedir nickname + contraseña hasta 3 intentos
    const int MAX_INTENTOS = 3;
    int contador = 0;
    while (contador < MAX_INTENTOS) {
        string nick_ingresado, clave_ingresada;
        cout << "Ingrese el nickname: ";
        cin >> ws;
        getline(cin, nick_ingresado);
        nick_ingresado = trim(nick_ingresado);

        cout << "Ingrese la contraseña: ";
        // para simplificar usamos getline; si quieres ocultar la clave, lo implementamos aparte
        getline(cin, clave_ingresada);
        clave_ingresada = trim(clave_ingresada);

        if (nick_ingresado.empty() || clave_ingresada.empty()) {
            cout << "Nickname o contraseña vacíos — intenta de nuevo.\n";
            continue; // no contamos como intento si fue vacío
        }
        string candidato = nick_ingresado + "-" + clave_ingresada;
        bool encontrado = false;
        for (const auto &entry : listaAdmins) {
            if (entry == candidato) {
                encontrado = true;
                break;
            }
        }
        if (encontrado) {
            cout << "Contraseña correcta. Acceso concedido.\n";
            return true;
        } else {
            contador++;
            cout << "Contraseña incorrecta. Intento " << contador << " de " << MAX_INTENTOS << ".\n";
        }
    }
    cout << "Usuario bloqueado tras " << MAX_INTENTOS << " intentos.\n";
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

        cout << "\nIngrese la cédula del usuario: ";
        cin >> u.cedula;
        cin.ignore();

        cout << "Ingrese la clave del usuario: ";
        getline(cin, u.clave);

        cout << "Ingrese el saldo inicial: ";
        cin >> u.saldo;

        // Escribir en formato cedula-clave-saldo
        out << u.cedula << "-" << u.clave << "-" << u.saldo << "\n";
        cout << "Usuario agregado exitosamente.\n";

        cout << "¿Desea agregar otro usuario? (1/2): ";
        cin >> opcion;
        cin.ignore();
    }

    out.close();
    codificar(archivoDescifrado, archivoCodificado, semilla, metodo);
    std::remove(archivoDescifrado.c_str());

    cout << "Base de datos actualizada y cifrada correctamente.\n";
}
