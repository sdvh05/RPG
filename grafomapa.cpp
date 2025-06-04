#include "GrafoMapa.h"

GrafoMapa::GrafoMapa() {}

void GrafoMapa::agregarZona(const QString& nombreZona) {
    zonas.insert(nombreZona);
}

bool GrafoMapa::existeZona(const QString& nombreZona) const {
    return zonas.contains(nombreZona);
}

QList<QString> GrafoMapa::obtenerZonas() const {
    return zonas.values();
}

void GrafoMapa::agregarConexion(const QString& origen, const QString& destino, int peso, bool esPermanente) {
    if(!existeZona(origen) || !existeZona(destino)) return;

    Conexion conexion;
    conexion.peso = peso;
    conexion.esPermanente = esPermanente;
    conexion.activa = esPermanente; // Las conexiones permanentes están activas por defecto

    grafo[origen][destino] = conexion;
}

bool GrafoMapa::existeConexion(const QString& origen, const QString& destino) const {
    if(!grafo.contains(origen)) return false;
    if(!grafo[origen].contains(destino)) return false;
    return grafo[origen][destino].activa;
}

int GrafoMapa::pesoConexion(const QString& origen, const QString& destino) const {
    if(!existeConexion(origen, destino)) return -1;
    return grafo[origen][destino].peso;
}

QList<QString> GrafoMapa::obtenerAdyacentes(const QString& zona) const {
    QList<QString> adyacentes;
    if(!grafo.contains(zona)) return adyacentes;

    for(const auto& conexion : grafo[zona].toStdMap()) {
        if(conexion.second.activa) {
            adyacentes.append(conexion.first);
        }
    }
    return adyacentes;
}

QList<QPair<QString, int>> GrafoMapa::obtenerConexiones(const QString& zona) const {
    QList<QPair<QString, int>> conexiones;
    if(!grafo.contains(zona)) return conexiones;

    for(const auto& conexion : grafo[zona].toStdMap()) {
        if(conexion.second.activa) {
            conexiones.append({conexion.first, conexion.second.peso});
        }
    }
    return conexiones;
}

void GrafoMapa::activarConexionCondicional(const QString& origen, const QString& destino) {
    if(grafo.contains(origen) && grafo[origen].contains(destino)) {
        grafo[origen][destino].activa = true;
        grafo[origen][destino].esPermanente=false;
    }
}

void GrafoMapa::desactivarConexionCondicional(const QString& origen, const QString& destino) {
    if(grafo.contains(origen) && grafo[origen].contains(destino) && !grafo[origen][destino].esPermanente) {
        grafo[origen][destino].activa = false;
    }
}

bool GrafoMapa::esConexionPermanente(const QString& origen, const QString& destino) const {
    if(!grafo.contains(origen)) return false;
    if(!grafo[origen].contains(destino)) return false;
    return grafo[origen][destino].esPermanente;
}

bool GrafoMapa::esConexionActiva(const QString& origen, const QString& destino) const {
    if(!grafo.contains(origen)) return false;
    if(!grafo[origen].contains(destino)) return false;
    return grafo[origen][destino].activa;
}
