#include "ArbolHabilidad.h"

ArbolHabilidad::ArbolHabilidad() : raiz(nullptr) {}
ArbolHabilidad::~ArbolHabilidad() { destruir(raiz); }

void ArbolHabilidad::setRaiz(NodoSkill* nodo) { raiz = nodo; }
NodoSkill* ArbolHabilidad::getRaiz() const { return raiz; }

void ArbolHabilidad::desbloquearPorNivel(int nivel) {
    std::function<void(NodoSkill*)> f = [&](NodoSkill* n) {
        if (!n) return;
        if (!n->desbloqueado && nivel >= n->nivelRequerido)
            n->desbloqueado = true;
        f(n->izquierda); f(n->derecha);
    };
    f(raiz);
}

void ArbolHabilidad::mostrarArbol(NodoSkill* nodo, int profundidad) const {
    if (!nodo) return;
    mostrarArbol(nodo->derecha, profundidad + 1);
    QString indent = QString("  ").repeated(profundidad);
    qDebug() << indent << (nodo->desbloqueado ? "[✔]" : "[ ]") << nodo->nombre;
    mostrarArbol(nodo->izquierda, profundidad + 1);
}

void ArbolHabilidad::destruir(NodoSkill* nodo) {
    if (!nodo) return;
    destruir(nodo->izquierda); destruir(nodo->derecha);
    delete nodo;
}
