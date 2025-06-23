#include "Were.h"

Were::Were(TipoWere tipo)
    : Personaje("", 0, 0, 0, false)
{
    QString rutaBase = "Personajes/Enemigos/Were/";
    QString nombre;

    switch (tipo) {
    case WereWolf:
        nombre = "WereWolf";
        vidaMax = vidaActual = 90;
        ataque = 27;

        agregarAnimacion("idle",   cargarDesdeSpritesheet(rutaBase + "Werewolf-Idle.png",   100, 100, 6), true);
        agregarAnimacion("attack", cargarDesdeSpritesheet(rutaBase + "Werewolf-Attack01.png", 100, 100, 7), false);
        agregarAnimacion("hurt",   cargarDesdeSpritesheet(rutaBase + "Werewolf-Hurt.png",   100, 100, 4), false);
        agregarAnimacion("death",  cargarDesdeSpritesheet(rutaBase + "Werewolf-Death.png",  100, 100, 4), false);
        agregarAnimacion("walk",   cargarDesdeSpritesheet(rutaBase + "Werewolf-Walk.png",   100, 100, 6), false);
        break;

    case WereBear:
        nombre = "WereBear";
        vidaMax = vidaActual = 250;
        ataque = 42;

        agregarAnimacion("idle",   cargarDesdeSpritesheet(rutaBase + "Werebear-Idle.png",   100, 100, 6), true);
        agregarAnimacion("attack", cargarDesdeSpritesheet(rutaBase + "Werebear-Attack01.png", 100, 100, 8), false);
        agregarAnimacion("hurt",   cargarDesdeSpritesheet(rutaBase + "Werebear-Hurt.png",   100, 100, 4), false);
        agregarAnimacion("death",  cargarDesdeSpritesheet(rutaBase + "Werebear-Death.png",  100, 100, 4), false);
        agregarAnimacion("walk",   cargarDesdeSpritesheet(rutaBase + "Werebear-Walk.png",   100, 100, 6), false);
        break;
    }

    this->nombre = nombre;
    setEstado("idle");

    connect(this, &Personaje::animacionTerminada, this, [=](QString estado){
        if (estado == "attack" || estado == "especial" || estado == "hurt")
            setEstado("idle");
    });
}
