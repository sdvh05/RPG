#ifndef ESQUELETO_H
#define ESQUELETO_H

#include "Personaje.h"

class Esqueleto : public Personaje {
public:
    enum TipoEsqueleto {
        ENormal,
        ESword,
        EArmored
    };

    explicit Esqueleto(TipoEsqueleto tipo);
};

#endif // ESQUELETO_H
