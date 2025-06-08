#ifndef EFECTOVISUAL_H
#define EFECTOVISUAL_H

#include <QPixmap>
#include <QRect>
#include <QTimer>

struct EfectoVisual {
    QPixmap sprite;
    QRect posicion;
    int duracion;
    QTimer* timer;
};

#endif // EFECTOVISUAL_H
