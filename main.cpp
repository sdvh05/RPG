#include "mainwindow.h"
#include "skilltreeviewer.h"
#include "personaje.h"
#include "ArbolHabilidad.h"
#include "personaje.h"
#include "Aliados.h"

#include "Curandera.h"
#include "BattleWidget.h"

#include "grafomapavisual.h"


#include <QApplication>
#include <QPixmap>
#include <QDebug>
#include <QLabel>
#include <QStatusBar>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();*/
    QApplication a(argc, argv);

    QPixmap test("Personajes/Aliados/Caballero/Knight-Idle.png");
    if (test.isNull()) {
        qDebug() << "ERROR cargando imagen desde ruta relativa.";
    } else {
        qDebug() << "Imagen cargada correctamente.";
    }

    /*
    Curandera* personaje = new Curandera();
    SkillTreeViewer* viewer = new SkillTreeViewer(personaje->getArbol(), personaje->getNivel(), nullptr);
    viewer->exec();
*/

   // QApplication a(argc, argv);
    BattleWidget w;
    w.show();
    //return a.exec();



//-----------------------------------
    MapaWidget mapa;
    mapa.show();
    return a.exec();

    return 0;
}
