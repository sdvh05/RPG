#ifndef TIENDAWIDGET_H
#define TIENDAWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include "Inventario.h"

class TiendaWidget : public QWidget {
    Q_OBJECT

public:
    explicit TiendaWidget(Inventario* inv, QWidget* parent = nullptr);

private:
    Inventario* inventario;
    QVBoxLayout* layoutPrincipal;
    QScrollArea* scrollArea;
    QWidget* contenedor;
    QVBoxLayout* layoutContenedor;
    QLabel* lblMonedas;
    QPixmap fondo;

    struct ItemTienda {
        QString nombre;
        QString rutaImagen;
        int precio;

        std::function<void()> accionComprar;
    };

    QVector<ItemTienda> itemsDisponibles;
    void crearInterfaz();
    void mostrarItems();
    QWidget* crearWidgetItem(const ItemTienda& item);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // TIENDAWIDGET_H
