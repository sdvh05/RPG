#include "Avl.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDataStream>
#include <QDebug>
#include <algorithm>

Avl::Avl() : root(nullptr) {}

Avl::~Avl() {
    limpiar();
}

void Avl::limpiar() {
    destruir(root);
    root = nullptr;
}

void Avl::destruir(NodoArbol* nodo) {
    if (!nodo) return;
    destruir(nodo->izq);
    destruir(nodo->der);
    delete nodo;
}

int Avl::altura(NodoArbol* n) const { return n ? n->altura : 0; }

void Avl::actualizarAltura(NodoArbol* n) {
    if (!n) return;
    n->altura = 1 + std::max(altura(n->izq), altura(n->der));
}

int Avl::factorBalance(NodoArbol* n) const {
    return n ? altura(n->izq) - altura(n->der) : 0;
}

NodoArbol* Avl::rotarDerecha(NodoArbol* y) {
    NodoArbol* x = y->izq;
    NodoArbol* T2 = x->der;

    // rotación
    x->der = y;
    y->izq = T2;

    // actualizar alturas
    actualizarAltura(y);
    actualizarAltura(x);

    return x;
}

NodoArbol* Avl::rotarIzquierda(NodoArbol* x) {
    NodoArbol* y = x->der;
    NodoArbol* T2 = y->izq;

    // rotación
    y->izq = x;
    x->der = T2;

    // actualizar alturas
    actualizarAltura(x);
    actualizarAltura(y);

    return y;
}

NodoArbol* Avl::balancear(NodoArbol* nodo) {
    if (!nodo) return nodo;

    actualizarAltura(nodo);
    int fb = factorBalance(nodo);

    // Left heavy
    if (fb > 1) {
        if (factorBalance(nodo->izq) < 0) {
            nodo->izq = rotarIzquierda(nodo->izq); // LR
        }
        return rotarDerecha(nodo); // LL
    }
    // Right heavy
    if (fb < -1) {
        if (factorBalance(nodo->der) > 0) {
            nodo->der = rotarDerecha(nodo->der); // RL
        }
        return rotarIzquierda(nodo); // RR
    }

    return nodo;
}

void Avl::insertar(int val) {
    root = insertarRec(root, val);
}

NodoArbol* Avl::insertarRec(NodoArbol* nodo, int val) {
    if (!nodo) return new NodoArbol(val);

    if (val < nodo->valor) nodo->izq = insertarRec(nodo->izq, val);
    else if (val > nodo->valor) nodo->der = insertarRec(nodo->der, val);
    else return nodo; // ignorar duplicados

    return balancear(nodo);
}

void Avl::eliminar(int val) {
    root = eliminarRec(root, val);
}

NodoArbol* Avl::minNodo(NodoArbol* nodo) const {
    while (nodo && nodo->izq) nodo = nodo->izq;
    return nodo;
}

NodoArbol* Avl::eliminarRec(NodoArbol* nodo, int val) {
    if (!nodo) return nullptr;

    if (val < nodo->valor) nodo->izq = eliminarRec(nodo->izq, val);
    else if (val > nodo->valor) nodo->der = eliminarRec(nodo->der, val);
    else {
        // encontrado
        if (!nodo->izq || !nodo->der) {
            NodoArbol* child = nodo->izq ? nodo->izq : nodo->der;
            if (!child) {
                delete nodo;
                return nullptr;
            } else {
                NodoArbol* tmp = child;
                delete nodo;
                nodo = tmp;
            }
        } else {
            NodoArbol* suc = minNodo(nodo->der);
            nodo->valor = suc->valor;
            nodo->der = eliminarRec(nodo->der, suc->valor);
        }
    }

    return balancear(nodo);
}

bool Avl::buscar(int val) const {
    return buscarRec(root, val);
}

bool Avl::buscarRec(NodoArbol* nodo, int val) const {
    if (!nodo) return false;
    if (val == nodo->valor) return true;
    if (val < nodo->valor) return buscarRec(nodo->izq, val);
    return buscarRec(nodo->der, val);
}

void Avl::imprimir() const {
    QVector<int> vals = getValues();
    QString s = "AVL inorder: [ ";
    for (int v : vals) s += QString::number(v) + " ";
    s += "]";
    qDebug() << s;
}

QVector<int> Avl::getValues() const {
    QVector<int> acc;
    inorder(root, acc);
    return acc;
}

void Avl::inorder(NodoArbol* nodo, QVector<int>& acc) const {
    if (!nodo) return;
    inorder(nodo->izq, acc);
    acc.append(nodo->valor);
    inorder(nodo->der, acc);
}

NodoArbol* Avl::getRoot() const { return root; }
bool Avl::isEmpty() const { return root == nullptr; }
int Avl::size() const { return contar(root); }

int Avl::contar(NodoArbol* nodo) const {
    if (!nodo) return 0;
    return 1 + contar(nodo->izq) + contar(nodo->der);
}

// =================== Persistencia ===================
void Avl::guardarArchivo(const QString& ruta) {
    QDir().mkpath(QFileInfo(ruta).absolutePath());
    QFile file(ruta);
    if (!file.open(QIODevice::WriteOnly)) return;

    QDataStream out(&file);
    QVector<int> vals = getValues(); // in-order
    for (int v : vals) out << v;

    file.close();
}

void Avl::cargarArchivo(const QString& ruta) {
    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly)) return;

    QDataStream in(&file);
    int v;
    limpiar();
    while (!in.atEnd()) {
        in >> v;
        insertar(v); // insert AVL -> reequilibra al vuelo
    }
    file.close();
}
