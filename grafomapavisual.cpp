#include "grafomapavisual.h"
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>

MapaWidget::MapaWidget(QWidget *parent) : QWidget(parent), zonaActual("")
{
    setFixedSize(800, 600);

    if(!cargarFondo()) {
        fondoMapa = QPixmap(size());
        fondoMapa.fill(Qt::darkGray);
    }

    inicializarGrafo();
    crearInterfaz();
}

void MapaWidget::inicializarGrafo()
{
    // Agregar zonas
    grafo.agregarZona("ISLA");
    grafo.agregarZona("Castillo Java");
    grafo.agregarZona("Desierto");
    grafo.agregarZona("Bosque JS");
    grafo.agregarZona("Castillo Codigo");
    grafo.agregarZona("Ruinas");
    grafo.agregarZona("Subterraneo");

    // Conexiones permanentes
    grafo.agregarConexion("ISLA", "Bosque JS", 40);

    grafo.agregarConexion("Bosque JS", "Desierto", 24);
    grafo.agregarConexion("Desierto", "Bosque JS", 24);

    grafo.agregarConexion("Desierto", "Castillo Java", 1);
    grafo.agregarConexion("Castillo Java", "Desierto", 1);

    grafo.agregarConexion("Ruinas", "Castillo Codigo", 1);
     grafo.agregarConexion("Castillo Codigo", "Ruinas", 1);

    grafo.agregarConexion("Ruinas", "Bosque JS", 22);
    grafo.agregarConexion("Bosque JS", "Ruinas", 22);

    grafo.agregarConexion("Subterraneo", "Castillo Codigo", 27);
    grafo.agregarConexion("Castillo Codigo", "Subterraneo", 27);

    grafo.agregarConexion("Subterraneo", "Bosque JS", 3);
    grafo.agregarConexion("Bosque JS", "Subterraneo", 3);

    grafo.agregarConexion("Subterraneo", "Ruinas", 18);
    grafo.agregarConexion("Ruinans", "Subterraneo", 18);


    // Conexiones condicionales (explorador)
    grafo.agregarConexion("Castillo Java", "Castillo Codigo", 46, false);
    grafo.activarConexionCondicional("Castillo Java", "Castillo Codigo");

}

bool MapaWidget::cargarFondo()
{
    return fondoMapa.load("Personajes/MapasCombate/Mapa.jpeg");
}

void MapaWidget::crearInterfaz()
{
    btnRegresar = new QPushButton("Regresar", this);
    btnRegresar->setGeometry(10, 10, 100, 30);

    lblZonaActual = new QLabel("Selecciona una zona", this);
    lblZonaActual->setGeometry(120, 10, 300, 30);
    lblZonaActual->setStyleSheet("color: white; font-weight: bold;");

    lblConexiones = new QLabel(this);
    lblConexiones->setGeometry(10, 550, 780, 40);
    lblConexiones->setStyleSheet("color: white; background-color: rgba(0,0,0,150);");
    lblConexiones->setAlignment(Qt::AlignCenter);
}

QPoint MapaWidget::obtenerPosicionZona(const QString& zona) const
{
    static const QMap<QString, QPoint> posiciones = {
        {"ISLA", QPoint(380, 150)},
        {"Castillo Java", QPoint(140, 370)},
        {"Desierto", QPoint(130, 490)},
        {"Bosque JS", QPoint(380,420)},
        {"Castillo Codigo", QPoint(660, 370)},
        {"Ruinas", QPoint(660, 490)},
        {"Subterraneo",QPoint(380,520)}
    };
    return posiciones.value(zona, QPoint(50, 50));
}

void MapaWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawPixmap(0, 0, fondoMapa);
    dibujarGrafo(painter);
}

void MapaWidget::dibujarGrafo(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing);

    // Dibujar conexiones primero
    for(const QString& origen : grafo.obtenerZonas()) {
        for(const auto& conexion : grafo.obtenerConexiones(origen)) {
            QString destino = conexion.first;
            int peso = conexion.second;

            QPoint p1 = obtenerPosicionZona(origen);
            QPoint p2 = obtenerPosicionZona(destino);

            // tipo de conexion
            bool esPermanente = grafo.esConexionPermanente(origen, destino);
            bool estaActiva = grafo.esConexionActiva(origen, destino);

            QPen pen;
            if(esPermanente) {
                pen.setColor(Qt::red);  // Conexión permanente roja
                pen.setWidth(2);
            } else if(estaActiva) {
                pen.setColor(Qt::green);  // Conexión condicional activa verde
                pen.setWidth(2);
                pen.setStyle(Qt::DashLine);
            } else {
                continue;  // No dibujar conexiones condicionales inactivas
            }

            painter.setPen(pen);
            painter.drawLine(p1, p2);

            // Dibujar peso
            QPoint centro = (p1 + p2) / 2;
            painter.setPen(Qt::white);
            painter.drawText(centro, QString::number(peso) + " Km");
        }
    }


    // Dibujar zonas
    for(const QString& zona : grafo.obtenerZonas()) {
        QPoint centro = obtenerPosicionZona(zona);

        // Dibujar círculo de zona
        painter.setBrush(zona == zonaActual ? Qt::yellow : Qt::blue);
        painter.setPen(QPen(Qt::white, 2));
        painter.drawEllipse(centro, 20, 20);

        // Dibujar nombre
        painter.setPen(Qt::white);
        painter.drawText(centro.x() - 30, centro.y() - 25, zona);
    }
}

void MapaWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint clickPos = event->pos();

    // Verificar clic en zonas
    for(const QString& zona : grafo.obtenerZonas()) {
        QPoint centro = obtenerPosicionZona(zona);
        if(QVector2D(clickPos - centro).length() <= 20) {
            zonaActual = zona;
            update();

            // Mostrar información de conexiones
            QStringList conexionesInfo;
            for(const auto& conexion : grafo.obtenerConexiones(zona)) {
                conexionesInfo << QString("%1 (distancia: %2)").arg(conexion.first).arg(conexion.second);
            }

            lblZonaActual->setText("Zona: " + zona);
            lblConexiones->setText("Conectado a: " + conexionesInfo.join(" | "));
            break;
        }
    }

    QWidget::mousePressEvent(event);
}
