#include "grafo.h"
#include <sstream>

Grafo::Grafo() {
    sucursales = new ListaGenerica<Sucursal*>();
}

Grafo::~Grafo() {
    NodoGenerico<Sucursal*>* aux = sucursales->getCabeza();
    while(aux) {
        delete aux->valor;
        aux = aux->siguiente;
    }
    delete sucursales;
}

void Grafo::agregarSucursal(Sucursal* s) {
    if (buscarSucursal(s->id) == nullptr) {
        sucursales->insertarFinal(s);
    }
}

void Grafo::conectar(std::string idOrig, std::string idDest, int tiempo, int costo) {
    Sucursal* orig = buscarSucursal(idOrig);
    Sucursal* dest = buscarSucursal(idDest);

    if (orig && dest) {
        orig->adyacentes->insertarFinal(new Conexion(idDest, tiempo, costo));
    }
}

Sucursal* Grafo::buscarSucursal(std::string id) {
    NodoGenerico<Sucursal*>* aux = sucursales->getCabeza();
    while(aux) {
        if (aux->valor->id == id) return aux->valor;
        aux = aux->siguiente;
    }
    return nullptr;
}

ListaGenerica<Sucursal*>* Grafo::getSucursales() {
    return sucursales;
}

std::string Grafo::generarDOT() {
    std::stringstream ss;
    ss << "digraph G {\n";
    ss << "  node [shape=circle, style=filled, fillcolor=lightblue];\n";
    ss << "  edge [fontsize=10];\n";

    NodoGenerico<Sucursal*>* auxS = sucursales->getCabeza();
    while(auxS) {
        Sucursal* s = auxS->valor;
        ss << "  \"" << s->id << "\" [label=\"" << s->nombre << "\\n(" << s->id << ")\"];\n";
        
        NodoGenerico<Conexion*>* auxC = s->adyacentes->getCabeza();
        while(auxC) {
            Conexion* c = auxC->valor;
            ss << "  \"" << s->id << "\" -> \"" << c->destinoID << "\" [label=\"T:" << c->tiempo << ", C:" << c->costo << "\"];\n";
            auxC = auxC->siguiente;
        }
        auxS = auxS->siguiente;
    }

    ss << "}\n";
    return ss.str();
}
 Broadway
