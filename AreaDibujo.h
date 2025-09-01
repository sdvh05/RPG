#ifndef AREADIBUJO_H
#define AREADIBUJO_H
#include "NodoArbol.h"

#include <QWidget>
#include <QVector>

class AreaDibujo : public QWidget {
    Q_OBJECT
public:
    explicit AreaDibujo(QWidget *parent = nullptr);

    void setValores(const QVector<int> &valores);
    void setEstructura(int tipo);
    void setArbol(NodoArbol* root);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<int> valores;
    int estructuraTipo = 0;
    NodoArbol* raiz= nullptr;

    void dibujarFlecha(QPainter &painter, QPointF inicio, QPointF fin, bool doble = false);
    void dibujarHeadTail(QPainter &painter, const QPointF &pos, const QString &texto);
    void dibujarNodoArbol(QPainter &painter, NodoArbol* nodo, int x, int y, int separacionX, int nivel);
};


#endif // AREADIBUJO_H
