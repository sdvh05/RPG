#ifndef BST_H
#define BST_H

#include "NodoArbol.h"
#include <QVector>
#include <QString>

class Bst {
public:
    Bst();
    ~Bst();

    // Operaciones básicas
    void insertar(int val);
    void eliminar(int val);
    bool buscar(int val) const;
    void imprimir() const;
    void limpiar();

    // Utilidades
    NodoArbol* getRoot() const;
    bool isEmpty() const;
    int size() const;
    QVector<int> getValues() const; // in-order

    // Persistencia (binario)
    void guardarArchivo(const QString& ruta);
    void cargarArchivo(const QString& ruta);

private:
    NodoArbol* root;

    // helpers recursivos
    NodoArbol* insertarRec(NodoArbol* nodo, int val);
    NodoArbol* eliminarRec(NodoArbol* nodo, int val);
    NodoArbol* minNodo(NodoArbol* nodo) const;

    bool buscarRec(NodoArbol* nodo, int val) const;
    void inorder(NodoArbol* nodo, QVector<int>& acc) const;
    void destruir(NodoArbol* nodo);
    int contar(NodoArbol* nodo) const;
};

#endif // BST_H
