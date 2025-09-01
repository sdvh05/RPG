#ifndef AVL_H
#define AVL_H

#include "NodoArbol.h"
#include <QVector>
#include <QString>

class Avl {
public:
    Avl();
    ~Avl();

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

    // Helpers AVL
    int altura(NodoArbol* n) const;
    int factorBalance(NodoArbol* n) const;
    void actualizarAltura(NodoArbol* n);

    NodoArbol* rotarDerecha(NodoArbol* y);
    NodoArbol* rotarIzquierda(NodoArbol* x);

    NodoArbol* insertarRec(NodoArbol* nodo, int val);
    NodoArbol* eliminarRec(NodoArbol* nodo, int val);
    NodoArbol* balancear(NodoArbol* nodo);

    NodoArbol* minNodo(NodoArbol* nodo) const;

    // Others
    bool buscarRec(NodoArbol* nodo, int val) const;
    void inorder(NodoArbol* nodo, QVector<int>& acc) const;
    void destruir(NodoArbol* nodo);
    int contar(NodoArbol* nodo) const;
};

#endif // AVL_H
