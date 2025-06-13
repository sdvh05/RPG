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


private:
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


    void cambiarAMundoNuevo();

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



};

#endif // GAMEWINDOW_H


