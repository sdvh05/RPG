#ifndef OGRO_H
#define OGRO_H

#include "Personaje.h"

class Ogro : public Personaje {
public:
    enum TipoOgro {
        Normal,
        Rider,
        Armored,
        Elite
    };

    explicit Ogro(TipoOgro tipo);
};

#endif // OGRO_H
