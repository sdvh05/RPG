#include "TiendaWidget.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QDebug>

TiendaWidget::TiendaWidget(Inventario* inv, QWidget* parent)
    : QWidget(parent), inventario(inv) {

    setFixedSize(700, 700);
    fondo.load("Personajes/MapasCombate/Shop.png");

    layoutPrincipal = new QVBoxLayout(this);
    layoutPrincipal->setAlignment(Qt::AlignTop);

    // Título
    QLabel* titulo = new QLabel("Tienda de Objetos");
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("font-size: 26px; font-weight: bold; padding: 20px;");
    layoutPrincipal->addWidget(titulo);

    // Monedas
    QHBoxLayout* layoutMonedas = new QHBoxLayout();
    QLabel* iconoMonedas = new QLabel(this);
    QPixmap monedaPixmap("Personajes/Items/Golden Coin.png");
    iconoMonedas->setPixmap(monedaPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconoMonedas->setFixedSize(36, 36);
    lblMonedas = new QLabel("Monedas: " + QString::number(inventario->getDineroActual()), this);
    lblMonedas->setStyleSheet("font-size: 16px; color: #222222; font-weight: bold;");
    layoutMonedas->addWidget(iconoMonedas);
    layoutMonedas->addSpacing(10);
    layoutMonedas->addWidget(lblMonedas);
    layoutMonedas->addStretch();
    layoutPrincipal->addLayout(layoutMonedas);


    // Scroll y contenedor
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    layoutPrincipal->addWidget(scrollArea);

    contenedor = new QWidget(this);
    layoutContenedor = new QVBoxLayout(contenedor);
    contenedor->setLayout(layoutContenedor);
    scrollArea->setWidget(contenedor);

    // Agregar ítems a la tienda
    itemsDisponibles = {
        { "Pocion Vida", "Personajes/Items/Red Potion.png", 20, [=]() {
             inventario->agregarPoscionVida(1);
         }},
        { "Pocion Mana", "Personajes/Items/Blue Potion.png", 20, [=]() {
             inventario->agregarPoscionMana(1);
         }},
        { "Pocion Vida XL", "Personajes/Items/Red Potion 3.png", 60, [=]() {
             inventario->agregarPoscionVidaGrande(1);
         }},
        { "Pocion Mana XL", "Personajes/Items/Blue Potion 3.png", 60, [=]() {
             inventario->agregarPoscionManaGrande(1);
         }}
    };

    mostrarItems();
}

void TiendaWidget::mostrarItems() {
    layoutContenedor->setAlignment(Qt::AlignTop);
    lblMonedas->setText("Monedas: " + QString::number(inventario->getDineroActual()));

    for (const ItemTienda& item : itemsDisponibles) {
        QWidget* w = crearWidgetItem(item);
        layoutContenedor->addWidget(w);
    }
}

QWidget* TiendaWidget::crearWidgetItem(const ItemTienda& item) {
    QWidget* card = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(card);
    layout->setContentsMargins(10, 10, 10, 10);

    QLabel* imagen = new QLabel(card);
    QPixmap pix(item.rutaImagen);
    imagen->setPixmap(pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imagen->setFixedSize(70, 70);
    imagen->setStyleSheet("border: 2px solid gold; border-radius: 4px;");

    QLabel* info = new QLabel(item.nombre + "\nPrecio: " + QString::number(item.precio));
    info->setStyleSheet("font-size: 14px; color: #222222;");
    info->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QPushButton* btnComprar = new QPushButton("Comprar");
    btnComprar->setCursor(Qt::PointingHandCursor);
    btnComprar->setStyleSheet(
        "background-color: #c0392b;""color: white;""padding: 6px;""border-radius: 4px;");

    btnComprar->setFixedWidth(100);

    connect(btnComprar, &QPushButton::clicked, this, [=]() {
        if (inventario->getDineroActual() >= item.precio) {
            inventario->gastarDinero(item.precio);
            item.accionComprar();
            qDebug() << "Compraste:" << item.nombre;
            layoutContenedor->parentWidget()->update();
            lblMonedas->setText("Monedas: " + QString::number(inventario->getDineroActual()));
        } else {
            qDebug() << "No tienes suficiente dinero.";
        }
    });

    layout->addWidget(imagen);
    layout->addWidget(info);
    layout->addStretch();
    layout->addWidget(btnComprar);
    card->setStyleSheet("background-color: #eee; border-radius: 8px;");
    return card;
}

void TiendaWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    if (!fondo.isNull()) {
        painter.drawPixmap(rect(), fondo);
    }
    QWidget::paintEvent(event);
}
