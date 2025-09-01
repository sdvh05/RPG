#include "AreaDibujo.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <cmath>

static constexpr double PI_D = 3.14159265358979323846;

AreaDibujo::AreaDibujo(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #565555;");
}

void AreaDibujo::setValores(const QVector<int> &valores) {
    this->valores = valores;
    update();
}

void AreaDibujo::setEstructura(int tipo) {
    estructuraTipo = tipo;
    update();
}


void AreaDibujo::setArbol(NodoArbol* root) {
    raiz = root;
    update();
}



void AreaDibujo::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::white, 2);
    QBrush brush(QColor("#143a64"));
    painter.setPen(pen);
    painter.setBrush(brush);

    int radio = 40;
    int separacion = std::max(100, radio * 2 + 30);
    int n = valores.size();

    // ======== LINKED LIST || QUEUE ========
    if (estructuraTipo == 0 || estructuraTipo == 3) {
        int x = 50;
        int y = height() / 2;

        for (int i = 0; i < n; i++) {
            // Nodo
            painter.setBrush(QColor("#143a64"));
            painter.setPen(Qt::black);
            painter.drawEllipse(QPointF(x, y), radio, radio);
            painter.setPen(Qt::white);
            painter.drawText(QRectF(x - radio, y - radio, radio * 2, radio * 2),
                             Qt::AlignCenter, QString::number(valores[i]));

            // Head/Tail
            if (n == 1) {
                dibujarHeadTail(painter, QPointF(x - 20, y + radio + 20), "Head/0 Tail/0");
            } else if (i == 0) {
                dibujarHeadTail(painter, QPointF(x - 15, y + radio + 20), "Head/0");
            } else if (i == n - 1) {
                dibujarHeadTail(painter, QPointF(x - 15, y + radio + 20), QString("Tail/%1").arg(i));
            }

            // Flecha
            if (i < n) {
                QPointF inicio(x + radio, y);
                QPointF fin(x + separacion - radio, y);
                painter.setPen(Qt::white);
                dibujarFlecha(painter, inicio, fin, false);
            }

            x += separacion;
        }

        // Nodo nullptr
        painter.setBrush(QColor("#9f9681"));
        painter.setPen(Qt::black);
        painter.drawEllipse(QPointF(x, y), radio, radio);
        painter.setPen(Qt::white);
        painter.drawText(QRectF(x - radio, y - radio, radio * 2, radio * 2),
                         Qt::AlignCenter, "nullptr");
    }

    // ======== DOUBLE LINKED LIST ========
    else if (estructuraTipo == 1) {
        int x = 50;
        int y = height() / 2;

        if (!valores.isEmpty()) {
            QPointF inicio0(x - separacion / 2 + radio, y);
            QPointF fin0(x - radio - 10, y);
            painter.setPen(Qt::white);
            dibujarFlecha(painter, inicio0, fin0, false);
        }

        for (int i = 0; i < n; i++) {
            // Nodo
            painter.setBrush(QColor("#143a64"));
            painter.setPen(Qt::black);
            painter.drawEllipse(QPointF(x, y), radio, radio);
            painter.setPen(Qt::white);
            painter.drawText(QRectF(x - radio, y - radio, radio * 2, radio * 2),
                             Qt::AlignCenter, QString::number(valores[i]));

            // Head/Tail
            if (n == 1) {
                dibujarHeadTail(painter, QPointF(x - 20, y + radio + 20), "Head/0 Tail/0");
            } else if (i == 0) {
                dibujarHeadTail(painter, QPointF(x - 15, y + radio + 20), "Head/0");
            } else if (i == n - 1) {
                dibujarHeadTail(painter, QPointF(x - 15, y + radio + 20), QString("Tail/%1").arg(i));
            }

            // Flechas doble
            QPointF inicio(x + radio, y);
            QPointF fin(x + separacion - radio, y);
            painter.setPen(Qt::white);
            dibujarFlecha(painter, inicio, fin, (i < n - 1));

            x += separacion;
        }

        // Nodo nullptr
        painter.setBrush(QColor("#9f9681"));
        painter.setPen(Qt::black);
        painter.drawEllipse(QPointF(x, y), radio, radio);
        painter.setPen(Qt::white);
        painter.drawText(QRectF(x - radio, y - radio, radio * 2, radio * 2),
                         Qt::AlignCenter, "nullptr");
    }

    // ======== STACK ========
    else if (estructuraTipo == 2) {
        int x = width() / 2;
        int y = 50;
        int separacionVertical = std::max(100, radio * 2 + 30);

        for (int i = 0; i < n; i++) {
            // Nodo
            painter.setBrush(QColor("#143a64"));
            painter.setPen(Qt::black);
            painter.drawEllipse(QPointF(x, y), radio, radio);
            painter.setPen(Qt::white);
            painter.drawText(QRectF(x - radio, y - radio, radio * 2, radio * 2),
                             Qt::AlignCenter, QString::number(valores[i]));

            // Head/Tail a la izquierda
            if (n == 1) {
                dibujarHeadTail(painter, QPointF(x - radio - 70, y + 5), "Head/0 Tail/0");
            } else if (i == 0) {
                dibujarHeadTail(painter, QPointF(x - radio - 50, y + 5), "Head/0");
            } else if (i == n - 1) {
                dibujarHeadTail(painter, QPointF(x - radio - 50, y + 5), QString("Tail/%1").arg(i));
            }

            // Flecha hacia abajo
            QPointF inicio(x, y + radio);
            QPointF fin(x, y + separacionVertical - radio);
            painter.setPen(Qt::white);
            dibujarFlecha(painter, inicio, fin, false);

            y += separacionVertical;
        }

        // Nodo nullptr
        painter.setBrush(QColor("#9f9681"));
        painter.setPen(Qt::black);
        painter.drawEllipse(QPointF(x, y), radio, radio);
        painter.setPen(Qt::white);
        painter.drawText(QRectF(x - radio, y - radio, radio * 2, radio * 2),
                         Qt::AlignCenter, "nullptr");
    }

    // ======== BST / AVL ========
    else if (estructuraTipo == 4 || estructuraTipo == 5) {
        if (raiz) {
            int startX = width() / 2;
            int startY = 80;
            int nivel = 0;
            int separacionX = width() / 4;
            dibujarNodoArbol(painter, raiz, startX, startY, separacionX, nivel);
        }
    }

}


