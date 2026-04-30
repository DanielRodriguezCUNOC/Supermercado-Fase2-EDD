#ifndef LISTAGENERICA_H
#define LISTAGENERICA_H

/**
 * @brief Una cajita genérica para guardar cualquier tipo de dato 'T'.
 * Como es una plantilla (template), el código real se genera cuando la usamos (ej: ListaGenerica<Product*>).
 */
template <typename T>
class NodoGenerico {
public:
    T valor;
    NodoGenerico<T>* siguiente;
    NodoGenerico<T>* anterior;

    NodoGenerico(T v) : valor(v), siguiente(nullptr), anterior(nullptr) {}
};

/**
 * @brief Una lista doblemente enlazada que no sabe (ni le importa) qué guarda.
 * Es ideal para pasar resultados de búsquedas sin depender de clases específicas.
 */
template <typename T>
class ListaGenerica {
private:
    NodoGenerico<T>* cabeza;
    NodoGenerico<T>* cola;
    int tamano;

public:
    ListaGenerica() : cabeza(nullptr), cola(nullptr), tamano(0) {}

    // Limpia la memoria de los nodos (pero NO de los objetos que apuntan, si son punteros)
    ~ListaGenerica() {
        NodoGenerico<T>* actual = cabeza;
        while (actual) {
            NodoGenerico<T>* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }

    void insertar(T valor) {
        NodoGenerico<T>* nuevo = new NodoGenerico<T>(valor);
        if (!cabeza) {
            cabeza = cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
        tamano++;
    }

    bool isEmpty() const {
        return cabeza == nullptr;
    }

    int getSize() const {
        return tamano;
    }

    NodoGenerico<T>* getCabeza() const {
        return cabeza;
    }
};

#endif // LISTAGENERICA_H
