#include "Queue.h"
#include <iostream>
#include <QDebug>


Queue::Queue() : frente(nullptr), final(nullptr) {}

Queue::~Queue() {
    limpiar();
}

void Queue::enqueue(int val) {
    Nodo* nuevo = new Nodo(val);
    if (estaVacia()) {
        frente = final = nuevo;
    } else {
        final->siguiente = nuevo;
        final = nuevo;
    }
}

int Queue::dequeue() {
    if (estaVacia())
        return -1;

    Nodo* temp = frente;
    int valor = temp->valor;
    frente = frente->siguiente;

    if (!frente)
        final = nullptr;
    delete temp;
    return valor;
}

int Queue::peek() const {
    return estaVacia() ? -1 : frente->valor;
}

bool Queue::estaVacia() const {
    return frente == nullptr;
}

void Queue::imprimir() const {
    Nodo* iterador = frente;
    QString resultado;
    while (iterador) {
        resultado += QString::number(iterador->valor) + " -> ";
        iterador = iterador->siguiente;
    }
    resultado += "nullptr";
    qDebug() << resultado;
}


void Queue::limpiar() {
    while (!estaVacia()) dequeue();
}

Nodo* Queue::getFront() const {
    return frente;
}

bool Queue::isEmpty() const {
    return frente == nullptr;
}

int Queue::size() const {
    int count = 0;
    Nodo* actual = frente;
    while (actual) {
        ++count;
        actual = actual->siguiente;
    }
    return count;
}

QVector<int> Queue::getValues() const {
    QVector<int> valores;
    Nodo* actual = frente;
    while (actual) {
        valores.append(actual->valor);
        actual = actual->siguiente;
    }
    return valores;
}

//------------------------------------------------------------------------
void Queue::guardarArchivo(const QString& ruta) {
    QDir().mkpath(QFileInfo(ruta).absolutePath());
    QFile file(ruta);
    if (!file.open(QIODevice::WriteOnly)) return;

    QDataStream out(&file);
    Nodo* temp = frente;
    while (temp) {
        out << temp->valor;
        temp = temp->siguiente;
    }
    file.close();
}

void Queue::cargarArchivo(const QString& ruta) {
    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly)) return;

    QDataStream in(&file);
    int val;
    limpiar();
    while (!in.atEnd()) {
        in >> val;
        enqueue(val);
    }
    file.close();
}

