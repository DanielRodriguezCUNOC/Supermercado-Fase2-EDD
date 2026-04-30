#include "arbolavl.h"
#include <iostream>

using namespace std;

// Creamos un nodo nuevo, siempre empieza con altura 1 porque no tiene hijos
NodoAVL::NodoAVL(Product p) : producto(p), izq(nullptr), der(nullptr), altura(1) {}

ArbolAVL::ArbolAVL() : raiz(nullptr) {}

ArbolAVL::~ArbolAVL() {
    destruirRec(raiz);
}

// Borra todo el árbol liberando la memoria, nodo por nodo de abajo hacia arriba
void ArbolAVL::destruirRec(NodoAVL* nodo) {
    if (nodo != nullptr) {
        destruirRec(nodo->izq);
        destruirRec(nodo->der);
        delete nodo;
    }
}

// Solo nos dice qué tan alto es un nodo de forma segura sin lanzar error si es nulo
int ArbolAVL::obtenerAltura(NodoAVL* N) {
    if (N == nullptr) return 0;
    return N->altura;
}

// Solo dice cuál de dos números es más grande
int ArbolAVL::max(int a, int b) {
    return (a > b) ? a : b;
}

// Cuando el árbol pesa mucho del lado izquierdo, hacemos un giro a la derecha para emparejarlo
NodoAVL* ArbolAVL::rotacionDerecha(NodoAVL* y) {
    NodoAVL* x = y->izq;
    NodoAVL* hijoDerechoDeX = x->der;

    // Hacemos el movimiento
    x->der = y;
    y->izq = hijoDerechoDeX;

    // Volvemos a calcular las alturas de los nodos que movimos
    y->altura = max(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;
    x->altura = max(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;

    // x es ahora la nueva cabeza de este pedacito de árbol
    return x;
}

// Cuando el árbol pesa mucho del lado derecho, giramos a la izquierda
NodoAVL* ArbolAVL::rotacionIzquierda(NodoAVL* x) {
    NodoAVL* y = x->der;
    NodoAVL* hijoIzquierdoDeY = y->izq;

    // Hacemos el movimiento
    y->izq = x;
    x->der = hijoIzquierdoDeY;

    // Calculamos cómo quedaron de altos
    x->altura = max(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;
    y->altura = max(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;

    // y se vuelve la nueva cabeza
    return y;
}

// Nos dice la diferencia de altura entre el lado izquierdo y el derecho.
// Si esto da más de 1 o menos de -1, el árbol está chueco y toca rotarlo.
int ArbolAVL::obtenerBalance(NodoAVL* N) {
    if (N == nullptr) return 0;
    return obtenerAltura(N->izq) - obtenerAltura(N->der);
}

void ArbolAVL::insertar(const Product& producto) {
    raiz = insertarRec(raiz, producto);
}

// Esta es la parte difícil, metemos el producto y arreglamos el árbol si se desarmó
NodoAVL* ArbolAVL::insertarRec(NodoAVL* nodo, const Product& producto) {
    // Si llegamos a un espacio vacío, ahí creamos el nodo y paramos
    if (nodo == nullptr)
        return new NodoAVL(producto);

    // Lo acomodamos por nombre. Si es menor a la izquierda, mayor a la derecha
    if (producto.getName() < nodo->producto.getName())
        nodo->izq = insertarRec(nodo->izq, producto);
    else if (producto.getName() > nodo->producto.getName())
        nodo->der = insertarRec(nodo->der, producto);
    else // Si se llaman igual, usamos su código de barras para desempatar
    {
        if (producto.getBarcode() < nodo->producto.getBarcode())
            nodo->izq = insertarRec(nodo->izq, producto);
        else if (producto.getBarcode() > nodo->producto.getBarcode())
            nodo->der = insertarRec(nodo->der, producto);
        else 
            return nodo; // Si es el mismísimo producto repetido, no lo metemos
    }

    // Ya metimos el producto, ahora actualizamos la altura para los cálculos
    nodo->altura = 1 + max(obtenerAltura(nodo->izq), obtenerAltura(nodo->der));

    // Revisamos si se desbalancearon los pesos
    int balance = obtenerBalance(nodo);

    // Hay cuatro formas en las que puede romperse el balance:

    // Está muy pesado a la izquierda y el nuevo hijo está a la izquierda
    if (balance > 1 && producto.getName() <= nodo->izq->producto.getName())
        return rotacionDerecha(nodo);

    // Está muy pesado a la derecha y el nuevo hijo está a la derecha
    if (balance < -1 && producto.getName() >= nodo->der->producto.getName())
        return rotacionIzquierda(nodo);

    // 3. Está pesado a la izquierda pero el nuevo hijo quedó metido como una rodilla hacia la derecha
    if (balance > 1 && producto.getName() > nodo->izq->producto.getName()) {
        nodo->izq = rotacionIzquierda(nodo->izq);
        return rotacionDerecha(nodo);
    }

    // Está pesado a la derecha pero el hijo hizo una rodilla hacia la izquierda
    if (balance < -1 && producto.getName() < nodo->der->producto.getName()) {
        nodo->der = rotacionDerecha(nodo->der);
        return rotacionIzquierda(nodo);
    }

    // Regresamos el nodo ya parejito
    return nodo;
}

// Busca el producto más a la izquierda (el que va primero en el abecedario)
NodoAVL* ArbolAVL::nodoValorMinimo(NodoAVL* nodo) {
    NodoAVL* actual = nodo;
    while (actual->izq != nullptr)
        actual = actual->izq;
    return actual;
}

// Elimina un producto por su código de barras
void ArbolAVL::eliminar(const std::string& barcode) {
    raiz = eliminarRec(raiz, barcode); 
}

// Busca el nodo a borrar arreglando el árbol al mismo tiempo en la bajada y subida
NodoAVL* ArbolAVL::eliminarRec(NodoAVL* root, const std::string& barcode) {
    if (root == nullptr)
        return root;

    // Como el árbol está ordenado por nombre, buscar el código de barras significa
    // que tenemos que revisar todas las ramas posibles en un "escáner completo"
    if (root->producto.getBarcode() != barcode) {
        NodoAVL* res = nullptr;
        // Revisamos de lado izquierdo
        if ((res = eliminarRec(root->izq, barcode)) != root->izq) {
            root->izq = res;
        } 
        // Si no estaba, revisamos de lado derecho
        else if ((res = eliminarRec(root->der, barcode)) != root->der) {
            root->der = res;
        } else {
            return root; // No lo encontramos en todo el árbol
        }
    } else {
        // Encontramos al que queremos borrar
        
        // Si no tiene hijos o tiene solo uno
        if ((root->izq == nullptr) || (root->der == nullptr)) {
            // Agarramos al único hijo o nos quedamos con nada
            NodoAVL* temp = root->izq ? root->izq : root->der;
            
            // Si no tenía nada, lo quitamos
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp; // Copiamos la info del hijo asumiendo su lugar
            }
            delete temp;
        } else {
            // Si el nodo tiene sus dos hijos, le robamos la info al nodo más chico del lado derecho
            // y luego mandamos a borrar ese nodo robado (que es más fácil porque está en una orilla)
            NodoAVL* temp = nodoValorMinimo(root->der);
            root->producto = temp->producto;
            root->der = eliminarRec(root->der, temp->producto.getBarcode());
        }
    }

    // Si borramos el último nodo, ya terminamos
    if (root == nullptr)
        return root;

    // Después de borrar toca actualizar la altura
    root->altura = 1 + max(obtenerAltura(root->izq), obtenerAltura(root->der));

    // Revisamos que tanta diferencia hay entre izquierda y derecha
    int balance = obtenerBalance(root);

    // Casos de desbalanceo (mismos que al agregar)
    if (balance > 1 && obtenerBalance(root->izq) >= 0)
        return rotacionDerecha(root);

    if (balance > 1 && obtenerBalance(root->izq) < 0) {
        root->izq = rotacionIzquierda(root->izq);
        return rotacionDerecha(root);
    }

    if (balance < -1 && obtenerBalance(root->der) <= 0)
        return rotacionIzquierda(root);

    if (balance < -1 && obtenerBalance(root->der) > 0) {
        root->der = rotacionDerecha(root->der);
        return rotacionIzquierda(root);
    }

    return root;
}

// Búsqueda rápida porque el árbol está ordenado desde el principio
Product* ArbolAVL::buscarPorNombre(const std::string& nombre) const {
    NodoAVL* resultado = buscarRec(raiz, nombre);
    if (resultado) return &(resultado->producto);
    return nullptr;
}

// Emula buscar en el diccionario, si la letra es menor nos vamos izquierda, si no a la derecha
NodoAVL* ArbolAVL::buscarRec(NodoAVL* root, const std::string& nombre) const {
    // Terminamos si quedó nulo o si le dimos al clavo
    if (root == nullptr || root->producto.getName() == nombre)
        return root;

    // Si nos pasamos en el abecedario, doblamos a la izquierda
    if (root->producto.getName() < nombre)
        return buscarRec(root->der, nombre);

    // De lo contrario a la derecha
    return buscarRec(root->izq, nombre);
}

void ArbolAVL::listarPorNombre() const {
    inOrderRec(raiz);
    cout << endl;
}

// Lo imprime ordenado (visitando izquierda, luego centro, luego derecha)
void ArbolAVL::inOrderRec(NodoAVL* root) const {
    if (root != nullptr) {
        inOrderRec(root->izq);
        cout << root->producto.getName() << " (" << root->producto.getBarcode() << ") - ";
        inOrderRec(root->der);
    }
}

#include <sstream>

// Función para generar el código para Graphviz (para dibujar el árbol)
static void generarDOTArbolAVLRec(NodoAVL* nodo, std::stringstream& ss) {
    if (!nodo) return;
    long id = reinterpret_cast<long>(nodo);
    
    // Hacemos que tenga forma de óvalo con colores
    ss << "  node" << id << " [label=\"" << nodo->producto.getName() << "\\n" << nodo->producto.getBarcode() << "\"];\n";

    // Unimos las flechas a los hijos
    if (nodo->izq) {
        long hid = reinterpret_cast<long>(nodo->izq);
        ss << "  node" << id << " -> node" << hid << ";\n";
        generarDOTArbolAVLRec(nodo->izq, ss);
    }
    if (nodo->der) {
        long hid = reinterpret_cast<long>(nodo->der);
        ss << "  node" << id << " -> node" << hid << ";\n";
        generarDOTArbolAVLRec(nodo->der, ss);
    }
}

// Prepara y devuelve todo el código Graphviz para dibujar este árbol
std::string ArbolAVL::generarDOT() const {
    std::stringstream ss;
    ss << "digraph AVLTree {\n";
    ss << "  node [shape=oval, style=filled, fillcolor=\"#FCE4EC\", color=\"#880E4F\"];\n";
    if (raiz) generarDOTArbolAVLRec(raiz, ss);
    ss << "}\n";
    return ss.str();
}

void ArbolAVL::buscarPorNombreRec(NodoAVL* nodo, const std::string& nombre, ListaGenerica<Product*>* resultados) const {
    if (nodo == nullptr) return;

    // Recorrido In-Order para mantener el orden alfabético
    buscarPorNombreRec(nodo->izq, nombre, resultados);

    if (nodo->producto.getName() == nombre) {
        // Creamos una copia del producto para el resultado
        resultados->insertar(new Product(nodo->producto));
    }

    buscarPorNombreRec(nodo->der, nombre, resultados);
}

void ArbolAVL::buscarPorNombreLista(const std::string& nombre, ListaGenerica<Product*>* resultados) const {
    buscarPorNombreRec(raiz, nombre, resultados);
}

void ArbolAVL::obtenerTodoEnOrdenRec(NodoAVL* nodo, ListaGenerica<Product*>* resultados) const {
    if (nodo == nullptr) return;

    obtenerTodoEnOrdenRec(nodo->izq, resultados);
    resultados->insertar(new Product(nodo->producto));
    obtenerTodoEnOrdenRec(nodo->der, resultados);
}

void ArbolAVL::obtenerTodoEnOrden(ListaGenerica<Product*>* resultados) const {
    obtenerTodoEnOrdenRec(raiz, resultados);
}
