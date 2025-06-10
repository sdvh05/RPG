#ifndef INVENTARIOWIDGET_H
#define INVENTARIOWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include "Inventario.h"

class InventarioWidget : public QWidget {
    Q_OBJECT

private:
    Inventario* inventario;

    QScrollArea* scrollArea;
    QWidget* contentWidget;
    QVBoxLayout* layout;

    // Muestra los objetos ordenados por rareza
    void mostrarObjetosPorRarezaOrdenada();

    // Crea el widget visual para un objeto
    QWidget* crearWidgetObjeto(const Objeto& obj);

protected:
    // Para detectar clicks en widgets objetos
    bool eventFilter(QObject* obj, QEvent* event) override;

public:
    explicit InventarioWidget(Inventario* inv, QWidget* parent = nullptr);

    // Actualiza la vista del inventario
    void actualizarVista();
};

#endif // INVENTARIOWIDGET_H
