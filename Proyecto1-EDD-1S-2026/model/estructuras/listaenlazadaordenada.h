#ifndef LISTAENLAZADAORDENADA_H
#define LISTAENLAZADAORDENADA_H
#include "model/entidades/nodo.h"
#include "listagenerica.h"

// Una lista donde los productos se acomodan automáticamente en orden alfabético
class ListaEnlazadaOrdenada
{
private:
    Nodo *cabeza; // El primero de la fila
    Nodo *cola;   // El último de la fila
    int size;     // La cantidad de productos

public:
    ListaEnlazadaOrdenada();
    
    Nodo *getCabeza() const;
    Nodo *getCola() const;
    
    // Busca un producto por el turno que ocupa en la fila
    Nodo *getAt(int position) const;
    
    // Mete un producto nuevo buscando su lugar correcto para que siga ordenado
    void insertar(Product *product);
    
    // Saca un producto de la fila
    void eliminar(Product *product);
    void eliminarPorCodigo(const std::string& barcode);
    
    int getSize() const;
    bool isEmpty() const;
    
    // Para dibujar la lista con Graphviz
    std::string generarDOT() const;
    
    // Busca todos los productos con el mismo nombre
    void buscarPorNombre(const std::string& nombre, ListaGenerica<Product*>* resultados) const;
};

#endif // LISTAENLAZADAORDENADA_H
