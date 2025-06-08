#include "princesa.h"
#include <cstdlib>
#include <ctime>

Princesa::Princesa()
    : Aliado("Princesa", 85, 35, 25) {

    arbol = new ArbolHabilidad();

    NodoSkill* base = new NodoSkill("Doble Flecha", "Ataca dos veces", 1);
    NodoSkill* rama1 = new NodoSkill("Daño Aumentado", "Daño +10", 2, 0, 10);
    NodoSkill* rama2 = new NodoSkill("Crítico", "20% probabilidad de crítico", 2, 0, 0, 0, true);
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
        //setEstado("especial");

        int danio = ataque * 0.5;
        for (auto enemigo : enemigos) {
            if (enemigo->getVidaActual() > 0) {
                enemigo->recibirDanio(danio);
                qDebug() << nombre << " golpea a " << enemigo->getNombre() << " con ataque grupal de " << danio;
            }
        }

        usarMana(15);
    } else {
        qDebug() << nombre << " no tiene suficiente maná para su especial.";
    }
}

