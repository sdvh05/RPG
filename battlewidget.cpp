#include "BattleWidget.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRandomGenerator64>
#include <QMouseEvent>
#include <QMessageBox>



BattleWidget::BattleWidget(QWidget* parent)
    : QWidget(parent), fondo("Personajes/pruebaF.jpeg")
{
    setFixedSize(800, 480);

    crearInterfaz();
    CargarAliados();
    MostrarAliados();
    CargarEnemigos();
    MostrarEnemigos();

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, QOverload<>::of(&BattleWidget::update));
    updateTimer->start(1000 / 60); // 60 FPS

    faseActual = PLANIFICAR;
    indiceAliado = 0;

    if (!aliados.isEmpty())
        lblSeleccion->setText("¿Qué hará " + aliados[0]->getNombre() + "?");
    if (!aliados.isEmpty()) {
        lblSeleccion->setText("¿Qué hará " + aliados[0]->getNombre() + "?");
        actualizarColorBotones(aliados[0]->getNombre());
    }
}

BattleWidget::~BattleWidget() {
    for (Personaje* p : aliados) delete p;
    for (Personaje* e : enemigos) delete e;
}


void BattleWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawPixmap(rect(), fondo);

    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    //Aliados
    for (Personaje* p : aliados) {
        QPixmap sprite = p->getFrameActual();
        QRect pos = p->getPosicion();
        //qDebug() << p->getNombre() << " → Frame size:" << sprite.size() << " Pos:" << pos;


        // Dibujar sprite o marcador si no cargó
        if (!sprite.isNull()) {
            painter.drawPixmap(pos, sprite);
        } else {
            painter.setBrush(Qt::red);
            painter.drawRect(pos);
            painter.setPen(Qt::white);
            painter.drawText(pos.center(), p->getNombre());

            if (faseActual == PLANIFICAR && aliados[indiceAliado] == p) {
                painter.setPen(QPen(Qt::green, 3));
                painter.drawRect(pos.adjusted(-2, -2, 2, 2));
            }
        }

        // Nombre
        painter.setPen(Qt::white);
        painter.drawText(pos.x(), pos.y() - 5, p->getNombre());

        // Barra de vida
        int barWidth = pos.width();
        QRect vidaRect(pos.x(), pos.y() - 15, barWidth, 5);
        painter.setBrush(Qt::darkRed);
        painter.drawRect(vidaRect);

        float vidaRatio = (float)p->getVidaActual() / p->getVidaMax(); // necesitas getVidaMax()
        QRect vidaActualRect(pos.x(), pos.y() - 15, barWidth * vidaRatio, 5);
        painter.setBrush(Qt::green);
        painter.drawRect(vidaActualRect);


    }

    //Enemigos
    for (Personaje* e : enemigos) {
        QPixmap sprite = e->getFrameActual();
        QRect pos = e->getPosicion();

        if (!sprite.isNull()) {
            painter.drawPixmap(pos, sprite);
        } else {
            painter.setBrush(Qt::red);
            painter.drawRect(pos);
            painter.setPen(Qt::white);
            painter.drawText(pos.center(), e->getNombre());
        }

        painter.setPen(Qt::white);
        painter.drawText(pos.x(), pos.y() - 5, e->getNombre());

        // Barra de vida
        int barWidth = pos.width();
        QRect vidaRect(pos.x(), pos.y() - 15, barWidth, 5);
        painter.setBrush(Qt::darkRed);
        painter.drawRect(vidaRect);

        float vidaRatio = (float)e->getVidaActual() / e->getVidaMax(); // necesitas getVidaMax()
        QRect vidaActualRect(pos.x(), pos.y() - 15, barWidth * vidaRatio, 5);
        painter.setBrush(Qt::green);
        painter.drawRect(vidaActualRect);
        // Resaltado del enemigo seleccionado
        if (e == enemigoSeleccionado) {
            QRect marco = pos.adjusted(-4, -4, 4, 4); // Un marco más amplio que no tapa
            painter.setPen(QPen(Qt::red, 3));
            painter.drawRect(marco);
        }


    }
}

