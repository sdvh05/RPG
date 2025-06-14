#include "Personaje.h"
#include "InventarioWidget.h"
#include <QDebug>
#include <QRandomGenerator>

Personaje::Personaje(QString nombre, int vida, int mana, int ataque, bool aliado)
    : nombre(nombre), vidaMax(vida), vidaActual(vida),
    manaMax(mana), manaActual(mana), ataque(ataque),
    esAliado(aliado), nivel(1), frameActual(0),
    estadoActual("idle"), animTimer(new QTimer(this)) {


    connect(animTimer, &QTimer::timeout, this, &Personaje::actualizarAnimacion);
    animTimer->start(150);
}

Personaje::~Personaje() {
    delete animTimer;
}

QString Personaje::getNombre() const { return nombre; }
int Personaje::getVidaActual() const { return vidaActual; }
int Personaje::getManaActual() const { return manaActual; }
int Personaje::getManaMax()const {return manaMax;}
int Personaje::getNivel() const { return nivel; }
bool Personaje::esAliadoPersonaje() const { return esAliado; }
QString Personaje::getEstadoActual() const {
    return estadoActual;
}



void Personaje::restaurarVida(int cantidad) {
    vidaActual = qMin(vidaMax, vidaActual + cantidad);
}

void Personaje::restaurarMana(int cantidad){
    manaActual = qMin(manaMax, manaActual + cantidad);
}

void Personaje::usarMana(int cantidad) {
    manaActual = qMax(0, manaActual - cantidad);
}

void Personaje::agregarAnimacion(const QString& estado, const QList<QPixmap>& frames, bool ciclica) {
    animaciones[estado] = Animacion{frames, ciclica};
}

void Personaje::setEstado(const QString& estado) {
    if (estadoActual != estado && animaciones.contains(estado)) {
        estadoActual = estado;
        frameActual = 0;
    }
}

int Personaje::getAtaque()const{
    return ataque;
}

int Personaje::getDefensa() const {
    return defensa;
}

void Personaje::aumentarDefensa(int cantidad) {
    defensa += cantidad;
}

void Personaje::resetearDefensa() {
    defensa = 0;
}


bool Personaje::recibirDanio(int cantidad) {
    int chance = QRandomGenerator::global()->bounded(100); // 0-99
    if (chance > 80) {
        qDebug() << nombre << " esquivó el ataque!";
        return true; // esquivó
    }

    int danoReducido = qMax(0, cantidad - defensa);
    vidaActual = qMax(0, vidaActual - danoReducido);

    if (vidaActual == 0)
        setEstado("death");
    else
        setEstado("hurt");

    qDebug() << nombre << " recibió" << danoReducido << "de daño. Vida restante:" << vidaActual;
    return false; // no esquivó
}



void Personaje::ataqueEspecial(std::vector<Personaje*>&, std::vector<Personaje*>&) {
    qDebug() << nombre << " no tiene ataque especial.";
}


void Personaje::actualizarAnimacion() {
    if (!animaciones.contains(estadoActual)) return;

    const Animacion& anim = animaciones[estadoActual];
    frameActual++;

    if (frameActual >= anim.frames.size()) {
        emit animacionTerminada(estadoActual);

        if (anim.ciclica) {
            frameActual = 0;
        } else {
            setEstado("idle");
            return;
        }
    }
}

QPixmap Personaje::getFrameActual() const {
    if (animaciones.contains(estadoActual)) {
        const Animacion& anim = animaciones[estadoActual];
        if (!anim.frames.isEmpty() && frameActual < anim.frames.size()) {
            return anim.frames[frameActual].scaled(
                anim.frames[frameActual].size() * escala,
                Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
    }
    return QPixmap();
}




void Personaje::setPosicion(QRect rect) {
    posicionPantalla = rect;
    if (posicionOriginal.isNull()) {
        posicionOriginal = rect;
    }
}

void Personaje::setPosicionOriginal(QRect rect) {
    posicionOriginal = rect;
}


QRect Personaje::getPosicion() const {
    return posicionPantalla;
}

QList<QPixmap> Personaje::cargarDesdeSpritesheet(const QString& ruta, int frameAncho, int frameAlto, int totalFrames) {
    QList<QPixmap> frames;
    QPixmap sheet(ruta);

    if (sheet.isNull()) {
        qWarning() << "No se pudo cargar el spritesheet:" << ruta;
        return frames;
    }

    for (int i = 0; i < totalFrames; ++i) {
        QPixmap frame = sheet.copy(i * frameAncho, 0, frameAncho, frameAlto);
        QPixmap recortado = recortarContenido(frame);
        frames.append(recortado);
    }

    return frames;
}

QPixmap Personaje::recortarContenido(const QPixmap& original) {
    QImage img = original.toImage();
    int minX = img.width(), minY = img.height();
    int maxX = 0, maxY = 0;

    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            QColor pixel = img.pixelColor(x, y);

            // Considerar como contenido si no es negro ni completamente transparente
            if (pixel.alpha() > 0 && pixel != Qt::black) {
                if (x < minX) minX = x;
                if (y < minY) minY = y;
                if (x > maxX) maxX = x;
                if (y > maxY) maxY = y;
            }
        }
    }

    // Verificación por si no se detectó contenido
    if (minX > maxX || minY > maxY)
        return original;

    QRect recorte(minX, minY, maxX - minX + 1, maxY - minY + 1);
    return original.copy(recorte);
}

void Personaje::abrirInventario(Inventario* inventario) {
    InventarioWidget* widget = new InventarioWidget(inventario);
    widget->show();

    connect(widget, &InventarioWidget::objetoUsado, this, [=](const Objeto& obj){
        if (obj.tipo == "Curacion") {
            if (obj.nombre.contains("Vida")) {
                restaurarVida(obj.valor);
                qDebug() << nombre << "usó" << obj.nombre << "y curó" << obj.valor << "de vida.";
            } else if (obj.nombre.contains("Mana")) {
                restaurarMana(obj.valor);
                qDebug() << nombre << "usó" << obj.nombre << "y recuperó" << obj.valor << "de maná.";
            }
        } else {
            qDebug() << "El objeto" << obj.nombre << "no tiene efecto directo sobre el personaje.";
        }
    });
}

