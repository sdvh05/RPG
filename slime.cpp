#include "Slime.h"

Slime::Slime()
    : Personaje("Slime", 45, 0, 10, false) {

    // Cargar animaciones desde spritesheets
    agregarAnimacion("idle",   cargarDesdeSpritesheet("Personajes/Enemigos/Slime/Slime-Idle.png",   100, 100, 6), true);
    agregarAnimacion("attack", cargarDesdeSpritesheet("Personajes/Enemigos/Slime/Slime-Attack01.png", 100, 100, 6), false);
    agregarAnimacion("hurt",   cargarDesdeSpritesheet("Personajes/Enemigos/Slime/Slime-Hurt.png",   100, 100, 4), false);
    agregarAnimacion("death",  cargarDesdeSpritesheet("Personajes/Enemigos/Slime/Slime-Death.png",  100, 100, 4), false);

    setEstado("idle");

    // Cuando termine la animación de ataque o daño, volver a idle
    connect(this, &Personaje::animacionTerminada, this, [=](QString estado){
        if (estado == "attack" || estado == "especial" || estado=="hurt") {
            setEstado("idle");
        }
    });
}
