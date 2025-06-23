#include "GameWindow.h"
#include "battlewidget.h"
#include <QMessageBox>
#include "CustomGraphicsView.h"
#include <QGraphicsPixmapItem>

#include "InventarioWidget.h"
#include"inventario.h"
#include "Objeto.h"
#include "grafomapavisual.h"

extern Inventario* inventarioGlobal;

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent), frameIndex(0), currentDirection(None), idleFrameIndex(0)
{
    this->setWindowTitle("Mapa del Juego");

    Curandera *curandera = new Curandera();
    Caballero *caballero = new Caballero();
    Princesa *princesa = new Princesa();

    aliados.append(curandera);
    aliados.append(caballero);
    aliados.append(princesa);


    scene = new QGraphicsScene(this);
    view = new CustomGraphicsView(scene, this);

    retratoMago.load("OpenSprite/retrato_mago.png");
    retratoCaballero.load("OpenSprite/retrato_caballero.png");
    retratoAxeman.load("OpenSprite/retrato_axeman.png"); // Asumiendo que lo tienes así



    // Cargar mapa
    QPixmap mapa("OpenSprite/c.png");

    if (mapa.isNull()) {
        qDebug("Error: No se pudo cargar el mapa.");
    } else {
        // Añadir mapa a la escena
        scene->addPixmap(mapa);
        scene->setSceneRect(0, 0, mapa.width(), mapa.height());

        // Tamaño visible de la ventana
        int anchoVentana = 700;
        int altoVentana = 700;

        this->resize(anchoVentana, altoVentana);
        view->setGeometry(0, 0, anchoVentana, altoVentana);

        view->resetTransform();
        view->scale(2.2, 2.2);  // Escalado visual opcional

        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    }

    // ------------------------------------------Caballero ----------------------------------------------------------------------------------
    // Cargar Knight-Walk
    walkSheet = QPixmap("OpenSprite/Knight-Walk.png");
    if (walkSheet.isNull()) {
        qDebug("Error: No se pudo cargar el Knight-Walk.");
    }

    // Cargar Knight-Idle
    idleSheet = QPixmap("OpenSprite/Knight-Idle.png");
    if (idleSheet.isNull()) {
        qDebug("Error: No se pudo cargar el Knight-Idle.");
    }

    if (!idleSheet.isNull()) {
        QPixmap frame = idleSheet.copy(0, 0, 100, 100);
        player = scene->addPixmap(frame);
        player->setPos(120, 160);
    }

    // Crear Timer de movimiento
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &GameWindow::updateMovement);

    // Crear Timer de Idle
    idleTimer = new QTimer(this);
    connect(idleTimer, &QTimer::timeout, this, &GameWindow::updateIdle);
    mundoCambiado = false;
    playerSpeed = 5;


    //------------------------------------------------- colisiones villac++ --------------------------------------------------------
    QGraphicsRectItem *bloque = scene->addRect(1, 1, 334, 127);
    bloque->setBrush(Qt::transparent);
    bloque->setPen(Qt::NoPen);
    bloqueadores.append(bloque);

    QGraphicsRectItem *bloque2 = scene->addRect(70, 200, 29, 29);
    bloque2->setBrush(Qt::transparent);
    bloque2->setPen(Qt::NoPen);
    bloqueadores.append(bloque2);

    QGraphicsRectItem *bloque3 = scene->addRect(44, 136, 44, 10);
    bloque3->setBrush(Qt::transparent);
    bloque3->setPen(Qt::NoPen);
    bloqueadores.append(bloque3);

    QGraphicsRectItem *bloque4 = scene->addRect(29, 156, 23, 22);
    bloque4->setBrush(Qt::transparent);
    bloque4->setPen(Qt::NoPen);
    bloqueadores.append(bloque4);

    QGraphicsRectItem *bloque5 = scene->addRect(24, 180, 10, 75);
    bloque5->setBrush(Qt::transparent);
    bloque5->setPen(Qt::NoPen);
    bloqueadores.append(bloque5);

    QGraphicsRectItem *bloque6 = scene->addRect(9, 265, 11, 48);
    bloque6->setBrush(Qt::transparent);
    bloque6->setPen(Qt::NoPen);
    bloqueadores.append(bloque6);

    QGraphicsRectItem *bloque7 = scene->addRect(26, 316, 11, 49);
    bloque7->setBrush(Qt::transparent);
    bloque7->setPen(Qt::NoPen);
    bloqueadores.append(bloque7);

    QGraphicsRectItem *bloque8 = scene->addRect(43, 348, 11, 50);
    bloque8->setBrush(Qt::transparent);
    bloque8->setPen(Qt::NoPen);
    bloqueadores.append(bloque8);

    QGraphicsRectItem *bloque9 = scene->addRect(55, 396, 30, 50);
    bloque9->setBrush(Qt::transparent);
    bloque9->setPen(Qt::NoPen);
    bloqueadores.append(bloque9);

    QGraphicsRectItem *bloque10 = scene->addRect(85, 412, 17, 38);
    bloque10->setBrush(Qt::transparent);
    bloque10->setPen(Qt::NoPen);
    bloqueadores.append(bloque10);

    QGraphicsRectItem *bloque11 = scene->addRect(52, 450, 17, 19);
    bloque11->setBrush(Qt::transparent);
    bloque11->setPen(Qt::NoPen);
    bloqueadores.append(bloque11);

    QGraphicsRectItem *bloque12 = scene->addRect(35, 478, 17, 19);
    bloque12->setBrush(Qt::transparent);
    bloque12->setPen(Qt::NoPen);
    bloqueadores.append(bloque12);

    QGraphicsRectItem *bloque13 = scene->addRect(17, 497, 17, 16);
    bloque13->setBrush(Qt::transparent);
    bloque13->setPen(Qt::NoPen);
    bloqueadores.append(bloque13);

    QGraphicsRectItem *bloque14 = scene->addRect(8, 518, 11, 72);
    bloque14->setBrush(Qt::transparent);
    bloque14->setPen(Qt::NoPen);
    bloqueadores.append(bloque14);

    QGraphicsRectItem *bloque15 = scene->addRect(21, 586, 16, 16);
    bloque15->setBrush(Qt::transparent);
    bloque15->setPen(Qt::NoPen);
    bloqueadores.append(bloque15);

    QGraphicsRectItem *bloque16 = scene->addRect(46, 600, 25, 16);
    bloque16->setBrush(Qt::transparent);
    bloque16->setPen(Qt::NoPen);
    bloqueadores.append(bloque16);

    QGraphicsRectItem *bloque17 = scene->addRect(74, 615, 60, 16);
    bloque17->setBrush(Qt::transparent);
    bloque17->setPen(Qt::NoPen);
    bloqueadores.append(bloque17);

    QGraphicsRectItem *bloque18 = scene->addRect(133, 631, 95, 14);
    bloque18->setBrush(Qt::transparent);
    bloque18->setPen(Qt::NoPen);
    bloqueadores.append(bloque18);

    QGraphicsRectItem *bloque19 = scene->addRect(225, 613, 26, 14);
    bloque19->setBrush(Qt::transparent);
    bloque19->setPen(Qt::NoPen);
    bloqueadores.append(bloque19);

    QGraphicsRectItem *bloque20 = scene->addRect(242, 598, 26, 14);
    bloque20->setBrush(Qt::transparent);
    bloque20->setPen(Qt::NoPen);
    bloqueadores.append(bloque20);

    QGraphicsRectItem *bloque21 = scene->addRect(255, 569, 26, 23);
    bloque21->setBrush(Qt::transparent);
    bloque21->setPen(Qt::NoPen);
    bloqueadores.append(bloque21);

    QGraphicsRectItem *bloque22 = scene->addRect(275, 550, 26, 23);
    bloque22->setBrush(Qt::transparent);
    bloque22->setPen(Qt::NoPen);
    bloqueadores.append(bloque22);

    QGraphicsRectItem *bloque23 = scene->addRect(290, 519, 26, 23);
    bloque23->setBrush(Qt::transparent);
    bloque23->setPen(Qt::NoPen);
    bloqueadores.append(bloque23);

    QGraphicsRectItem *bloque24 = scene->addRect(306, 483, 26, 31);
    bloque24->setBrush(Qt::transparent);
    bloque24->setPen(Qt::NoPen);
    bloqueadores.append(bloque24);

    QGraphicsRectItem *bloque25 = scene->addRect(320, 316, 8, 184);
    bloque25->setBrush(Qt::transparent);
    bloque25->setPen(Qt::NoPen);
    bloqueadores.append(bloque25);

    QGraphicsRectItem *bloque26 = scene->addRect(301, 189, 8, 136);
    bloque26->setBrush(Qt::transparent);
    bloque26->setPen(Qt::NoPen);
    bloqueadores.append(bloque26);

    QGraphicsRectItem *bloque27 = scene->addRect(286, 199, 8, 18);
    bloque27->setBrush(Qt::transparent);
    bloque27->setPen(Qt::NoPen);
    bloqueadores.append(bloque27);

    QGraphicsRectItem *bloque28 = scene->addRect(270, 179, 16, 17);
    bloque28->setBrush(Qt::transparent);
    bloque28->setPen(Qt::NoPen);
    bloqueadores.append(bloque28);

    QGraphicsRectItem *bloque29 = scene->addRect(240, 163, 23, 17);
    bloque29->setBrush(Qt::transparent);
    bloque29->setPen(Qt::NoPen);
    bloqueadores.append(bloque29);

    QGraphicsRectItem *bloque30 = scene->addRect(208, 134, 24, 30);
    bloque30->setBrush(Qt::transparent);
    bloque30->setPen(Qt::NoPen);
    bloqueadores.append(bloque30);

    QGraphicsRectItem *bloque31 = scene->addRect(273, 364, 16, 22);
    bloque31->setBrush(Qt::transparent);
    bloque31->setPen(Qt::NoPen);
    bloqueadores.append(bloque31);

    QGraphicsRectItem *bloque32 = scene->addRect(137, 281, 40, 71);
    bloque32->setBrush(Qt::transparent);
    bloque32->setPen(Qt::NoPen);
    bloqueadores.append(bloque32);

    QGraphicsRectItem *bloque33 = scene->addRect(164, 284, 16, 68);
    bloque33->setBrush(Qt::transparent);
    bloque33->setPen(Qt::NoPen);
    bloqueadores.append(bloque33);

    QGraphicsRectItem *bloque34 = scene->addRect(184, 305, 16, 47);
    bloque34->setBrush(Qt::transparent);
    bloque34->setPen(Qt::NoPen);
    bloqueadores.append(bloque34);

    QGraphicsRectItem *bloque35 = scene->addRect(193, 315, 20, 38);
    bloque35->setBrush(Qt::transparent);
    bloque35->setPen(Qt::NoPen);
    bloqueadores.append(bloque35);

    QGraphicsRectItem *bloque36 = scene->addRect(206, 326, 19, 27);
    bloque36->setBrush(Qt::transparent);
    bloque36->setPen(Qt::NoPen);
    bloqueadores.append(bloque36);


    QGraphicsRectItem *bloque37 = scene->addRect(174, 346, 19, 27);
    bloque37->setBrush(Qt::transparent);
    bloque37->setPen(Qt::NoPen);
    bloqueadores.append(bloque37);

    QGraphicsRectItem *bloque38 = scene->addRect(188, 359, 37, 18);
    bloque38->setBrush(Qt::transparent);
    bloque38->setPen(Qt::NoPen);
    bloqueadores.append(bloque38);

    QGraphicsRectItem *bloque39 = scene->addRect(79, 286, 20, 29);
    bloque39->setBrush(Qt::transparent);
    bloque39->setPen(Qt::NoPen);
    bloqueadores.append(bloque39);

    QGraphicsRectItem *bloque40 = scene->addRect(227, 223, 2, 30);
    bloque40->setBrush(Qt::transparent);
    bloque40->setPen(Qt::NoPen);
    bloqueadores.append(bloque40);

    QGraphicsRectItem *bloque41 = scene->addRect(187, 223, 2, 29);
    bloque41->setBrush(Qt::transparent);
    bloque41->setPen(Qt::NoPen);
    bloqueadores.append(bloque41);

    QGraphicsRectItem *bloque42 = scene->addRect(187, 225, 42, 1);
    bloque42->setBrush(Qt::transparent);
    bloque42->setPen(Qt::NoPen);
    bloqueadores.append(bloque42);

    QGraphicsRectItem *bloque43 = scene->addRect(225, 414, 4, 8);
    bloque43->setBrush(Qt::transparent);
    bloque43->setPen(Qt::NoPen);
    bloqueadores.append(bloque43);

    QGraphicsRectItem *bloque44 = scene->addRect(145, 458, 34, 40);
    bloque44->setBrush(Qt::transparent);
    bloque44->setPen(Qt::NoPen);
    bloqueadores.append(bloque44);

    QGraphicsRectItem *bloque45 = scene->addRect(148, 498, 17, 64);
    bloque45->setBrush(Qt::transparent);
    bloque45->setPen(Qt::NoPen);
    bloqueadores.append(bloque45);

    QGraphicsRectItem *bloque46 = scene->addRect(163, 558, 14, 29);
    bloque46->setBrush(Qt::transparent);
    bloque46->setPen(Qt::NoPen);
    bloqueadores.append(bloque46);

    QGraphicsRectItem *bloque47 = scene->addRect(130, 545, 29, 15);
    bloque47->setBrush(Qt::transparent);
    bloque47->setPen(Qt::NoPen);
    bloqueadores.append(bloque47);

    QGraphicsRectItem *bloque48 = scene->addRect(81, 532, 40, 12);
    bloque48->setBrush(Qt::transparent);
    bloque48->setPen(Qt::NoPen);
    bloqueadores.append(bloque48);

    QGraphicsRectItem *bloque49 = scene->addRect(83, 539, 12, 21);
    bloque49->setBrush(Qt::transparent);
    bloque49->setPen(Qt::NoPen);
    bloqueadores.append(bloque49);

    QGraphicsRectItem *bloque50 = scene->addRect(135, 475, 1, 38);
    bloque50->setBrush(Qt::transparent);
    bloque50->setPen(Qt::NoPen);
    bloqueadores.append(bloque50);

    QGraphicsRectItem *bloque51 = scene->addRect(122, 489, 17, 22);
    bloque51->setBrush(Qt::transparent);
    bloque51->setPen(Qt::NoPen);
    bloqueadores.append(bloque51);

    QGraphicsRectItem *bloque52 = scene->addRect(100, 506, 11, 22);
    bloque52->setBrush(Qt::transparent);
    bloque52->setPen(Qt::NoPen);
    bloqueadores.append(bloque52);

    QGraphicsRectItem *bloque53 = scene->addRect(242, 459, 15, 38);
    bloque53->setBrush(Qt::transparent);
    bloque53->setPen(Qt::NoPen);
    bloqueadores.append(bloque53);

    QGraphicsRectItem *bloque54 = scene->addRect(257, 445, 15, 34);
    bloque54->setBrush(Qt::transparent);
    bloque54->setPen(Qt::NoPen);
    bloqueadores.append(bloque54);

    QGraphicsRectItem *bloque55 = scene->addRect(229, 485, 22, 27);
    bloque55->setBrush(Qt::transparent);
    bloque55->setPen(Qt::NoPen);
    bloqueadores.append(bloque55);

    QGraphicsRectItem *bloque56 = scene->addRect(133, 211, 11, 11);
    bloque56->setBrush(Qt::transparent);
    bloque56->setPen(Qt::NoPen);
    bloqueadores.append(bloque56);

    //------------------------------------------------------------ Mago --------------------------------------------------------------------------------------------------

    // Cargar Wizard-Idle
    wizardIdleSheet = QPixmap("OpenSprite/Wizard-Idle.png");

    if (wizardIdleSheet.isNull()) {
        qDebug("Error: No se pudo cargar Wizard-Idle.");
    } else {
        QPixmap frame = wizardIdleSheet.copy(0, 0, 100, 100);
        wizard = scene->addPixmap(frame);

        wizard->setPos(125, 490);
        bloqueadores.append(wizard);


        wizardIdleFrameIndex = 0;
        wizardIdleTimer = new QTimer(this);
        connect(wizardIdleTimer, &QTimer::timeout, this, &GameWindow::updateWizardIdle);
        wizardIdleTimer->start(250);


    }


    // ------------------------------------------------------------ Slime (NPC) --------------------------------------------------------------------------------------------------

    slimeWalkSheet = QPixmap("OpenSprite/Slime-Walk.png");

    if (slimeWalkSheet.isNull()) {
        qDebug("Error: No se pudo cargar Slime-Walk.");
    } else {
        QPixmap frame = slimeWalkSheet.copy(0, 0, 100, 100);
        slime = scene->addPixmap(frame);

        slime->setPos(110, 350);
        slime->setZValue(1);

        slimeFrameIndex = 0;
        slimeDx = 1.0;
        slimeDy = 1.0;

        slimeTimer = new QTimer(this);
        connect(slimeTimer, &QTimer::timeout, this, &GameWindow::updateSlime);
        slimeTimer->start(270);
    }

    // ------------------------------------------------------------ Segundo Slime -------------------------------------------------------------------------------------

    QPixmap frame2 = slimeWalkSheet.copy(0, 0, 100, 100);
    slime2 = scene->addPixmap(frame2);
    slime2->setPos(80, 100);
    slime2->setZValue(1);

    slime2FrameIndex = 0;
    slime2Dx = -1.0;
    slime2Dy = 1.0;

    slime2Timer = new QTimer(this);
    connect(slime2Timer, &QTimer::timeout, this, &GameWindow::updateSlime2);
    slime2Timer->start(270);






    //  Crear label del Mago
    labelMago = scene->addText("Mago");
    labelMago->setDefaultTextColor(Qt::yellow);
    labelMago->setFont(QFont("Arial", 7, QFont::Bold));
    labelMago->setZValue(2);
    labelMago->setPos(wizard->x() + 20, wizard->y() + 120);
    labelMago->setVisible(false);

    //  Crear label del Slime
    labelSlime = scene->addText("Slime");
    labelSlime->setDefaultTextColor(Qt::green);
    labelSlime->setFont(QFont("Arial", 9, QFont::Bold));
    labelSlime->setZValue(2);
    labelSlime->setPos(slime->x() + 20, slime->y() - 20);
    labelSlime->setVisible(false);

    // Crear label del slime 2
    labelSlime2= scene->addText("Slime");
    labelSlime2->setDefaultTextColor(Qt::red);
    labelSlime2->setFont(QFont("Arial", 9, QFont::Bold));
    labelSlime2->setZValue(2);
    labelSlime2->setPos(slime2->x() + 20, slime2->y() - 20);
    labelSlime2->setVisible(false);

    mundoTresActivo = false;

    this->setFocus();
}

