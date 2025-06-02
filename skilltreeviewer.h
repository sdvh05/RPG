#ifndef SKILLTREEVIEWER_H
#define SKILLTREEVIEWER_H

#include <QDialog>
#include <QTreeWidget>
#include <QPushButton>
#include "ArbolHabilidad.h"

class SkillTreeViewer : public QDialog {
    Q_OBJECT

    ArbolHabilidad* arbol;
    int nivelActual;

    QTreeWidget* treeWidget;
    QPushButton* btnDesbloquear;

public:
    SkillTreeViewer(ArbolHabilidad* arbol, int nivel, QWidget* parent = nullptr);

private slots:
    void desbloquearHabilidades();
    void actualizarArbolVisual();

    void recorrerNodoVisual(NodoSkill* nodo, QTreeWidgetItem* parent = nullptr);
};

#endif // SKILLTREEVIEWER_H
