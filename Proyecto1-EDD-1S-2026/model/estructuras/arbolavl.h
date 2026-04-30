#ifndef ARBOLAVL_H
#define ARBOLAVL_H

#include "listagenerica.h"
#include "../entidades/product.h"
#include <string>

// Nodo para el árbol AVL 
class NodoAVL {
public:
    Product producto; // El producto que guardamos aquí
    NodoAVL* izq;     // Puntero al hijo izquierdo 
    NodoAVL* der;     // Puntero al hijo derecho 
    int altura;       // Nos sirve para saber si el árbol está chueco y hay que arreglarlo

    NodoAVL(Product p);
};


// El Árbol AVL es como un árbol binario normal, pero se asegura de no quedar inclinado
// hacia un solo lado. Si lo hace, gira sus ramas para volver a estar parejito.
class ArbolAVL {
private:
    NodoAVL* raiz;

    // Funciones internas para mantener el orden y el balance
    int obtenerAltura(NodoAVL* N);
    int max(int a, int b);
    NodoAVL* rotacionDerecha(NodoAVL* y);    // Gira los nodos hacia la derecha para balancear
    NodoAVL* rotacionIzquierda(NodoAVL* x);  // Gira los nodos hacia la izquierda para balancear
    int obtenerBalance(NodoAVL* N);          // Revisa la diferencia de altura entre el lado izquierdo y derecho
    
    // Funciones recursivas de apoyo
    NodoAVL* insertarRec(NodoAVL* nodo, const Product& producto);
    NodoAVL* nodoValorMinimo(NodoAVL* nodo); // Busca el nodo más pequeño en una rama
    NodoAVL* eliminarRec(NodoAVL* root, const std::string& barcode);
    
    NodoAVL* buscarRec(NodoAVL* root, const std::string& nombre) const;
    void inOrderRec(NodoAVL* root) const;
    void obtenerTodoEnOrdenRec(NodoAVL* nodo, ListaGenerica<Product*>* resultados) const;
    void buscarPorNombreRec(NodoAVL* nodo, const std::string& nombre, ListaGenerica<Product*>* resultados) const;
    void destruirRec(NodoAVL* nodo);         // Limpia la memoria cuando ya no usamos el árbol

public:
    ArbolAVL();
    ~ArbolAVL();

    // Las acciones principales que podemos hacer con el árbol
    void insertar(const Product& producto);
    void eliminar(const std::string& barcode);
    
    // Sirve para encontrar rápido un producto por su nombre aprovechando el orden del árbol
    Product* buscarPorNombre(const std::string& nombre) const;
    
    // Muestra todos los productos ordenados alfabéticamente
    void listarPorNombre() const;
    
    // Busca todos los productos que coincidan con un nombre
    void buscarPorNombreLista(const std::string& nombre, ListaGenerica<Product*>* resultados) const;

    // Obtiene todos los productos ordenados por nombre
    void obtenerTodoEnOrden(ListaGenerica<Product*>* resultados) const;

    // Genera el código para dibujar el árbol con Graphviz
    std::string generarDOT() const;
};

#endif // ARBOLAVL_H
