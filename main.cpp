#include "mainwindow.h"
#include "skilltreeviewer.h"
#include "personaje.h"
#include "ArbolHabilidad.h"
#include "personaje.h"
#include "Aliados.h"
#include "InventarioWidget.h"
#include"inventario.h"
#include "Objeto.h"

#include "Curandera.h"
#include "BattleWidget.h"

#include "grafomapavisual.h"
#include "gamewindow.h"

#include <QApplication>
#include <QPixmap>
#include <QDebug>
#include <QLabel>
#include <QStatusBar>
#include <QMainWindow>

Inventario* inventarioGlobal = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //return a.exec();


    /*
    Curandera* personaje = new Curandera();
    SkillTreeViewer* viewer = new SkillTreeViewer(personaje->getArbol(), personaje->getNivel(), nullptr);
    viewer->exec();
*/

   // QApplication a(argc, argv);

    /*QVector<Personaje*> aliados;
    aliados.append(new Curandera());
    aliados.append(new Caballero());
    aliados.append(new Princesa());
    BattleWidget* batalla = new BattleWidget("BosqueJS", "ogros normales",aliados);
    batalla->show();*/

    //return a.exec();

    inventarioGlobal = new Inventario();

    inventarioGlobal->agregarNucleo();
    inventarioGlobal->agregarPoscionVidaGrande();
    inventarioGlobal->agregarPoscionVidaGrande();
    inventarioGlobal->agregarLlave();
    inventarioGlobal->agregarPoscionVida(3);
    inventarioGlobal->agregarPoscionMana(3);


    //InventarioWidget* inventarioUI = new InventarioWidget(inventarioGlobal);
    //inventarioUI->show();



//-----------------------------------
    //MapaWidget mapa;
    //mapa.show();
    return a.exec();

    return 0;
}
