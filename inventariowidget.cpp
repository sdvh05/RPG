#include "InventarioWidget.h"
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

InventarioWidget::InventarioWidget(Inventario* inv, QWidget* parent)
    : QWidget(parent), inventario(inv) {
    layout = new QVBoxLayout(this);
    setLayout(layout);
    actualizarVista();
}

void InventarioWidget::mostrarObjetosVisual(NodoInventario* nodo) {
    if (!nodo) return;

    mostrarObjetosVisual(nodo->izq);

    // Widget contenedor para cada objeto
    QWidget* card = new QWidget(this);
    QHBoxLayout* cardLayout = new QHBoxLayout(card);
    card->setLayout(cardLayout);

    // Imagen del objeto con borde según rareza
    QLabel* imagen = new QLabel(card);
    QPixmap pix(nodo->obj.RutaImagen);
    imagen->setPixmap(pix.scaled(64, 64, Qt::KeepAspectRatio));
    imagen->setFixedSize(70, 70); // espacio para borde

    QString colorBorde;

    if (nodo->obj.rareza == "Legendario") colorBorde = "#FFD700";  // Dorado
    else if (nodo->obj.rareza == "Epico") colorBorde = "#800080";  // Morado
    else if (nodo->obj.rareza == "Raro") colorBorde = "#1E90FF";   // Azul
    else colorBorde = "#228B22";                                   // Verde

    imagen->setStyleSheet(QString("border: 3px solid %1; border-radius: 6px;").arg(colorBorde));

    // Información del objeto
    QLabel* info = new QLabel(card);
    QString texto = QString("%1\nTipo: %2\nRareza: %3\nx%4")
                        .arg(nodo->obj.nombre)
                        .arg(nodo->obj.tipo)
                        .arg(nodo->obj.rareza)
                        .arg(nodo->obj.cantidad);
    info->setText(texto);
    info->setStyleSheet("font-size: 14px;");

    cardLayout->addWidget(imagen);
    cardLayout->addWidget(info);
    layout->addWidget(card);

    mostrarObjetosVisual(nodo->der);
}

void InventarioWidget::actualizarVista() {
    // Limpia la vista anterior
    QLayoutItem* child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    // Mostrar ordenado por rareza
    mostrarObjetosPorRarezaOrdenada();
}


//--------------------- Ordenamiento -------------
void InventarioWidget::mostrarObjetosPorRarezaOrdenada() {
    // Rarezas en orden de prioridad
    QStringList ordenRarezas = { "Legendario", "Epico", "Raro", "Comun" };

    for (const QString& rareza : ordenRarezas) {
        QVector<Objeto*> lista;
        inventario->recogerPorRareza(rareza, lista);
        for (Objeto* obj : lista) {
            QWidget* card = new QWidget(this);
            QHBoxLayout* cardLayout = new QHBoxLayout(card);

            QLabel* imagen = new QLabel(card);
            QPixmap pix(obj->RutaImagen);
            imagen->setPixmap(pix.scaled(64, 64, Qt::KeepAspectRatio));
            imagen->setFixedSize(70, 70);

            QString colorBorde;
            if (obj->rareza == "Legendario") colorBorde = "#FFD700";
            else if (obj->rareza == "Epico") colorBorde = "#800080";
            else if (obj->rareza == "Raro") colorBorde = "#1E90FF";
            else colorBorde = "#228B22";

            imagen->setStyleSheet(QString("border: 3px solid %1; border-radius: 6px;").arg(colorBorde));

            QLabel* info = new QLabel(card);
            QString texto = QString("%1\nTipo: %2\nRareza: %3\nx%4")
                                .arg(obj->nombre)
                                .arg(obj->tipo)
                                .arg(obj->rareza)
                                .arg(obj->cantidad);
            info->setText(texto);
            info->setStyleSheet("font-size: 14px;");

            cardLayout->addWidget(imagen);
            cardLayout->addWidget(info);
            layout->addWidget(card);
        }
    }
}

