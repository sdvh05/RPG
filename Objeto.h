#ifndef OBJETO_H
#define OBJETO_H

#include <QString>

struct Objeto {
    QString nombre;
    QString rareza;
    QString tipo; // "poscion, revivir, etc"
    int cantidad;
    int valor; // efecto: cuánto cura, etc.

    Objeto(QString n, QString t, int c, int v)
        : nombre(n), tipo(t), cantidad(c), valor(v) {}
};


#endif // OBJETO_H
