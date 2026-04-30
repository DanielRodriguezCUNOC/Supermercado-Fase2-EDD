#ifndef LISTAENLAZADANOORDENADA_H
#define LISTAENLAZADANOORDENADA_H
#include "model/entidades/nodo.h"
#include "listagenerica.h"

// Una lista simple donde los productos se guardan sin ningún orden en especial.
// Solo se van agregando uno tras otro.
class ListaEnlazadaNoOrdenada
{
private:
    Nodo *cabeza; // El primer carrito de la fila
    Nodo *cola;   // El último carrito de la fila
    int size;     // Cuántos productos tenemos en total

public:
    ListaEnlazadaNoOrdenada();
    
    // Para ver quién es el primero o el último
    Nodo *getCabeza() const;
    Nodo *getCola() const;
    
    // Busca un producto por su posición en la fila (ej. el 3ero)
    Nodo *getAt(int position) const;
    
    // Mete un producto nuevo a la fila
    void insertar(Product *product);
    
    // Saca un producto de la fila
    void eliminar(Product *product);
    void eliminarPorCodigo(const std::string& barcode);
    
    // Devuelve cuántos hay
    int getSize() const;
    
    // Nos dice si la fila está vacía
    bool isEmpty() const;
    
    // Genera el código para dibujar la fila con Graphviz
    std::string generarDOT() const;
    
    // Busca todos los productos con el mismo nombre
    void buscarPorNombre(const std::string& nombre, ListaGenerica<Product*>* resultados) const;
};

#endif // LISTAENLAZADANOORDENADA_H
