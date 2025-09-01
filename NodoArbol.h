#ifndef NODOARBOL_H
#define NODOARBOL_H

class NodoArbol {
public:
    int valor;
    int altura;       //AVL
    NodoArbol* izq;
    NodoArbol* der;

    explicit NodoArbol(int v)
        : valor(v), altura(1), izq(nullptr), der(nullptr) {}
};

#endif // NODOARBOL_H
