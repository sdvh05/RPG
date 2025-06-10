#include "InventarioWidget.h"
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMouseEvent>

InventarioWidget::InventarioWidget(Inventario* inv, QWidget* parent)
    : QWidget(parent), inventario(inv) {

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);

    contentWidget = new QWidget;
    scrollArea->setWidget(contentWidget);

    layout = new QVBoxLayout(contentWidget);
    layout->setAlignment(Qt::AlignTop);

    actualizarVista();
}

// Filtro de eventos para detectar clics en los widgets de objetos
bool InventarioWidget::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QWidget* w = qobject_cast<QWidget*>(obj);
        if (w) {
            QString nombreObjeto = w->property("nombreObjeto").toString();
            if (!nombreObjeto.isEmpty()) {
                qDebug() << "Seleccionaste:" << nombreObjeto;
            }
        }
        return true; // evento consumido
    }
    return QWidget::eventFilter(obj, event);
}

void InventarioWidget::actualizarVista() {
    // Limpia la vista anterior
    QLayoutItem* child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    mostrarObjetosPorRarezaOrdenada();
}

void InventarioWidget::mostrarObjetosPorRarezaOrdenada() {
    QStringList ordenRarezas = { "Legendario", "Epico", "Raro", "Comun" };

    for (const QString& rareza : ordenRarezas) {
        QVector<Objeto*> lista;
        inventario->recogerPorRareza(rareza, lista);
        for (Objeto* obj : lista) {
            QWidget* card = crearWidgetObjeto(*obj);
            card->installEventFilter(this);  // instalamos el event filter
            card->setProperty("nombreObjeto", obj->nombre);
            layout->addWidget(card);
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

    // Cambiar el cursor para indicar que es clickeable
    card->setCursor(Qt::PointingHandCursor);

    return card;
}