void BattleWidget::crearInterfaz() {
    btnAtacar = new QPushButton("Atacar", this);
    btnEspecial = new QPushButton("Ataque Especial", this);
    btnInventario = new QPushButton("Inventario", this);
    btnHuir = new QPushButton("Huir", this);

    lblSeleccion = new QLabel(this);
    lblSeleccion->setGeometry(10, 10, 300, 30);
   // lblSeleccion->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 4px;");
    lblSeleccion->setText("Elige acción para...");

    btnAtacar->setGeometry(10, 400, 120, 30);
    btnEspecial->setGeometry(140, 400, 120, 30);
    btnInventario->setGeometry(270, 400, 120, 30);
    btnHuir->setGeometry(400, 400, 120, 30);

    connect(btnAtacar, &QPushButton::clicked, this, [=]() {
        accionSeleccionada("atacar");
    });
    connect(btnEspecial, &QPushButton::clicked, this, [=]() {
        accionSeleccionada("especial");
    });
    connect(btnHuir, &QPushButton::clicked, this, [=]() {
        qDebug() << "Intentaste huir...";
    });
}

void BattleWidget::CargarAliados() {
    Curandera* c = new Curandera();
    Caballero* k = new Caballero();
    Princesa* p = new Princesa();
    aliados = { c, k, p };
}

void BattleWidget::MostrarAliados() {
    int startY = 100;
    for (int i = 0; i < aliados.size(); ++i) {
        QRect pos(50, startY + i * 100, 64, 64);
        aliados[i]->setPosicion(pos);
    }
}

void BattleWidget::CargarEnemigos() {
    enemigos.clear();

    int cantidad = QRandomGenerator::global()->bounded(1, 3); // 1 a 2 enemigos
    for (int i = 0; i < cantidad; ++i) {
        Slime* s = new Slime();
        enemigos.append(s);
    }
}

void BattleWidget::MostrarEnemigos() {
    int startY = 100;
    for (int i = 0; i < enemigos.size(); ++i) {
        QRect pos(640, startY + i * 100, 64, 64);
        enemigos[i]->setPosicion(pos);
    }
}

void BattleWidget::accionSeleccionada(QString tipo) {
    if (faseActual != PLANIFICAR || indiceAliado >= aliados.size()) return;

    // Evitar que personaje muerto actúe
    if (aliados[indiceAliado]->getVidaActual() == 0) {
        indiceAliado++;
        if (indiceAliado < aliados.size()) {
            actualizarColorBotones(aliados[indiceAliado]->getNombre());
            lblSeleccion->setText("¿Qué hará " + aliados[indiceAliado]->getNombre() + "?");
        } else {
            faseActual = EJECUTAR;
            lblSeleccion->hide();
            ejecutarAccionesAliadas();
        }
        return;
    }

    Personaje* personajeActual = aliados[indiceAliado];

    // Elegir enemigo seleccionado o el primero si no hay uno
    Personaje* objetivo = (enemigoSeleccionado && enemigos.contains(enemigoSeleccionado))
                              ? enemigoSeleccionado
                              : (!enemigos.isEmpty() ? enemigos[0] : nullptr);

    // Guardamos la acción planificada
    accionesAliados.append({ personajeActual, tipo, objetivo });

    // Reset para el siguiente personaje
    enemigoSeleccionado = nullptr;
    indiceAliado++;

    if (indiceAliado < aliados.size()) {
        actualizarColorBotones(aliados[indiceAliado]->getNombre());
        lblSeleccion->setText("¿Qué hará " + aliados[indiceAliado]->getNombre() + "?");
    } else {
        faseActual = EJECUTAR;
        lblSeleccion->hide();
        ejecutarAccionesAliadas();
    }
}



void BattleWidget::actualizarColorBotones(const QString& nombrePersonaje) {
    QString color;

    if (nombrePersonaje.contains("Curandera", Qt::CaseInsensitive))
        color = "#4CAF50"; // Verde
    else if (nombrePersonaje.contains("Caballero", Qt::CaseInsensitive))
        color = "#F44336"; // Rojo
    else if (nombrePersonaje.contains("Princesa", Qt::CaseInsensitive))
        color = "#E91E63"; // Rosado
    else
        color = "#607D8B"; // Gris por defecto

    QString estiloBoton = QString("background-color: %1; color: white; font-weight: bold;").arg(color);
    btnAtacar->setStyleSheet(estiloBoton);
    btnEspecial->setStyleSheet(estiloBoton);
    btnInventario->setStyleSheet(estiloBoton);
    btnHuir->setStyleSheet(estiloBoton);

    QString estiloLabel = QString("background-color: %1; color: white; font-weight: bold; padding: 4px;").arg(color);
    lblSeleccion->setStyleSheet(estiloLabel);
}




