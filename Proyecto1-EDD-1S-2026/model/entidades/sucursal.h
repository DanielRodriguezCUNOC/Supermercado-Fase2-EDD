#ifndef SUCURSAL_H
#define SUCURSAL_H

#include <string>
#include "model/estructuras/listagenerica.h"
#include "model/entidades/conexion.h"
#include "model/estructuras/tablahash.h"

class Sucursal {
public:
    std::string id;
    std::string nombre;
    std::string ubicacion;
    int t_ingreso;
    int t_traspaso;
    int t_despacho;

    TablaHash* inventario;
    ListaGenerica<Conexion*>* adyacentes;

    Sucursal(std::string _id, std::string _nombre, std::string _ub, int ti, int tt, int td)
        : id(_id), nombre(_nombre), ubicacion(_ub), t_ingreso(ti), t_traspaso(tt), t_despacho(td) 
    {
        inventario = new TablaHash();
        adyacentes = new ListaGenerica<Conexion*>();
    }

    ~Sucursal() {
        delete inventario;
        // Limpiar conexiones
        NodoGenerico<Conexion*>* aux = adyacentes->getCabeza();
        while(aux) {
            delete aux->valor;
            aux = aux->siguiente;
        }
        delete adyacentes;
    }
};

#endif
 Broadway
