#ifndef DOUBLELL_H
#define DOUBLELL_H

#include "NodoDLL.h"
#include <QVector>
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QString>

class DoubleLL {
private:
    NodoDLL* head;
    NodoDLL* tail;

public:
    DoubleLL();
    ~DoubleLL();

    bool insertarPos(int pos, int val);
    void insertarInicio(int val);
    void insertarFinal(int val);

    void eliminarVal(int val);
    bool eliminarPos(int pos);


    void imprimirAdelante() const;
    void imprimirAtras() const;

    bool buscar(int val) const;


    void limpiar();

    NodoDLL* getHead() const;
    NodoDLL* getTail() const;
    bool isEmpty() const;
    int size() const;
    QVector<int> getValuesForward() const;
    QVector<int> getValuesBackward() const;

    void guardarArchivo(const QString& ruta);
    void cargarArchivo(const QString& ruta);
};

#endif // DOUBLELL_H
