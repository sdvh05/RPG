#include "LinkedList.h"
#include <iostream>
#include <QDebug>


LinkedList::LinkedList() : head(nullptr) {}

LinkedList::~LinkedList() {
    limpiar();
}


//Insertar
//-----------------------------------------------------------
bool LinkedList::insertarPos(int pos, int val) {
    if (pos < 0) return false;

    Nodo* nuevo = new Nodo(val);

    if (pos == 0) { //Cambiar Head
        nuevo->siguiente = head;
        head = nuevo;
        return true;
    }

    Nodo* iterador = head;
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
    iterador->siguiente = nuevo;
    return true;
}


void LinkedList::insertarInicio(int val) {
    insertarPos(0, val);
}

void LinkedList::insertarFinal(int val) {
    int tamaño = 0;
    Nodo* iterador = head;
    while (iterador) {
        ++tamaño;
        iterador = iterador->siguiente;
    }
    insertarPos(tamaño, val);
}
//--------------------------------------------------------------



//Eliminar
//--------------------------------------------------------------
void LinkedList::eliminarVal(int val) {
    if (!head) return;

    if (head->valor == val) {
        Nodo* temp = head;
        head = head->siguiente;
        delete temp;
        return;
    }

    Nodo* iterador = head;
    while (iterador->siguiente && iterador->siguiente->valor != val) {
        iterador = iterador->siguiente;
    }

    if (iterador->siguiente) {
        Nodo* temp = iterador->siguiente;
        iterador->siguiente = temp->siguiente;
        delete temp;
    }
}

bool LinkedList::eliminarPos(int pos) {
    if (!head || pos < 0) return false;

    if (pos == 0) {
        Nodo* temp = head;
        head = head->siguiente;
        delete temp;
        return true;
    }

    Nodo* iterador = head;
    int indice = 0;

    while (iterador->siguiente && indice < pos - 1) {
        iterador = iterador->siguiente;
        ++indice;
    }

    if (!iterador->siguiente) return false;

    Nodo* temp = iterador->siguiente;
    iterador->siguiente = temp->siguiente;
    delete temp;
    return true;
}

//--------------------------------------------------------------


bool LinkedList::buscar(int val) const {
    Nodo* iterador = head;
    while (iterador) {
        if (iterador->valor == val) return true;
        iterador = iterador->siguiente;
    }
    return false;
}

void LinkedList::imprimir() const {
    Nodo* iterador = head;
    QString resultado;

    while (iterador) {
        resultado += QString::number(iterador->valor) + " -> ";
        iterador = iterador->siguiente;
    }
    resultado += "nullptr";

    qDebug() << resultado;
}


void LinkedList::limpiar() {
    while (head) {
        Nodo* temp = head;
        head = head->siguiente;
        delete temp;
    }
}

Nodo* LinkedList::getHead() const {
    return head;
}

bool LinkedList::isEmpty() const {
    return head == nullptr;
}

int LinkedList::size() const {
    int count = 0;
    Nodo* actual = head;
    while (actual) {
        ++count;
        actual = actual->siguiente;
    }
    return count;
}

QVector<int> LinkedList::getValues() const {
    QVector<int> valores;
    Nodo* actual = head;
    while (actual) {
        valores.append(actual->valor);
        actual = actual->siguiente;
    }
    return valores;
}

//--------------------------------------------------------------
void LinkedList::guardarArchivo(const QString& ruta) {
    QDir().mkpath(QFileInfo(ruta).absolutePath());
    QFile file(ruta);
    if (!file.open(QIODevice::WriteOnly)) return;

    QDataStream out(&file);
    Nodo* temp = head;
    while (temp) {
        out << temp->valor;
        temp = temp->siguiente;
    }
    file.close();
}

// Cargar desde archivo binario
void LinkedList::cargarArchivo(const QString& ruta) {
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

