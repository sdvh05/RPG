#include "InventarioWidget.h"
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QHBoxLayout>
#include <QEvent>
#include <QFrame>

InventarioWidget::InventarioWidget(Inventario* inv, QWidget* parent)
    : QWidget(parent), inventario(inv) {

    layoutPrincipal = new QVBoxLayout(this);
    setLayout(layoutPrincipal);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    layoutPrincipal->addWidget(scrollArea);

    contenedor = new QWidget(this);
    layoutContenedor = new QVBoxLayout(contenedor);
    contenedor->setLayout(layoutContenedor);
    scrollArea->setWidget(contenedor);

    botonUsar = new QPushButton("Usar objeto", this);
    layoutPrincipal->addWidget(botonUsar);

    connect(botonUsar, &QPushButton::clicked, this, &InventarioWidget::usarObjetoSeleccionado);

    actualizarVista();
}

void InventarioWidget::actualizarVista() {
    // Limpiar vista
    QLayoutItem* child;
    while ((child = layoutContenedor->takeAt(0)) != nullptr) {
        if (child->widget()) {
            mapaItems.remove(child->widget());
            child->widget()->removeEventFilter(this);
            delete child->widget();
        }
        delete child;
    }

    seleccionadoActual.clear();
    mostrarObjetos();
}

void InventarioWidget::mostrarObjetos() {
    QStringList ordenRarezas = { "Legendario", "Epico", "Raro", "Comun" };

    for (const QString& rareza : ordenRarezas) {
        QVector<Objeto*> lista;
        inventario->recogerPorRareza(rareza, lista);
        for (Objeto* obj : lista) {
            QWidget* w = crearWidgetObjeto(*obj);
            mapaItems[w] = obj->nombre;
            layoutContenedor->addWidget(w);
        }
    }
}

QWidget* InventarioWidget::crearWidgetObjeto(const Objeto& obj) {
    QWidget* card = new QWidget(this);
    QHBoxLayout* layoutCard = new QHBoxLayout(card);
    layoutCard->setContentsMargins(10, 10, 10, 10);

    QLabel* imagen = new QLabel(card);
    QPixmap pix(obj.RutaImagen);
    imagen->setPixmap(pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imagen->setFixedSize(70, 70);

    QString color;
    if (obj.rareza == "Legendario") color = "#FFD700";
    else if (obj.rareza == "Epico") color = "#800080";
    else if (obj.rareza == "Raro") color = "#1E90FF";
    else color = "#228B22";

    imagen->setStyleSheet(QString("border: 3px solid %1; border-radius: 6px;").arg(color));

    QLabel* info = new QLabel(card);
    info->setText(QString("%1\nTipo: %2\nRareza: %3\nx%4")
                      .arg(obj.nombre)
                      .arg(obj.tipo)
                      .arg(obj.rareza)
                      .arg(obj.cantidad));
    info->setStyleSheet("font-size: 14px;");

    layoutCard->addWidget(imagen);
    layoutCard->addWidget(info);

    card->installEventFilter(this);
    card->setStyleSheet("background-color: gray; border: 1px solid #ccc; border-radius: 8px;");
    return card;
}

bool InventarioWidget::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QWidget* widget = qobject_cast<QWidget*>(watched);
        if (widget && mapaItems.contains(widget)) {
            seleccionadoActual = mapaItems[widget];
            qDebug() << "Seleccionaste:" << seleccionadoActual;

            // Resaltar seleccionado
            for (auto it = mapaItems.begin(); it != mapaItems.end(); ++it) {
                if (it.key() == widget) {
                    it.key()->setStyleSheet("background-color: #a8dadc; border: 2px solid #457b9d; border-radius: 8px;");
                } else {
                    it.key()->setStyleSheet("background-color: gray; border: 1px solid #ccc; border-radius: 8px;");
                }
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void InventarioWidget::usarObjetoSeleccionado() {
    if (seleccionadoActual.isEmpty()) {
        qDebug() << "No has seleccionado ningún objeto.";
        return;
    }

    Objeto* obj = inventario->obtenerObjeto(seleccionadoActual);
    if (!obj) {
        qDebug() << "El objeto no existe en el inventario.";
        return;
    }

    if (obj->tipo == "Curacion") {
        inventario->usarObjeto(obj->nombre);
        emit objetoUsado(*obj);
        actualizarVista();
        this->close();
    } else {
        qDebug() << obj->nombre << "no se puede usar.";
    }
}


