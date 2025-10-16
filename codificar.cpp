#include "codificar.h"
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <algorithm>


using namespace std;

string textoABits(const string& texto);
string invertirBits(const string& bits);
string invertirCadaNBits(string bits, int n);
int contarUnos(const string& bits);

void codificar(const string& nombreArchivo, const string& nombreSalida, int semilla, int metodo) {
    ifstream entrada(nombreArchivo, ios::in | ios::binary);
    if (!entrada) {
        cerr << "Error al abrir el archivo de entrada." << endl;
        return;
    }

    ofstream salida(nombreSalida, ios::out | ios::binary);
    if (!salida) {
        cerr << "Error al crear el archivo de salida." << endl;
        return;
    }

    string contenido((istreambuf_iterator<char>(entrada)), istreambuf_iterator<char>());

    string bits;
    bits = textoABits(contenido);

    string resultado;
    unsigned long pos = 0;
    if (metodo == 1) {
        // --- PRIMER MÉTODO ---
        pos = 0;
        string bloqueAnterior = "";
        string bloqueCodi = "";

        while (pos < bits.size()) {
            string bloque = bits.substr(pos, semilla);

            if (bloque.empty()) break;

            // Primer bloque → invertir todo
            if (bloqueAnterior.empty()) {
                bloqueCodi = invertirBits(bloque);
            } else {
                int unos = contarUnos(bloqueAnterior);
                int ceros = bloqueAnterior.size() - unos;

                if (unos == ceros)
                    bloqueCodi = invertirBits(bloque);
                else if (ceros > unos)
                    bloqueCodi = invertirCadaNBits(bloque, 2);
                else
                    bloqueCodi = invertirCadaNBits(bloque, 3);
            }
            resultado += bloqueCodi;
            bloqueAnterior = bloque;
            pos += semilla;
        }
    }
    else if (metodo == 2) {
        // --- SEGUNDO MÉTODO ---
        pos = 0;
        while (pos < bits.size()) {
            string bloque = bits.substr(pos, semilla);

            if (bloque.empty()) break;
            char ultimo = bloque.back();
            bloque.pop_back();
            bloque.insert(bloque.begin(), ultimo);

            resultado += bloque;
            pos += semilla;
        }
    }
    else {
        cerr << "Método no implementado aún." << endl;
        return;
    }

    // Escribir resultado
    salida << resultado;
    cout << "Archivo codificado con éxito: " << nombreSalida << endl;
}

// --- Funciones auxiliares ---
string textoABits(const string& texto) {
    string bits;
    for (unsigned char c : texto)
        bits += bitset<8>(c).to_string();
    return bits;
}

string invertirBits(const string& bits) {
    string r = bits;
    for (char& b : r)
        b = (b == '0') ? '1' : '0';
    return r;
}

string invertirCadaNBits(string bits, int n) {
    if (n <= 0) return bits;             // protección
    // recorrer índices 0-based: el n-ésimo bit está en índice n-1, luego 2n-1, ...
    for (size_t idx = n - 1; idx < bits.size(); idx += n) {
        if (bits[idx] == '0') bits[idx] = '1';
        else if (bits[idx] == '1') bits[idx] = '0';
        // si hay otros caracteres, los dejamos intactos
    }
    return bits;
}

int contarUnos(const string& bits) {
    return count(bits.begin(), bits.end(), '1');
}
