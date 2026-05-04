#include "tablahash.h"

TablaHash::TablaHash() {
    capacidad = 37;
    contador_productos = 0;
    contador_colisiones = 0;
    buckets = new ListaEnlazadaNoOrdenada*[capacidad];
    for (int i = 0; i < capacidad; i++) {
        buckets[i] = new ListaEnlazadaNoOrdenada();
    }
}

TablaHash::~TablaHash() {
    for (int i = 0; i < capacidad; i++) {
        delete buckets[i];
    }
    delete[] buckets;
}

int TablaHash::funcion_hash(std::string llave) {
    long hash = 0;
    long p = 31;
    long m = capacidad;
    long p_pow = 1;
    for (char c : llave) {
        hash = (hash + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    if (hash < 0) {
        hash += m;
    }
    return (int)hash;
}

void TablaHash::insertar(Product* producto) {
    int indice = funcion_hash(producto->barcode);
    if (!buckets[indice]->isEmpty()) {
        contador_colisiones++;
    }
    buckets[indice]->insertar(producto);
    contador_productos++;
}

Product* TablaHash::buscar(std::string codigo_barra) {
    int indice = funcion_hash(codigo_barra);
    ListaEnlazadaNoOrdenada* lista = buckets[indice];
    Nodo* actual = lista->getCabeza();
    while (actual != nullptr) {
        if (actual->getValue()->barcode == codigo_barra) {
            return actual->getValue();
        }
        actual = actual->getNext();
    }
    return nullptr;
}

void TablaHash::eliminar(std::string codigo_barra) {
    int indice = funcion_hash(codigo_barra);
    buckets[indice]->eliminarPorCodigo(codigo_barra);
    contador_productos--;
}

double TablaHash::getFactorCarga() {
    return (double)contador_productos / capacidad;
}

int TablaHash::getColisiones() {
    return contador_colisiones;
}

int TablaHash::getCapacidad() {
    return capacidad;
}

ListaEnlazadaNoOrdenada* TablaHash::getBucket(int indice) {
    if (indice >= 0 && indice < capacidad) {
        return buckets[indice];
    }
    return nullptr;
}

std::string TablaHash::generarDOT() {
    std::string dot = "digraph G {\n";
    dot += "node [shape=record];\n";
    dot += "rankdir=LR;\n";

    // Representación de los buckets 
    dot += "buckets [label=\"";
    for (int i = 0; i < capacidad; i++) {
        dot += "<f" + std::to_string(i) + "> " + std::to_string(i);
        if (i < capacidad - 1) dot += "|";
    }
    dot += "\"];\n";

    // Representación de las listas en cada bucket
    for (int i = 0; i < capacidad; i++) {
        Nodo* actual = buckets[i]->getCabeza();
        int nodeCount = 0;
        while (actual != nullptr) {
            std::string nodeId = "node_" + std::to_string(i) + "_" + std::to_string(nodeCount);
            dot += nodeId + " [label=\"" + actual->getValue()->barcode + "\", shape=box];\n";

            if (nodeCount == 0) {
                dot += "buckets:f" + std::to_string(i) + " -> " + nodeId + ";\n";
            } else {
                std::string prevId = "node_" + std::to_string(i) + "_" + std::to_string(nodeCount - 1);
                dot += prevId + " -> " + nodeId + ";\n";
            }

            actual = actual->getNext();
            nodeCount++;
        }
    }

    dot += "}\n";
    return dot;
}
