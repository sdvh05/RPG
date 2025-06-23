#include "Aliados.h"

Aliado::Aliado(QString nombre, int vida, int mana, int ataque)
    : Personaje(nombre, vida, mana, ataque, true), arbol(nullptr) {}

ArbolHabilidad* Aliado::getArbol() const {
    return arbol;
}

void Aliado::subirNivel() {
    nivel++;

    // Mejora de estadísticas base
    int aumentoVida = 10;
    int aumentoMana = 10;
    int aumentoAtaque = 3;

    vidaMax += aumentoVida;
    manaMax += aumentoMana;
    ataque += aumentoAtaque;

    // Restaura vida y maná parcial o completo si quieres
    vidaActual = qMin(vidaMax, vidaActual + aumentoVida);
    manaActual = qMin(manaMax, manaActual + aumentoMana);

    if (arbol) {
        arbol->desbloquearPorNivel(nivel);
    }

    qDebug() << nombre << " ha subido al nivel" << nivel
             << " → Vida:" << vidaMax
             << " Maná:" << manaMax
             << " Ataque:" << ataque;
}



void Aliado::ganarExperiencia(int cantidad) {
    experiencia += cantidad;
    qDebug() << nombre << " ganó" << cantidad << "puntos de experiencia. Total:" << experiencia << "/" << experienciaNecesaria;

    while (experiencia >= experienciaNecesaria) {
        experiencia -= experienciaNecesaria;
        subirNivel();


        experienciaNecesaria += 5;
    }
}

