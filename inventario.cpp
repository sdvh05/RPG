#include "inventario.h"
#include <QDebug>

Inventario::Inventario() : raiz(nullptr) {}

// Destructor
Inventario::~Inventario() {
    destruir(raiz);
}

void Inventario::destruir(NodoInventario* nodo) {
    if (!nodo) return;
    destruir(nodo->izq);
    destruir(nodo->der);
    delete nodo;
}

// Inserción en ABB
NodoInventario* Inventario::insertar(NodoInventario* nodo, const Objeto& obj) {
    if (!nodo)
        return new NodoInventario(obj);

    if (obj.nombre < nodo->obj.nombre) {
        nodo->izq = insertar(nodo->izq, obj);
    } else if (obj.nombre > nodo->obj.nombre) {
        nodo->der = insertar(nodo->der, obj);
    } else {
        nodo->obj.cantidad += obj.cantidad;
    }
    return nodo;
}

void Inventario::agregarObjeto(const Objeto& obj) {
    raiz = insertar(raiz, obj);
}

// Búsqueda por nombre
Objeto* Inventario::buscar(NodoInventario* nodo, const QString& nombre) {
    if (!nodo) return nullptr;

    if (nombre < nodo->obj.nombre)
        return buscar(nodo->izq, nombre);
    else if (nombre > nodo->obj.nombre)
        return buscar(nodo->der, nombre);
    else
        return &nodo->obj;
}

Objeto* Inventario::obtenerObjeto(const QString& nombre) {
    return buscar(raiz, nombre);
}

// Uso de objeto (resta cantidad si es posible)
void Inventario::usarObjeto(const QString& nombre) {
    Objeto* obj = obtenerObjeto(nombre);
    if (obj && obj->cantidad > 0) {
        obj->cantidad--;
        qDebug() << "Usaste" << nombre << ". Quedan:" << obj->cantidad;
    } else {
        qDebug() << "No tienes" << nombre << "disponible.";
    }
}



// Recorrido en orden que guarda punteros a objetos
void Inventario::recogerObjetos(QVector<Objeto*>& lista) const {
    recogerObjetosRec(raiz, lista);
}

void Inventario::recogerObjetosRec(NodoInventario* nodo, QVector<Objeto*>& lista) const {
    if (!nodo) return;
    recogerObjetosRec(nodo->izq, lista);
    lista.append(&nodo->obj);
    recogerObjetosRec(nodo->der, lista);
}

void Inventario::recogerPorTipo(const QString& tipo, QVector<Objeto*>& lista) const {
    recogerPorTipoRec(raiz, tipo, lista);
}

void Inventario::recogerPorTipoRec(NodoInventario* nodo, const QString& tipo, QVector<Objeto*>& lista) const {
    if (!nodo) return;
    recogerPorTipoRec(nodo->izq, tipo, lista);
    if (nodo->obj.tipo == tipo)
        lista.append(&nodo->obj);
    recogerPorTipoRec(nodo->der, tipo, lista);
}

void Inventario::recogerPorRareza(const QString& rareza, QVector<Objeto*>& lista) const {
    recogerPorRarezaRec(raiz, rareza, lista);
}

void Inventario::recogerPorRarezaRec(NodoInventario* nodo, const QString& rareza, QVector<Objeto*>& lista) const {
    if (!nodo) return;
    recogerPorRarezaRec(nodo->izq, rareza, lista);
    if (nodo->obj.rareza == rareza)
        lista.append(&nodo->obj);
    recogerPorRarezaRec(nodo->der, rareza, lista);
}

// Permitir acceso al árbol
NodoInventario* Inventario::getRaiz() const {
    return raiz;
}


//----------------------------------------
void Inventario::agregarNucleo(int cantidad){
    agregarObjeto(Objeto("Pieza del Nucleo", "Legendario", "Nucleo", "Personajes/Items/Emerald.png", cantidad, 1));
}
void Inventario::agregarPoscionVida(int cantidad){
    agregarObjeto(Objeto("Pocion Vida", "Comun", "Curacion", "Personajes/Items/Red Potion.png", cantidad, 20));
}
void Inventario::agregarPoscionMana(int cantidad){
    agregarObjeto(Objeto("Pocion Mana", "Comun", "Curacion", "Personajes/Items/Blue Potion.png", cantidad, 20));
}
void Inventario::agregarPoscionVidaGrande(int cantidad){
    agregarObjeto(Objeto("Pocion Vida XL", "Raro", "Curacion", "Personajes/Items/Red Potion 3.png", cantidad, 60));
}
void Inventario::agregarPoscionManaGrande(int cantidad){
    agregarObjeto(Objeto("Pocion Mana XL", "Raro", "Curacion", "Personajes/Items/Blue Potion 3.png", cantidad, 60));
}
void Inventario::agregarLlave(int cantidad){
    agregarObjeto(Objeto("LLave", "Epico", "Llave", "Personajes/Items/Golden Key.png", cantidad, 1));
}
