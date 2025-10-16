#include "decodificar.h"
#include "codificar.h"
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
//#include <algorithm>

using namespace std;

string BitsCaracter(const string& texto);

void decodificar(const string& nombreArchivo, const string& nombreSalida, int semilla, int metodo) {
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

    string resultado;
    unsigned long pos = 0;

    if(metodo == 1){
        pos = 0;
        string bloqueAnterior = "";
        string bloquedeCodi = "";

        while (pos < contenido.size()){
            string bloque = contenido.substr(pos, semilla);

             if (bloque.empty()) break;

             if (bloqueAnterior.empty()) {
                 bloquedeCodi = invertirBits(bloque);
             }
             else{
                 int unos = contarUnos(bloqueAnterior);
                 int ceros = bloqueAnterior.size() - unos;

                 if (unos == ceros)
                     bloquedeCodi = invertirBits(bloque);
                 else if (ceros > unos)
                     bloquedeCodi = invertirCadaNBits(bloque, 2);
                 else
                     bloquedeCodi = invertirCadaNBits(bloque, 3);
             }
             resultado += bloquedeCodi;
             bloqueAnterior = bloquedeCodi;
             pos += semilla;
        }
    }
    else if(metodo ==2){
        // --- SEGUNDO MÉTODO ---
        pos = 0;
        while (pos < contenido.size()) {
            string bloque = contenido.substr(pos, semilla);

            if (bloque.empty()) break;
            char primero = bloque.front();
            bloque.erase(bloque.begin());
            bloque.push_back(primero);

            resultado += bloque;
            pos += semilla;
        }
    }
    else{
        cout << "Metodo no implementado. "<< endl;
        return;
    }

    string cadena = BitsCaracter(resultado);
    salida << cadena;
    //cout << "Archivo decodificado con éxito: " << nombreSalida << endl;
    return;
}


string BitsCaracter(const string& bits){
    string texto;
    for (size_t i = 0; i < bits.size(); i += 8) {
        string byteStr = bits.substr(i, 8);
        if (byteStr.size() < 8) break;  // evita bytes incompletos
        bitset<8> b(byteStr);
        unsigned char c = static_cast<unsigned char>(b.to_ulong());

        texto.push_back(c);
    }
    return texto;
}