void AreaDibujo::dibujarFlecha(QPainter &painter, QPointF inicio, QPointF fin, bool doble) {
    // Linea
    painter.drawLine(inicio, fin);

    //Punta Triangular
    const double arrowSize = 8.0;

    // Angulo
    double angle = std::atan2(fin.y() - inicio.y(), fin.x() - inicio.x());

    QPointF pA = fin - QPointF(std::cos(angle - PI_D/6.0) * arrowSize,
                               std::sin(angle - PI_D/6.0) * arrowSize);
    QPointF pB = fin - QPointF(std::cos(angle + PI_D/6.0) * arrowSize,
                               std::sin(angle + PI_D/6.0) * arrowSize);

    // Flecha next
    QBrush oldBrush = painter.brush();
    painter.setBrush(painter.pen().color());
    painter.drawPolygon(QPolygonF() << fin << pA << pB);
    painter.setBrush(oldBrush);

    if (doble) {
        //Para Douvle LinKed List, flecha previous
        double angleBack = std::atan2(inicio.y() - fin.y(), inicio.x() - fin.x());
        QPointF qA = inicio - QPointF(std::cos(angleBack - PI_D/6.0) * arrowSize,
                                      std::sin(angleBack - PI_D/6.0) * arrowSize);
        QPointF qB = inicio - QPointF(std::cos(angleBack + PI_D/6.0) * arrowSize,
                                      std::sin(angleBack + PI_D/6.0) * arrowSize);
        painter.setBrush(painter.pen().color());
        painter.drawPolygon(QPolygonF() << inicio << qA << qB);
        painter.setBrush(oldBrush);
    }
}

void AreaDibujo::dibujarHeadTail(QPainter &painter, const QPointF &pos, const QString &texto) {
    painter.setPen(Qt::yellow);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(pos, texto);
}

void AreaDibujo::dibujarNodoArbol(QPainter &painter, NodoArbol* nodo, int x, int y, int separacionX, int nivel) {
    if (!nodo) return;

    int radio = 30;
    int yStep = 90; // separación vertical entre niveles

    // Dibujar nodo (círculo con valor)
    painter.setBrush(QColor("#143a64"));
    painter.setPen(Qt::black);
    painter.drawEllipse(QPointF(x, y), radio, radio);
    painter.setPen(Qt::white);
    painter.drawText(QRectF(x - radio, y - radio, radio * 2, radio * 2),
                     Qt::AlignCenter, QString::number(nodo->valor));

    // Conectar a hijo izquierdo
    if (nodo->izq) {
        int childX = x - separacionX;
        int childY = y + yStep;
        painter.setPen(Qt::white);
        painter.drawLine(x, y + radio, childX, childY - radio);
        dibujarNodoArbol(painter, nodo->izq, childX, childY, separacionX / 2, nivel + 1);
    }

    // Conectar a hijo derecho
    if (nodo->der) {
        int childX = x + separacionX;
        int childY = y + yStep;
        painter.setPen(Qt::white);
        painter.drawLine(x, y + radio, childX, childY - radio);
        dibujarNodoArbol(painter, nodo->der, childX, childY, separacionX / 2, nivel + 1);
    }
}


