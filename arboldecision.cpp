#include "arboldecision.h"
#include <iostream>

NodoDecision::NodoDecision(const QString& nombre, const QString& descripcion, std::function<void()> efecto)
    : nombre(nombre), descripcion(descripcion), efecto(efecto), izquierda(nullptr), derecha(nullptr) {}

NodoDecision::~NodoDecision() {
    delete izquierda;
    delete derecha;
}

ArbolDecision::ArbolDecision() {
    // Construcción del árbol de decisiones:

    // Nodo base
    raiz = new NodoDecision("Ir con el mago", "Decides seguir al mago hacia  el bosque JS");

    // Rama izquierda: Axeman
    NodoDecision* escucharAxeman = new NodoDecision("Escuchar al Axeman", "Escuchar porque esta pidiendo ayuda");
    NodoDecision* ayudarlo = new NodoDecision("Ayudar al Axeman", "Te acercas para ayudarlo.");
    NodoDecision* ignorarlo = new NodoDecision("Ignorarlo", "Decides ignorarlo y evitar riesgos de la mision del rescate");

    NodoDecision* ejecutarRescate = new NodoDecision("Ejecutar rescate", "Planeas un rescate para su amigo.");
    NodoDecision* aceptarRutaAlt = new NodoDecision("Aceptar ruta alternativa", "Aceptas una nueva ruta secreta.");
    NodoDecision* negarRutaAlt = new NodoDecision("Negar ruta alternativa", "Rechazas desviarte del camino.");

    // Rama derecha: combates
    NodoDecision* derrotarMiniJefes = new NodoDecision("Derrotar mini jefes", "Te enfrentas a enemigos menores en el camino.");
    NodoDecision* derrotarJefeFinal = new NodoDecision("Derrotar jefe final", "Ha llegado la hora del combate final, con el cual podras restaurar la paz.");

    // Armar árbol
    raiz->izquierda = escucharAxeman;
    raiz->derecha = derrotarMiniJefes;

    escucharAxeman->izquierda = ayudarlo;
    escucharAxeman->derecha = ignorarlo;

    ayudarlo->izquierda = ejecutarRescate;
    ejecutarRescate->izquierda = aceptarRutaAlt;
    ejecutarRescate->derecha = negarRutaAlt;

    derrotarMiniJefes->derecha = derrotarJefeFinal;
}

ArbolDecision::~ArbolDecision() {
    liberar(raiz);
}

void ArbolDecision::liberar(NodoDecision* nodo) {
    if (!nodo) return;
    liberar(nodo->izquierda);
    liberar(nodo->derecha);
    delete nodo;
}

NodoDecision* ArbolDecision::getRaiz() const {
    return raiz;
}

void ArbolDecision::recorrer(NodoDecision* nodoActual) {
    if (!nodoActual) return;

    std::cout << "\n>> " << nodoActual->nombre.toStdString() << "\n";
    std::cout << nodoActual->descripcion.toStdString() << "\n";

    if (nodoActual->efecto) nodoActual->efecto();

    if (!nodoActual->izquierda && !nodoActual->derecha) {
        std::cout << "Fin de la decisión.\n";
        return;
    }

    char eleccion;
    std::cout << "Elegir (i = izquierda / d = derecha): ";
    std::cin >> eleccion;

    if (eleccion == 'i')
        recorrer(nodoActual->izquierda);
    else if (eleccion == 'd')
        recorrer(nodoActual->derecha);
    else
        std::cout << "Opción inválida.\n";
}