void BattleWidget::ejecutarAccionesAliadas() {
    setBotonesHabilitados(false);
    verificarVictoria();

    if (accionesAliados.isEmpty()) return;



    QTimer* timer = new QTimer(this);
    int index = 0;

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        if (index >= accionesAliados.size()) {
            timer->stop();
            faseActual = ENEMIGOS;
            accionesAliados.clear();
            ejecutarTurnoEnemigos();
            return;
        }

        AccionPlanificada accion = accionesAliados[index++];

        if (accion.actor->getVidaActual() == 0) {
            index++;
            return;
        }


        if (accion.tipo == "atacar") {
            if (accion.objetivo) {
                accion.objetivo->recibirDanio(accion.actor->getAtaque());
                qDebug() << accion.actor->getNombre() << " ataca a " << accion.objetivo->getNombre();
                accion.actor->setEstado("attack");
            }
        } else if (accion.tipo == "especial") {
            accion.objetivo->recibirDanio(accion.actor->getAtaque());
            qDebug() << accion.actor->getNombre() << "usa su ataque especial " ;
            accion.actor->setEstado("especial");
        }

        update();
    });

    eliminarMuertos();
    verificarVictoria();

    timer->start(1000);
}

void BattleWidget::ejecutarTurnoEnemigos() {
     verificarVictoria();
    eliminarMuertos();
    QTimer* timer = new QTimer(this);
    int index = 0;

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        if (index >= enemigos.size()) {
            timer->stop();
            faseActual = PLANIFICAR;
            indiceAliado = 0;
            setBotonesHabilitados(true);
            eliminarMuertos();

            if (!aliados.isEmpty()) {
                lblSeleccion->show();
                lblSeleccion->setText("¿Qué hará " + aliados[0]->getNombre() + "?");
            }

            return;
        }

        Personaje* enemigo = enemigos[index++];
        if (!aliados.isEmpty()) {
            Personaje* objetivo = aliados[rand() % aliados.size()];
            objetivo->recibirDanio(enemigo->getAtaque());
            enemigo->setEstado("attack");
            qDebug() << enemigo->getNombre() << " ataca a " << objetivo->getNombre();
        }

        update();

    });

    verificarVictoria();
    timer->start(1000);
}

void BattleWidget::eliminarMuertos() {
    aliados.erase(std::remove_if(aliados.begin(), aliados.end(),
                                 [](Personaje* p) { return p->getVidaActual() == 0; }), aliados.end());

    enemigos.erase(std::remove_if(enemigos.begin(), enemigos.end(),
                                  [](Personaje* p) { return p->getVidaActual() == 0; }), enemigos.end());
}

void BattleWidget::setBotonesHabilitados(bool habilitado) {
    btnAtacar->setEnabled(habilitado);
    btnEspecial->setEnabled(habilitado);
    btnInventario->setEnabled(habilitado);
    btnHuir->setEnabled(habilitado);
}

void BattleWidget::mousePressEvent(QMouseEvent* event) {
    if (faseActual != PLANIFICAR || indiceAliado >= aliados.size()) return;

    QPoint clickPos = event->pos();

    // Verifica si se ha hecho clic sobre algún enemigo
    for (Personaje* enemigo : enemigos) {
        if (enemigo->getPosicion().contains(clickPos)) {
            enemigoSeleccionado = enemigo;
            qDebug() << "Enemigo seleccionado:" << enemigoSeleccionado->getNombre();
            update();
            break;
        }
    }
}

void BattleWidget::verificarVictoria() {
    qDebug() << "Verificando victoria... Enemigos:" << enemigos.size() << " Aliados:" << aliados.size();

    int enemigosIniciales = enemigos.size();
    int aliadosIniciales = aliados.size();

    bool enemigosMuertos = std::all_of(enemigos.begin(), enemigos.end(), [](Personaje* p) {
        return p->getVidaActual() <= 0;
    });

    bool aliadosMuertos = std::all_of(aliados.begin(), aliados.end(), [](Personaje* p) {
        return p->getVidaActual() <= 0;
    });

    if (enemigosMuertos && enemigosIniciales > 0) {
        QMessageBox::information(this, "¡Victoria!", "¡Has derrotado a todos los enemigos!");
        setBotonesHabilitados(false);
        lblSeleccion->hide();
        faseActual = ESPERA;
    } else if (aliadosMuertos && aliadosIniciales > 0) {
        QMessageBox::critical(this, "Derrota", "Todos tus personajes han sido derrotados...");
        setBotonesHabilitados(false);
        lblSeleccion->hide();
        faseActual = ESPERA;
    }


}




