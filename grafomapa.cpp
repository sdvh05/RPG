#include "GrafoMapa.h"
#include <QQueue>

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

QList<QString> GrafoMapa::rutaMasCorta(const QString& origen, const QString& destino) const {
    QMap<QString, int> distancias;
    QMap<QString, QString> anteriores;
    QSet<QString> visitados;

    //INF
    for (const QString& zona : zonas) {
        distancias[zona] = INT_MAX;
    }
    distancias[origen] = 0;

    while (visitados.size() < zonas.size()) {
        QString nodoActual;
        int minDist = INT_MAX;

        // SELECCION DEL MiNIMO - Paso 2 de Dijkstra
        for (const QString& zona : zonas) {
            if (!visitados.contains(zona) && distancias[zona] < minDist) {
                minDist = distancias[zona];
                nodoActual = zona;
            }
        }

        //Visitados
        if (nodoActual.isEmpty()) break;
        visitados.insert(nodoActual);

        for (const auto& conexion : grafo[nodoActual].toStdMap()) {
            const QString& vecino = conexion.first;
            const Conexion& datos = conexion.second;

            if (!datos.esPermanente && !datos.activa)
                continue;

            int nuevaDist = distancias[nodoActual] + datos.peso;
            if (nuevaDist < distancias[vecino]) {
                distancias[vecino] = nuevaDist;
                anteriores[vecino] = nodoActual;
            }
        }
    }

    //Reconstruccion del camino desde destino hasta origen
    QList<QString> ruta;
    QString actual = destino;

    while (anteriores.contains(actual)) {
        ruta.prepend(actual);
        actual = anteriores[actual];
    }

    if (actual == origen) {
        ruta.prepend(origen);
        int total = 0;
        for (int i = 0; i < ruta.size() - 1; ++i)
            total += pesoConexion(ruta[i], ruta[i + 1]);

        ruta.append("(" + QString::number(total) + " km)");
        return ruta;
    }

    return {};
}



QList<QList<QString>> GrafoMapa::todasLasRutas(const QString& origen, const QString& destino) const {
    struct NodoRuta {
        QString nodo;
        QList<QString> camino;
        int costo;
    };

    QList<QList<QString>> rutas;

    QQueue<NodoRuta> cola;
    cola.enqueue({origen, {origen}, 0});

    while (!cola.isEmpty()) {
        NodoRuta actual = cola.dequeue();

        if (actual.nodo == destino) {
            QList<QString> rutaFinal = actual.camino;
            int total = actual.costo;
            rutaFinal.append("(" + QString::number(total) + " km)");
            rutas.append(rutaFinal);
            continue;
        }

        for (const auto& vecino : obtenerConexiones(actual.nodo)) {
            QString siguiente = vecino.first;
            int peso = vecino.second;

            // Solo si es conexión activa o permanente
            if (!esConexionPermanente(actual.nodo, siguiente) && !esConexionActiva(actual.nodo, siguiente))
                continue;

            // Evitar ciclos
            if (actual.camino.contains(siguiente))
                continue;

            int nuevoCosto = actual.costo + peso;
            QList<QString> nuevoCamino = actual.camino;
            nuevoCamino.append(siguiente);
            cola.enqueue({siguiente, nuevoCamino, nuevoCosto});
        }
    }

    // Obtener ruta más corta real
    QList<QString> rutaCorta = rutaMasCorta(origen, destino);

    // Eliminarla de la lista de rutas encontradas
    rutas.erase(std::remove_if(rutas.begin(), rutas.end(), [&](const QList<QString>& ruta) {
                    // Comparamos sin el peso final
                    QList<QString> copia = ruta;
                    copia.removeLast();
                    QList<QString> cortaSinPeso = rutaCorta;
                    cortaSinPeso.removeLast();
                    return copia == cortaSinPeso;
                }), rutas.end());

    // Ordenar por longitud del camino (sin contar el km)
    std::sort(rutas.begin(), rutas.end(), [](const QList<QString>& a, const QList<QString>& b) {
        return a.size() < b.size();
    });

    return rutas;
}




