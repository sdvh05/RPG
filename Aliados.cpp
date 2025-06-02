#include "Aliados.h"

Aliado::Aliado(QString nombre, int vida, int mana, int ataque)
    : Personaje(nombre, vida, mana, ataque, true), arbol(nullptr) {}

ArbolHabilidad* Aliado::getArbol() const {
    return arbol;
}

void Aliado::subirNivel() {
    nivel++;
    if (arbol) {
        arbol->desbloquearPorNivel(nivel);
        qDebug() << nombre << " ha subido al nivel" << nivel;
    }
}
