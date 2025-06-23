#include "SkillTreeViewer.h"
#include "Aliados.h"
#include "arbolhabilidad.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsScene>

SkillTreeViewer::SkillTreeViewer(QVector<Personaje*>& aliados, QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Árboles de Habilidad");
    setFixedSize(1000, 400);

    QHBoxLayout* layoutPrincipal = new QHBoxLayout(this);

    for (Personaje* p : aliados) {
        QWidget* contenedor = new QWidget(this);
        contenedor->setFixedSize(300, 380);
        contenedor->setStyleSheet("background-color: #111; border: 2px solid white;");
        layoutPrincipal->addWidget(contenedor);

        crearVistaPara(p, contenedor);
    }
}

void SkillTreeViewer::crearVistaPara(Personaje* personaje, QWidget* contenedor) {
    Aliado* aliado = dynamic_cast<Aliado*>(personaje);
    if (!aliado || !aliado->getArbol()) return;

    QVBoxLayout* layout = new QVBoxLayout(contenedor);

    QLabel* nombre = new QLabel(personaje->getNombre());
    nombre->setStyleSheet("color: white; font-size: 16px;");
    nombre->setAlignment(Qt::AlignCenter);
    layout->addWidget(nombre);

    QWidget* arbolWidget = new QWidget();
    arbolWidget->setMinimumHeight(300);
    layout->addWidget(arbolWidget);

    QVBoxLayout* arbolLayout = new QVBoxLayout(arbolWidget);
    NodoSkill* raiz = aliado->getArbol()->getRaiz();

    // Recorremos recursivamente el árbol
    std::function<void(NodoSkill*, int)> recorrer = [&](NodoSkill* nodo, int nivel) {
        if (!nodo) return;

        QPushButton* btn = new QPushButton(nodo->nombre);
        btn->setEnabled(!nodo->desbloqueado && personaje->getNivel() >= nodo->nivelRequerido);
        btn->setStyleSheet(nodo->desbloqueado
                               ? "background-color: green; color: white;"
                               : "background-color: gray; color: white;");

        connect(btn, &QPushButton::clicked, this, [=]() {
            nodo->desbloqueado = true;
            btn->setEnabled(false);
            btn->setStyleSheet("background-color: green; color: white;");
        });

        arbolLayout->addWidget(btn);
        recorrer(nodo->izquierda, nivel + 1);
        recorrer(nodo->derecha, nivel + 1);
    };

    recorrer(raiz, 0);
}
