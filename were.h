#ifndef WERE_H
#define WERE_H

#include "Personaje.h"

class Were : public Personaje {
public:
    enum TipoWere {
        WereWolf,
        WereBear
    };

    explicit Were(TipoWere tipo);
};

#endif // WERE_H
