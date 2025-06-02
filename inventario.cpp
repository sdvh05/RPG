#include "Inventario.h"
#include <iostream>

NodoInventario* Inventario::insertar(NodoInventario* nodo, Objeto obj) {
    if (!nodo) return new NodoInventario(obj);
    if (obj.nombre < nodo->obj.nombre)
        nodo->izq = insertar(nodo->izq, obj);
    else if (obj.nombre > nodo->obj.nombre)
        nodo->der = insertar(nodo->der, obj);
    else
        nodo->obj.cantidad += obj.cantidad;
    return nodo;
}

void Inventario::agregarObjeto(Objeto obj) {
    raiz = insertar(raiz, obj);
}

Objeto* Inventario::buscar(NodoInventario* nodo, const QString& nombre) {
    if (!nodo) return nullptr;
    if (nombre < nodo->obj.nombre)
        return buscar(nodo->izq, nombre);
    else if (nombre > nodo->obj.nombre)
        return buscar(nodo->der, nombre);
    return &nodo->obj;
}

Objeto* Inventario::obtenerObjeto(const QString& nombre) {
    return buscar(raiz, nombre);
}

void Inventario::usarObjeto(const QString& nombre) {
    Objeto* obj = obtenerObjeto(nombre);
    if (obj && obj->cantidad > 0) {
        obj->cantidad--;
        std::cout << "Usaste " << nombre.toStdString() << ". Quedan: " << obj->cantidad << std::endl;
    } else {
        std::cout << "No tienes " << nombre.toStdString() << " disponible.\n";
    }
}
