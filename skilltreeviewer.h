#ifndef SKILLTREEVIEWER_H
#define SKILLTREEVIEWER_H

#include <QDialog>
#include <QTreeWidget>
#include <QPushButton>
#include "ArbolHabilidad.h"

#include <QWidget>
#include <QVector>
#include "Personaje.h"

class SkillTreeViewer : public QWidget {
    Q_OBJECT

public:
    SkillTreeViewer(QVector<Personaje*>& aliados, QWidget* parent = nullptr);

private:
    void crearVistaPara(Personaje* personaje, QWidget* contenedor);
};

#endif // SKILLTREEVIEWER_H
