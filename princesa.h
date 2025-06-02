#ifndef PRINCESA_H
#define PRINCESA_H

#include "Aliados.h"

class Princesa : public Aliado {
public:
    Princesa();
    void ataqueEspecial(std::vector<Personaje*>& aliados, std::vector<Personaje*>& enemigos) override;
};

#endif