GameWindow::~GameWindow()
{
}
void GameWindow::wheelEvent(QWheelEvent *event)
{

    event->ignore();
}


// ---------------------------------------------------------------------------------  caballero ------------------------------------------------------------------------------------------

void GameWindow::updateIdle()
{
    if (!player) return;

    int frameWidth = 100;
    int frameHeight = 100;
    int idleFrames = 4;

    QPixmap frame = idleSheet.copy(idleFrameIndex * frameWidth, 0, frameWidth, frameHeight);

    if (reflejarSprite) {
        frame = frame.transformed(QTransform().scale(-1, 1));
    }

    player->setPixmap(frame);


    idleFrameIndex = (idleFrameIndex + 1) % idleFrames;
}

//------------------------------------------------------------------------------ mago -------------------------------------------------------------------------------------------
void GameWindow::updateWizardIdle()
{
    if (!wizard) return;

    int frameWidth = 100;
    int frameHeight = 100;
    int idleFrames = 6;

    wizard->setPixmap(wizardIdleSheet.copy(wizardIdleFrameIndex * frameWidth, 0, frameWidth, frameHeight));

    wizardIdleFrameIndex = (wizardIdleFrameIndex + 1) % idleFrames;
}
void GameWindow::checkWizardInteraction()
{
    if (!player || !wizard) return;
    if (mundoCambiado || dialogoActivo) return;

    qreal distance = QLineF(player->scenePos(), wizard->scenePos()).length();

    if (distance < 35.0)
    {
        if (slimeTimer && slimeTimer->isActive()) slimeTimer->stop();
        if (moveTimer && moveTimer->isActive()) moveTimer->stop();

        dialogoWizard.clear();
        dialogoWizard
            << "Mago: ¡Saludos, caballero C++!"
            << "Caballero: ¿Quién eres tú?"
            << "Mago: Soy un mago de esta tierra... y necesito tu ayuda urgente."
            << "Caballero: ¿Ayuda? ¿Por qué yo? ¿Qué está ocurriendo?"
            << "Mago: El mundo corre peligro. Existe una piedra mágica llamada Éterium que mantenía el equilibrio."
            << "Mago: Pero el jefe oscuro la destruyó, y sus fragmentos se han esparcido por todo el reino."
            << "Caballero: ¿Y qué se supone que haga yo con eso?"
            << "Mago: Solo un valiente como tú puede reunir los fragmentos, restaurar la piedra y vencer al jefe."
            << "Caballero: No estoy seguro... No sé si puedo hacerlo."
            << "Mago: No estarás solo. Mis aliadas, la Curandera y la Princesa C#, lucharán contigo."
            << "Mago: Juntos, pueden devolver la paz al mundo. Solo necesitas dar el primer paso."
            << "Caballero: ...Está bien. Lo haré. ¡Salvaré el mundo!";

        fraseActualWizard = 0;
        dialogoActivo = true;

        // Cargar imagen de pergamino
        QPixmap pergamino("OpenSprite/pergamino.PNG");
        dialogoCaja = scene->addPixmap(pergamino);
        dialogoCaja->setZValue(9);

        // Escalado del pergamino
        qreal escalaPergamino = 0.35;
        dialogoCaja->setScale(escalaPergamino);

        // Calcular posición centrada en la parte inferior
        qreal anchoVisible = pergamino.width() * escalaPergamino;
        qreal altoVisible  = pergamino.height() * escalaPergamino;

        qreal posX = (scene->width() - anchoVisible) / 2;
        qreal posY = scene->height() - altoVisible + 40;

        dialogoCaja->setPos(posX, posY);

        // Mostrar retrato
        mostrarRetrato("Mago");

        // Crear texto del diálogo
        textoParcialWizard = "";
        letraActualWizard = 0;

        dialogoTexto = scene->addText(textoParcialWizard);
        dialogoTexto->setDefaultTextColor(Qt::black);
        dialogoTexto->setFont(QFont("Arial", 7, QFont::Normal));
        dialogoTexto->setTextWidth(anchoVisible - 50);
        dialogoTexto->setZValue(11);
        dialogoTexto->setPos(posX + 30, posY + 100);

        // Texto de ayuda
        dialogoAyuda = scene->addText("[Espacio] → Siguiente");
        dialogoAyuda->setDefaultTextColor(Qt::black);
        dialogoAyuda->setFont(QFont("Arial", 6, QFont::Normal));
        dialogoAyuda->setZValue(11);
        dialogoAyuda->setPos(posX + anchoVisible - 115, posY + altoVisible - 150);

        // Iniciar timer
        timerTextoWizard = new QTimer(this);
        connect(timerTextoWizard, &QTimer::timeout, this, &GameWindow::updateLetraWizard);
        timerTextoWizard->start(30);
    }
}




void GameWindow::updateLetraWizard()
{
    if (fraseActualWizard >= dialogoWizard.size())
    {
        timerTextoWizard->stop();

        if (retratoDialogo) {
            scene->removeItem(retratoDialogo);
            delete retratoDialogo;
            retratoDialogo = nullptr;
        }

        return;
    }

    const QString &frase = dialogoWizard[fraseActualWizard];

    if (letraActualWizard == 0)
    {
        if (frase.startsWith("Mago:")) {
            mostrarRetrato("Mago");
        } else if (frase.startsWith("Caballero:")) {
            mostrarRetrato("Caballero");
        }
    }

    if (letraActualWizard < frase.length())
    {
        textoParcialWizard += frase[letraActualWizard];
        dialogoTexto->setPlainText(textoParcialWizard);
        letraActualWizard++;
    }
    else
    {
        timerTextoWizard->stop();
    }
}

// -----------------------------------------------------------------------------------------Slime 1 y 2 ---------------------------------------------------------------------

void GameWindow::checkSlimeInteraction()
{
    if (!player || !slime) return;

    qreal distance = QLineF(player->scenePos(), slime->scenePos()).length();

    if (distance < 20.0)
    {
        qDebug() << "¡Knight ha chocado con la babosa!";

        if (slimeTimer && slimeTimer->isActive()) {
            slimeTimer->stop();
        }

        if (moveTimer && moveTimer->isActive()) {
            moveTimer->stop();
        }

        //this->setAttribute(Qt::WA_QuitOnClose, false);
        //this->hide();
        //QMessageBox::information(this, "¡Enemigo!", "Oh no, te has topado con una babosa enemiga!");
        BattleWidget* batalla = new BattleWidget("BosqueJS", "slime",aliados);
        batalla->show();
        //this->hide();


        if (labelSlime) {
            scene->removeItem(labelSlime);
            delete labelSlime;
            labelSlime = nullptr;
        }

        if (slime) {
            scene->removeItem(slime);
            delete slime;
            slime = nullptr;
        }

        currentDirection = None;

        if (!idleSheet.isNull()) {
            idleFrameIndex = 0;
            idleTimer->start(250);
        }
        qDebug() << "Babosa eliminada. Knight en Idle.";
    }
}

void GameWindow::checkSlime2Interaction()
{
    if (!player || !slime2) return;

    qreal distance = QLineF(player->scenePos(), slime2->scenePos()).length();

    if (distance < 20.0)
    {
        qDebug() << "¡Knight ha chocado con la babosa!";

        if (slime2Timer && slime2Timer->isActive()) {
            slime2Timer->stop();
        }

        if (moveTimer && moveTimer->isActive()) {
            moveTimer->stop();
        }

        //this->setAttribute(Qt::WA_QuitOnClose, false);
        //this->hide();
        //QMessageBox::information(this, "¡Enemigo!", "Oh no, te has topado con una babosa enemiga!");
        BattleWidget* batalla = new BattleWidget("BosqueJS", "slime",aliados);
        batalla->show();
        //this->hide();


        if (labelSlime2) {
            scene->removeItem(labelSlime2);
            delete labelSlime2;
            labelSlime2 = nullptr;
        }

        if (slime2) {
            scene->removeItem(slime2);
            delete slime2;
            slime2 = nullptr;
        }

        currentDirection = None;

        if (!idleSheet.isNull()) {
            idleFrameIndex = 0;
            idleTimer->start(250);
        }
        qDebug() << "Babosa eliminada. Knight en Idle.";
    }
}

void GameWindow::updateSlime()
{
    if (!slime) return;

    // Intentar mover en X
    if (!colisionaConBloqueadores(slime, slimeDx, 0)) {
        slime->moveBy(slimeDx, 0);
    } else {
        // Si colisiona  cambiar dirección X
        slimeDx = -slimeDx;
    }

    // Intentar mover en Y
    if (!colisionaConBloqueadores(slime, 0, slimeDy)) {
        slime->moveBy(0, slimeDy);
    } else {
        // Si colisiona  cambiar dirección Y
        slimeDy = -slimeDy;
    }

    int frameWidth = 100;
    int frameHeight = 100;
    int walkFrames = 6;

    slime->setPixmap(slimeWalkSheet.copy(slimeFrameIndex * frameWidth, 0, frameWidth, frameHeight));

    slimeFrameIndex = (slimeFrameIndex + 1) % walkFrames;
}

void GameWindow::updateSlime2()
{
    if (!slime2) return;

    // Movimiento en X
    if (!colisionaConBloqueadores(slime2, slime2Dx, 0)) {
        slime2->moveBy(slime2Dx, 0);
    } else {
        slime2Dx = -slime2Dx;
    }

    // Movimiento en Y
    if (!colisionaConBloqueadores(slime2, 0, slime2Dy)) {
        slime2->moveBy(0, slime2Dy);
    } else {
        slime2Dy = -slime2Dy;
    }

    int frameWidth = 100;
    int frameHeight = 100;
    int walkFrames = 6;

    slime2->setPixmap(slimeWalkSheet.copy(slime2FrameIndex * frameWidth, 0, frameWidth, frameHeight));
    slime2FrameIndex = (slime2FrameIndex + 1) % walkFrames;
}



//--------------------------------------------------------------------------------------------- SEGUNDO MAPA -----------------------------------------------------------------------------------------------------


