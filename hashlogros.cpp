#include "hashlogros.h"
#include <iostream>

int HashLogros::hash(const QString& clave) const {
    int valor = 0;
    for (QChar c : clave) {
        valor += c.unicode();
    }
    return valor % TAM;
}

HashLogros::HashLogros() {
    insertar("Derrotar primer enemigo");
    insertar("Derrotar primer minijefe");
    insertar("Juntarte con tu equipo");
    insertar("Conseguir primer núcleo");
    insertar("Conseguir 3 núcleos");
    insertar("Conseguir todos los núcleos");
    insertar("Llegar al Castillo del Código");
    insertar("Vencer al jefe final");
    insertar("Restaurar el orden");

    insertar("Subir de Nivel por Primera Vez");
    insertar("Alcanzar el Nivel Maximo del Caballero");
    insertar("Alcanzar el Nivel Maximo de la Princesa");
    insertar("Alcanzar el Nivel Maximo de la Curandera");

    insertar("Encontrar Al Axeman");
    insertar("Rescatar al Explorador");

}

void HashLogros::insertar(const QString& nombre) {
    int indice = hash(nombre);
    for (Logro& l : tabla[indice]) {
        if (l.nombre == nombre)
            return; // ya está insertado
    }
    tabla[indice].append(Logro(nombre));
}

void HashLogros::desbloquear(const QString& nombre) {
    int indice = hash(nombre);
    for (Logro& l : tabla[indice]) {
        if (l.nombre == nombre) {
            l.desbloqueado = true;
            std::cout << "Logro desbloqueado: " << nombre.toStdString() << "\n";
            return;
        }
    }
    std::cout << "Logro no encontrado: " << nombre.toStdString() << "\n";
}

bool HashLogros::estaDesbloqueado(const QString& nombre) const {
    int indice = hash(nombre);
    for (const Logro& l : tabla[indice]) {
        if (l.nombre == nombre)
            return l.desbloqueado;
    }
    return false;
}

void HashLogros::mostrarLogros() const {
    std::cout << "\n🗺️ Lista de logros:\n";
    for (int i = 0; i < TAM; ++i) {
        for (const Logro& l : tabla[i]) {
            std::cout << "- " << l.nombre.toStdString()
            << ": " << (l.desbloqueado ? "[✓]" : "[ ]") << "\n";
        }
    }
}
