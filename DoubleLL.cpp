#include "DoubleLL.h"
#include <iostream>
#include <QDebug>


DoubleLL::DoubleLL() : head(nullptr), tail(nullptr) {}

DoubleLL::~DoubleLL() {
    limpiar();
}

//Insertar
//-----------------------------------------------------------
bool DoubleLL::insertarPos(int pos, int val) {
    if (pos < 0) return false;

    NodoDLL* nuevo = new NodoDLL(val);

    if (pos == 0) {
        nuevo->siguiente = head;
        if (head){
            head->anterior = nuevo;
        }
        else{
            tail = nuevo;
        }
        head = nuevo;
        return true;
    }

    NodoDLL* iterador = head;
    int indice = 0;
    while (iterador && indice < pos - 1) {
        iterador = iterador->siguiente;
        ++indice;
    }

    if (!iterador) {
        delete nuevo;
        return false;
    }

    nuevo->siguiente = iterador->siguiente;
    nuevo->anterior = iterador;

    if (iterador->siguiente) {
        iterador->siguiente->anterior = nuevo;
    } else {
        tail = nuevo;
    }

    iterador->siguiente = nuevo;
    return true;
}

void DoubleLL::insertarInicio(int val) {
    insertarPos(0, val);
}

void DoubleLL::insertarFinal(int val) {
    int tamaño = 0;
    NodoDLL* iterador = head;
    while (iterador) {
        ++tamaño;
        iterador = iterador->siguiente;
    }
    insertarPos(tamaño, val);
}
//--------------------------------------------------------

//Eliminar
//-----------------------------------------------------------

void DoubleLL::eliminarVal(int val) {
    NodoDLL* iterador = head;

    while (iterador && iterador->valor != val) {
        iterador = iterador->siguiente;
    }

    if (!iterador) return;

    if (iterador->anterior)
        iterador->anterior->siguiente = iterador->siguiente;

    else
        head = iterador->siguiente;

    if (iterador->siguiente)
        iterador->siguiente->anterior = iterador->anterior;
    else
        tail = iterador->anterior;

    delete iterador;
}


bool DoubleLL::eliminarPos(int pos) {
    if (!head || pos < 0) return false;

    NodoDLL* iterador = head;
    int indice = 0;

    while (iterador && indice < pos) {
        iterador = iterador->siguiente;
        ++indice;
    }

    if (!iterador) return false;

    if (iterador->anterior)
        iterador->anterior->siguiente = iterador->siguiente;
    else
        head = iterador->siguiente;

    if (iterador->siguiente)
        iterador->siguiente->anterior = iterador->anterior;
    else
        tail = iterador->anterior;

    delete iterador;
    return true;
}

//-----------------------------------------------------------

bool DoubleLL::buscar(int val) const {
    NodoDLL* actual = head;
    while (actual != nullptr) {
        if (actual->valor == val) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}


void DoubleLL::imprimirAdelante() const {
    NodoDLL* iterador = head;
    QString resultado;

    while (iterador) {
        resultado += QString::number(iterador->valor) + " <-> ";
        iterador = iterador->siguiente;
    }
    resultado += "nullptr";

    qDebug() << "Desde head:" << resultado;
}


void DoubleLL::imprimirAtras() const {
    NodoDLL* iterador = tail;
    QString resultado;

    while (iterador) {
        resultado += QString::number(iterador->valor) + " <-> ";
        iterador = iterador->anterior;
    }
    resultado += "nullptr";

    qDebug() << "Desde tail:" << resultado;
}


void DoubleLL::limpiar() {
    while (head) {
        NodoDLL* temp = head;
        head = head->siguiente;
        delete temp;
    }
    tail = nullptr;
}


NodoDLL* DoubleLL::getHead() const {
    return head;
}

NodoDLL* DoubleLL::getTail() const {
    return tail;
}

bool DoubleLL::isEmpty() const {
    return head == nullptr;
}

int DoubleLL::size() const {
    int count = 0;
    NodoDLL* actual = head;
    while (actual) {
        ++count;
        actual = actual->siguiente;
    }
    return count;
}

QVector<int> DoubleLL::getValuesForward() const {
    QVector<int> valores;
    NodoDLL* actual = head;
    while (actual) {
        valores.append(actual->valor);
        actual = actual->siguiente;
    }
    return valores;
}

QVector<int> DoubleLL::getValuesBackward() const {
    QVector<int> valores;
    NodoDLL* actual = tail;
    while (actual) {
        valores.append(actual->valor);
        actual = actual->anterior;
    }
    return valores;
}

//-----------------------------------------------------------------------------
void DoubleLL::guardarArchivo(const QString& ruta) {
    QDir().mkpath(QFileInfo(ruta).absolutePath());
    QFile file(ruta);
    if (!file.open(QIODevice::WriteOnly)) return;

    QDataStream out(&file);
    NodoDLL* temp = head;
    while (temp) {
        out << temp->valor;
        temp = temp->siguiente;
    }
    file.close();
}

void DoubleLL::cargarArchivo(const QString& ruta) {
    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly)) return;

    QDataStream in(&file);
    int val;
    limpiar();
    while (!in.atEnd()) {
        in >> val;
        insertarFinal(val);
    }
    file.close();
}
