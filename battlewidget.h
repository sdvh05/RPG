#ifndef BATTLEWIDGET_H
#define BATTLEWIDGET_H

#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include "Curandera.h"
#include "Caballero.h"
#include "Princesa.h"
#include "Slime.h"
#include "ogro.h"

class BattleWidget : public QWidget {
    Q_OBJECT

    enum FaseTurno {
        PLANIFICAR,
        EJECUTAR,
        ENEMIGOS,
        ESPERA
    };

    struct AccionPlanificada {
        Personaje* actor;
        QString tipo; // atacar, especial, inventario, huir
        Personaje* objetivo;
    };

public:
    explicit BattleWidget(QWidget* parent = nullptr);
    ~BattleWidget();


    FaseTurno faseActual;
    int indiceAliado;
    QVector<AccionPlanificada> accionesAliados;
    void mousePressEvent(QMouseEvent* event) override;



private:
    // Personajes
    QVector<Personaje*> aliados;
    QVector<Personaje*> enemigos;

    // Botones de acción
    QPushButton* btnAtacar;
    QPushButton* btnEspecial;
    QPushButton* btnInventario;
    QPushButton* btnHuir;

    // Fondo y HUD
    QPixmap fondo;
    QTimer* updateTimer;

    void crearInterfaz();
    QLabel* lblSeleccion;
    void CargarAliados();
    void MostrarAliados();

    void CargarEnemigos();//Prueba "vieja"
    void CargarEnemigos(const QString& tipo); //A Futuro

    void MostrarEnemigos();
    void verificarVictoria();


    void accionSeleccionada(QString tipo);
    void ejecutarAccionesAliadas();
    void ejecutarTurnoEnemigos();
    void setBotonesHabilitados(bool habilitado);
    void eliminarMuertos();
    void actualizarColorBotones(const QString& nombrePersonaje);
    Personaje* enemigoSeleccionado = nullptr; // a quien atacar


protected:
    void paintEvent(QPaintEvent* event) override;
};



#endif // BATTLEWIDGET_H
