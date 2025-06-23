#ifndef ARBOLDECISION_H
#define ARBOLDECISION_H

#include <QString>
#include <functional>

class NodoDecision {
public:
    QString nombre;
    QString descripcion;
    std::function<void()> efecto;

    NodoDecision* izquierda;
    NodoDecision* derecha;

    NodoDecision(const QString& nombre,
                 const QString& descripcion,
                 std::function<void()> efecto = nullptr);
    ~NodoDecision();
};

class ArbolDecision {
private:
    NodoDecision* raiz;

public:
    ArbolDecision();
    ~ArbolDecision();

    NodoDecision* getRaiz() const;
    void recorrer(NodoDecision* nodoActual);

    void liberar(NodoDecision* nodo);
};

#endif // ARBOLDECISION_H