void GameWindow::cambiarAMundoNuevo()
{


    if (labelMago) {
        scene->removeItem(labelMago);
        delete labelMago;
        labelMago = nullptr;
    }

    if (labelSlime) {
        scene->removeItem(labelSlime);
        delete labelSlime;
        labelSlime = nullptr;
    }

    if (slimeTimer && slimeTimer->isActive()) {
        slimeTimer->stop();
    }

    if (slime) {
        scene->removeItem(slime);
        delete slime;
        slime = nullptr;
    }

    // Eliminar segundo Slime
    if (slime2Timer && slime2Timer->isActive()) {
        slime2Timer->stop();
        delete slime2Timer;
        slime2Timer = nullptr;
    }

    if (slime2) {
        scene->removeItem(slime2);
        delete slime2;
        slime2 = nullptr;
    }

    if (labelSlime2) {
        scene->removeItem(labelSlime2);
        delete labelSlime2;
        labelSlime2 = nullptr;
    }


    if (axemanTextoTimer && axemanTextoTimer->isActive()) {
        axemanTextoTimer->stop();
        delete axemanTextoTimer;
        axemanTextoTimer = nullptr;
    }

    if (axeman) {
        scene->removeItem(axeman);
        delete axeman;
        axeman = nullptr;
    }

    if (globoOvalo) {
        scene->removeItem(globoOvalo);
        delete globoOvalo;
        globoOvalo = nullptr;
    }

    if (labelAxeman) {
        scene->removeItem(labelAxeman);
        delete labelAxeman;
        labelAxeman = nullptr;
    }


    scene->clear();
    bloqueadores.clear();

    QPixmap nuevoMapa("OpenSprite/a.png");

    if (nuevoMapa.isNull()) {
        qDebug("Error: No se pudo cargar el nuevo mapa.");
        return;
    }


    QGraphicsPixmapItem *mapItem = scene->addPixmap(nuevoMapa);
    mapItem->setZValue(-1);
    scene->setSceneRect(0, 0, nuevoMapa.width(), nuevoMapa.height());
    view->setFixedSize(700, 700);
    this->setFixedSize(700, 700);


    view->resetTransform();
    view->scale(1.2, 1.2);

    // Crear Knight
    if (!idleSheet.isNull()) {
        QPixmap frame = idleSheet.copy(0, 0, 100, 100);
        player = scene->addPixmap(frame);
        player->setZValue(1);
        player->setPos(600, 1);
        view->centerOn(player);


        // ESCALAR Knight:
        player->setScale(1.6);
    }
    // mapa 4
    zonaTransicionAMapa4 = scene->addRect(1, 475, 9, 165);
    zonaTransicionAMapa4->setBrush(Qt::red);
    zonaTransicionAMapa4->setPen(Qt::NoPen);
    bloqueadores.append(zonaTransicionAMapa4);

    zonaTransicionAMapa7 = scene->addRect(653, 932, 32, 4, QPen(Qt::red));
    zonaTransicionAMapa7->setBrush(Qt::transparent);


    // Crear Wizard
    if (!wizardIdleSheet.isNull()) {
        QPixmap frame = wizardIdleSheet.copy(0, 0, 100, 100);
        wizard = scene->addPixmap(frame);
        wizard->setZValue(1);
        wizard->setPos(552, 1);

        wizard->setScale(1.6);

        bloqueadores.append(wizard);

    }



    // ------------------------------------------------------------ Armored Axeman (NPC) --------------------------------------------------------------------------------------------------

    axemanIdleSheet = QPixmap("OpenSprite/Armored Axeman-Idle.png");

    if (axemanIdleSheet.isNull()) {
        qDebug("Error: No se pudo cargar Armored Axeman-Idle.");
    } else {
        QPixmap frame = axemanIdleSheet.copy(0, 0, 100, 100);
        axeman = scene->addPixmap(frame);

        axeman->setPos(360, 680);
        axeman->setZValue(1);
        axeman->setScale(1.6);
        bloqueadores.append(axeman);

        axemanIdleFrameIndex = 0;
        axemanIdleTimer = new QTimer(this);
        connect(axemanIdleTimer, &QTimer::timeout, this, &GameWindow::updateAxemanIdle);
        axemanIdleTimer->start(250);
    }



    if (!dialogoAxemanYaMostrado) {
        // Crear globo ovalado
        globoOvalo = scene->addEllipse(0, 0, 110, 55, QPen(Qt::black), QBrush(Qt::white));
        globoOvalo->setZValue(2);
        globoOvalo->setPos(axeman->x() - 5, axeman->y() - 10);

        // Crear texto del globo
        labelAxeman = scene->addText("¡Necesito ayuda!");
        labelAxeman->setDefaultTextColor(Qt::black);
        labelAxeman->setFont(QFont("Arial", 8, QFont::Bold));
        labelAxeman->setZValue(3);
        labelAxeman->setPos(axeman->x() + 5, axeman->y() - 1);



        // Frases que dice
        frasesAxeman << "¡Necesito ayuda!" << "¡Por favor!" << "¡Ayudaaa!";

        // Timer para cambiar frases
        axemanTextoTimer = new QTimer(this);
        connect(axemanTextoTimer, &QTimer::timeout, this, [=]() {
            if (labelAxeman) {
                labelAxeman->setPlainText(frasesAxeman[fraseActualAxeman]);
                fraseActualAxeman = (fraseActualAxeman + 1) % frasesAxeman.size();
            }
        });
        axemanTextoTimer->start(2500);

    }
    // ---------------------- ORC Enemigo (solo en segundo mapa) ----------------------

    orcWalkSheet = QPixmap("OpenSprite/Orc-Walk.png");

    if (orcWalkSheet.isNull()) {
        qDebug("Error: No se pudo cargar Orc-Walk.");
    } else {
        QPixmap frame = orcWalkSheet.copy(0, 0, 100, 100);
        orc = scene->addPixmap(frame);

        orc->setPos(537, 352);  // Posición inicial
        orc->setZValue(1);
        orc->setScale(1.6);

        orcFrameIndex = 0;
        orcDx = -1.2;
        orcDy = 1.0;

        orcTimer = new QTimer(this);
        connect(orcTimer, &QTimer::timeout, this, &GameWindow::updateOrc);
        orcTimer->start(270);
    }

    // ---------------------- Segundo ORC Enemigo ----------------------

    if (!orcWalkSheet.isNull()) {
        QPixmap frame2 = orcWalkSheet.copy(0, 0, 100, 100);
        orc2 = scene->addPixmap(frame2);

        orc2->setPos(610, 950);  // ← Tu nueva posición personalizada
        orc2->setZValue(1);
        orc2->setScale(1.6);

        orc2FrameIndex = 0;
        orc2Dx = 1.5;
        orc2Dy = -1.2;

        orc2Timer = new QTimer(this);
        connect(orc2Timer, &QTimer::timeout, this, &GameWindow::updateOrc2);
        orc2Timer->start(270);
    }

    // Cargar sprite del NPC
    QPixmap spriteNPC("OpenSprite/Soldier-Idle.png");
    QPixmap frameNPC = spriteNPC.copy(0, 0, 100, 100); // ajusta según tamaño del sprite
    npcTienda = scene->addPixmap(frameNPC);
    npcTienda->setPos(500, 600);
    npcTienda->setScale(2.0); // opcional

    // Texto flotante del NPC
    mensajeTienda = scene->addText("");
    mensajeTienda->setDefaultTextColor(Qt::white);
    mensajeTienda->setFont(QFont("Arial", 10));
    mensajeTienda->setPos(520, 650); // justo encima del NPC
    mensajeTienda->setZValue(10);


    // colisiones exterior
    QGraphicsRectItem *bloqueNuevo1 = scene->addRect(567, 16, 213, 10);
    bloqueNuevo1->setBrush(Qt::transparent);
    bloqueNuevo1->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo1);

    QGraphicsRectItem *bloqueNuevo2 = scene->addRect(748, 34, 49, 32);
    bloqueNuevo2->setBrush(Qt::transparent);
    bloqueNuevo2->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo2);

    QGraphicsRectItem *bloqueNuevo3 = scene->addRect(783, 74, 139, 149);
    bloqueNuevo3->setBrush(Qt::transparent);
    bloqueNuevo3->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo3);

    QGraphicsRectItem *bloqueNuevo4 = scene->addRect(871, 210, 45, 52);
    bloqueNuevo4->setBrush(Qt::transparent);
    bloqueNuevo4->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo4);

    QGraphicsRectItem *bloqueNuevo5 = scene->addRect(909, 233, 45, 91);
    bloqueNuevo5->setBrush(Qt::transparent);
    bloqueNuevo5->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo5);

    QGraphicsRectItem *bloqueNuevo6 = scene->addRect(939, 295, 9, 212);
    bloqueNuevo6->setBrush(Qt::transparent);
    bloqueNuevo6->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo6);

    QGraphicsRectItem *bloqueNuevo7 = scene->addRect(937, 323, 262, 154);
    bloqueNuevo7->setBrush(Qt::transparent);
    bloqueNuevo7->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo7);
    /*
    QGraphicsRectItem *bloqueNuevo8 = scene->addRect(1075, 453, 38, 356);
    bloqueNuevo8->setBrush(Qt::transparent);
    bloqueNuevo8->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo8);
*/
    QGraphicsRectItem *bloqueNu = scene->addRect(938, 730, 261, 21);
    bloqueNu->setBrush(Qt::transparent);
    bloqueNu->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNu);

    QGraphicsRectItem *bloqueNuevo9 = scene->addRect(870, 729, 201, 193);
    bloqueNuevo9->setBrush(Qt::transparent);
    bloqueNuevo9->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo9);

    QGraphicsRectItem *bloqueNuevo10 = scene->addRect(1003, 913, 29, 192);
    bloqueNuevo10->setBrush(Qt::transparent);
    bloqueNuevo10->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo10);

    QGraphicsRectItem *bloqueNuevo11 = scene->addRect(958, 969, 30, 51);
    bloqueNuevo11->setBrush(Qt::transparent);
    bloqueNuevo11->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo11);

    QGraphicsRectItem *bloqueNuevo12 = scene->addRect(882, 1014, 123, 110);
    bloqueNuevo12->setBrush(Qt::transparent);
    bloqueNuevo12->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo12);

    QGraphicsRectItem *bloqueNuevo13 = scene->addRect(593, 1071, 300, 41);
    bloqueNuevo13->setBrush(Qt::transparent);
    bloqueNuevo13->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo13);

    QGraphicsRectItem *bloqueNuevo14 = scene->addRect(812, 1040, 13, 24);
    bloqueNuevo14->setBrush(Qt::transparent);
    bloqueNuevo14->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo14);

    QGraphicsRectItem *bloqueNuevo15 = scene->addRect(717, 1031, 13, 32);
    bloqueNuevo15->setBrush(Qt::transparent);
    bloqueNuevo15->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo15);

    QGraphicsRectItem *bloqueNuevo16 = scene->addRect(161, 1013, 443, 128);
    bloqueNuevo16->setBrush(Qt::transparent);
    bloqueNuevo16->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo16);

    QGraphicsRectItem *bloqueNuevo17 = scene->addRect(162, 806, 55, 296);
    bloqueNuevo17->setBrush(Qt::transparent);
    bloqueNuevo17->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo17);

    QGraphicsRectItem *bloqueNuevo18 = scene->addRect(212, 972, 43, 42);
    bloqueNuevo18->setBrush(Qt::transparent);
    bloqueNuevo18->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo18);

    QGraphicsRectItem *bloqueNuevo19 = scene->addRect(2, 646, 268, 15);
    bloqueNuevo19->setBrush(Qt::transparent);
    bloqueNuevo19->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo19);
    /*
    QGraphicsRectItem *bloqueNuevo20 = scene->addRect(136, 416, 25, 323);
    bloqueNuevo20->setBrush(Qt::transparent);
    bloqueNuevo20->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo20);
*/
    QGraphicsRectItem *bloqueNuevo21 = scene->addRect(3, 444, 268, 15);
    bloqueNuevo21->setBrush(Qt::transparent);
    bloqueNuevo21->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo21);

    QGraphicsRectItem *bloqueNuevo22 = scene->addRect(289, 232, 26, 162);
    bloqueNuevo22->setBrush(Qt::transparent);
    bloqueNuevo22->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo22);

    QGraphicsRectItem *bloqueNuevo23 = scene->addRect(319, 94, 90, 150);
    bloqueNuevo23->setBrush(Qt::transparent);
    bloqueNuevo23->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo23);


    QGraphicsRectItem *bloqueNuevo24 = scene->addRect(396, 42, 171, 82);
    bloqueNuevo24->setBrush(Qt::transparent);
    bloqueNuevo24->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo24);

    QGraphicsRectItem *bloqueNuevo25 = scene->addRect(554, 35, 53, 36);
    bloqueNuevo25->setBrush(Qt::transparent);
    bloqueNuevo25->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo25);

    //colisiones internas
    QGraphicsRectItem *bloqueNuevo26 = scene->addRect(707, 146, 64, 49);
    bloqueNuevo26->setBrush(Qt::transparent);
    bloqueNuevo26->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo26);

    QGraphicsRectItem *bloqueNuevo27 = scene->addRect(595, 152, 36, 50);
    bloqueNuevo27->setBrush(Qt::transparent);
    bloqueNuevo27->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo27);

    QGraphicsRectItem *bloqueNuevo28 = scene->addRect(578, 178, 17, 27);
    bloqueNuevo28->setBrush(Qt::transparent);
    bloqueNuevo28->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo28);

    QGraphicsRectItem *bloqueNuevo29 = scene->addRect(429, 128, 17, 27);
    bloqueNuevo29->setBrush(Qt::transparent);
    bloqueNuevo29->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo29);

    QGraphicsRectItem *bloqueNuevo30 = scene->addRect(787, 220, 17, 27);
    bloqueNuevo30->setBrush(Qt::transparent);
    bloqueNuevo30->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo30);

    QGraphicsRectItem *bloqueNuevo31 = scene->addRect(801, 335, 122, 1);
    bloqueNuevo31->setBrush(Qt::transparent);
    bloqueNuevo31->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo31);

    QGraphicsRectItem *bloqueNuevo32 = scene->addRect(804, 335, 1, 164);
    bloqueNuevo32->setBrush(Qt::transparent);
    bloqueNuevo32->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo32);

    QGraphicsRectItem *bloqueNuevo33 = scene->addRect(818, 497, 40, 82);
    bloqueNuevo33->setBrush(Qt::transparent);
    bloqueNuevo33->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo33);

    QGraphicsRectItem *bloqueNuevo34 = scene->addRect(806, 454, 25, 51);
    bloqueNuevo34->setBrush(Qt::transparent);
    bloqueNuevo34->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo34);

    QGraphicsRectItem *bloqueNuevo35 = scene->addRect(802, 600, 1, 62);
    bloqueNuevo35->setBrush(Qt::transparent);
    bloqueNuevo35->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo35);

    QGraphicsRectItem *bloqueNuevo36 = scene->addRect(874, 657, 27, 62);
    bloqueNuevo36->setBrush(Qt::transparent);
    bloqueNuevo36->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo36);

    QGraphicsRectItem *bloqueNuevo37 = scene->addRect(922, 573, 1, 146);
    bloqueNuevo37->setBrush(Qt::transparent);
    bloqueNuevo37->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo37);

    QGraphicsRectItem *bloqueNuevo38 = scene->addRect(921, 482, 1, 42);
    bloqueNuevo38->setBrush(Qt::transparent);
    bloqueNuevo38->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo38);

    QGraphicsRectItem *bloqueNuevo39 = scene->addRect(937, 615, 9, 42);
    bloqueNuevo39->setBrush(Qt::transparent);
    bloqueNuevo39->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo39);

    QGraphicsRectItem *bloqueNuevo40 = scene->addRect(803, 652, 1, 80);
    bloqueNuevo40->setBrush(Qt::transparent);
    bloqueNuevo40->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo40);

    QGraphicsRectItem *bloqueNuevo41 = scene->addRect(583, 816, 179, 63);
    bloqueNuevo41->setBrush(Qt::transparent);
    bloqueNuevo41->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo41);

    QGraphicsRectItem *bloqueNuevo42 = scene->addRect(699, 859, 56, 55);
    bloqueNuevo42->setBrush(Qt::transparent);
    bloqueNuevo42->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo42);

    QGraphicsRectItem *bloqueNuevo43 = scene->addRect(385, 883, 253, 35);
    bloqueNuevo43->setBrush(Qt::transparent);
    bloqueNuevo43->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo43);

    QGraphicsRectItem *bloqueNuevo44 = scene->addRect(384, 886, 1, 63);
    bloqueNuevo44->setBrush(Qt::transparent);
    bloqueNuevo44->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo44);

    QGraphicsRectItem *bloqueNuevo45 = scene->addRect(313, 884, 1, 63);
    bloqueNuevo45->setBrush(Qt::transparent);
    bloqueNuevo45->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo45);

    QGraphicsRectItem *bloqueNuevo46 = scene->addRect(418, 815, 207, 1);
    bloqueNuevo46->setBrush(Qt::transparent);
    bloqueNuevo46->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo46);

    QGraphicsRectItem *bloqueNuevo47 = scene->addRect(362, 751, 49, 41);
    bloqueNuevo47->setBrush(Qt::transparent);
    bloqueNuevo47->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo47);

    QGraphicsRectItem *bloqueNuevo48 = scene->addRect(351, 565, 1, 180);
    bloqueNuevo48->setBrush(Qt::transparent);
    bloqueNuevo48->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo48);

    QGraphicsRectItem *bloqueNuevo49 = scene->addRect(284, 546, 1, 180);
    bloqueNuevo49->setBrush(Qt::transparent);
    bloqueNuevo49->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo49);

    QGraphicsRectItem *bloqueNuevo50 = scene->addRect(355, 562, 111, 31);
    bloqueNuevo50->setBrush(Qt::transparent);
    bloqueNuevo50->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo50);
    /*
    QGraphicsRectItem *bloqueNuevo51 = scene->addRect(168, 615, 22, 37);
    bloqueNuevo51->setBrush(Qt::transparent);
    bloqueNuevo51->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo51);
*/
    QGraphicsRectItem *bloqueNuevo52 = scene->addRect(265, 433, 29, 48);
    bloqueNuevo52->setBrush(Qt::transparent);
    bloqueNuevo52->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo52);

    QGraphicsRectItem *bloqueNuevo53 = scene->addRect(475, 494, 1, 67);
    bloqueNuevo53->setBrush(Qt::transparent);
    bloqueNuevo53->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo53);

    QGraphicsRectItem *bloqueNuevo54 = scene->addRect(475, 498, 97, 41);
    bloqueNuevo54->setBrush(Qt::transparent);
    bloqueNuevo54->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo54);

    QGraphicsRectItem *bloqueNuevo55 = scene->addRect(557, 505, 18, 59);
    bloqueNuevo55->setBrush(Qt::transparent);
    bloqueNuevo55->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo55);

    QGraphicsRectItem *bloqueNuevo56 = scene->addRect(638, 496, 1, 65);
    bloqueNuevo56->setBrush(Qt::transparent);
    bloqueNuevo56->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo56);

    QGraphicsRectItem *bloqueNuevo57 = scene->addRect(638, 496, 91, 40);
    bloqueNuevo57->setBrush(Qt::transparent);
    bloqueNuevo57->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo57);

    QGraphicsRectItem *bloqueNuevo58 = scene->addRect(703, 539, 14, 23);
    bloqueNuevo58->setBrush(Qt::transparent);
    bloqueNuevo58->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo58);

    QGraphicsRectItem *bloqueNuevo59 = scene->addRect(730, 334, 1, 162);
    bloqueNuevo59->setBrush(Qt::transparent);
    bloqueNuevo59->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo59);

    QGraphicsRectItem *bloqueNuevo60 = scene->addRect(512, 334, 219, 1);
    bloqueNuevo60->setBrush(Qt::transparent);
    bloqueNuevo60->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo60);

    QGraphicsRectItem *bloqueNuevo61 = scene->addRect(329, 369, 52, 34);
    bloqueNuevo61->setBrush(Qt::transparent);
    bloqueNuevo61->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo61);

    QGraphicsRectItem *bloqueNuevo62 = scene->addRect(453, 369, 52, 34);
    bloqueNuevo62->setBrush(Qt::transparent);
    bloqueNuevo62->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo62);

    QGraphicsRectItem *bloqueNuevo63 = scene->addRect(450, 370, 1, 65);
    bloqueNuevo63->setBrush(Qt::transparent);
    bloqueNuevo63->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo63);

    QGraphicsRectItem *bloqueNuevo64 = scene->addRect(382, 370, 1, 65);
    bloqueNuevo64->setBrush(Qt::transparent);
    bloqueNuevo64->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo64);

    QGraphicsRectItem *bloqueNuevo65 = scene->addRect(324, 251, 31, 34);
    bloqueNuevo65->setBrush(Qt::transparent);
    bloqueNuevo65->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo65);

    QGraphicsRectItem *bloqueNuevo66 = scene->addRect(409, 265, 14, 40);
    bloqueNuevo66->setBrush(Qt::transparent);
    bloqueNuevo66->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo66);

    QGraphicsRectItem *bloqueNuevo67 = scene->addRect(507, 275, 1, 88);
    bloqueNuevo67->setBrush(Qt::transparent);
    bloqueNuevo67->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo67);

    QGraphicsRectItem *bloqueNuevo68 = scene->addRect(425, 270, 80, 1);
    bloqueNuevo68->setBrush(Qt::transparent);
    bloqueNuevo68->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo68);

    // para que no se salga del mapa 2
    QGraphicsRectItem *bloqueNuevo69 = scene->addRect(1198, 482,1, 248);
    bloqueNuevo69->setBrush(Qt::transparent);
    bloqueNuevo69->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo69);

    QGraphicsRectItem *bloqueNuevo70 = scene->addRect(654, 914, 33, 4);
    bloqueNuevo70->setBrush(Qt::transparent);
    bloqueNuevo70->setPen(Qt::NoPen);
    bloqueadores.append(bloqueNuevo70);


    currentDirection = None;
    moveTimer->stop();
    playerSpeed = 8;


    qDebug(" Nuevo mundo cargado.");
    if (!regresoDesdeMapaTres && !regresoDesdeMapaCuatro && !dialogoWizardYaMostrado) {

        dialogoSegundoMapa.clear();
        dialogoSegundoMapa
            << "Caballero: ¡ESPERA! ¿Cómo hiciste eso? ¿Dónde estamos? ¿Cómo lograste traerme aquí?"
            << "Brujo: Tranquilo, no olvides que soy mago... y los magos hacemos magia. Esta es una tierra sagrada,"
               "  y yo soy su protector. Pero al perderse el Éterium, esta tierra quedó herida y fragmentada."
            << "Caballero: Ya veo... El Éterium realmente es importante."
            << "Brujo: Lo es. Mantiene el equilibrio de nuestro mundo. Sin él, todos estamos en peligro."
            << "Caballero: ¿Y qué puedo hacer? No quiero que mi hogar acabe destruido."
            << "Brujo: Deberás enfrentarte a grandes desafíos. Pero no estarás solo. Harás aliados en el camino."
               " Tu misión es reunir todos los fragmentos del Éterium para restaurar la paz."
            << "Caballero: Pero... no podré hacerlo solo."
            << "Brujo: Ya te lo advertí antes: mis aliadas, la Curandera y la Princesa C#, estarán contigo."
            << "Caballero: Ah... cierto, lo había olvidado. Entonces dime, ¿por dónde empiezo?"
            << "Brujo: Tu primera misión es ayudar a el Axeman. Los ogros del jefe secuestraron a su hermano."
               " Él está cerca de aquí. Debes encontrarlo y ofrecerle tu ayuda."
            << "Caballero: ¿Qué pasó con su hermano?"
            << "Brujo: Creemos que logró robar un fragmento del Éterium, pero los ogros lo capturaron."
            << "Caballero: Entiendo. Entonces ayudaré a el Axeman... y juntos, salvaremos este mundo.";

        fraseActualSegundoMapa = 0;
        dialogoActivo = true;

        // Eliminar diálogo anterior si existía
        if (dialogoCaja) { scene->removeItem(dialogoCaja); delete dialogoCaja; dialogoCaja = nullptr; }
        if (dialogoTexto) { scene->removeItem(dialogoTexto); delete dialogoTexto; dialogoTexto = nullptr; }
        if (dialogoAyuda) { scene->removeItem(dialogoAyuda); delete dialogoAyuda; dialogoAyuda = nullptr; }
        if (retratoDialogo) { scene->removeItem(retratoDialogo); delete retratoDialogo; retratoDialogo = nullptr; }

        // Cargar imagen de pergamino
        QPixmap pergamino("OpenSprite/pergamino.PNG");
        dialogoCaja = scene->addPixmap(pergamino);
        dialogoCaja->setZValue(10);

        // Escalar y posicionar
        qreal anchoVisible = pergamino.width() * 1.2;
        qreal altoVisible  = pergamino.height() * 0.4;

        // Escala horizontal mayor, altura igual
        dialogoCaja->setTransform(QTransform::fromScale(1.2, 0.4));

        qreal escalaPergamino = 0.80;
        dialogoCaja->setScale(escalaPergamino);
        qreal posX = (scene->width() - anchoVisible) / 2+130;
        qreal posY = scene->height() - altoVisible - 550;
        dialogoCaja->setPos(posX, posY);

        // Mostrar retrato (empieza hablando el Caballero)
        mostrarRetrato("Caballero");

        // Texto de diálogo
        textoParcialSegundoMapa = "";
        letraActualSegundoMapa = 0;

        dialogoTexto = scene->addText("");
        dialogoTexto->setDefaultTextColor(Qt::black);
        dialogoTexto->setFont(QFont("Arial", 9, QFont::Normal));
        dialogoTexto->setTextWidth(anchoVisible - 400);
        dialogoTexto->setZValue(11);
        dialogoTexto->setPos(posX + 160, posY + 65);

        // Texto de ayuda
        dialogoAyuda = scene->addText("[Espacio] → Siguiente");
        dialogoAyuda->setDefaultTextColor(Qt::black);
        dialogoAyuda->setFont(QFont("Arial", 9));
        dialogoAyuda->setZValue(11);
        dialogoAyuda->setPos(posX + anchoVisible - 350, posY + altoVisible - 125);

        // Timer del diálogo
        if (!timerTextoSegundoMapa) {
            timerTextoSegundoMapa = new QTimer(this);
            connect(timerTextoSegundoMapa, &QTimer::timeout, this, &GameWindow::updateLetraSegundoMapa);
        }
        timerTextoSegundoMapa->start(40);

    }
    regresoDesdeMapaTres = false;



}




