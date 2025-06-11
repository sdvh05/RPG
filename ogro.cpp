#include "Ogro.h"

Ogro::Ogro(TipoOgro tipo)
    : Personaje("", 0, 0, 0, false)
{
    QString rutaBase = "Personajes/Enemigos/Ogros/";
    QString nombre;

    switch (tipo) {
    case Normal:
        nombre = "Ogro ";
        vidaMax = vidaActual = 70;
        ataque = 25;

        agregarAnimacion("idle",   cargarDesdeSpritesheet(rutaBase + "Orc-Idle.png",   100, 100, 6), true);
        agregarAnimacion("attack", cargarDesdeSpritesheet(rutaBase + "Orc-Attack01.png", 100, 100, 6), false);
        agregarAnimacion("hurt",   cargarDesdeSpritesheet(rutaBase + "Orc-Hurt.png",   100, 100, 4), false);
        agregarAnimacion("death",  cargarDesdeSpritesheet(rutaBase + "Orc-Death.png",  100, 100, 4), false);
        agregarAnimacion("walk",   cargarDesdeSpritesheet(rutaBase + "Orc-Walk.png",   100, 100, 6), false);
        break;

    case Rider:
        nombre = "Ogro Rider";
        vidaMax = vidaActual = 120;
        ataque = 20;

        agregarAnimacion("idle",   cargarDesdeSpritesheet(rutaBase + "Orc rider-Idle.png",   100, 100, 6), true);
        agregarAnimacion("attack", cargarDesdeSpritesheet(rutaBase + "Orc rider-Attack01.png", 100, 100, 8), false);
        agregarAnimacion("hurt",   cargarDesdeSpritesheet(rutaBase + "Orc rider-Hurt.png",   100, 100, 4), false);
        agregarAnimacion("death",  cargarDesdeSpritesheet(rutaBase + "Orc rider-Death.png",  100, 100, 4), false);
        agregarAnimacion("walk",   cargarDesdeSpritesheet(rutaBase + "Orc rider-Walk.png",   100, 100, 6), false);
        break;

    case Armored:
        nombre = "Ogro Armored";
        vidaMax = vidaActual = 150;
        ataque = 20;

        agregarAnimacion("idle",   cargarDesdeSpritesheet(rutaBase + "Armored Orc-Idle.png",   100, 100, 6), true);
        agregarAnimacion("attack", cargarDesdeSpritesheet(rutaBase + "Armored Orc-Attack01.png", 100, 100, 7), false);
        agregarAnimacion("hurt",   cargarDesdeSpritesheet(rutaBase + "Armored Orc-Hurt.png",   100, 100, 4), false);
        agregarAnimacion("death",  cargarDesdeSpritesheet(rutaBase + "Armored Orc-Death.png",  100, 100, 4), false);
        agregarAnimacion("walk",   cargarDesdeSpritesheet(rutaBase + "Armored Orc-Walk.png",   100, 100, 6), false);
        break;

    case Elite:
        nombre = "Ogro Elite";
        vidaMax = vidaActual = 200;
        //ataque = 25;
        ataque = 150;

        agregarAnimacion("idle",   cargarDesdeSpritesheet(rutaBase + "Elite Orc-Idle.png",   100, 100, 6), true);
        agregarAnimacion("attack", cargarDesdeSpritesheet(rutaBase + "Elite Orc-Attack01.png", 100, 100, 6), false);
        agregarAnimacion("hurt",   cargarDesdeSpritesheet(rutaBase + "Elite Orc-Hurt.png",   100, 100, 4), false);
        agregarAnimacion("death",  cargarDesdeSpritesheet(rutaBase + "Elite Orc-Death.png",  100, 100, 4), false);
        agregarAnimacion("walk",   cargarDesdeSpritesheet(rutaBase + "Elite Orc-Walk.png",   100, 100, 6), false);
        break;
    }

    this->nombre = nombre;
    setEstado("idle");

    connect(this, &Personaje::animacionTerminada, this, [=](QString estado){
        if (estado == "attack" || estado == "especial" || estado == "hurt")
            setEstado("idle");
    });
}
