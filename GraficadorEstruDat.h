#ifndef GRAFICADORESTRUDAT_H
#define GRAFICADORESTRUDAT_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsView>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QString>
#include <QFileDialog>

#include "AreaDibujo.h"

#include "LinkedList.h"
#include "DoubleLL.h"
#include "Stack.h"
#include "Queue.h"
#include "bst.h"
#include "avl.h"

//valor buscar recolorear
//mostrar head y tail

class GraficadorEstruDat : public QWidget
{
    Q_OBJECT

public:
    explicit GraficadorEstruDat(QWidget *parent = nullptr);


private:
    // ENUM para controlar la estructura actual
    enum EstructuraActual {
        LINKED_LIST,
        DOUBLE_LL,
        STACK,
        QUEUE,
        BST,
        AVL
    } estructuraActual;

     QString extensionPorEstructura() const;

    //Estilos de Los botones
    const QString ESTILO_BOTON_NORMAL =
        "QPushButton {"
        "background-color: #da6e45;"
        "color: #ffffff;" //color de la letra
        "border: 1px solid #cccccc;"
        "border-radius: 5px;"
        "}";

    const QString ESTILO_BOTON_SELECCIONADO =
        "QPushButton {"
        "background-color: #143a64;"
        "color: #ffffff;"
        "border: 1px solid #1cfcff;"
        "border-radius: 5px;"
        "}";

    const QString ESTILO_CONTROLADOR =
        "QPushButton {"
        "background-color: #505948;"
        "color: #ffffff;"
        "border: 1px solid #cccccc;"
        "border-radius: 5px;"
        "}";


    // UI
    QHBoxLayout *layoutPrincipal;      // Header
    QVBoxLayout *layoutIzquierda;      // Panel de control
    QVBoxLayout *layoutDerecha;        //areaGrafico
    QHBoxLayout *layoutArriba;
    //QFrame *areaGrafico;
    AreaDibujo *areaGrafico;

    // Botones de selección de estructura
    QMap<EstructuraActual, QPushButton*> botonesEstructuras;

    // Botones panel de control
    QPushButton *btnAccion1; // Insertar
    QPushButton *btnAccion2; // Eliminar
    QPushButton *btnAccion3; // Datos

    // Barra superior
    QLabel *lblTituloEstructura;
    QPushButton *btnGuardar;
    QPushButton *btnConfig;


    // Estructuras de datos
    LinkedList listaSimple;
    DoubleLL listaDoble;
    Stack pila;
    Queue cola;
    Bst bst;
    Avl avl;

    //Funcoines
    void crearInterfaz();
    void cambiarTextos();

    void accionPila(int opcion);
    void accionCola(int opcion);
    void accionLista(int opcion);
    void accionDLL(int opcion);
    void accionBST(int opcion);
    void accionAVL(int opcion);

    void dibujarNodos(const QVector<int> &valores);
    void crearCarpetas();
    QString carpetaPorEstructura();

private slots:
    void seleccionarEstructura(EstructuraActual estructura);
    void onAccion1(); // Insertar
    void onAccion2(); // Eliminar
    void onAccion3(); // Datos

    void guardarDatos();
    void cargarDatos();
};

#endif // GRAFICADORESTRUDAT_H
