#ifndef QUEUE_H
#define QUEUE_H

#include "Nodo.h"
#include <QVector>
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QString>

class Queue {
private:
    Nodo* frente;
    Nodo* final;

public:
    Queue();
    ~Queue();

    void enqueue(int val);

    int dequeue();

    int peek() const;

    bool estaVacia() const;
    void imprimir() const;
    void limpiar();


    Nodo* getFront() const;
    bool isEmpty() const;
    int size() const;
    QVector<int> getValues() const;

    void guardarArchivo(const QString& ruta);
    void cargarArchivo(const QString& ruta);
};

#endif // QUEUE_H
