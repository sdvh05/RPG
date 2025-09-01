#ifndef NODODLL_H
#define NODODLL_H

//Para Double Linked List

class NodoDLL {
public:
    int valor;
    NodoDLL* siguiente;
    NodoDLL* anterior;

    NodoDLL(int val) : valor(val), siguiente(nullptr), anterior(nullptr) {}
};

#endif // NODODLL_H
