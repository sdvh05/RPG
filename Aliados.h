#ifndef ALIADO_H
#define ALIADO_H

#include "Personaje.h"
#include "ArbolHabilidad.h"

class Aliado : public Personaje {
    Q_OBJECT

protected:
    ArbolHabilidad* arbol;

public:
    Aliado(QString nombre, int vida, int mana, int ataque);
    virtual void ataqueEspecial(std::vector<Personaje*>& aliados, std::vector<Personaje*>& enemigos) = 0;

    ArbolHabilidad* getArbol() const;
    void subirNivel();
};

#endif
