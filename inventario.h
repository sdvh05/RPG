#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "Objeto.h"

class NodoInventario {
public:
    Objeto obj;
    NodoInventario* izq;
    NodoInventario* der;

    NodoInventario(Objeto o) : obj(o), izq(nullptr), der(nullptr) {}
};

class Inventario {
private:
    NodoInventario* raiz;

    NodoInventario* insertar(NodoInventario* nodo, Objeto obj);
    Objeto* buscar(NodoInventario* nodo, const QString& nombre);

public:
    Inventario() : raiz(nullptr) {}
    void agregarObjeto(Objeto obj);
    Objeto* obtenerObjeto(const QString& nombre);
    void usarObjeto(const QString& nombre);
};

#endif
