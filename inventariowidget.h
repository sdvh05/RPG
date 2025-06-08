#ifndef INVENTARIOWIDGET_H
#define INVENTARIOWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "Inventario.h"


class InventarioWidget : public QWidget {
    Q_OBJECT

private:
    Inventario* inventario;
    QVBoxLayout* layout;

    void mostrarObjetosVisual(NodoInventario* nodo);
    void mostrarObjetosPorRarezaOrdenada();

public:
    explicit InventarioWidget(Inventario* inv, QWidget* parent = nullptr);
    void actualizarVista();

};

#endif // INVENTARIOWIDGET_H
