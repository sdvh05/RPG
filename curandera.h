#ifndef CURANDERA_H
#define CURANDERA_H

#include "Aliados.h"

class Curandera : public Aliado {
public:
    Curandera();
    void ataqueEspecial(std::vector<Personaje*>& aliados, std::vector<Personaje*>& enemigos) override;
};

#endif // CURANDERA_H
