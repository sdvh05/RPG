#ifndef NODO_H
#define NODO_H

//Para Linked List
//     Queue
//     Stack

class Nodo {
public:
    int valor;
    Nodo* siguiente;

    Nodo(int val) : valor(val), siguiente(nullptr) {}
};

#endif // NODO_H
