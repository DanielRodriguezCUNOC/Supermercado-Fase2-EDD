#ifndef ARBOLB_H
#define ARBOLB_H

#include "entidades/product.h"
#include "listagenerica.h"
#include <string>

class Product;

/**
 * Un Árbol B es una estructura de datos equilibrada diseñada para mantener los datos ordenados.
 * A diferencia del Árbol B+, el Árbol B almacena registros completos (en este caso, objetos Product)
 * tanto en las ramas como en las hojas, optimizando el acceso a las claves frecuentemente consultadas.
 */
class NodoB
{
public:
    Product* claves; /**< Arreglo dinámico que contiene los productos almacenados en el nodo. */
    NodoB** hijos;   /**< Arreglo dinámico de punteros hacia los nodos descendientes (subárboles). */
    int n;           /**< Cantidad actual de claves activas que alberga este nodo. */
    bool hoja;       /**< Bandera booleana: es verdadero si el nodo reside en el nivel inferior (sin hijos). */
    int t;           /**< Grado mínimo del árbol (t). Un nodo no-raíz debe tener al menos t-1 claves, y máximo 2t-1. */

    /**
     *  Constructor del Nodo B.
     *  _t Grado mínimo del árbol, el cual define las reglas de capacidad térmica del nodo.
     *  _hoja Indicador de si el nodo se inicializa como hoja.
     */
    NodoB(int _t, bool _hoja);

    /**
     *  Destructor del Nodo B. Libera dinámicamente la memoria de su propio conglomerado.
     */
    ~NodoB();

    /**
     *  Realiza la inserción recursiva de un producto en un nodo garantizado a no estar lleno.
     *  k Producto a insertar. Se ordena en función a la lógica interna de validación (fecha/código).
     */
    void insertarNoLleno(const Product &k);

    /**
     *  Divide el nodo hijo estructurado en dos nodos independientes para subsanar el desbordamiento.
     *  i El índice correspondiente dentro del arreglo de hijos del nodo padre actual.
     *  y Puntero al hijo saturado que aloja (2t-1) claves.
     */
    void dividirHijo(int i, NodoB *y);

    /**
     * Imprime la totalidad de las claves correspondientes a este subárbol.
     */
    void recorrer();

    // =========================================================================
    // Métodos para gestionar la eliminación de claves y el rebalanceo
    // =========================================================================

    int buscarClave(const std::string &codigo);
    void eliminar(const std::string &codigo);
    void removerDeHoja(int idx);
    void removerDeNoHoja(int idx);
    
    Product obtenerPredecesor(int idx);
    Product obtenerSucesor(int idx);
    
    void llenar(int idx);
    void pedirPrestadoAnterior(int idx);
    void pedirPrestadoSiguiente(int idx);
    void fusionar(int idx);

    void buscarPorRangoFechaRec(const std::string &inicio, const std::string &fin, ListaGenerica<Product*>* resultados);
};

/**
 *  Clase administradora de la jerarquía global del Árbol B.
 * 
 * Orquesta y provee la interfaz pública para mutar y acceder a la colección estructurada de productos.
 */
class ArbolB
{
public:
    NodoB *raiz; /**< Puntero direccionado a la base primordial (la raíz) del árbol. */
    int t;       /**< Parámetro estructural que define el grado mínimo global del árbol. */

    /**
     *  Instancia el controlador principal del árbol con la restricción volumétrica dada.
     *  _t Grado mínimo del Árbol B (Ej: si t=5, un nodo se divide al tener 9 claves).
     */
    ArbolB(int _t);
    ~ArbolB();

    /**
     *  Inserta un nuevo objeto producto en la jerarquía, orquestando splits desde la raíz si procede.
     *  k El Producto que se registrará.
     */
    void insertar(const Product &k);
    
    /**
     *  Dispara el recorrido en inorden sobre la unidad fundamental logueándolo por salida general.
     */
    void imprimir();
    
    /**
     *  Transforma el modelo esquemático del árbol al lenguaje DOT estructurado.
     *  Retorna std::string conteniendo el código fuente DOT capaz de ser leído por la suite Graphviz.
     */
    std::string generarDOT() const;
    
    void buscarPorRangoFecha(const std::string &inicio, const std::string &fin, ListaGenerica<Product*>* resultados);

    /**
     *  Invoca el algoritmo de eliminación en cascada de balances, filtrado puramente por código abstracto.
     */
    void eliminarPorCodigo(const std::string &codigo);

private:
    /**
     *  Realiza la purga iterativa de la estructura para ceder la memoria de vuelta al SO.
     */
    void destruirRec(NodoB* nodo);
};

#endif // ARBOLB_H
