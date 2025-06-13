#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QtWidgets>

class CustomGraphicsView : public QGraphicsView
{
public:
    using QGraphicsView::QGraphicsView;

protected:
    void wheelEvent(QWheelEvent *event) override {
        event->ignore();
    }

    void mousePressEvent(QMouseEvent *event) override {
        event->ignore();
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        event->ignore();
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        event->ignore();
    }

    void keyPressEvent(QKeyEvent *event) override {
        event->ignore();
    }
};

#endif // CUSTOMGRAPHICSVIEW_H
