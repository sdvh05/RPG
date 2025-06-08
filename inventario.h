#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "Objeto.h"
#include <QString>
#include <QDebug>


class NodoInventario {
public:
    Objeto obj;
    NodoInventario* izq;
    NodoInventario* der;

    NodoInventario(const Objeto& o) : obj(o), izq(nullptr), der(nullptr) {}
};

class Inventario {
private:
    NodoInventario* raiz;
    void destruir(NodoInventario* nodo);
    NodoInventario* insertar(NodoInventario* nodo, const Objeto& obj);
    Objeto* buscar(NodoInventario* nodo, const QString& nombre);

    void recogerObjetosRec(NodoInventario* nodo, QVector<Objeto*>& lista) const;
    void recogerPorTipoRec(NodoInventario* nodo, const QString& tipo, QVector<Objeto*>& lista) const;
    void recogerPorRarezaRec(NodoInventario* nodo, const QString& rareza, QVector<Objeto*>& lista) const;

public:
    Inventario();
    ~Inventario();

    void agregarObjeto(const Objeto& obj);
    Objeto* obtenerObjeto(const QString& nombre);
    void usarObjeto(const QString& nombre);

    void recogerObjetos(QVector<Objeto*>& lista) const;
    void recogerPorTipo(const QString& tipo, QVector<Objeto*>& lista) const;
    void recogerPorRareza(const QString& rareza, QVector<Objeto*>& lista) const;

    NodoInventario* getRaiz() const;

public: //Agregar items especificos
    void agregarNucleo(int valor=1);
    void agregarPoscionVida(int valor=1);
    void agregarPoscionMana(int valor=1);
    void agregarPoscionVidaGrande(int valor=1);
    void agregarPoscionManaGrande(int valor=1);
    void agregarLlave(int valor=1);


};


#endif // INVENTARIO_H
