#include "BattleWidget.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRandomGenerator64>
#include <QMouseEvent>
#include <QMessageBox>



BattleWidget::BattleWidget(QWidget* parent)
    : QWidget(parent), /*fondo("Personajes/pruebaF.jpeg")*/fondo("Personajes/MapasCombate/BosqueJS.png")
{
    setFixedSize(800, 480);

    crearInterfaz();
    CargarAliados();
    MostrarAliados();



    //CargarEnemigos();
    //CargarEnemigos("slime");
    CargarEnemigos("ogros normales");
    //CargarEnemigos("elite");
    //CargarEnemigos("rider");
    //CargarEnemigos("armored");

    //setFondo("Personajes/MapasCombate/BosqueJS.png");



    //MostrarEnemigos();

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

void BattleWidget::setFondo(const QString& ruta) {
    QPixmap nuevoFondo(ruta);
    if (nuevoFondo.isNull()) {
        qWarning() << "No se pudo cargar el fondo:" << ruta;
        return;
    }
    fondo = nuevoFondo;
    update();
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
        painter.drawText(pos.x(), pos.y() - 22, p->getNombre());

        // Barra de vida
        int barWidth = pos.width();
        QRect vidaRect(pos.x(), pos.y() - 15, barWidth, 5);
        painter.setBrush(Qt::darkRed);
        painter.drawRect(vidaRect);

        float vidaRatio = (float)p->getVidaActual() / p->getVidaMax();
        QRect vidaActualRect(pos.x(), pos.y() - 15, barWidth * vidaRatio, 5);
        painter.setBrush(Qt::green);
        painter.drawRect(vidaActualRect);

        //MANA
        int manaMax = p->getManaMax();
        int manaActual = p->getManaActual();
        float manaRatio = (float)manaActual / manaMax;

        QRect manaRect(pos.x(), pos.y() - 8, barWidth, 4);
        painter.setBrush(Qt::blue);
        painter.drawRect(manaRect.adjusted(0, 0, -barWidth * (1 - manaRatio), 0));
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
        painter.drawText(pos.x(), pos.y() - 22, e->getNombre());

        // Barra de vida
        int barWidth = pos.width();
        QRect vidaRect(pos.x(), pos.y() - 15, barWidth, 5);
        painter.setBrush(Qt::darkRed);
        painter.drawRect(vidaRect);

        float vidaRatio = (float)e->getVidaActual() / e->getVidaMax();
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

void BattleWidget::MostrarEnemigos() {
    int startY = 100;
    for (int i = 0; i < enemigos.size(); ++i) {
        QRect pos(640, startY + i * 100, 64, 64);
        enemigos[i]->setPosicion(pos);
    }
}

//AQUI ESTA LA MAGIA --- Hacer MULTIPLES EVENTOS acorde a un parametro QString/ int futuro -- o una variable global, como sea
void BattleWidget::CargarEnemigos() {
    enemigos.clear();

    int cantidad = QRandomGenerator::global()->bounded(1, 4); // 1 a 3 enemigos

    //SLIMES
    for (int i = 0; i < cantidad; ++i) {
        Slime* s = new Slime();
        enemigos.append(s);
    }

    //Ogros
}
void BattleWidget::CargarEnemigos(const QString& tipo) {
    enemigos.clear();

    if (tipo == "slime") {
        int cantidad = QRandomGenerator::global()->bounded(1, 4);
        for (int i = 0; i < cantidad; ++i)
            enemigos.append(new Slime());

    } else if (tipo == "ogros normales") {
        for (int i = 0; i < 3; ++i)
            enemigos.append(new Ogro(Ogro::Normal));

    } else if (tipo == "rider") {
        enemigos.append(new Ogro(Ogro::Rider));
        enemigos.append(new Ogro(Ogro::Rider));
         enemigos.append(new Ogro(Ogro::Normal));

    } else if (tipo == "armored") {
        enemigos.append(new Ogro(Ogro::Armored));
        enemigos.append(new Ogro(Ogro::Armored));
         enemigos.append(new Ogro(Ogro::Normal));

    } else if (tipo == "elite") {
        enemigos.append(new Ogro(Ogro::Normal));
        enemigos.append(new Ogro(Ogro::Elite));
        enemigos.append(new Ogro(Ogro::Normal));

    }

    MostrarEnemigos();
}




void BattleWidget::accionSeleccionada(QString tipo) {
    if (faseActual != PLANIFICAR || indiceAliado >= aliados.size()) return;

    // los muertos no actuan
    if (aliados[indiceAliado]->getVidaActual() == 0) {
        indiceAliado++;
        if (indiceAliado < aliados.size()) {
            lblSeleccion->setText("¿Qué hará " + aliados[indiceAliado]->getNombre() + "?");
            actualizarColorBotones(aliados[indiceAliado]->getNombre());
        } else {
            faseActual = EJECUTAR;
            lblSeleccion->hide();
            ejecutarAccionesAliadas();
        }
        return;
    }

    Personaje* personajeActual = aliados[indiceAliado];
    actualizarColorBotones(personajeActual->getNombre());

    //Elegir objetivo: sino se elige ataca al de mas arriba
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
    if (accionesAliados.isEmpty()) return;

    int* index = new int(0);
    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        if (*index >= accionesAliados.size()) {
            timer->stop();
            delete index;
            accionesAliados.clear();
            faseActual = ENEMIGOS;
            ejecutarTurnoEnemigos();
            return;
        }

        AccionPlanificada accion = accionesAliados[*index];

        if (accion.actor->getVidaActual() == 0) {
            (*index)++;
            return;
        }

        if (accion.tipo == "atacar") {
            Personaje* objetivo = accion.objetivo;
            if (!objetivo || objetivo->getVidaActual() == 0)
                objetivo = obtenerObjetivoVivo(true);

            if (objetivo) {
                timer->stop();

                CaminarParaAtacar(accion.actor, objetivo, [=]() mutable {
                    eliminarMuertos();
                    update();
                    (*index)++;
                    timer->start(100);
                });

                return;
            } else {
                (*index)++;
            }
        } else if (accion.tipo == "especial") {
            std::vector<Personaje*> aliadosVector(aliados.begin(), aliados.end());
            std::vector<Personaje*> enemigosVector(enemigos.begin(), enemigos.end());
            accion.actor->ataqueEspecial(aliadosVector, enemigosVector);
            accion.actor->setEstado("especial");
            eliminarMuertos();
            update();
            (*index)++;
        }

    });

    timer->start(100);
}


