#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Nodo.h"
#include <QVector>
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QString>

class LinkedList {
private:
    Nodo* head;

public:
    LinkedList();
    ~LinkedList();

    bool insertarPos(int pos, int val);
    void insertarInicio(int val);
    void insertarFinal(int val);

    void eliminarVal(int val);
    bool eliminarPos(int pos);

    bool buscar(int val) const;

    void imprimir() const;
    void limpiar();

    Nodo* getHead() const;
    bool isEmpty() const;
    int size() const;
    QVector<int> getValues() const;

    void guardarArchivo(const QString& ruta);
    void cargarArchivo(const QString& ruta);
};

#endif // LINKEDLIST_H
