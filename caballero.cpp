#include "Caballero.h"

Caballero::Caballero()
    : Aliado("Caballero C++", 130, 30, 20) {

    arbol = new ArbolHabilidad();

    NodoSkill* base = new NodoSkill("Defensa Grupal", "Aumenta la defensa de todos temporalmente", 1);
    NodoSkill* rama1 = new NodoSkill("Estadísticas Defensivas", "+10 vida y +5 ataque", 2, 0, 5);
    NodoSkill* rama2 = new NodoSkill("Mejora de Defensa", "Defensa extra durante el efecto", 3);

    base->izquierda = rama1;
    base->derecha = rama2;

    arbol->setRaiz(base);


    // Cargar animaciones desde spritesheets
    agregarAnimacion("idle",   cargarDesdeSpritesheet("Personajes/Aliados/Caballero/Knight-Idle.png",   100, 100,6), true);
    agregarAnimacion("attack", cargarDesdeSpritesheet("Personajes/Aliados/Caballero/Knight-Attack01.png", 100, 100, 6), false);
    agregarAnimacion("hurt",   cargarDesdeSpritesheet("Personajes/Aliados/Caballero/Knight-Hurt.png",   100, 100, 4), false);
    agregarAnimacion("death",   cargarDesdeSpritesheet("Personajes/Aliados/Caballero/Knight-Death.png",   100, 100, 4), false);
    agregarAnimacion("walk",   cargarDesdeSpritesheet("Personajes/Aliados/Caballero/Knight-Walk.png",   100, 100, 6), true);

    setEstado("idle");
    connect(this, &Personaje::animacionTerminada, this, [=](QString estado){
        if (estado == "attack" || estado == "especial" || estado=="hurt") {
            setEstado("idle");
        }
    });
}

void Caballero::ataqueEspecial(std::vector<Personaje*>& aliados, std::vector<Personaje*>&) {
    NodoSkill* base = arbol->getRaiz();
    int defensaExtra = 5;

    if (base->izquierda && base->izquierda->desbloqueado) {
        vidaMax += 10;
        ataque += base->izquierda->danioExtra;
    }
    if (base->derecha && base->derecha->desbloqueado) {
        defensaExtra += 5; // Mejora de Defensa activa
    }

    if (manaActual >= 10) {
        for (auto aliado : aliados) {
            if (aliado->esAliadoPersonaje()) {
                aliado->aumentarDefensa(defensaExtra);
                qDebug() << aliado->getNombre() << " obtiene +" << defensaExtra << " de defensa temporal.";
            }
        }
        usarMana(10);
        qDebug() << nombre << " usó Defensa Grupal.";
    }
}

