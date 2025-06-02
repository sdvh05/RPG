#ifndef ARBOLHABILIDAD_H
#define ARBOLHABILIDAD_H

#include "NodoSkill.h"
#include <QDebug>

class ArbolHabilidad {
    NodoSkill* raiz;

public:
    ArbolHabilidad();
    ~ArbolHabilidad();

    void setRaiz(NodoSkill* nodo);
    NodoSkill* getRaiz() const;

    void desbloquearPorNivel(int nivel);
    void mostrarArbol(NodoSkill* nodo, int profundidad = 0) const;
    void destruir(NodoSkill* nodo);
};

#endif
