#ifndef HASHLOGROS_H
#define HASHLOGROS_H

#include <QString>
#include <QList>

struct Logro {
    QString nombre;
    bool desbloqueado;

    Logro(const QString& nombre, bool desbloqueado = false)
        : nombre(nombre), desbloqueado(desbloqueado) {}
};

class HashLogros {
private:
    static const int TAM = 23;
    QList<Logro> tabla[TAM];

    int hash(const QString& clave) const;

public:
    HashLogros();

    void insertar(const QString& nombre);
    void desbloquear(const QString& nombre);
    bool estaDesbloqueado(const QString& nombre) const;
    void mostrarLogros() const;
};

#endif // HASHLOGROS_H
