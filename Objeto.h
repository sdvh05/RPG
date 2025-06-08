#ifndef OBJETO_H
#define OBJETO_H

#include <QString>

struct Objeto {
    QString nombre;
    QString rareza;
    QString tipo; // "poscion, revivir, etc"
    QString RutaImagen;
    int cantidad;
    int valor; // efecto: cuánto cura, etc.

    Objeto(QString n, QString r, QString t,QString  ri, int c, int v)
        : nombre(n),rareza(r), tipo(t), RutaImagen(ri),cantidad(c), valor(v) {}
};



#endif // OBJETO_H
