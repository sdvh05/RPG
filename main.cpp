#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "GraficadorEstruDat.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);


    //MainWindow w;
    //w.show();

    GraficadorEstruDat G;
    G.show();

    return a.exec();
}