void GameWindow::cambiarAMundoTres() {

    if (npcTienda) {
        scene->removeItem(npcTienda);
        delete npcTienda;
        npcTienda = nullptr;
    }
    if (mensajeTienda) {
        scene->removeItem(mensajeTienda);
        delete mensajeTienda;
        mensajeTienda = nullptr;
    }
    // Eliminar elementos del mapa anterior (segundo mapa)

    // Eliminar orc1
    if (orcTimer && orcTimer->isActive()) {
        orcTimer->stop();
        delete orcTimer;
        orcTimer = nullptr;
    }
    if (orc) {
        scene->removeItem(orc);
        delete orc;
        orc = nullptr;
    }

    // Eliminar orc2
    if (orc2Timer && orc2Timer->isActive()) {
        orc2Timer->stop();
        delete orc2Timer;
        orc2Timer = nullptr;
    }
    if (orc2) {
        scene->removeItem(orc2);
        delete orc2;
        orc2 = nullptr;
    }

    // Eliminar wizard
    if (wizard) {
        scene->removeItem(wizard);
        delete wizard;
        wizard = nullptr;
    }

    // Eliminar axeman
    if (axeman) {
        scene->removeItem(axeman);
        delete axeman;
        axeman = nullptr;
    }
    if (labelAxeman) {
        scene->removeItem(labelAxeman);
        delete labelAxeman;
        labelAxeman = nullptr;
    }
    if (globoOvalo) {
        scene->removeItem(globoOvalo);
        delete globoOvalo;
        globoOvalo = nullptr;
    }
    if (axemanTextoTimer && axemanTextoTimer->isActive()) {
        axemanTextoTimer->stop();
        delete axemanTextoTimer;
        axemanTextoTimer = nullptr;
    }







    scene->clear();
    bloqueadores.clear();

    QPixmap nuevoMapa("OpenSprite/map_3.png");
    if (nuevoMapa.isNull()) {
        qDebug("Error: No se pudo cargar map_3.png");
        return;
    }

    // Agregar nuevo fondo
    QGraphicsPixmapItem *mapItem = scene->addPixmap(nuevoMapa);
    mapItem->setZValue(-1);
    scene->setSceneRect(0, 0, nuevoMapa.width(), nuevoMapa.height());

    // Ajustar tamaño de ventana
    this->setFixedSize(700, 700);
    view->setFixedSize(700, 700);
    view->resetTransform();
    view->scale(0.8, 0.8);

    // Colocar al caballero
    if (!idleSheet.isNull()) {
        QPixmap frame = idleSheet.copy(0, 0, 350, 350);
        player = scene->addPixmap(frame);
        player->setZValue(1);
        player->setPos(25, 440);
        player->setScale(2.5);
        view->centerOn(player);
    }

    // Zona de regreso al mapa 2
    QGraphicsRectItem *zonaRegreso = scene->addRect(8, 450, 30, 230);
    zonaRegreso->setBrush(Qt::red);
    zonaRegreso->setPen(Qt::NoPen);
    bloqueadores.append(zonaRegreso);

    // Guardar el rectángulo en una variable si deseas referencia directa
    zonaTransicionMapaTres = zonaRegreso;

    QGraphicsRectItem* Mapa3colison1 = scene->addRect(3, 248, 21, 162);  // x, y, ancho, alto
    Mapa3colison1->setBrush(Qt::transparent);
    Mapa3colison1->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison1);

    QGraphicsRectItem* Mapa3colison2 = scene->addRect(35, 188, 21, 154);
    Mapa3colison2->setBrush(Qt::transparent);
    Mapa3colison2->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison2);

    QGraphicsRectItem* Mapa3colison3 = scene->addRect(36, 57, 75, 154);
    Mapa3colison3->setBrush(Qt::transparent);
    Mapa3colison3->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison3);

    QGraphicsRectItem* Mapa3colison4 = scene->addRect(84, 50, 177, 108);
    Mapa3colison4->setBrush(Qt::transparent);
    Mapa3colison4->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison4);

    QGraphicsRectItem* Mapa3colison5 = scene->addRect(390, 46, 139, 108);
    Mapa3colison5->setBrush(Qt::transparent);
    Mapa3colison5->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison5);

    QGraphicsRectItem* Mapa3colison6 = scene->addRect(514, 75, 599, 17);  // x, y, ancho, alto
    Mapa3colison6->setBrush(Qt::transparent);
    Mapa3colison6->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison6);


    QGraphicsRectItem* Mapa3colison7 = scene->addRect(1109, 38, 195, 107);
    Mapa3colison7->setBrush(Qt::transparent);
    Mapa3colison7->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison7);

    QGraphicsRectItem* Mapa3colison8 = scene->addRect(1309, 110, 137, 107);
    Mapa3colison8->setBrush(Qt::transparent);
    Mapa3colison8->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison8);

    QGraphicsRectItem* Mapa3colison9 = scene->addRect(1569, 101, 137, 107);
    Mapa3colison9->setBrush(Qt::transparent);
    Mapa3colison9->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison9);

    QGraphicsRectItem* Mapa3colison10 = scene->addRect(1713, 3, 193, 148);
    Mapa3colison10->setBrush(Qt::transparent);
    Mapa3colison10->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison10);

    QGraphicsRectItem* Mapa3colison11 = scene->addRect(1786, 1, 307, 21);
    Mapa3colison11->setBrush(Qt::transparent);
    Mapa3colison11->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison11);

    QGraphicsRectItem* Mapa3colison12 = scene->addRect(2020, 6, 73, 198);
    Mapa3colison12->setBrush(Qt::transparent);
    Mapa3colison12->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison12);

    QGraphicsRectItem* Mapa3colison13 = scene->addRect(2096, 152, 1, 1010);
    Mapa3colison13->setBrush(Qt::transparent);
    Mapa3colison13->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison13);

    QGraphicsRectItem* Mapa3colison14 = scene->addRect(4, 1165, 2092, 9);  // x, y, ancho, alto
    Mapa3colison14->setBrush(Qt::transparent);
    Mapa3colison14->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison14);

    QGraphicsRectItem* Mapa3colison15 = scene->addRect(4, 700, 5, 474);
    Mapa3colison15->setBrush(Qt::transparent);
    Mapa3colison15->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison15);

    QGraphicsRectItem* Mapa3colison16 = scene->addRect(1586, 510,22, 66); // x, y, ancho, alto
    Mapa3colison16->setBrush(Qt::transparent);
    Mapa3colison16->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison16);

    QGraphicsRectItem* Mapa3colison17 = scene->addRect(1590, 544,212, 26);
    Mapa3colison17->setBrush(Qt::transparent);
    Mapa3colison17->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison17);

    QGraphicsRectItem* Mapa3colison18 = scene->addRect(1788, 544,26, 84);
    Mapa3colison18->setBrush(Qt::transparent);
    Mapa3colison18->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison18);

    QGraphicsRectItem* Mapa3colison19 = scene->addRect(1800, 612,210, 24);
    Mapa3colison19->setBrush(Qt::transparent);
    Mapa3colison19->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison19);

    QGraphicsRectItem* Mapa3colison20 = scene->addRect(1990, 612,14, 362);
    Mapa3colison20->setBrush(Qt::transparent);
    Mapa3colison20->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison20);

    QGraphicsRectItem* Mapa3colison21 = scene->addRect(1956, 958,50, 32);
    Mapa3colison21->setBrush(Qt::transparent);
    Mapa3colison21->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison21);

    QGraphicsRectItem* Mapa3colison22 = scene->addRect(1691, 480,110, 8);
    Mapa3colison22->setBrush(Qt::transparent);
    Mapa3colison22->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison22);

    QGraphicsRectItem* Mapa3colison23 = scene->addRect(5, 687,195, 8); // x, y, ancho, alto
    Mapa3colison23->setBrush(Qt::transparent);
    Mapa3colison23->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison23);

    QGraphicsRectItem* Mapa3colison24 = scene->addRect(221, 613,3, 77);
    Mapa3colison24->setBrush(Qt::transparent);
    Mapa3colison24->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison24);

    QGraphicsRectItem* Mapa3colison25 = scene->addRect(221, 613,203, 4);
    Mapa3colison25->setBrush(Qt::transparent);
    Mapa3colison25->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison25);

    QGraphicsRectItem* Mapa3colison26 = scene->addRect(424, 624,2, 195);
    Mapa3colison26->setBrush(Qt::transparent);
    Mapa3colison26->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison26);

    QGraphicsRectItem* Mapa3colison27 = scene->addRect(438, 683,371, 4);
    Mapa3colison27->setBrush(Qt::transparent);
    Mapa3colison27->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison27);

    QGraphicsRectItem* Mapa3colison28 = scene->addRect(554, 686,2, 76); // x, y, ancho, alto
    Mapa3colison28->setBrush(Qt::transparent);
    Mapa3colison28->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison28);

    QGraphicsRectItem* Mapa3colison29 = scene->addRect(688, 685,2, 136);
    Mapa3colison29->setBrush(Qt::transparent);
    Mapa3colison29->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison29);

    QGraphicsRectItem* Mapa3colison30 = scene->addRect(816, 682,2, 70);
    Mapa3colison30->setBrush(Qt::transparent);
    Mapa3colison30->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison30);

    QGraphicsRectItem* Mapa3colison31 = scene->addRect(950, 754,2, 127);
    Mapa3colison31->setBrush(Qt::transparent);
    Mapa3colison31->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison31);

    QGraphicsRectItem* Mapa3colison32 = scene->addRect(819, 750,379, 2);
    Mapa3colison32->setBrush(Qt::transparent);
    Mapa3colison32->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison32);

    QGraphicsRectItem* Mapa3colison33 = scene->addRect(1078, 753,2, 131); // x, y, ancho, alto
    Mapa3colison33->setBrush(Qt::transparent);
    Mapa3colison33->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison33);

    QGraphicsRectItem* Mapa3colison34 = scene->addRect(1211, 749,2, 205);
    Mapa3colison34->setBrush(Qt::transparent);
    Mapa3colison34->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison34);

    QGraphicsRectItem* Mapa3colison35 = scene->addRect(1343, 821,2, 132);
    Mapa3colison35->setBrush(Qt::transparent);
    Mapa3colison35->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison35);

    QGraphicsRectItem* Mapa3colison36 = scene->addRect(1476, 824,2, 132);
    Mapa3colison36->setBrush(Qt::transparent);
    Mapa3colison36->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison36);

    QGraphicsRectItem* Mapa3colison37 = scene->addRect(1213, 816,387, 2);
    Mapa3colison37->setBrush(Qt::transparent);
    Mapa3colison37->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison37);

    QGraphicsRectItem* Mapa3colison38 = scene->addRect(1602, 816,2, 73);
    Mapa3colison38->setBrush(Qt::transparent);
    Mapa3colison38->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison38);

    QGraphicsRectItem* Mapa3colison39 = scene->addRect(1607, 885,130, 2);
    Mapa3colison39->setBrush(Qt::transparent);
    Mapa3colison39->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison39);

    QGraphicsRectItem* Mapa3colison40 = scene->addRect(771, 948,89, 8); // x, y, ancho, alto
    Mapa3colison40->setBrush(Qt::transparent);
    Mapa3colison40->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison40);

    QGraphicsRectItem* Mapa3colison41 = scene->addRect(451, 936,12, 82);
    Mapa3colison41->setBrush(Qt::transparent);
    Mapa3colison41->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison41);

    QGraphicsRectItem* Mapa3colison42 = scene->addRect(312, 686,27, 54);
    Mapa3colison42->setBrush(Qt::transparent);
    Mapa3colison42->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison42);

    QGraphicsRectItem* Mapa3colison43 = scene->addRect(504, 554,93, 7);
    Mapa3colison43->setBrush(Qt::transparent);
    Mapa3colison43->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison43);

    QGraphicsRectItem* Mapa3colison44 = scene->addRect(32, 354,187, 7); // x, y, ancho, alto
    Mapa3colison44->setBrush(Qt::transparent);
    Mapa3colison44->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison44);

    QGraphicsRectItem* Mapa3colison45 = scene->addRect(427, 355,187, 7); // x, y, ancho, alto
    Mapa3colison45->setBrush(Qt::transparent);
    Mapa3colison45->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison45);

    QGraphicsRectItem* Mapa3colison46 = scene->addRect(617, 358,2, 74); // x, y, ancho, alto
    Mapa3colison46->setBrush(Qt::transparent);
    Mapa3colison46->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison46);

    QGraphicsRectItem* Mapa3colison47 = scene->addRect(640, 426,361, 2); // x, y, ancho, alto
    Mapa3colison47->setBrush(Qt::transparent);
    Mapa3colison47->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison47);

    QGraphicsRectItem* Mapa3colison48 = scene->addRect(1008, 419,2, 78); // x, y, ancho, alto
    Mapa3colison48->setBrush(Qt::transparent);
    Mapa3colison48->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison48);

    QGraphicsRectItem* Mapa3colison49 = scene->addRect(1008, 493,400, 2); // x, y, ancho, alto
    Mapa3colison49->setBrush(Qt::transparent);
    Mapa3colison49->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison49);

    QGraphicsRectItem* Mapa3colison50 = scene->addRect(1407, 487,2, 75); // x, y, ancho, alto
    Mapa3colison50->setBrush(Qt::transparent);
    Mapa3colison50->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison50);

    QGraphicsRectItem* Mapa3colison51 = scene->addRect(1103, 352,19, 63); // x, y, ancho, alto
    Mapa3colison51->setBrush(Qt::transparent);
    Mapa3colison51->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison51);

    QGraphicsRectItem* Mapa3colison52 = scene->addRect(268, 85,120, 18); // x, y, ancho, alto
    Mapa3colison52->setBrush(Qt::transparent);
    Mapa3colison52->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison52);

    QGraphicsRectItem* Mapa3colison53 = scene->addRect(1450, 147,117, 12); // x, y, ancho, alto
    Mapa3colison53->setBrush(Qt::transparent);
    Mapa3colison53->setPen(Qt::NoPen);
    bloqueadores.append(Mapa3colison53);

    playerSpeed = 15;

    qDebug("Mapa 3 cargado correctamente.");

}




