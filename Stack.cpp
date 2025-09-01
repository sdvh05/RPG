#include "Stack.h"
#include <iostream>
#include <QDebug>


Stack::Stack() : cima(nullptr) {}

Stack::~Stack() {
    limpiar();
}

void Stack::push(int val) {
    Nodo* nuevo = new Nodo(val);
    nuevo->siguiente = cima;
    cima = nuevo;
}

int Stack::pop() {
    if (estaVacia())
        return -1;

    Nodo* temp = cima;
    int val = temp->valor;
    cima = cima->siguiente;
    delete temp;
    return val;
}

int Stack::peek() const {
    return estaVacia() ? -1 : cima->valor;
}

bool Stack::estaVacia() const {
    return cima == nullptr;
}

void Stack::imprimir() const {
    Nodo* iterador = cima;
    QString resultado;
    while (iterador) {
        resultado += QString::number(iterador->valor) + " -> ";
        iterador = iterador->siguiente;
    }
    resultado += "nullptr";
    qDebug() << resultado;
}


void Stack::limpiar() {
    while (!estaVacia()) pop();
}

Nodo* Stack::getTop() const {
    return cima;
}

bool Stack::isEmpty() const {
    return cima == nullptr;
}

int Stack::size() const {
    int count = 0;
    Nodo* actual = cima;
    while (actual) {
        ++count;
        actual = actual->siguiente;
    }
    return count;
}

QVector<int> Stack::getValues() const {
    QVector<int> valores;
    Nodo* actual = cima;
    while (actual) {
        valores.append(actual->valor);
        actual = actual->siguiente;
    }
    return valores;
}

//-------------------------------------------------------------------------------
void Stack::guardarArchivo(const QString& ruta) {
    QDir().mkpath(QFileInfo(ruta).absolutePath());
    QFile file(ruta);
    if (!file.open(QIODevice::WriteOnly)) return;

    QDataStream out(&file);
    Nodo* temp = cima;
    while (temp) {
        out << temp->valor;
        temp = temp->siguiente;
    }
    file.close();
}

void Stack::cargarArchivo(const QString& ruta) {
    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly)) return;

    QDataStream in(&file);
    int val;
    limpiar();
    QVector<int> buffer;
    while (!in.atEnd()) {
        in >> val;
        buffer.append(val);
    }

    //Insertar Alrevez, de fin a inicio
    for (int i = buffer.size() - 1; i >= 0; --i) {
        push(buffer[i]);
    }
    file.close();
}
