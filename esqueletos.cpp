#include "esqueletos.h""

Esqueleto::Esqueleto(TipoEsqueleto tipo)
    : Personaje("", 0, 0, 0, false)
{
    QString rutaBase = "Personajes/Enemigos/Esqueletos/";
    QString nombre;

    switch (tipo) {
    case ENormal:
        nombre = "Esqueleto";
        vidaMax = vidaActual = 50;
        ataque = 15;

        agregarAnimacion("idle",   cargarDesdeSpritesheet(rutaBase + "Skeleton-Idle.png",   100, 100, 6), true);
        agregarAnimacion("attack", cargarDesdeSpritesheet(rutaBase + "Skeleton-Attack01.png", 100, 100, 6), false);
        agregarAnimacion("hurt",   cargarDesdeSpritesheet(rutaBase + "Skeleton-Hurt.png",   100, 100, 4), false);
        agregarAnimacion("death",  cargarDesdeSpritesheet(rutaBase + "Skeleton-Death.png",  100, 100, 4), false);
        agregarAnimacion("walk",   cargarDesdeSpritesheet(rutaBase + "Skeleton-Walk.png",   100, 100, 6), false);
        break;

    case ESword:
        nombre = "Esqueleto Swords";
        vidaMax = vidaActual = 70;
        ataque = 20;

        agregarAnimacion("idle",   cargarDesdeSpritesheet(rutaBase + "Greatsword Skeleton-Idle.png",   100, 100, 6), true);
        agregarAnimacion("attack", cargarDesdeSpritesheet(rutaBase + "Greatsword Skeleton-Attack01.png", 100, 100, 6), false);
        agregarAnimacion("hurt",   cargarDesdeSpritesheet(rutaBase + "Greatsword Skeleton-Hurt.png",   100, 100, 4), false);
        agregarAnimacion("death",  cargarDesdeSpritesheet(rutaBase + "Greatsword Skeleton-Death.png",  100, 100, 4), false);
        agregarAnimacion("walk",   cargarDesdeSpritesheet(rutaBase + "Greatsword Skeleton-Walk.png",   100, 100, 6), false);
        break;

    case EArmored:
        nombre = "Esqueleto Armored";
        vidaMax = vidaActual = 100;
        ataque = 25;

        agregarAnimacion("idle",   cargarDesdeSpritesheet(rutaBase + "Armored Skeleton-Idle.png",   100, 100, 6), true);
        agregarAnimacion("attack", cargarDesdeSpritesheet(rutaBase + "Armored Skeleton-Attack01.png", 100, 100, 6), false);
        agregarAnimacion("hurt",   cargarDesdeSpritesheet(rutaBase + "Armored Skeleton-Hurt.png",   100, 100, 4), false);
        agregarAnimacion("death",  cargarDesdeSpritesheet(rutaBase + "Armored Skeleton-Death.png",  100, 100, 4), false);
        agregarAnimacion("walk",   cargarDesdeSpritesheet(rutaBase + "Armored Skeleton-Walk.png",   100, 100, 6), false);
        break;
    }

    this->nombre = nombre;
    setEstado("idle");

    connect(this, &Personaje::animacionTerminada, this, [=](QString estado){
        if (estado == "attack" || estado == "especial" || estado == "hurt")
            setEstado("idle");
    });
}
