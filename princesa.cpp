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
    agregarAnimacion("idle",   cargarDesdeSpritesheet("Personajes/Aliados/Princesa/Swordsman-Idle.png",   100, 100, 6));
    agregarAnimacion("attack", cargarDesdeSpritesheet("Personajes/Aliados/Princesa/Swordsman-Attack01.png", 100, 100, 6));
    agregarAnimacion("hurt",   cargarDesdeSpritesheet("Personajes/Aliados/Princesa/Swordsman-Hurt.png",   100, 100, 6));
    agregarAnimacion("death",   cargarDesdeSpritesheet("Personajes/Aliados/Princesa/Swordsman-Death.png",   100, 100, 6));

    connect(this, &Personaje::animacionTerminada, this, [=](QString estado){
        if (estado == "attack" || estado == "especial" || estado=="hurt") {
            setEstado("idle");
        }
    });
}

void Princesa::ataqueEspecial(std::vector<Personaje*>&, std::vector<Personaje*>& enemigos) {
    if (enemigos.empty()) return;
    NodoSkill* base = arbol->getRaiz();

    int danio = ataque;
    bool critico = false;

    if (base->izquierda && base->izquierda->desbloqueado)
        danio += base->izquierda->danioExtra;

    if (base->derecha && base->derecha->desbloqueado && (std::rand() % 100 < 20))
        critico = true;

    if (base->izquierda && base->izquierda->izquierda && base->izquierda->izquierda->desbloqueado)
        danio += base->izquierda->izquierda->danioExtra;

    if (critico) danio *= 2;

    Personaje* objetivo = enemigos.front();
    objetivo->recibirDanio(danio);
    objetivo->recibirDanio(danio);

    usarMana(10);
    qDebug() << nombre << " ataca dos veces a" << objetivo->getNombre() << "con" << danio << "de daño" << (critico ? "(Crítico!)" : "");


}