// --------------------------------------------------- Enemigos segundo mapa ----------------------------------------------
void GameWindow::updateOrc()
{
    if (!orc) return;

    // Movimiento X
    if (!colisionaConBloqueadores(orc, orcDx, 0)) {
        orc->moveBy(orcDx, 0);
    } else {
        orcDx = -orcDx;
    }

    // Movimiento Y
    if (!colisionaConBloqueadores(orc, 0, orcDy)) {
        orc->moveBy(0, orcDy);
    } else {
        orcDy = -orcDy;
    }

    int frameWidth = 100;
    int frameHeight = 100;
    int walkFrames = 8;

    orc->setPixmap(orcWalkSheet.copy(orcFrameIndex * frameWidth, 0, frameWidth, frameHeight));
    orcFrameIndex = (orcFrameIndex + 1) % walkFrames;
}

void GameWindow::checkOrcInteraction()
{
    if (!player || !orc) return;

    qreal distance = QLineF(player->scenePos(), orc->scenePos()).length();

    if (distance < 20.0)
    {
        qDebug() << "¡Knight ha chocado con el ORCO!";

        if (orcTimer && orcTimer->isActive()) {
            orcTimer->stop();
        }

        if (moveTimer && moveTimer->isActive()) {
            moveTimer->stop();
        }

        BattleWidget* batalla = new BattleWidget("BosqueJS", "ogros normales",aliados);
        batalla->show();

        if (orc) {
            scene->removeItem(orc);
            delete orc;
            orc = nullptr;
        }

        currentDirection = None;

        if (!idleSheet.isNull()) {
            idleFrameIndex = 0;
            idleTimer->start(250);
        }

        qDebug() << "Orco eliminado. Knight en Idle.";
    }
}

void GameWindow::updateOrc2()
{
    if (!orc2) return;

    if (!colisionaConBloqueadores(orc2, orc2Dx, 0)) {
        orc2->moveBy(orc2Dx, 0);
    } else {
        orc2Dx = -orc2Dx;
    }

    if (!colisionaConBloqueadores(orc2, 0, orc2Dy)) {
        orc2->moveBy(0, orc2Dy);
    } else {
        orc2Dy = -orc2Dy;
    }

    int frameWidth = 100;
    int frameHeight = 100;
    int walkFrames = 8;

    orc2->setPixmap(orcWalkSheet.copy(orc2FrameIndex * frameWidth, 0, frameWidth, frameHeight));
    orc2FrameIndex = (orc2FrameIndex + 1) % walkFrames;
}

void GameWindow::checkOrc2Interaction()
{
    if (!player || !orc2) return;

    qreal distance = QLineF(player->scenePos(), orc2->scenePos()).length();

    if (distance < 20.0)
    {
        qDebug() << "¡Knight ha chocado con el segundo ORCO!";

        if (orc2Timer && orc2Timer->isActive()) {
            orc2Timer->stop();
        }

        if (moveTimer && moveTimer->isActive()) {
            moveTimer->stop();
        }

        BattleWidget* batalla = new BattleWidget("BosqueJS", "ogros normales",aliados);
        batalla->show();

        if (orc2) {
            scene->removeItem(orc2);
            delete orc2;
            orc2 = nullptr;
        }

        currentDirection = None;

        if (!idleSheet.isNull()) {
            idleFrameIndex = 0;
            idleTimer->start(250);
        }

        qDebug() << "Segundo Orco eliminado. Knight en Idle.";
    }
}


// -----------------------------------------------------------Axeman (Steve latin)------------------------------------------------------------------------
void GameWindow::updateAxemanIdle()
{
    if (!axeman) return;

    int frameWidth = 100;
    int frameHeight = 100;
    int idleFrames = 6;

    axeman->setPixmap(axemanIdleSheet.copy(axemanIdleFrameIndex * frameWidth, 0, frameWidth, frameHeight));

    axemanIdleFrameIndex = (axemanIdleFrameIndex + 1) % idleFrames;
}

void GameWindow::checkAxemanInteraction() {
    if (!axeman || !player || dialogoActivo || dialogoAxemanYaMostrado)
        return;

    qreal distancia = QLineF(player->pos(), axeman->pos()).length();
    if (distancia < 45.0) {
        // Eliminar globo y texto si aún está visible
        if (globoOvalo) {
            scene->removeItem(globoOvalo);
            delete globoOvalo;
            globoOvalo = nullptr;
        }
        if (labelAxeman) {
            scene->removeItem(labelAxeman);
            delete labelAxeman;
            labelAxeman = nullptr;
        }

        // Iniciar diálogo
        dialogoActivo = true;
        iniciarDialogoAxeman();
        dialogoAxemanYaMostrado = true;
    }
}


void GameWindow::iniciarDialogoAxeman() {
    frasesAxeman.clear();
    frasesAxeman
        << "Caballero: ¿Qué te pasa?"
        << "Axeman: ¡Necesito ayuda! Mi hermano fue secuestrado por los ogros."
        << "Caballero: Oye, el brujo me comentó de eso. Me dijo que te tenía que ayudar."
        << "Axeman: ¿¡En serio!? ¡Por favor, ayúdame!"
        << "Caballero: Está bien, buscaré a tu hermano."
        << "Axeman: Por favor... Creo que lo tienen en el castillo del jefe, pero no estoy seguro."
        << "Caballero: No te preocupes, yo lo encontraré.";

    fraseActualAxeman = 0;
    textoParcialSegundoMapa = "";
    letraActualSegundoMapa = 0;
    dialogoAxemanActivo = true;

    // Quita globo
    if (labelAxeman) labelAxeman->hide();
    if (globoOvalo) globoOvalo->hide();

    // Borra diálogos anteriores
    if (dialogoCaja) scene->removeItem(dialogoCaja);
    if (dialogoTexto) scene->removeItem(dialogoTexto);
    if (dialogoAyuda) scene->removeItem(dialogoAyuda);
    if (retratoDialogo) scene->removeItem(retratoDialogo);

    // Crea pergamino
    QPixmap pergamino("OpenSprite/pergamino.PNG");
    dialogoCaja = scene->addPixmap(pergamino);
    dialogoCaja->setZValue(10);
    dialogoCaja->setTransform(QTransform::fromScale(0.8, 0.4));
    dialogoCaja->setPos(view->width() / 2 - 120, view->height() + 90);

    mostrarRetrato("Caballero");

    dialogoTexto = scene->addText("");
    dialogoTexto->setDefaultTextColor(Qt::black);
    dialogoTexto->setFont(QFont("Arial", 10));
    dialogoTexto->setTextWidth(230);
    dialogoTexto->setZValue(11);
    dialogoTexto->setPos(dialogoCaja->x() + 160, dialogoCaja->y() + 75);

    dialogoAyuda = scene->addText("[Espacio] → Siguiente");
    dialogoAyuda->setDefaultTextColor(Qt::black);
    dialogoAyuda->setFont(QFont("Arial", 9));
    dialogoAyuda->setZValue(11);
    dialogoAyuda->setPos(dialogoCaja->x() + 250, dialogoCaja->y() + 160);

    if (!timerTextoSegundoMapa) {
        timerTextoSegundoMapa = new QTimer(this);
        connect(timerTextoSegundoMapa, &QTimer::timeout, this, &GameWindow::updateLetraSegundoMapa);
    }
    timerTextoSegundoMapa->start(40);
}


