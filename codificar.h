#ifndef CODIFICAR_H
#define CODIFICAR_H

#include <string>
using namespace std;
// Prototipo de la función codificar
void codificar(const string& nombreArchivo, const string& nombreSalida, int semilla, int metodo);
string invertirBits(const string& bits);
string invertirCadaNBits(string bits, int n);
int contarUnos(const string& bits);


#endif
