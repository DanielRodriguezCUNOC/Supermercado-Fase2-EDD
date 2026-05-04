#ifndef TABLAHASH_H
#define TABLAHASH_H

#include "model/estructuras/listaenlazadanoordenada.h"
#include <string>

class TablaHash {
private:
    ListaEnlazadaNoOrdenada** buckets;
    int capacidad;
    int contador_productos;
    int contador_colisiones;

    int funcion_hash(std::string llave);

public:
    TablaHash();
    ~TablaHash();

    void insertar(Product* producto);
    Product* buscar(std::string codigo_barra);
    void eliminar(std::string codigo_barra);

    double getFactorCarga();
    int getColisiones();
    int getCapacidad();
    ListaEnlazadaNoOrdenada* getBucket(int indice);
    std::string generarDOT();
};

#endif
