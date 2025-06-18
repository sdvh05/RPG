#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>
#include <QList>
#include "grafomapavisual.h"
#include "grafomapa.h"
#include "personaje.h"

#include <QWidget>


#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>

#include <QStringList>

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


private slots:
    void updateMovement();
    void updateIdle();
    void updateWizardIdle();
    void updateSlime2();
    void checkSlime2Interaction();    // Colisión del segundo slime con el jugador
    void cambiarAMundoNuevo();        // Cambio de mapa

    // En private slots:
    void checkAxemanInteraction();    // Detecta proximidad
    void iniciarDialogoAxeman();      // Inicia diálogo de historia

    void checkOrcInteraction();
    void updateOrc();
    void checkOrc2Interaction();
    void updateOrc2();

    // void cambiarAMundoTres();

private:
     QVector<Personaje*> aliados;

    MapaWidget* mapaWidget = nullptr;
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *player;

    QPixmap walkSheet;
    QPixmap idleSheet;

    int frameIndex;
    int idleFrameIndex;
    QTimer *moveTimer;
    QTimer *idleTimer;

    enum Direction { None, Up, Down, Left, Right };
    Direction currentDirection;

    QList<QGraphicsItem*> bloqueadores;
    bool colisionaConBloqueadores(QGraphicsItem *item, qreal dx, qreal dy);

    QGraphicsPixmapItem *wizard;
    QPixmap wizardIdleSheet;
    QTimer *wizardIdleTimer;
    int wizardIdleFrameIndex;

    void checkWizardInteraction();
    void checkSlimeInteraction();


    // void cambiarAMundoNuevo();

    bool mundoCambiado;
    int playerSpeed;

    QGraphicsPixmapItem *slime;
    QPixmap slimeWalkSheet;
    int slimeFrameIndex;
    QTimer *slimeTimer;
    qreal slimeDx;
    void updateSlime();
    qreal slimeDy;


    QStringList dialogoWizard;
    int fraseActualWizard = 0;
    bool dialogoActivo = false;

    QGraphicsPixmapItem *dialogoCaja;

    //QGraphicsRectItem *dialogoCaja = nullptr;
    QGraphicsTextItem *dialogoTexto = nullptr;
    QGraphicsTextItem *dialogoAyuda;

    QGraphicsTextItem *labelMago;
    QGraphicsTextItem *labelSlime;


    QTimer *timerTextoWizard = nullptr;
    int letraActualWizard = 0;
    QString textoParcialWizard;
    void updateLetraWizard();



    QStringList dialogoSegundoMapa;
    int fraseActualSegundoMapa = 0;

    QTimer *timerTextoSegundoMapa = nullptr;
    QString textoParcialSegundoMapa;
    int letraActualSegundoMapa = 0;

    void updateLetraSegundoMapa();


    QGraphicsPixmapItem *axeman = nullptr;
    QPixmap axemanIdleSheet;
    int axemanIdleFrameIndex = 0;
    QTimer *axemanIdleTimer = nullptr;

    void updateAxemanIdle();

    QGraphicsPixmapItem *retratoDialogo = nullptr;
    QPixmap retratoMago, retratoCaballero;

    void mostrarRetrato(const QString &quien);

    bool reflejarSprite = false;
    void wheelEvent(QWheelEvent *event) override;

    // Slime 2
    QGraphicsPixmapItem* slime2 = nullptr;
    QTimer* slime2Timer = nullptr;
    int slime2FrameIndex = 0;
    qreal slime2Dx = 0.0, slime2Dy = 0.0;
    QGraphicsTextItem* labelSlime2 = nullptr;



    QGraphicsEllipseItem* globoOvalo = nullptr;
    QGraphicsTextItem* labelAxeman = nullptr;

    QStringList frasesAxeman;
    int fraseActualAxeman = 0;
    QTimer* axemanTextoTimer = nullptr;

    QPixmap retratoAxeman;

    bool dialogoAxemanActivo = false;

    bool dialogoAxemanYaMostrado = false;


    QGraphicsPixmapItem *orc = nullptr;
    QTimer *orcTimer = nullptr;
    QPixmap orcWalkSheet;
    int orcFrameIndex = 0;
    qreal orcDx = 1.0, orcDy = 1.0;

    QGraphicsPixmapItem *orc2 = nullptr;
    QTimer *orc2Timer = nullptr;
    int orc2FrameIndex = 0;
    qreal orc2Dx = 1.0, orc2Dy = -1.0;


    bool mundoTresActivo = false;
    void cambiarAMundoTres();

    QGraphicsRectItem* zonaTransicionMapaTres = nullptr;
    bool regresoDesdeMapaTres = false;
   // bool dialogoAxemanYaMostrado = false;


};

#endif // GAMEWINDOW_H