void GameWindow::updateLetraSegundoMapa()
{
    QString frase;

    if (dialogoAxemanActivo) {
        if (fraseActualAxeman >= frasesAxeman.size()) {
            timerTextoSegundoMapa->stop();
            return;
        }

        frase = frasesAxeman[fraseActualAxeman];

        if (letraActualSegundoMapa == 0) {
            if (frase.startsWith("Caballero:")) {
                mostrarRetrato("Caballero");
            } else if (frase.startsWith("Axeman:")) {
                mostrarRetrato("Axeman");
            }
        }

        if (letraActualSegundoMapa < frase.length()) {
            textoParcialSegundoMapa += frase[letraActualSegundoMapa];
            dialogoTexto->setPlainText(textoParcialSegundoMapa);
            letraActualSegundoMapa++;
        } else {
            timerTextoSegundoMapa->stop();  // Espera a presionar espacio
        }

    } else {
        // Diálogo del segundo mapa (con el Brujo)
        if (fraseActualSegundoMapa >= dialogoSegundoMapa.size()) {
            timerTextoSegundoMapa->stop();
            return;
        }

        frase = dialogoSegundoMapa[fraseActualSegundoMapa];

        if (letraActualSegundoMapa == 0) {
            if (frase.startsWith("Caballero:")) {
                mostrarRetrato("Caballero");
            } else if (frase.startsWith("Brujo:")) {
                mostrarRetrato("Mago");
            }
        }

        if (letraActualSegundoMapa < frase.length()) {
            textoParcialSegundoMapa += frase[letraActualSegundoMapa];
            dialogoTexto->setPlainText(textoParcialSegundoMapa);
            letraActualSegundoMapa++;
        } else {
            timerTextoSegundoMapa->stop();
        }
    }
}










// ------------------------------------------------- botones para mover caballero ------------------------------------------------
void GameWindow::keyPressEvent(QKeyEvent *event)
{
    // Si estamos en diálogo, SOLO procesar Espacio:
    if (dialogoActivo)
    {
        if (event->key() == Qt::Key_Space)
        {
            // ---- Diálogo del Axeman ----
            if (dialogoAxemanActivo) {
                if (timerTextoSegundoMapa && timerTextoSegundoMapa->isActive()) {
                    timerTextoSegundoMapa->stop();
                    dialogoTexto->setPlainText(frasesAxeman[fraseActualAxeman]);
                } else {
                    fraseActualAxeman++;

                    if (fraseActualAxeman < frasesAxeman.size()) {
                        letraActualSegundoMapa = 0;
                        textoParcialSegundoMapa.clear();
                        dialogoTexto->setPlainText("");

                        timerTextoSegundoMapa->start(40);
                    } else {
                        // Fin del diálogo del Axeman
                        scene->removeItem(dialogoCaja);
                        scene->removeItem(dialogoTexto);
                        scene->removeItem(dialogoAyuda);

                        delete dialogoCaja;
                        delete dialogoTexto;
                        delete dialogoAyuda;

                        dialogoCaja = nullptr;
                        dialogoTexto = nullptr;
                        dialogoAyuda = nullptr;

                        if (retratoDialogo) {
                            scene->removeItem(retratoDialogo);
                            delete retratoDialogo;
                            retratoDialogo = nullptr;
                        }

                        dialogoAxemanActivo = false;
                        dialogoActivo = false;
                        dialogoAxemanYaMostrado = true;

                        qDebug("¡Diálogo del Axeman terminado!");
                    }
                }

                return;
            }

            // ---- Diálogo del segundo mapa ----
            if (mundoCambiado)
            {
                if (timerTextoSegundoMapa && timerTextoSegundoMapa->isActive())
                {
                    timerTextoSegundoMapa->stop();
                    dialogoTexto->setPlainText(dialogoSegundoMapa[fraseActualSegundoMapa]);
                }
                else
                {
                    fraseActualSegundoMapa++;

                    if (fraseActualSegundoMapa < dialogoSegundoMapa.size())
                    {
                        letraActualSegundoMapa = 0;
                        textoParcialSegundoMapa.clear();
                        dialogoTexto->setPlainText("");

                        timerTextoSegundoMapa->start(40);
                    }
                    else
                    {
                        scene->removeItem(dialogoCaja);
                        scene->removeItem(dialogoTexto);
                        scene->removeItem(dialogoAyuda);

                        delete dialogoCaja;
                        delete dialogoTexto;
                        delete dialogoAyuda;

                        dialogoCaja = nullptr;
                        dialogoTexto = nullptr;
                        dialogoAyuda = nullptr;

                        if (retratoDialogo) {
                            scene->removeItem(retratoDialogo);
                            delete retratoDialogo;
                            retratoDialogo = nullptr;
                        }

                        dialogoActivo = false;
                        dialogoWizardYaMostrado = true;

                        qDebug("¡Diálogo del segundo mapa terminado!");
                    }
                }

                return;
            }

            // ---- Diálogo del mago (primer mapa) ----
            else
            {
                if (timerTextoWizard && timerTextoWizard->isActive())
                {
                    timerTextoWizard->stop();
                    dialogoTexto->setPlainText(dialogoWizard[fraseActualWizard]);
                }
                else
                {
                    fraseActualWizard++;

                    if (fraseActualWizard < dialogoWizard.size())
                    {
                        letraActualWizard = 0;
                        textoParcialWizard.clear();

                        dialogoTexto->setPlainText("");

                        if (timerTextoWizard == nullptr)
                        {
                            timerTextoWizard = new QTimer(this);
                            connect(timerTextoWizard, &QTimer::timeout, this, &GameWindow::updateLetraWizard);
                        }

                        timerTextoWizard->start(40);
                    }
                    else
                    {
                        scene->removeItem(dialogoCaja);
                        scene->removeItem(dialogoTexto);
                        scene->removeItem(dialogoAyuda);

                        delete dialogoCaja;
                        delete dialogoTexto;
                        delete dialogoAyuda;

                        dialogoCaja = nullptr;
                        dialogoTexto = nullptr;
                        dialogoAyuda = nullptr;

                        if (retratoDialogo) {
                            scene->removeItem(retratoDialogo);
                            delete retratoDialogo;
                            retratoDialogo = nullptr;
                        }

                        dialogoActivo = false;

                        QMessageBox::StandardButton reply;
                        reply = QMessageBox::question(this, "Mago", "¿Deseas ir a otro mundo?",
                                                      QMessageBox::Yes | QMessageBox::No);

                        if (reply == QMessageBox::Yes)
                        {
                            qDebug("Cambiando a nuevo mundo...");
                            cambiarAMundoNuevo();
                            mundoCambiado = true;
                        }
                        else
                        {
                            qDebug("Te quedas en este mundo.");

                            if (slimeTimer && !slimeTimer->isActive()) {
                                slimeTimer->start(150);
                            }
                        }
                    }
                }

                return;
            }
        }

        return;
    }

    // --- Movimiento del jugador ---
    if (!player) return;
    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_M) {
        if (!mapaWidget) {
            mapaWidget = new MapaWidget();
            connect(mapaWidget, &MapaWidget::cerrarMapa, this, [=]() {
                mapaWidget->deleteLater();
                mapaWidget = nullptr;
            });
            mapaWidget->show();
        } else {
            mapaWidget->raise();
            mapaWidget->activateWindow();
        }
    }

    if (event->key() == Qt::Key_I) {
        if (!inventarioUI) {
            inventarioUI = new InventarioWidget(inventarioGlobal);
            inventarioUI->setAttribute(Qt::WA_DeleteOnClose);
            connect(inventarioUI, &QWidget::destroyed, this, [=]() {
                inventarioUI = nullptr;
            });
            inventarioUI->show();
        } else {
            inventarioUI->raise();
            inventarioUI->activateWindow();
        }
    }
    if (event->key() == Qt::Key_K) {
        if (!skillTreeUI) {
            skillTreeUI = new SkillTreeViewer(aliados);
            skillTreeUI->setAttribute(Qt::WA_DeleteOnClose);
            connect(skillTreeUI, &QWidget::destroyed, this, [=]() {
                skillTreeUI = nullptr;
            });
            skillTreeUI->show();
        } else {
            skillTreeUI->raise();
            skillTreeUI->activateWindow();
        }
    }
    if (event->key() == Qt::Key_T) {
        if (!tiendaUI) {
            tiendaUI =new TiendaWidget(inventarioGlobal);  // usa tus aliados existentes
            tiendaUI->setAttribute(Qt::WA_DeleteOnClose);
            connect(tiendaUI, &QWidget::destroyed, this, [=]() {
                tiendaUI = nullptr;
            });
            tiendaUI->show();
        } else {
            tiendaUI->raise();
            tiendaUI->activateWindow();
        }
    }

    if (idleTimer->isActive()) {
        idleTimer->stop();
        idleFrameIndex = 0;
    }

    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {

        // ----- Mapa 7 → Mapa 3 -----
        if (mundoSieteActivo) {
            if (zonaTransicionAMapa3DesdeMapa7 && player) {
                QRectF zona = zonaTransicionAMapa3DesdeMapa7->sceneBoundingRect();
                QRectF caballero = player->sceneBoundingRect();

                if (zona.intersects(caballero)) {
                    qDebug("Transición del mapa 7 al mapa 3...");
                    mundoSieteActivo = false;
                    mundoTresActivo = true;
                    cambiarAMundoTres();
                    player->setPos(200, 90);  // Posición en mapa 3
                    view->centerOn(player);
                    return;
                }
            }

            qDebug("ENTER presionado en mapa 7 sin zona válida de salida.");
            return;
        }



        // ----- Mapa 5 -----
        if (mundoCincoActivo) {
            if (zonaTransicionAMapa3DesdeMapa5 && player) {
                QRectF zona = zonaTransicionAMapa3DesdeMapa5->sceneBoundingRect();
                QRectF caballero = player->sceneBoundingRect();

                if (zona.intersects(caballero)) {
                    qDebug("Regresando del mapa 5 al mapa 3...");
                    mundoCincoActivo = false;
                    mundoTresActivo = true;
                    cambiarAMundoTres();
                    player->setPos(1380, 170);  // Posición en mapa 3
                    view->centerOn(player);
                    return;
                }
            }

            qDebug("ENTER presionado en mapa 5 sin zona de salida.");
            return;
        }




        if (mundoSeisActivo) {
            if (zonaTransicionDesdeMapa6 && player) {
                QRectF zona = zonaTransicionDesdeMapa6->sceneBoundingRect();
                QRectF caballero = player->sceneBoundingRect();

                if (zona.intersects(caballero)) {
                    qDebug("Regresando del mapa 6 al mapa 4...");
                    mundoSeisActivo = false;
                    mundoCuatroActivo = true;
                    cambiarAMundoCuatro();
                    player->setPos(870, 220);
                    view->centerOn(player);
                    return;
                } else {
                    qDebug("ENTER presionado en mapa 6 fuera de zona de salida.");

                    // --- Verificar si está en la zona que lleva al mapa 5
                    // ----- Mapa 6 → Mapa 5 -----
                    if (mundoSeisActivo && zonaTransicionAMapa5DesdeMapa6 && player) {
                        QRectF zona5 = zonaTransicionAMapa5DesdeMapa6->sceneBoundingRect();
                        QRectF caballero = player->sceneBoundingRect();

                        if (zona5.intersects(caballero)) {
                            qDebug("Entrando al mapa 5 desde mapa 6...");
                            mundoSeisActivo = false;
                            mundoCincoActivo = true;
                            cambiarAMundoCinco();
                            player->setPos(200, 1520); // ← Posición en el mapa 5
                            view->centerOn(player);
                            return;
                        }
                    }

                    return;
                }
            } else {
                qDebug("ENTER presionado en mapa 6 pero zona o jugador inválido.");
                return;
            }
        }



        // ----- Mapa 3 → Mapa 2 -----
        if (mundoTresActivo) {
            if (zonaTransicionMapaTres && player) {
                QRectF zona = zonaTransicionMapaTres->sceneBoundingRect();
                QRectF caballero = player->sceneBoundingRect();

                if (zona.intersects(caballero)) {
                    mundoTresActivo = false;
                    regresoDesdeMapaTres = true;
                    cambiarAMundoNuevo();
                    player->setPos(1050, 530);
                    view->centerOn(player);
                    return;
                }
            } else {
                qDebug("No se puede regresar del mapa 3 al 2: zona o jugador inválido.");
                return;
            }
        }

        // ----- Mapa 4 → Mapa 2 -----
        if (mundoCuatroActivo && zonaTransicionDesdeMapa4 && player) {
            QRectF zona = zonaTransicionDesdeMapa4->sceneBoundingRect();
            QRectF caballero = player->sceneBoundingRect();

            if (zona.intersects(caballero)) {
                mundoCuatroActivo = false;
                regresoDesdeMapaCuatro = true;
                cambiarAMundoNuevo();
                mundoCambiado = true;
                player->setPos(40, 470);
                view->centerOn(player);
                return;
            }
        }


        if (!mundoTresActivo && !mundoCuatroActivo && !mundoCincoActivo && !mundoSeisActivo && !mundoSieteActivo) {
            if (zonaTransicionAMapa7 && player) {
                QRectF zona = zonaTransicionAMapa7->sceneBoundingRect();
                QRectF caballero = player->sceneBoundingRect();

                if (zona.intersects(caballero)) {
                    qDebug("Transición al Mapa 7 desde el Mapa 2...");
                    mundoSieteActivo = true;
                    cambiarAMundoSiete();
                    return;
                }
            }
        }

        // ----- Mapa 2 → Mapa 3 -----
        if (!mundoTresActivo && !mundoCuatroActivo && !mundoCincoActivo && !mundoSeisActivo && puedeCambiarAMapa3) {
            qDebug("Entrando al mapa 3 con Enter...");
            mundoTresActivo = true;
            cambiarAMundoTres();
            puedeCambiarAMapa3 = false;
            return;
        }


        // ----- Mapa 3 → Mapa 5 -----
        if (mundoTresActivo && puedeCambiarAMapa5) {
            mundoTresActivo = false;
            mundoCincoActivo = true;
            cambiarAMundoCinco();
            puedeCambiarAMapa5 = false;
            return;
        }

        // ----- Mapa 2 → Mapa 4 -----
        if (!mundoTresActivo && !mundoCuatroActivo) {
            if (zonaTransicionAMapa4 && player) {
                QRectF zona = zonaTransicionAMapa4->sceneBoundingRect();
                QRectF caballero = player->sceneBoundingRect();

                if (zona.intersects(caballero)) {
                    cambiarAMundoCuatro();
                    return;
                }
            } else {
                qDebug("No se puede pasar al mapa 4: zona o jugador inválido.");
                return;
            }
        }

        // ----- Mapa 4 → Mapa 6 -----
        if (mundoCuatroActivo && puedeCambiarAMapa6) {
            mundoCuatroActivo = false;
            mundoSeisActivo = true;
            cambiarAMundoSeis();
            puedeCambiarAMapa6 = false;
            return;
        }
    }


    switch (event->key()) {
    case Qt::Key_Up:
        currentDirection = Up;
        if (!moveTimer->isActive()) moveTimer->start(100);
        break;
    case Qt::Key_Down:
        currentDirection = Down;
        if (!moveTimer->isActive()) moveTimer->start(100);
        break;
    case Qt::Key_Left:
        currentDirection = Left;
        if (!moveTimer->isActive()) moveTimer->start(100);
        break;
    case Qt::Key_Right:
        currentDirection = Right;
        if (!moveTimer->isActive()) moveTimer->start(100);
        break;

    default:
        QWidget::keyPressEvent(event);
    }
}




void GameWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;

    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Left:
    case Qt::Key_Right:
        currentDirection = None;
        moveTimer->stop();

        // Iniciar Idle animado
        if (!idleSheet.isNull()) {
            idleFrameIndex = 0;
            idleTimer->start(250);
        }
        break;
    default:
        QWidget::keyReleaseEvent(event);
    }
}

void GameWindow::updateMovement()
{
    if (!player || currentDirection == None) return;

    int step = playerSpeed;
    int frameWidth = 100;
    int frameHeight = 100;
    int walkFrames = 8;

    qreal dx = 0, dy = 0;

    switch (currentDirection) {
    case Up:
        dy = -step;
        break;
    case Down:
        dy = step;
        break;
    case Left:
        dx = -step;
        reflejarSprite = true;
        break;
    case Right:
        dx = step;
        reflejarSprite = false;
        break;
    default:
        return;
    }

    // Verificar colisión
    if (!colisionaConBloqueadores(player, dx, dy)) {
        player->moveBy(dx, dy);
        checkWizardInteraction();
        checkSlimeInteraction();
        checkSlime2Interaction();
        checkAxemanInteraction();
        checkOrcInteraction();
        checkOrc2Interaction();


        view->centerOn(player);
    } else {
        qDebug("Colisión detectada!");
    }

    // Animación Walk con reflejo
    frameIndex = (frameIndex + 1) % walkFrames;
    QPixmap frame = walkSheet.copy(frameIndex * frameWidth, 0, frameWidth, frameHeight);

    if (reflejarSprite) {
        frame = frame.transformed(QTransform().scale(-1, 1));
    }

    player->setPixmap(frame);


    // Mostrar/ocultar label del Mago
    if (labelMago && wizard) {
        qreal distanceMago = QLineF(player->scenePos(), wizard->scenePos()).length();
        if (distanceMago < 50.0) {
            labelMago->setVisible(true);
            labelMago->setPos(wizard->x() + 20, wizard->y() - 20);
        } else {
            labelMago->setVisible(false);
        }
    }

    // Mostrar/ocultar label del Slime
    if (labelSlime && slime) {
        qreal distanceSlime = QLineF(player->scenePos(), slime->scenePos()).length();
        if (distanceSlime < 50.0) {
            labelSlime->setVisible(true);
            labelSlime->setPos(slime->x() + 20, slime->y() - 20);
        } else {
            labelSlime->setVisible(false);
        }
    }
    // Mostrar/ocultar label del Slime2
    if (labelSlime2 && slime2) {
        qreal distanceSlime2 = QLineF(player->scenePos(), slime2->scenePos()).length();
        if (distanceSlime2 < 50.0) {
            labelSlime2->setVisible(true);
            labelSlime2->setPos(slime2->x() + 20, slime2->y() - 20);
        } else {
            labelSlime2->setVisible(false);
        }
    }

    if (mundoCambiado && npcTienda && mensajeTienda && player) {
        QPointF posJugador = player->pos();
        QPointF posNPC = npcTienda->pos();

        qreal distancia = QLineF(posJugador, posNPC).length();

        if (distancia < 80 ) {
            mensajeTienda->setPlainText("¡Preciona 'T' para ver Mi Tienda!");

        } else if (distancia >= 80 ) {
            mensajeTienda->setPlainText("");  // Oculta el mensaje si se aleja

        }
    }


    // Verificar si el jugador está en zona de transición al mapa 3 (pero sin cambiar aún)
    if (mundoCambiado && !mundoTresActivo) {
        QRectF zonaTransicion(1188, 486, 5, 258);
        QRectF rectJugador = player->sceneBoundingRect();

        if (zonaTransicion.intersects(rectJugador)) {
            puedeCambiarAMapa3 = true;
        } else {
            puedeCambiarAMapa3 = false;
        }
    }



    // Verificar si el jugador está en zona de transición al mapa 5 desde el mapa 3
    if (player && mundoTresActivo && !mundoCincoActivo) {
        QRectF zonaAMapa5(1452, 191, 111, 13);
        QRectF rectJugador = player->sceneBoundingRect();

        if (zonaAMapa5.intersects(rectJugador)) {
            puedeCambiarAMapa5 = true;
        } else {
            puedeCambiarAMapa5 = false;
        }
    } else {
        puedeCambiarAMapa5 = false;
    }


    if (mundoCuatroActivo && !mundoSeisActivo && zonaTransicionAMapa6) {
        QRectF zona = zonaTransicionAMapa6->sceneBoundingRect();
        QRectF caballero = player->sceneBoundingRect();

        puedeCambiarAMapa6 = zona.intersects(caballero);
    } else {
        puedeCambiarAMapa6 = false;
    }

    if (mundoSeisActivo && zonaTransicionDesdeMapa6 && player) {
        QRectF zona = zonaTransicionDesdeMapa6->sceneBoundingRect();
        QRectF caballero = player->sceneBoundingRect();

        puedeRegresarDesdeMapa6 = zona.intersects(caballero);
    } else {
        puedeRegresarDesdeMapa6 = false;
    }


    // Verificar si el jugador está en zona de transición del mapa 6 al mapa 5
    if (mundoSeisActivo && !mundoCincoActivo) {
        if (zonaTransicionAMapa5DesdeMapa6 && player) {
            QRectF zona = zonaTransicionAMapa5DesdeMapa6->sceneBoundingRect();
            QRectF caballero = player->sceneBoundingRect();

            puedeCambiarAMapa5DesdeMapa6 = zona.intersects(caballero);
        } else {
            puedeCambiarAMapa5DesdeMapa6 = false;
        }
    }

    if (mundoSieteActivo && !mundoTresActivo) {
        if (zonaTransicionAMapa3DesdeMapa7 && player) {
            QRectF zona = zonaTransicionAMapa3DesdeMapa7->sceneBoundingRect();
            QRectF caballero = player->sceneBoundingRect();

            puedeCambiarAMapa3DesdeMapa7 = zona.intersects(caballero);
        } else {
            puedeCambiarAMapa3DesdeMapa7 = false;
        }
    }
}

//------------------------------------------------colisiones villa c++ --------------------------------------------
bool GameWindow::colisionaConBloqueadores(QGraphicsItem *item, qreal dx, qreal dy)
{
    if (!item) return false;

    item->moveBy(dx, dy);
    QList<QGraphicsItem*> colisiones = item->collidingItems();
    item->moveBy(-dx, -dy);

    for (QGraphicsItem *colItem : colisiones) {

        if (bloqueadores.contains(colItem)) {
            return true;
        }
    }
    return false;
}

// -------------------------------------- Retratos de los dialogos -------------------------------------------------------
void GameWindow::mostrarRetrato(const QString &quien)
{
    if (retratoDialogo) {
        scene->removeItem(retratoDialogo);
        delete retratoDialogo;
        retratoDialogo = nullptr;
    }

    QPixmap retrato;
    if (quien == "Mago") {
        retrato = retratoMago;
    } else if (quien == "Caballero") {
        retrato = retratoCaballero;
    }
    if (quien == "Axeman") {
        retrato = retratoAxeman;
    }

    QSize tamanoRetrato = mundoCambiado ? QSize(180, 180) : QSize(100, 100);

    retratoDialogo = scene->addPixmap(retrato.scaled(tamanoRetrato, Qt::KeepAspectRatio));
    retratoDialogo->setZValue(11);

    if (dialogoCaja) {
        qreal x = dialogoCaja->pos().x() + (mundoCambiado ? 25 : 10);
        qreal y = dialogoCaja->pos().y() + (mundoCambiado ? 25 : 30);
        retratoDialogo->setPos(x, y);
    } else {
        retratoDialogo->setPos(15, scene->height() - 290);
    }
}


void GameWindow::cambiarAMundoCuatro()
{

    if (npcTienda) {
        scene->removeItem(npcTienda);
        delete npcTienda;
        npcTienda = nullptr;
    }
    if (mensajeTienda) {
        scene->removeItem(mensajeTienda);
        delete mensajeTienda;
        mensajeTienda = nullptr;
    }
    // Verificar antes de borrar Wizard
    if (wizard) {
        scene->removeItem(wizard);
        delete wizard;
        wizard = nullptr;
    }

    // Verificar antes de borrar Axeman
    if (axeman) {
        scene->removeItem(axeman);
        delete axeman;
        axeman = nullptr;
    }
    if (labelAxeman) {
        scene->removeItem(labelAxeman);
        delete labelAxeman;
        labelAxeman = nullptr;
    }
    if (globoOvalo) {
        scene->removeItem(globoOvalo);
        delete globoOvalo;
        globoOvalo = nullptr;
    }
    if (axemanTextoTimer && axemanTextoTimer->isActive()) {
        axemanTextoTimer->stop();
        delete axemanTextoTimer;
        axemanTextoTimer = nullptr;
    }

    // Verificar antes de borrar orcos
    if (orcTimer && orcTimer->isActive()) {
        orcTimer->stop();
        delete orcTimer;
        orcTimer = nullptr;
    }
    if (orc) {
        scene->removeItem(orc);
        delete orc;
        orc = nullptr;
    }
    if (orc2Timer && orc2Timer->isActive()) {
        orc2Timer->stop();
        delete orc2Timer;
        orc2Timer = nullptr;
    }
    if (orc2) {
        scene->removeItem(orc2);
        delete orc2;
        orc2 = nullptr;
    }

    mundoCambiado = false;
    mundoTresActivo = false;
    mundoCuatroActivo = true;

    scene->clear();
    bloqueadores.clear();

    QPixmap mapa4("OpenSprite/Mapa_Desierto.png");
    if (mapa4.isNull()) {
        qDebug("Error: No se pudo cargar el mapa 4.");
        return;
    }

    zonaTransicionAMapa6 = scene->addRect(940, 245, 233, 11, QPen(Qt::red));
    zonaTransicionAMapa6->setBrush(Qt::transparent);
    bloqueadores.append(zonaTransicionAMapa6);


    QGraphicsPixmapItem* mapItem = scene->addPixmap(mapa4);
    mapItem->setZValue(-1);
    scene->setSceneRect(0, 0, mapa4.width(), mapa4.height());

    view->setFixedSize(700, 700);
    this->setFixedSize(700, 700);
    view->resetTransform();
    view->scale(0.6, 0.6);

    // Crear al caballero en la nueva posición
    if (!idleSheet.isNull()) {
        QPixmap frame = idleSheet.copy(0, 0, 100, 100);
        player = scene->addPixmap(frame);
        player->setZValue(1);
        player->setPos(1770, 440);  // Posición al llegar al mapa 4
        player->setScale(4.0);
        view->centerOn(player);
    }

    playerSpeed = 20;

    QGraphicsRectItem* Mapa4colison1 = scene->addRect(1762, 18, 332, 428);
    Mapa4colison1->setBrush(Qt::transparent);
    Mapa4colison1->setPen(Qt::NoPen);
    bloqueadores.append(Mapa4colison1);

    QGraphicsRectItem* Mapa4colison2 = scene->addRect(1454, 110, 332, 270);
    Mapa4colison2->setBrush(Qt::transparent);
    Mapa4colison2->setPen(Qt::NoPen);
    bloqueadores.append(Mapa4colison2);

    QGraphicsRectItem* Mapa4colison3 = scene->addRect(1204, 52, 332, 170);
    Mapa4colison3->setBrush(Qt::transparent);
    Mapa4colison3->setPen(Qt::NoPen);
    bloqueadores.append(Mapa4colison3);

    QGraphicsRectItem* Mapa4colison4 = scene->addRect(572, 48, 332, 170);
    Mapa4colison4->setBrush(Qt::transparent);
    Mapa4colison4->setPen(Qt::NoPen);
    bloqueadores.append(Mapa4colison4);

    QGraphicsRectItem* Mapa4colison5 = scene->addRect(316, 96, 332, 266);
    Mapa4colison5->setBrush(Qt::transparent);
    Mapa4colison5->setPen(Qt::NoPen);
    bloqueadores.append(Mapa4colison5);

    QGraphicsRectItem* Mapa4colison6 = scene->addRect(94, 136, 228, 266);
    Mapa4colison6->setBrush(Qt::transparent);
    Mapa4colison6->setPen(Qt::NoPen);
    bloqueadores.append(Mapa4colison6);

    QGraphicsRectItem* Mapa4colison7 = scene->addRect(68, 326, 58, 762);
    Mapa4colison7->setBrush(Qt::transparent);
    Mapa4colison7->setPen(Qt::NoPen);
    bloqueadores.append(Mapa4colison7);

    QGraphicsRectItem* Mapa4colison8 = scene->addRect(72, 1014, 1788, 72);
    Mapa4colison8->setBrush(Qt::transparent);
    Mapa4colison8->setPen(Qt::NoPen);
    bloqueadores.append(Mapa4colison8);

    QGraphicsRectItem* Mapa4colison9 = scene->addRect(1726, 784, 372, 282);
    Mapa4colison9->setBrush(Qt::transparent);
    Mapa4colison9->setPen(Qt::NoPen);
    bloqueadores.append(Mapa4colison9);

    QGraphicsRectItem* Mapa4colison10 = scene->addRect(1324, 212, 176, 120);
    Mapa4colison10->setBrush(Qt::transparent);
    Mapa4colison10->setPen(Qt::NoPen);
    bloqueadores.append(Mapa4colison10);

    QGraphicsRectItem* Mapa4colison11 = scene->addRect(1541, 361, 32, 82);
    Mapa4colison11->setBrush(Qt::transparent);
    Mapa4colison11->setPen(Qt::NoPen);
    bloqueadores.append(Mapa4colison11);

    QGraphicsRectItem* Mapa4colison12 = scene->addRect(530, 357, 32, 82);
    Mapa4colison12->setBrush(Qt::transparent);
    Mapa4colison12->setPen(Qt::NoPen);
    bloqueadores.append(Mapa4colison12);

    zonaTransicionDesdeMapa4 = scene->addRect(2088, 524, 10, 211); // Ajusta valores si es necesario
    zonaTransicionDesdeMapa4->setBrush(Qt::red);
    zonaTransicionDesdeMapa4->setPen(Qt::NoPen);
    bloqueadores.append(zonaTransicionDesdeMapa4);



    qDebug("Mapa 4 cargado correctamente.");
}


