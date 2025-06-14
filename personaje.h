#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "Inventario.h"
#include "InventarioWidget.h"

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QMap>
#include <QTimer>
#include <QRect>
#include <QList>

class Personaje : public QObject {
    Q_OBJECT

protected:
    QString nombre;
    int vidaMax;
    int vidaActual;
    int manaMax;
    int manaActual;
    int ataque;
    int defensa = 0;
    bool esAliado;
    int nivel;
    bool enAnimacionDeMuerte = false;


    struct Animacion {
        QList<QPixmap> frames;
        bool ciclica;
    };

    QMap<QString, Animacion> animaciones;
    QString estadoActual;
    int frameActual;
    QTimer* animTimer;
    QRect posicionPantalla;
    int escala = 12;
    QRect posicionOriginal;

public:
    Personaje(QString nombre, int vida, int mana, int ataque, bool aliado);
    virtual ~Personaje();

    QString getNombre() const;
    int getAtaque()const;
    int getVidaActual() const;
    int getManaActual() const;
    int getManaMax()const;
    int getNivel() const;
    int getDefensa() const;
    QString getEstadoActual() const;
    void aumentarDefensa(int cantidad);
    void resetearDefensa();
    bool esAliadoPersonaje() const;
    int getVidaMax() const { return vidaMax; }
    QRect getPosicionOriginal() const { return posicionOriginal; }
    bool estaMuriendo() const { return enAnimacionDeMuerte; }
    void marcarMuriendo() { enAnimacionDeMuerte = true; }
    void setPosicionOriginal(QRect rect);



    //void recibirDanio(int cantidad);
    bool recibirDanio(int cantidad);
    virtual void ataqueEspecial(std::vector<Personaje*>& aliados, std::vector<Personaje*>& enemigos);
    void restaurarVida(int cantidad);
    void restaurarMana(int cantidad);
    void usarMana(int cantidad);

    void agregarAnimacion(const QString& estado, const QList<QPixmap>& frames, bool ciclica = true);
    void setEstado(const QString& estado);
    QPixmap getFrameActual() const;
    static QPixmap recortarContenido(const QPixmap& original);


    void setPosicion(QRect rect);
    QRect getPosicion() const;

    static QList<QPixmap> cargarDesdeSpritesheet(const QString& ruta, int frameAncho, int frameAlto, int totalFrames);

    void abrirInventario(Inventario* inventario);


protected slots:
    void actualizarAnimacion();

signals:
    void animacionTerminada(QString estado);
    void esquivarSolicitado(Personaje* personaje, std::function<void()> onFinalizado);
};

#endif // PERSONAJE_H
