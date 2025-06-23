#include "princesa.h"
#include <cstdlib>
#include <QRandomGenerator>
#include <ctime>

Princesa::Princesa()
    : Aliado("Princesa", 90, 35, 25) {

    arbol = new ArbolHabilidad();

    NodoSkill* base = new NodoSkill("Critico", "Probabilidad de Critico 10%", 1);
    NodoSkill* rama1 = new NodoSkill("Daño Aumentado", "Daño +5", 2, 0, 5);
    NodoSkill* rama2 = new NodoSkill("Crítico Aumentado", "25% probabilidad de crítico", 2, 0, 0, 0, true);
    NodoSkill* rama3 = new NodoSkill("Doble Mejorado", "Cada golpe +5 extra", 3, 0, 5);

    base->izquierda = rama1;
    base->derecha = rama2;
    rama1->izquierda = rama3;

    arbol->setRaiz(base);
    std::srand(std::time(nullptr));


    // Cargar animaciones desde spritesheets
    agregarAnimacion("idle",   cargarDesdeSpritesheet("Personajes/Aliados/Princesa/Swordsman-Idle.png",   100, 100, 6),true);
    agregarAnimacion("attack", cargarDesdeSpritesheet("Personajes/Aliados/Princesa/Swordsman-Attack01.png", 100, 100, 6),false);
    agregarAnimacion("especial", cargarDesdeSpritesheet("Personajes/Aliados/Princesa/Swordsman-Attack02.png", 100, 100, 6),false);
    agregarAnimacion("hurt",   cargarDesdeSpritesheet("Personajes/Aliados/Princesa/Swordsman-Hurt.png",   100, 100, 4),false);
    agregarAnimacion("death",   cargarDesdeSpritesheet("Personajes/Aliados/Princesa/Swordsman-Death.png",   100, 100, 4),false);
    agregarAnimacion("walk",   cargarDesdeSpritesheet("Personajes/Aliados/Princesa/Swordsman-Walk.png",   100, 100, 6),false);

    connect(this, &Personaje::animacionTerminada, this, [=](QString estado){
        if (estado == "attack" || estado == "especial" || estado=="hurt") {
            setEstado("idle");
        }
    });
}

void Princesa::ataqueEspecial(std::vector<Personaje*>&, std::vector<Personaje*>& enemigos) {
    if (manaActual >= 15) {
        setEstado("especial");

        NodoSkill* nodo = arbol->getRaiz();
        int danioExtra = 0;
        bool critico = false;

        if (nodo->desbloqueado) {
            if (nodo->izquierda && nodo->izquierda->desbloqueado)
                danioExtra += nodo->izquierda->danioExtra;

            if (nodo->derecha && nodo->derecha->desbloqueado)
                critico = nodo->derecha->probCritico;

            if (nodo->izquierda && nodo->izquierda->izquierda && nodo->izquierda->izquierda->desbloqueado)
                danioExtra += nodo->izquierda->izquierda->danioExtra; // Extra del nodo "Doble Mejorado"
        }

        int danioBase = ataque * 0.5;
        int final = danioBase + danioExtra;

        for (auto enemigo : enemigos) {
            if (enemigo->getVidaActual() > 0) {
                int total = final;

                if (critico) {
                    int chance = QRandomGenerator::global()->bounded(100);
                    if (chance < 25) {
                        total *= 2;
                        qDebug() << "¡Golpe crítico!";
                    }
                }

                enemigo->recibirDanio(total);
                qDebug() << nombre << " golpea a " << enemigo->getNombre() << " por " << total;
            }
        }

        usarMana(15);
    } else {
        qDebug() << nombre << " no tiene suficiente maná para su especial.";
    }
}