void GameWindow::cambiarAMundoCinco() {
    if (moveTimer && moveTimer->isActive()) moveTimer->stop();
    if (idleTimer && idleTimer->isActive()) idleTimer->stop();

    scene->clear();
    bloqueadores.clear();

    QPixmap fondo("OpenSprite/Mapa_5.png");
    if (fondo.isNull()) {
        qDebug(" No se pudo cargar Mapa_5.png.");
        return;
    }
    view->resetTransform();
    view->scale(0.5, 0.5);

    scene->setSceneRect(0, 0, fondo.width(), fondo.height());
    QGraphicsPixmapItem *fondoItem = scene->addPixmap(fondo);
    fondoItem->setZValue(-1);

    zonaTransicionAMapa3DesdeMapa5 = scene->addRect(616, 2967, 129, 13, QPen(Qt::red));
    zonaTransicionAMapa3DesdeMapa5->setBrush(Qt::transparent);

    // Restaurar caballero
    if (!idleSheet.isNull()) {
        QPixmap frame = idleSheet.copy(0, 0, 100, 100);
        player = scene->addPixmap(frame);
        player->setZValue(1);
        player->setPos(530, 2744);  // Posición inicial
        player->setScale(3.0);
        view->centerOn(player);
    }

    playerSpeed = 15;


    if (moveTimer) moveTimer->start(50);
    if (idleTimer) idleTimer->start(250);

    QGraphicsRectItem* Mapa5colision1 = scene->addRect(592, 2995, 191, 5);
    Mapa5colision1->setBrush(Qt::transparent);  // invisible
    Mapa5colision1->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision1);

    QGraphicsRectItem* Mapa5colision2 = scene->addRect(817, 2780, 147, 213);
    Mapa5colision2->setBrush(Qt::transparent);
    Mapa5colision2->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision2);

    QGraphicsRectItem* Mapa5colision3 = scene->addRect(414, 2779, 147, 213);
    Mapa5colision3->setBrush(Qt::transparent);
    Mapa5colision3->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision3);

    QGraphicsRectItem* Mapa5colision4 = scene->addRect(880, 2742, 735, 62);
    Mapa5colision4->setBrush(Qt::transparent);
    Mapa5colision4->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision4);

    QGraphicsRectItem* Mapa5colision5 = scene->addRect(1590, 2632, 498, 168);
    Mapa5colision5->setBrush(Qt::transparent);
    Mapa5colision5->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision5);

    QGraphicsRectItem* Mapa5colision6 = scene->addRect(2013, 33, 46, 2730);
    Mapa5colision6->setBrush(Qt::transparent);
    Mapa5colision6->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision6);

    QGraphicsRectItem* Mapa5colision7 = scene->addRect(1045, 44, 1022, 72);
    Mapa5colision7->setBrush(Qt::transparent);
    Mapa5colision7->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision7);

    QGraphicsRectItem* Mapa5colision8 = scene->addRect(1043, 26, 113, 718);
    Mapa5colision8->setBrush(Qt::transparent);
    Mapa5colision8->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision8);

    QGraphicsRectItem* Mapa5colision9 = scene->addRect(799, 26, 276, 487);
    Mapa5colision9->setBrush(Qt::transparent);
    Mapa5colision9->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision9);

    QGraphicsRectItem* Mapa5colision10 = scene->addRect(12, 179, 894, 150);
    Mapa5colision10->setBrush(Qt::transparent);
    Mapa5colision10->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision10);

    QGraphicsRectItem* Mapa5colision11 = scene->addRect(12, 122, 71, 1956);
    Mapa5colision11->setBrush(Qt::transparent); // invisible
    Mapa5colision11->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision11);

    QGraphicsRectItem* Mapa5colision12 = scene->addRect(13, 1974, 582, 350);
    Mapa5colision12->setBrush(Qt::transparent);
    Mapa5colision12->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision12);

    QGraphicsRectItem* Mapa5colision13 = scene->addRect(116, 2199, 404, 233);
    Mapa5colision13->setBrush(Qt::transparent);
    Mapa5colision13->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision13);

    QGraphicsRectItem* Mapa5colision14 = scene->addRect(65, 2154, 52, 684);
    Mapa5colision14->setBrush(Qt::transparent);
    Mapa5colision14->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision14);

    QGraphicsRectItem* Mapa5colision15 = scene->addRect(35, 2686, 464, 131);
    Mapa5colision15->setBrush(Qt::transparent);
    Mapa5colision15->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision15);

    QGraphicsRectItem* Mapa5colision16 = scene->addRect(838, 1538, 615, 894);
    Mapa5colision16->setBrush(Qt::transparent); // invisible
    Mapa5colision16->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision16);

    QGraphicsRectItem* Mapa5colision17 = scene->addRect(766, 1940, 140, 380);
    Mapa5colision17->setBrush(Qt::transparent);
    Mapa5colision17->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision17);

    QGraphicsRectItem* Mapa5colision18 = scene->addRect(1034, 2390, 230, 122);
    Mapa5colision18->setBrush(Qt::transparent);
    Mapa5colision18->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision18);

    QGraphicsRectItem* Mapa5colision19 = scene->addRect(69, 1590, 103, 122);
    Mapa5colision19->setBrush(Qt::transparent);
    Mapa5colision19->setPen(Qt::NoPen);
    bloqueadores.append(Mapa5colision19);


    qDebug(" Mapa 5 cargado con éxito.");
}

void GameWindow::cambiarAMundoSeis() {

    if (moveTimer && moveTimer->isActive()) moveTimer->stop();
    if (idleTimer && idleTimer->isActive()) idleTimer->stop();

    scene->clear();
    bloqueadores.clear();

    QPixmap fondo("OpenSprite/Mapa_6.png");
    scene->setSceneRect(0, 0, fondo.width(), fondo.height());
    scene->addPixmap(fondo);

    view->resetTransform();
    view->scale(1.0, 1.0);

    zonaTransicionDesdeMapa6 = scene->addRect(509, 1026, 180, 8, QPen(Qt::red));
    zonaTransicionDesdeMapa6->setBrush(Qt::transparent);
    bloqueadores.append(zonaTransicionDesdeMapa6);

    zonaTransicionAMapa5DesdeMapa6 = scene->addRect(1108, 215, 42, 49, QPen(Qt::red));
    zonaTransicionAMapa5DesdeMapa6->setBrush(Qt::transparent);


    // Restaurar caballero
    if (!idleSheet.isNull()) {
        QPixmap frame = idleSheet.copy(0, 0, 100, 100);
        player = scene->addPixmap(frame);
        player->setZValue(1);
        player->setPos(500, 880);  // Posición inicial
        player->setScale(2.0);
        view->centerOn(player);
    }

    playerSpeed = 8;

    if (moveTimer) moveTimer->start(50);
    if (idleTimer) idleTimer->start(250);

    QGraphicsRectItem* Mapa6colision1 = scene->addRect(2, 1, 1193, 93);
    Mapa6colision1->setBrush(Qt::transparent); // invisible
    Mapa6colision1->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision1);

    QGraphicsRectItem* Mapa6colision2 = scene->addRect(2, 5, 3, 1026);
    Mapa6colision2->setBrush(Qt::transparent);
    Mapa6colision2->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision2);

    QGraphicsRectItem* Mapa6colision3 = scene->addRect(1190, 5, 3, 1026);
    Mapa6colision3->setBrush(Qt::transparent);
    Mapa6colision3->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision3);

    QGraphicsRectItem* Mapa6colision4 = scene->addRect(21, 1030, 1153, 2);
    Mapa6colision4->setBrush(Qt::transparent);
    Mapa6colision4->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision4);

    QGraphicsRectItem* Mapa6colision5 = scene->addRect(1130, 227, 19, 21);
    Mapa6colision5->setBrush(Qt::transparent);
    Mapa6colision5->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision5);

    QGraphicsRectItem* Mapa6colision6 = scene->addRect(268, 80, 3, 232);
    Mapa6colision6->setBrush(Qt::transparent); // invisible
    Mapa6colision6->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision6);

    QGraphicsRectItem* Mapa6colision7 = scene->addRect(264, 268, 282, 58);
    Mapa6colision7->setBrush(Qt::transparent);
    Mapa6colision7->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision7);

    QGraphicsRectItem* Mapa6colision8 = scene->addRect(633, 264, 298, 62);
    Mapa6colision8->setBrush(Qt::transparent);
    Mapa6colision8->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision8);

    QGraphicsRectItem* Mapa6colision9 = scene->addRect(926, 84, 5, 227);
    Mapa6colision9->setBrush(Qt::transparent);
    Mapa6colision9->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision9);

    QGraphicsRectItem* Mapa6colision10 = scene->addRect(5, 510, 84, 125);
    Mapa6colision10->setBrush(Qt::transparent); // invisible
    Mapa6colision10->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision10);

    QGraphicsRectItem* Mapa6colision11 = scene->addRect(67, 529, 477, 165);
    Mapa6colision11->setBrush(Qt::transparent);
    Mapa6colision11->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision11);
    /*
    QGraphicsRectItem* Mapa6colision12 = scene->addRect(2, 1, 1193, 93);
    Mapa6colision12->setBrush(Qt::transparent);
    Mapa6colision12->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision12);*/

    QGraphicsRectItem* Mapa6colision13 = scene->addRect(266, 464, 95, 309);
    Mapa6colision13->setBrush(Qt::transparent);
    Mapa6colision13->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision13);


    QGraphicsRectItem* Mapa6colision14 = scene->addRect(655, 531, 459, 164);
    Mapa6colision14->setBrush(Qt::transparent); // invisible
    Mapa6colision14->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision14);

    QGraphicsRectItem* Mapa6colision15 = scene->addRect(842, 463, 97, 307);
    Mapa6colision15->setBrush(Qt::transparent); // invisible
    Mapa6colision15->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision15);

    QGraphicsRectItem* Mapa6colision16 = scene->addRect(1105, 507, 97, 139);
    Mapa6colision16->setBrush(Qt::transparent); // invisible
    Mapa6colision16->setPen(Qt::NoPen);
    bloqueadores.append(Mapa6colision16);


}

void GameWindow::cambiarAMundoSiete() {

    if (npcTienda) {
        scene->removeItem(npcTienda);
        delete npcTienda;
        npcTienda = nullptr;
    }
    if (mensajeTienda) {
        scene->removeItem(mensajeTienda);
        delete mensajeTienda;
        mensajeTienda = nullptr;
    }
    // Eliminar orc1
    if (orcTimer && orcTimer->isActive()) {
        orcTimer->stop();
        delete orcTimer;
        orcTimer = nullptr;
    }
    if (orc) {
        scene->removeItem(orc);
        delete orc;
        orc = nullptr;
    }

    // Eliminar orc2
    if (orc2Timer && orc2Timer->isActive()) {
        orc2Timer->stop();
        delete orc2Timer;
        orc2Timer = nullptr;
    }
    if (orc2) {
        scene->removeItem(orc2);
        delete orc2;
        orc2 = nullptr;
    }

    // Eliminar wizard
    if (wizard) {
        scene->removeItem(wizard);
        delete wizard;
        wizard = nullptr;
    }

    // Eliminar axeman
    if (axeman) {
        scene->removeItem(axeman);
        delete axeman;
        axeman = nullptr;
    }
    if (labelAxeman) {
        scene->removeItem(labelAxeman);
        delete labelAxeman;
        labelAxeman = nullptr;
    }
    if (globoOvalo) {
        scene->removeItem(globoOvalo);
        delete globoOvalo;
        globoOvalo = nullptr;
    }
    if (axemanTextoTimer && axemanTextoTimer->isActive()) {
        axemanTextoTimer->stop();
        delete axemanTextoTimer;
        axemanTextoTimer = nullptr;
    }
    if (moveTimer && moveTimer->isActive()) moveTimer->stop();
    if (idleTimer && idleTimer->isActive()) idleTimer->stop();

    scene->clear();
    bloqueadores.clear();

    QPixmap fondo("OpenSprite/Mapa_7.png");
    scene->setSceneRect(0, 0, fondo.width(), fondo.height());
    scene->addPixmap(fondo);

    view->resetTransform();
    view->scale(0.5, 0.5);

    zonaTransicionAMapa3DesdeMapa7 = scene->addRect(1455, 351, 30, 288, QPen(Qt::red));
    zonaTransicionAMapa3DesdeMapa7->setBrush(Qt::transparent);
    bloqueadores.append(zonaTransicionAMapa3DesdeMapa7);



    if (!idleSheet.isNull()) {
        QPixmap frame = idleSheet.copy(0, 0, 100, 100);
        player = scene->addPixmap(frame);
        player->setZValue(1);
        player->setPos(330, 1410); // ← Posición inicial en mapa 7
        player->setScale(4.0);
        view->centerOn(player);
    }
    playerSpeed = 15;
    if (moveTimer) moveTimer->start(50);
    if (idleTimer) idleTimer->start(250);

    QGraphicsRectItem* Mapa7colision1 = scene->addRect(711, 1317, 696, 336);
    Mapa7colision1->setBrush(Qt::transparent); // invisible
    Mapa7colision1->setPen(Qt::NoPen);
    bloqueadores.append(Mapa7colision1);

    QGraphicsRectItem* Mapa7colision2 = scene->addRect(1152, 723, 312, 741);
    Mapa7colision2->setBrush(Qt::transparent); // invisible
    Mapa7colision2->setPen(Qt::NoPen);
    bloqueadores.append(Mapa7colision2);

    QGraphicsRectItem* Mapa7colision3 = scene->addRect(984, 648, 411, 192);
    Mapa7colision3->setBrush(Qt::transparent); // invisible
    Mapa7colision3->setPen(Qt::NoPen);
    bloqueadores.append(Mapa7colision3);

    QGraphicsRectItem* Mapa7colision4 = scene->addRect(984, 21, 489, 306);
    Mapa7colision4->setBrush(Qt::transparent); // invisible
    Mapa7colision4->setPen(Qt::NoPen);
    bloqueadores.append(Mapa7colision4);

    QGraphicsRectItem* Mapa7colision5 = scene->addRect(18, 21, 957, 168);
    Mapa7colision5->setBrush(Qt::transparent); // invisible
    Mapa7colision5->setPen(Qt::NoPen);
    bloqueadores.append(Mapa7colision5);

    QGraphicsRectItem* Mapa7colision6 = scene->addRect(18, 21, 177, 1170);
    Mapa7colision6->setBrush(Qt::transparent); // invisible
    Mapa7colision6->setPen(Qt::NoPen);
    bloqueadores.append(Mapa7colision6);

    QGraphicsRectItem* Mapa7colision7 = scene->addRect(18, 1146, 327, 510);
    Mapa7colision7->setBrush(Qt::transparent); // invisible
    Mapa7colision7->setPen(Qt::NoPen);
    bloqueadores.append(Mapa7colision7);

    QGraphicsRectItem* Mapa7colision8 = scene->addRect(258, 1653, 453, 10);
    Mapa7colision8->setBrush(Qt::transparent); // invisible
    Mapa7colision8->setPen(Qt::NoPen);
    bloqueadores.append(Mapa7colision8);

    QGraphicsRectItem* Mapa7colision9 = scene->addRect(474, 150, 54, 402);
    Mapa7colision9->setBrush(Qt::transparent); // invisible
    Mapa7colision9->setPen(Qt::NoPen);
    bloqueadores.append(Mapa7colision9);

    QGraphicsRectItem* Mapa7colision10 = scene->addRect(486, 801, 528, 60);
    Mapa7colision10->setBrush(Qt::transparent); // invisible
    Mapa7colision10->setPen(Qt::NoPen);
    bloqueadores.append(Mapa7colision10);
}
