#include "BattleWidget.h"
#include "personaje.h"
#include "gamewindow.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRandomGenerator64>
#include <QMouseEvent>
#include <QMessageBox>
#include "grafomapavisual.h"

extern Inventario* inventarioGlobal;



BattleWidget::BattleWidget(const QString& lugar, const QString& enemigo, QVector<Personaje*>& aliadosExistentes, bool recompensaEspecial,QWidget* parent)
    : QWidget(parent),
    aliados(aliadosExistentes),
    tieneRecompensaEspecial(recompensaEspecial)
{
    setFixedSize(700, 700);
    enemigoActual = enemigo;


    crearInterfaz();
    //CargarAliados();
    MostrarAliados();
    CargarEnemigos(enemigo);
    showFondo(lugar);

    if (enemigoActual.toLower().contains("slime")) {
        QVector<Personaje*> soloCaballero;
        for (Personaje* p : aliados) {
            if (p->getNombre().contains("Caballero", Qt::CaseInsensitive)) {
                soloCaballero.append(p);
                break;
            }
        }
        aliados = soloCaballero;
    }


    //CargarEnemigos();
    //CargarEnemigos("slime");
    //CargarEnemigos("ogros normales");
    //CargarEnemigos("elite");
    //CargarEnemigos("rider");
    //CargarEnemigos("armored");
    //showFondo("Desierto");

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
    //for (Personaje* p : aliados) delete p; -> YA NO BORRAR ALIADOS
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

void BattleWidget::showFondo(const QString& ruta) {
    if(ruta.contains("Isla")){
        setFondo("Personajes/MapasCombate/BosqueJS.png");
    }
    if(ruta.contains("Bosque")){
        setFondo("Personajes/MapasCombate/BosqueJS.png");
    }
    if(ruta.contains("Desierto")){
        setFondo("Personajes/MapasCombate/Desierto.png");
    }
    if(ruta.contains("Castillo Java")){
        setFondo("Personajes/MapasCombate/CastilloJava.png");
    }
    if(ruta.contains("Castillo Codigo")){
        setFondo("Personajes/MapasCombate/CastilloCodigo.png");
    }
    if(ruta.contains("Subterraneo")){
        setFondo("Personajes/MapasCombate/Sub.png");
    }
    if(ruta.contains("Ruinas")){
        setFondo("Personajes/MapasCombate/Ruinas.png");
    }
    //else{
      //  setFondo("Personajes/MapasCombate/BosqueJS.png");
        //}
    qDebug() << ruta;

}


void BattleWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawPixmap(rect(), fondo);

    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);

    //Aliados
    for (Personaje* p : aliados) {
        QPixmap sprite = p->getFrameActual();
        QRect pos = p->getPosicion();
        //qDebug() << p->getNombre() << " → Frame size:" << sprite.size() << " Pos:" << pos;


        // Dibujar sprite
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

        for (EfectoVisual* efecto : efectosVisuales) {
            if (!efecto->sprite.isNull()) {
                painter.drawPixmap(efecto->posicion, efecto->sprite);
            }
        }

    }

    //Enemigos
    for (Personaje* e : enemigos) {
        QPixmap spriteOriginal = e->getFrameActual();
        QPixmap sprite = spriteOriginal.transformed(QTransform().scale(-1, 1));

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
            QRect marco = pos.adjusted(-4, -4, 4, 4);
            painter.setPen(QPen(Qt::red, 3));
            painter.drawRect(marco);
        }
    }
}


