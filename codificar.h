#ifndef CODIFICAR_H
#define CODIFICAR_H

#include <string>
using namespace std;
// Prototipo de la función codificar
void codificar(const string& nombreArchivo, int semilla, int metodo, int tipo);
string invertirBits(const string& bits);
string invertirCadaNBits(string bits, int n);
int contarUnos(const string& bits);


#endif
