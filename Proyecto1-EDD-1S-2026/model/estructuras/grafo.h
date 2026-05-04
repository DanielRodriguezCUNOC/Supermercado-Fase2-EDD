#ifndef GRAFO_H
#define GRAFO_H

#include "model/entidades/sucursal.h"
#include "model/estructuras/listagenerica.h"
#include <string>

class Grafo {
private:
    ListaGenerica<Sucursal*>* sucursales;

public:
    Grafo();
    ~Grafo();

    void agregarSucursal(Sucursal* s);
    void conectar(std::string idOrig, std::string idDest, int tiempo, int costo);
    Sucursal* buscarSucursal(std::string id);
    
    ListaGenerica<Sucursal*>* getSucursales();
    std::string generarDOT();
};

#endif