void BattleWidget::crearInterfaz() {
    btnAtacar = new QPushButton("Atacar", this);
    btnEspecial = new QPushButton("Ataque Especial", this);
    btnInventario = new QPushButton("Inventario", this);
    btnHuir = new QPushButton("Regresar", this);

    lblSeleccion = new QLabel(this);
    lblSeleccion->setGeometry(20, 25, 250, 50);
    lblSeleccion->setText("Elige acción para...");
    lblSeleccion->setAlignment(Qt::AlignCenter);
    QFont font = lblSeleccion->font();
    font.setPointSize(12);
    lblSeleccion->setFont(font);

    //WIDGET STATS ALIADO
    contenedorStatsAliado = new QWidget(this);
    contenedorStatsAliado->setGeometry(350, 10, 250, 100);
    contenedorStatsAliado->setStyleSheet("background-color: #2196F3; border: 2px solid white;");
    lblImagenAliado = new QLabel(contenedorStatsAliado);
    lblImagenAliado->setGeometry(10, 15, 64, 64);
    lblImagenAliado->setStyleSheet("background-color: white;");
    lblTextoStats = new QLabel(contenedorStatsAliado);
    lblTextoStats->setGeometry(84, 6, 150, 85);
    lblTextoStats->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    lblTextoStats->setWordWrap(true);

    //WIDGET ENEMIGO
    contenedorStatsEnemigo = new QWidget(this);
    contenedorStatsEnemigo->setGeometry(350, 115, 250, 90);
    contenedorStatsEnemigo->setStyleSheet("background-color: #B71C1C; border: 2px solid white;");
    contenedorStatsEnemigo->setVisible(false);
    lblImagenEnemigo = new QLabel(contenedorStatsEnemigo);
    lblImagenEnemigo->setGeometry(10, 10, 64, 64);
    lblImagenEnemigo->setStyleSheet("background-color: white;");
    lblTextoStatsEnemigo = new QLabel(contenedorStatsEnemigo);
    lblTextoStatsEnemigo->setGeometry(84, 7, 150, 75);
    lblTextoStatsEnemigo->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    lblTextoStatsEnemigo->setWordWrap(true);
    lblTextoStatsEnemigo->setStyleSheet("color: white;");


    QFont fontStats;
    fontStats.setPointSize(9);
    lblTextoStats->setFont(fontStats);


    btnAtacar->setGeometry(10, 620, 150, 40);
    btnEspecial->setGeometry(170, 620, 150, 40);
    btnInventario->setGeometry(330, 620, 150, 40);
    btnHuir->setGeometry(490, 620, 150, 40);

    connect(btnAtacar, &QPushButton::clicked, this, [=]() {
        accionSeleccionada("atacar");
    });
    connect(btnEspecial, &QPushButton::clicked, this, [=]() {
        accionSeleccionada("especial");
    });
    connect(btnInventario, &QPushButton::clicked, this, [=]() {
        accionSeleccionada("inventario");
    });

    connect(btnHuir, &QPushButton::clicked, this, [=]() {
        connect(btnHuir, &QPushButton::clicked, this, [=]() {
            if (indiceAliado > 0) {
                indiceAliado--;
                // Remueve la última acción añadida (si ya se eligió acción)
                if (!accionesAliados.isEmpty())
                    accionesAliados.removeLast();

                Personaje* actual = aliados[indiceAliado];
                lblSeleccion->show();
                lblSeleccion->setText("¿Qué hará " + actual->getNombre() + "?");
                actualizarColorBotones(actual->getNombre());
                contenedorStatsAliado->show();

                qDebug() << "Regresaste al turno de:" << actual->getNombre();
            } else {
                qDebug() << "Ya estás en el primer aliado. No puedes retroceder más.";
            }
        });
    });
}

void BattleWidget::CargarAliados() {
    Curandera* c = new Curandera();
    Caballero* k = new Caballero();
    Princesa* p = new Princesa();
    aliados = { c, k, p };
}

void BattleWidget::MostrarAliados() {
    int startY = 300;
    for (int i = 0; i < aliados.size(); ++i) {
        QRect pos(50, startY + i * 100, 64, 64);
        aliados[i]->setPosicion(pos);
    }
}

