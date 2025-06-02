#ifndef CABALLERO_H
#define CABALLERO_H

#include "Aliados.h"

class Caballero : public Aliado {
public:
    Caballero();
    void ataqueEspecial(std::vector<Personaje*>& aliados, std::vector<Personaje*>& enemigos) override;
};

#endif
