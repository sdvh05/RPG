#include "Bst.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDataStream>
#include <QDebug>

Bst::Bst() : root(nullptr) {}

Bst::~Bst() {
    limpiar();
}

void Bst::limpiar() {
    destruir(root);
    root = nullptr;
}

void Bst::destruir(NodoArbol* nodo) {
    if (!nodo) return;
    destruir(nodo->izq);
    destruir(nodo->der);
    delete nodo;
}

void Bst::insertar(int val) {
    root = insertarRec(root, val);
}

NodoArbol* Bst::insertarRec(NodoArbol* nodo, int val) {
    if (!nodo) return new NodoArbol(val);
    if (val < nodo->valor) nodo->izq = insertarRec(nodo->izq, val);
    else if (val > nodo->valor) nodo->der = insertarRec(nodo->der, val);
    // si es igual, no insertamos duplicados (puedes cambiar esta política)
    return nodo;
}

void Bst::eliminar(int val) {
    root = eliminarRec(root, val);
}

NodoArbol* Bst::minNodo(NodoArbol* nodo) const {
    while (nodo && nodo->izq) nodo = nodo->izq;
    return nodo;
}

NodoArbol* Bst::eliminarRec(NodoArbol* nodo, int val) {
    if (!nodo) return nullptr;

    if (val < nodo->valor) nodo->izq = eliminarRec(nodo->izq, val);
    else if (val > nodo->valor) nodo->der = eliminarRec(nodo->der, val);
    else {
        // encontrado
        if (!nodo->izq) {
            NodoArbol* r = nodo->der;
            delete nodo;
            return r;
        } else if (!nodo->der) {
            NodoArbol* l = nodo->izq;
            delete nodo;
            return l;
        } else {
            // 2 hijos: reemplazar por sucesor (mínimo del subárbol derecho)
            NodoArbol* suc = minNodo(nodo->der);
            nodo->valor = suc->valor;
            nodo->der = eliminarRec(nodo->der, suc->valor);
        }
    }
    return nodo;
}

bool Bst::buscar(int val) const {
    return buscarRec(root, val);
}

bool Bst::buscarRec(NodoArbol* nodo, int val) const {
    if (!nodo) return false;
    if (val == nodo->valor) return true;
    if (val < nodo->valor) return buscarRec(nodo->izq, val);
    return buscarRec(nodo->der, val);
}

void Bst::imprimir() const {
    QVector<int> vals = getValues();
    QString s = "BST inorder: [ ";
    for (int v : vals) s += QString::number(v) + " ";
    s += "]";
    qDebug() << s;
}

QVector<int> Bst::getValues() const {
    QVector<int> acc;
    inorder(root, acc);
    return acc;
}

void Bst::inorder(NodoArbol* nodo, QVector<int>& acc) const {
    if (!nodo) return;
    inorder(nodo->izq, acc);
    acc.append(nodo->valor);
    inorder(nodo->der, acc);
}

NodoArbol* Bst::getRoot() const { return root; }
bool Bst::isEmpty() const { return root == nullptr; }
int Bst::size() const { return contar(root); }

int Bst::contar(NodoArbol* nodo) const {
    if (!nodo) return 0;
    return 1 + contar(nodo->izq) + contar(nodo->der);
}

// =================== Persistencia ===================
void Bst::guardarArchivo(const QString& ruta) {
    QDir().mkpath(QFileInfo(ruta).absolutePath());
    QFile file(ruta);
    if (!file.open(QIODevice::WriteOnly)) return;

    QDataStream out(&file);
    // guardamos en in-order
    QVector<int> vals = getValues();
    for (int v : vals) out << v;

    file.close();
}

void Bst::cargarArchivo(const QString& ruta) {
    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly)) return;

    QDataStream in(&file);
    int v;
    limpiar();
    while (!in.atEnd()) {
        in >> v;
        insertar(v); // reconstruye el BST
    }
    file.close();
}