void BattleWidget::MostrarEnemigos() {
    int startY;
    if(enemigos.size()==1)
        startY=400;
    else if(enemigos.size()==2)
        startY=350;
    else
    startY = 300;

    for (int i = 0; i < enemigos.size(); ++i) {
        qDebug() << enemigos.size();
        QRect pos(540, startY + i * 100, 64, 64);
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

}
void BattleWidget::CargarEnemigos(const QString& tipo) {
    enemigos.clear();

    if (tipo == "slime") {
        enemigos.append(new Slime());

    } else if (tipo == "ogros normales") {
        for (int i = 0; i < 3; ++i)
        enemigos.append(new Ogro(Ogro::Normal));

    } else if (tipo == "rider") {
        enemigos.append(new Ogro(Ogro::Rider));
        enemigos.append(new Ogro(Ogro::Rider));
        //enemigos.append(new Ogro(Ogro::Normal));

    } else if (tipo == "armored") {
        enemigos.append(new Ogro(Ogro::Armored));
        enemigos.append(new Ogro(Ogro::Armored));
        //enemigos.append(new Ogro(Ogro::Normal));

    } else if (tipo == "elite") {
        //enemigos.append(new Ogro(Ogro::Normal));
        enemigos.append(new Ogro(Ogro::Elite));
        //enemigos.append(new Ogro(Ogro::Normal));

    } else if (tipo == "ENormal") {
        for (int i = 0; i < 3; ++i)
            enemigos.append(new Esqueleto(Esqueleto::ENormal));

    } else if (tipo == "ESword") {
        enemigos.append(new Esqueleto(Esqueleto::ESword));
        enemigos.append(new Esqueleto(Esqueleto::ESword));

    } else if (tipo == "EArmored") {
        enemigos.append(new Esqueleto(Esqueleto::EArmored));
        enemigos.append(new Esqueleto(Esqueleto::EArmored));
        enemigos.append(new Esqueleto(Esqueleto::ENormal));

    } else if (tipo == "WereWolf") {
        enemigos.append(new Were(Were::WereWolf));
        enemigos.append(new Were(Were::WereWolf));

    } else if (tipo == "WereBear") {
        enemigos.append(new Were(Were::WereBear));
    }

    MostrarEnemigos();
}





void BattleWidget::accionSeleccionada(QString tipo) {
    if (faseActual != PLANIFICAR || indiceAliado >= aliados.size()) return;

    // los muertos no actúan
    if (aliados[indiceAliado]->getVidaActual() == 0) {
        indiceAliado++;
        if (indiceAliado < aliados.size()) {
            lblSeleccion->setText("¿Qué hará " + aliados[indiceAliado]->getNombre() + "?");
            actualizarColorBotones(aliados[indiceAliado]->getNombre());
        } else {
            faseActual = EJECUTAR;
            lblSeleccion->hide();
            contenedorStatsAliado->hide();
            ejecutarAccionesAliadas();
        }
        return;
    }

    Personaje* personajeActual = aliados[indiceAliado];
    actualizarColorBotones(personajeActual->getNombre());

    // Caso especial: Inventario
    if (tipo == "inventario") {
        setBotonesHabilitados(false);

        personajeActual->abrirInventario(inventarioGlobal);

        // Esperar a que se use un objeto
          connect(personajeActual, &Personaje::objetoUsadoEnInventario, this, [=](const Objeto& obj) {
            // Registrar la acción
            accionesAliados.append({ personajeActual, "inventario", nullptr });

            // Avanzar al siguiente aliado
            indiceAliado++;

            if (indiceAliado < aliados.size()) {
                lblSeleccion->show();
                lblSeleccion->setText("¿Qué hará " + aliados[indiceAliado]->getNombre() + "?");
                actualizarColorBotones(aliados[indiceAliado]->getNombre());
                setBotonesHabilitados(true);
            } else {
                lblSeleccion->hide();
                faseActual = EJECUTAR;
                ejecutarAccionesAliadas();
            }
        });

        return; // Detener aquí hasta que se use un objeto
    }


    // Elegir objetivo (si no hay, al primero)
    Personaje* objetivo = (enemigoSeleccionado && enemigos.contains(enemigoSeleccionado))
                          ? enemigoSeleccionado
                          : (!enemigos.isEmpty() ? enemigos[0] : nullptr);

    // Guardamos acción
    accionesAliados.append({ personajeActual, tipo, objetivo });

    enemigoSeleccionado = nullptr;
    contenedorStatsEnemigo->setVisible(false);
    indiceAliado++;

    if (indiceAliado < aliados.size()) {
        actualizarColorBotones(aliados[indiceAliado]->getNombre());
        lblSeleccion->setText("¿Qué hará " + aliados[indiceAliado]->getNombre() + "?");
    } else {
        faseActual = EJECUTAR;
        lblSeleccion->hide();
        contenedorStatsAliado->hide();
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
        color = "#607D8B"; // Gris

    QString estiloBoton = QString("background-color: %1; color: white; font-weight: bold;").arg(color);
    btnAtacar->setStyleSheet(estiloBoton);
    btnEspecial->setStyleSheet(estiloBoton);
    btnInventario->setStyleSheet(estiloBoton);
    btnHuir->setStyleSheet(estiloBoton);

    QString estiloLabel = QString("background-color: %1; color: white; font-weight: bold; padding: 4px;").arg(color);
    lblSeleccion->setStyleSheet(estiloLabel);

    // Mostrar estadísticas del aliado actual
    for (Personaje* p : aliados) {
        if (p->getNombre() == nombrePersonaje) {
            QString stats = QString("Nombre: %1\nAtaque: %2\nVida: %3/%4\nManá: %5/%6 \nNivel: %7")
                                .arg(p->getNombre())
                                .arg(p->getAtaque())
                                .arg(p->getVidaActual())
                                .arg(p->getVidaMax())
                                .arg(p->getManaActual())
                                .arg(p->getManaMax())
                                .arg(p->getNivel());
            lblTextoStats->setText(stats);

            QPixmap sprite = p->getFrameActual();
            if (!sprite.isNull()) {
                lblImagenAliado->setPixmap(sprite.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                lblImagenAliado->clear();
            }

            break;
        }
    }
}





void BattleWidget::ejecutarAccionesAliadas() {
    setBotonesHabilitados(false);
    enemigoSeleccionado = nullptr;
    contenedorStatsEnemigo->setVisible(false);
    if (accionesAliados.isEmpty()) return;

    int* index = new int(0);
    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        if (*index >= accionesAliados.size()) {
            timer->stop();
            delete index;
            accionesAliados.clear();
            eliminarMuertos();
            faseActual = ENEMIGOS;
            enemigoSeleccionado = nullptr;
            contenedorStatsEnemigo->setVisible(false);
            ejecutarTurnoEnemigos();
            return;
        }

        AccionPlanificada accion = accionesAliados[*index];

        // Si el actor murió antes de actuar, omitirlo
        if (accion.actor->getVidaActual() == 0) {
            (*index)++;
            eliminarMuertos(); // ← aseguramos limpieza de cadáveres
            return;
        }


        if (accion.tipo == "atacar") {
            Personaje* objetivo = accion.objetivo;
            if (!objetivo || objetivo->getVidaActual() == 0)
                objetivo = obtenerObjetivoVivo(true);

            if (objetivo) {
                timer->stop(); // detener temporizador antes del movimiento

                CaminarParaAtacar(accion.actor, objetivo, [=]() mutable {
                    eliminarMuertos(); // ← después del daño, eliminar si muere
                    update();
                    (*index)++;
                    timer->start(100);
                });

                return;
            } else {
                (*index)++;
                eliminarMuertos();
            }
        }


        else if (accion.tipo == "especial") {
            std::vector<Personaje*> aliadosVector(aliados.begin(), aliados.end());
            std::vector<Personaje*> enemigosVector(enemigos.begin(), enemigos.end());

            if (accion.actor->getNombre().contains("Caballero", Qt::CaseInsensitive) ||
                accion.actor->getNombre().contains("Princesa", Qt::CaseInsensitive)) {

                Personaje* objetivo = obtenerObjetivoVivo(true);
                if (objetivo) {
                    timer->stop();

                    CaminarParaAtacar(accion.actor, objetivo, [=]() mutable {
                        accion.actor->ataqueEspecial(aliadosVector, enemigosVector);
                        eliminarMuertos(); // ← luego del especial
                        update();
                        (*index)++;
                        timer->start(100);
                    });

                    return;
                } else {
                    (*index)++;
                    eliminarMuertos();
                }
            }

            else if (accion.actor->getNombre().contains("Curandera", Qt::CaseInsensitive)) {
                // Curandera no se mueve, solo cura
                accion.actor->ataqueEspecial(aliadosVector, enemigosVector);
                accion.actor->setEstado("especial");

                for (Personaje* aliado : aliados) {
                    if (aliado->getVidaActual() > 0 && aliado->esAliadoPersonaje()) {
                        mostrarEfectoCuracion(aliado);
                    }
                }

                eliminarMuertos();
                update();
                (*index)++;
                return;
            }

            // Para otros especiales que no caminan
            eliminarMuertos();
            update();
            (*index)++;
        }

        else if (accion.tipo == "inventario") {
            qDebug() << accion.actor->getNombre() << " usó un objeto del inventario.";
            eliminarMuertos();
            update();
            (*index)++;
            return;
        }
    });

    timer->start(100);
}



void BattleWidget::ejecutarTurnoEnemigos() {
    eliminarMuertos();
    int* index = new int(0);
    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        if (*index >= enemigos.size()) {
            timer->stop();
            delete index;
            faseActual = PLANIFICAR;
            enemigoSeleccionado = nullptr;
            contenedorStatsEnemigo->setVisible(false);
            indiceAliado = 0;
            setBotonesHabilitados(true);
            eliminarMuertos();
            verificarVictoria();

            if (!aliados.isEmpty()) {
                lblSeleccion->show();
                contenedorStatsAliado->show();
                lblSeleccion->setText("¿Qué hará " + aliados[0]->getNombre() + "?");
                actualizarColorBotones(aliados[0]->getNombre());
            }

            return;
        }

        Personaje* enemigo = enemigos[*index];

        if (enemigo->getVidaActual() == 0) {
            (*index)++;
            eliminarMuertos();
            return;
        }

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
    for (Personaje* p : aliados + enemigos) {
        if (p->getVidaActual() == 0 && p->getEstadoActual() != "death" && !p->estaMuriendo()) {
            p->marcarMuriendo();
            p->setEstado("death");

            connect(p, &Personaje::animacionTerminada, this, [=](QString estado) {
                if (estado == "death") {
                    aliados.removeOne(p);
                    enemigos.removeOne(p);
                    p->deleteLater();
                    update();
                }
            });
        }
    }
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
    bool enemigoDetectado = false;

    for (Personaje* enemigo : enemigos) {
        if (enemigo->getPosicion().contains(clickPos)) {
            enemigoSeleccionado = enemigo;
            enemigoDetectado = true;
            qDebug() << "Enemigo seleccionado:" << enemigoSeleccionado->getNombre() << enemigoSeleccionado->getVidaActual();

            // Actualizar imagen
            QPixmap foto = enemigo->getFrameActual().scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            lblImagenEnemigo->setPixmap(foto);

            // Actualizar texto
            QString texto = QString("%1\nVida: %2/%3\nAtaque: %4")
                                .arg(enemigo->getNombre())
                                .arg(enemigo->getVidaActual())
                                .arg(enemigo->getVidaMax())
                                .arg(enemigo->getAtaque());

            lblTextoStatsEnemigo->setText(texto);

            contenedorStatsEnemigo->setVisible(true);
            update();
            break;
        }
    }

    if (!enemigoDetectado) {
        enemigoSeleccionado = nullptr;
        contenedorStatsEnemigo->setVisible(false);
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
        PostBatalla(true);  // Ganamos
    } else if (aliadosDerrotados) {
        PostBatalla(false); // Perdimos
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

            // Aquí decidimos si esquiva
            if (objetivo->recibirDanio(atacante->getAtaque())) {
                animacionEsquivar(objetivo, [=]() {
                    QTimer::singleShot(600, this, [=]() {
                        CaminarDeVuelta(atacante, onFinalizado);
                    });
                });
            } else {
                eliminarMuertos();
                update();

                QTimer::singleShot(800, this, [=]() {
                    CaminarDeVuelta(atacante, onFinalizado);
                });
            }

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

void BattleWidget::mostrarEfectoCuracion(Personaje* personaje) {
    QList<QPixmap> frames = Aliado::cargarDesdeSpritesheet("Personajes/Aliados/Curandera/Priest-Heal_Effect.png", 100, 100, 4);

    if (frames.isEmpty()) return;

    QLabel* efecto = new QLabel(this);
    efecto->setPixmap(frames[0]);
    efecto->setGeometry(personaje->getPosicion().adjusted(10, 10, 20, 20));
    efecto->show();

    // Eliminar 1s
    QTimer::singleShot(1000, efecto, &QLabel::deleteLater);
}



void BattleWidget::animacionEsquivar(Personaje* personaje, std::function<void()> onFinalizado) {
    if (!personaje) {
        if (onFinalizado) onFinalizado();
        return;
    }

    QRect origen = personaje->getPosicion();

    // Movimiento hacia atrás: -20 si aliado, +20 si enemigo
    int desplazamiento = personaje->esAliadoPersonaje() ? -20 : +20;
    QRect rectEsquivar(origen.x() + desplazamiento, origen.y(), origen.width(), origen.height());

    personaje->setEstado("walk");

    // Primer movimiento: hacia atrás
    QTimer* moverAtras = new QTimer(this);
    connect(moverAtras, &QTimer::timeout, this, [=]() mutable {
        QRect actual = personaje->getPosicion();
        int dx = (rectEsquivar.x() - actual.x()) / 4;
        int dy = (rectEsquivar.y() - actual.y()) / 4;

        if (abs(dx) < 2 && abs(dy) < 2) {
            personaje->setPosicion(rectEsquivar);
            moverAtras->stop();

            // Pausa breve al esquivar
            QTimer::singleShot(300, this, [=]() {
                // Volver a la posición original
                QTimer* moverVuelta = new QTimer(this);
                connect(moverVuelta, &QTimer::timeout, this, [=]() mutable {
                    QRect actual = personaje->getPosicion();
                    int dx = (origen.x() - actual.x()) / 4;
                    int dy = (origen.y() - actual.y()) / 4;

                    if (abs(dx) < 2 && abs(dy) < 2) {
                        personaje->setPosicion(origen);
                        moverVuelta->stop();
                        personaje->setEstado("idle");
                        if (onFinalizado) onFinalizado();
                    } else {
                        QRect nuevo(actual.x() + dx, actual.y() + dy, actual.width(), actual.height());
                        personaje->setPosicion(nuevo);
                    }

                    update();
                });

                moverVuelta->start(40);
            });

        } else {
            QRect nuevo(actual.x() + dx, actual.y() + dy, actual.width(), actual.height());
            personaje->setPosicion(nuevo);
        }

        update();
    });

    moverAtras->start(40);
}


void BattleWidget::PostBatalla(bool victoria) {
    setBotonesHabilitados(false);
    lblSeleccion->hide();
    faseActual = ESPERA;

    if (victoria) {
        int experienciaGanada = QRandomGenerator::global()->bounded(5, 10);
        for (Personaje* p : aliados) {
            if (p->esAliadoPersonaje()) {
                Aliado* aliado = dynamic_cast<Aliado*>(p);
                if (aliado)
                    aliado->ganarExperiencia(experienciaGanada);
            }
        }
    }

    QWidget* panelVictoria = new QWidget(this);
    panelVictoria->setGeometry((width() - 300) / 2, (height() - 200) / 2, 300, 200);
    panelVictoria->setStyleSheet("background-color: green; border: 3px solid black; border-radius: 15px;");

    QLabel* lblVictoria = new QLabel("¡Victoria!", panelVictoria);
    lblVictoria->setGeometry(20, 50, 260, 60);
    lblVictoria->setAlignment(Qt::AlignCenter);
    lblVictoria->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");

    QPushButton* btnContinuar = new QPushButton("Continuar", panelVictoria);
    btnContinuar->setGeometry(100, 130, 100, 30);
    btnContinuar->setStyleSheet("color: black; font-size: 15px; font-weight: bold;");

    panelVictoria->show();

    connect(btnContinuar, &QPushButton::clicked, this, [=]() {
        panelVictoria->deleteLater();

        // Mostrar panel de monedas
        QWidget* panelMonedas = new QWidget(this);
        panelMonedas->setGeometry((width() - 300) / 2, (height() - 200) / 2, 300, 200);
        panelMonedas->setStyleSheet("background-color: white; border: 3px solid black; border-radius: 15px;");

        int monedas = QRandomGenerator::global()->bounded(80, 101);
        inventarioGlobal->agregarDinero(monedas);

        QLabel* lblMonedas = new QLabel("¡Ganaste " + QString::number(monedas) + " monedas!", panelMonedas);
        lblMonedas->setGeometry(20, 50, 260, 30);
        lblMonedas->setAlignment(Qt::AlignCenter);
        lblMonedas->setStyleSheet("color: #FFD700; font-size: 18px; font-weight: bold;");

        QLabel* iconoMoneda = new QLabel(panelMonedas);
        iconoMoneda->setGeometry(130, 90, 40, 40);
        QPixmap moneda("Personajes/Items/Golden Coin.png");
        iconoMoneda->setPixmap(moneda.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        QPushButton* btnSiguiente = new QPushButton("Continuar", panelMonedas);
        btnSiguiente->setGeometry(100, 140, 100, 30);
        btnSiguiente->setStyleSheet("color: black; font-size: 15px; font-weight: bold;");

        panelMonedas->show();

        connect(btnSiguiente, &QPushButton::clicked, this, [=]() {
            panelMonedas->deleteLater();

            // ¿Hay núcleo?
            if (tieneRecompensaEspecial) {
                QWidget* panelNucleo = new QWidget(this);
                panelNucleo->setGeometry((width() - 300) / 2, (height() - 200) / 2, 300, 200);
                panelNucleo->setStyleSheet("background-color: #1B5E20; border: 3px solid black; border-radius: 15px;");

                QLabel* lblNucleo = new QLabel("¡Obtuviste un Núcleo!", panelNucleo);
                lblNucleo->setGeometry(20, 50, 260, 30);
                lblNucleo->setAlignment(Qt::AlignCenter);
                lblNucleo->setStyleSheet("color: green; font-size: 18px; font-weight: bold;");
                inventarioGlobal->agregarNucleo();

                QLabel* iconoNucleo = new QLabel(panelNucleo);
                iconoNucleo->setGeometry(130, 90, 40, 40);
                QPixmap nucleo("Personajes/Items/Emerald.png");
                iconoNucleo->setPixmap(nucleo.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));

                QPushButton* btnFinal = new QPushButton("Salir", panelNucleo);
                btnFinal->setGeometry(100, 140, 100, 30);
                btnFinal->setStyleSheet("color: black; font-size: 15px; font-weight: bold;");

                connect(btnFinal, &QPushButton::clicked, this, [=]() {
                    panelNucleo->deleteLater();
                    this->close();
                });

                panelNucleo->show();
            } else {
                this->close();
            }
        });
    });

    // DERROTA
    if (!victoria) {
        panelVictoria->deleteLater();

        QWidget* panelDerrota = new QWidget(this);
        panelDerrota->setGeometry((width() - 300) / 2, (height() - 200) / 2, 300, 200);
        panelDerrota->setStyleSheet("background-color: red; border: 3px solid black; border-radius: 15px;");

        QLabel* lblDerrota = new QLabel("Derrota...", panelDerrota);
        lblDerrota->setGeometry(20, 50, 260, 60);
        lblDerrota->setAlignment(Qt::AlignCenter);
        lblDerrota->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");

        QPushButton* btnSalir = new QPushButton("Salir", panelDerrota);
        btnSalir->setGeometry(100, 130, 100, 30);
        btnSalir->setStyleSheet("color: black; font-size: 15px; font-weight: bold;");

        connect(btnSalir, &QPushButton::clicked, this, [=]() {
            panelDerrota->deleteLater();
            this->close();
        });

        panelDerrota->show();
    }
}











