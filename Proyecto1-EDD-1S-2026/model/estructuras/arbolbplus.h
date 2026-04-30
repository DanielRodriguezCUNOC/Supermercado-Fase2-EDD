#ifndef ARBOLBPLUS_H
#define ARBOLBPLUS_H

#include <string>
#include "../entidades/product.h"
#include "../entidades/nodo.h"
#include "listagenerica.h"

// Nodo del Árbol B+
// Si es una "hoja", guarda directamente los productos. 
// Si NO es hoja (interno), guarda rutas (punteros) para saber a qué hijo ir.
class BPlusNode
{
public:
    bool esHoja;               // Si es true, guardamos los productos.
    int grado;                 // El límite máximo de hijos que puedo tener (orden del árbol).
    int numClaves;             // Cuántas categorías (claves) tengo guardadas actualmente.
    
    std::string *claves;       // Un arreglo con los nombres de las categorías
    BPlusNode **hijos;         // A quí guardo los punteros a los nodos de más abajo.
    Nodo **productos;          // A quí guardo las listas enlazadas de productos.
    BPlusNode *siguienteHoja;  // Un puente directo a la siguiente hoja (solo útil en las hojas, para leer todo de corrido sin subir).

    BPlusNode(int _grado, bool _hoja);
    ~BPlusNode();
};

// El Árbol B+ en sí
// Sabe dónde está la raíz y cómo insertar o buscar.
class ArbolBPlus
{
public:
    BPlusNode *raiz;           // El punto de inicio de nuestro árbol.
    int grado;                 // De qué orden es el árbol.

    // Empezamos con un grado por defecto.
    ArbolBPlus(int _grado = 4);
    ~ArbolBPlus();

    // Funciones principales
    bool insertarProducto(const Product &producto, std::string &errorRollback);
    void buscarPorCategoria(const std::string &categoria, Nodo *&resultado) const;
    void buscarPorCategoriaLista(const std::string &categoria, ListaGenerica<Product*>* resultados) const;
    bool eliminarProducto(const std::string &codigoBarra, std::string &errorRollback);
    
    // Funciones para exportar los datos y para hacer los dibujos molones (Graphviz).
    std::string exportarCSV() const;
    std::string generarDOT() const;

private:
    void destruirRec(BPlusNode *nodo); // Borra todo de forma recursiva.
    
    // Esta función es el motor del B+. Se llama a sí misma para bajar hasta la hoja correcta, 
    // y si la hoja se llena, parte el nodo a la mitad y manda un "aviso" (clavePromovida) hacia el padre.
    BPlusNode* insertarRec(BPlusNode *nodo, const Product &producto, std::string &clavePromovida, std::string &error, bool &exito);
};

#endif // ARBOLBPLUS_H
