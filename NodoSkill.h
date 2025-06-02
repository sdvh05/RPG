#ifndef NODOSKILL_H
#define NODOSKILL_H

#include <QString>

struct NodoSkill {
    QString nombre;
    QString descripcion;
    bool desbloqueado;
    int nivelRequerido;

    int curacionExtra;
    int danioExtra;
    int manaMod;
    bool probCritico;
    bool efectoGlobal;

    NodoSkill* izquierda;
    NodoSkill* derecha;

    NodoSkill(QString nombre, QString descripcion, int nivel,
              int curacion = 0, int danio = 0, int mana = 0,
              bool critico = false, bool global = false)
        : nombre(nombre), descripcion(descripcion), desbloqueado(false),
        nivelRequerido(nivel), curacionExtra(curacion), danioExtra(danio),
        manaMod(mana), probCritico(critico), efectoGlobal(global),
        izquierda(nullptr), derecha(nullptr) {}
};

#endif
