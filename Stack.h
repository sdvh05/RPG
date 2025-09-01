#ifndef STACK_H
#define STACK_H

#include "Nodo.h"
#include <QVector>
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QString>

class Stack {
private:
    Nodo* cima;

public:
    Stack();
    ~Stack();

    void push(int val);

    int pop();

    int peek() const;

    bool estaVacia() const;
    void imprimir() const;
    void limpiar();


    Nodo* getTop() const;
    bool isEmpty() const;
    int size() const;
    QVector<int> getValues() const;

    void guardarArchivo(const QString& ruta);
    void cargarArchivo(const QString& ruta);
};

#endif // STACK_H