void BattleWidget::ejecutarTurnoEnemigos() {
    int* index = new int(0);
    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        if (*index >= enemigos.size()) {
            timer->stop();
            delete index;
            faseActual = PLANIFICAR;
            indiceAliado = 0;
            setBotonesHabilitados(true);
            eliminarMuertos();
            verificarVictoria();

            if (!aliados.isEmpty()) {
                lblSeleccion->show();
                lblSeleccion->setText("¿Qué hará " + aliados[0]->getNombre() + "?");
                actualizarColorBotones(aliados[0]->getNombre());
            }

            return;
        }

        Personaje* enemigo = enemigos[*index];
        QVector<Personaje*> vivos;
        for (Personaje* a : aliados)
            if (a->getVidaActual() > 0)
                vivos.append(a);

        if (!vivos.isEmpty()) {
            int randIndex = QRandomGenerator::global()->bounded(vivos.size());
            Personaje* objetivo = vivos[randIndex];

            timer->stop();

            CaminarParaAtacar(enemigo, objetivo, [=]() mutable {
                eliminarMuertos();
                update();
                (*index)++;
                timer->start(100);
            });

            return;
        } else {
            (*index)++;
        }
    });

    timer->start(100);
}


Personaje* BattleWidget::obtenerObjetivoVivo(bool esEnemigo) {
    const auto& lista = esEnemigo ? enemigos : aliados;
    for (Personaje* p : lista) {
        if (p->getVidaActual() > 0)
            return p;
    }
    return nullptr;
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

    bool enemigosDerrotados = enemigos.isEmpty() ||
                              std::all_of(enemigos.begin(), enemigos.end(), [](Personaje* p) {
                                  return p->getVidaActual() <= 0;
                              });

    bool aliadosDerrotados = aliados.isEmpty() ||
                             std::all_of(aliados.begin(), aliados.end(), [](Personaje* p) {
                                 return p->getVidaActual() <= 0;
                             });

    if (enemigosDerrotados) {
        QMessageBox::information(this, "¡Victoria!", "¡Has derrotado a todos los enemigos!");
        setBotonesHabilitados(false);
        lblSeleccion->hide();
        faseActual = ESPERA;
    } else if (aliadosDerrotados) {
        QMessageBox::critical(this, "Derrota", "Todos tus personajes han sido derrotados...");
        setBotonesHabilitados(false);
        lblSeleccion->hide();
        faseActual = ESPERA;
    }

}


void BattleWidget::CaminarParaAtacar(Personaje* atacante, Personaje* objetivo, std::function<void()> onFinalizado) {
    if (!atacante || !objetivo) {
        if (onFinalizado) onFinalizado();
        return;
    }

    QPoint origen = atacante->getPosicion().topLeft();
    QPoint destino = objetivo->getPosicion().topLeft();

    int offset = atacante->esAliadoPersonaje() ? +60 : -60;
    QPoint puntoObjetivo(destino.x() - offset, destino.y());

    QRect rectDestino(puntoObjetivo, atacante->getPosicion().size());

    atacante->setEstado("walk");

    QTimer* mover = new QTimer(this);
    connect(mover, &QTimer::timeout, this, [=]() mutable {
        QRect actual = atacante->getPosicion();
        int dx = (rectDestino.x() - actual.x()) / 5;
        int dy = (rectDestino.y() - actual.y()) / 5;

        if (abs(dx) < 2 && abs(dy) < 2) {
            atacante->setPosicion(rectDestino);
            mover->stop();

            atacante->setEstado("attack");
            objetivo->recibirDanio(atacante->getAtaque());
            update();

            QTimer::singleShot(400, this, [=]() {
                CaminarDeVuelta(atacante, onFinalizado);
            });

        } else {
            QRect nuevo(actual.x() + dx, actual.y() + dy, actual.width(), actual.height());
            atacante->setPosicion(nuevo);
        }

        update();
    });

    mover->start(40);
}

void BattleWidget::CaminarDeVuelta(Personaje* personaje, std::function<void()> onFinalizado) {
    personaje->setEstado("walk");
    QRect destino = personaje->getPosicionOriginal();
    QTimer* mover = new QTimer(this);

    connect(mover, &QTimer::timeout, this, [=]() mutable {
        QRect actual = personaje->getPosicion();
        int dx = (destino.x() - actual.x()) / 5;
        int dy = (destino.y() - actual.y()) / 5;

        if (abs(dx) < 2 && abs(dy) < 2) {
            personaje->setPosicion(destino);
            mover->stop();
            personaje->setEstado("idle");
            if (onFinalizado) onFinalizado();
        } else {
            QRect nuevo(actual.x() + dx, actual.y() + dy, actual.width(), actual.height());
            personaje->setPosicion(nuevo);
        }

        update();
    });

    mover->start(40);
}










