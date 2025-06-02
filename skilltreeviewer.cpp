#include "SkillTreeViewer.h"
#include <QVBoxLayout>
#include <QTreeWidgetItem>

SkillTreeViewer::SkillTreeViewer(ArbolHabilidad* arbol, int nivel, QWidget* parent)
    : QDialog(parent), arbol(arbol), nivelActual(nivel) {

    setWindowTitle("Árbol de Habilidades");
    resize(400, 500);

    treeWidget = new QTreeWidget(this);
    treeWidget->setHeaderLabels(QStringList() << "Habilidad" << "Estado");

    btnDesbloquear = new QPushButton("Desbloquear según nivel", this);
    connect(btnDesbloquear, &QPushButton::clicked, this, &SkillTreeViewer::desbloquearHabilidades);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(treeWidget);
    layout->addWidget(btnDesbloquear);

    actualizarArbolVisual();
}

void SkillTreeViewer::desbloquearHabilidades() {
    if (arbol) {
        arbol->desbloquearPorNivel(nivelActual);
        actualizarArbolVisual();
    }
}

void SkillTreeViewer::actualizarArbolVisual() {
    treeWidget->clear();
    if (arbol && arbol->getRaiz()) {
        recorrerNodoVisual(arbol->getRaiz(), nullptr);
    }
}

void SkillTreeViewer::recorrerNodoVisual(NodoSkill* nodo, QTreeWidgetItem* parent) {
    if (!nodo) return;

    QString estado = nodo->desbloqueado ? "✔ Desbloqueado" : "❌ Bloqueado";
    QString texto = nodo->nombre + ": " + nodo->descripcion;

    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << texto << estado);
    if (parent)
        parent->addChild(item);
    else
        treeWidget->addTopLevelItem(item);

    if (nodo->izquierda)
        recorrerNodoVisual(nodo->izquierda, item);
    if (nodo->derecha)
        recorrerNodoVisual(nodo->derecha, item);

    item->setExpanded(true);
}
