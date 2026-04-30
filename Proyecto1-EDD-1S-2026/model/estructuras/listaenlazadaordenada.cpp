#include "listaenlazadaordenada.h"
#include <sstream>

// Empezamos con la fila vacía
ListaEnlazadaOrdenada::ListaEnlazadaOrdenada() : cabeza(nullptr), cola(nullptr), size(0) {}

// Saca a un producto en específico
void ListaEnlazadaOrdenada::eliminar(Product *product)
{
  if (isEmpty()) return;
  Nodo *actual = cabeza;
  
  // Recorremos la lista buscando al culpable
  while (actual != nullptr) {
    if (actual->getValue() == product) {
      // Si era el primero
      if (actual == cabeza) {
        cabeza = actual->getNext(); // El segundo se vuelve el primero
        if (cabeza != nullptr) cabeza->setPrev(nullptr);
        else cola = nullptr;
      } 
      // Si era el último
      else if (actual == cola) {
        cola = actual->getPrev(); // El penúltimo se vuelve el último
        if (cola != nullptr) cola->setNext(nullptr);
        else cabeza = nullptr;
      } 
      // Si estaba a la mitad de la fila
      else {
        // Hacemos el puente entre el de atrás y el de adelante
        actual->getPrev()->setNext(actual->getNext());
        actual->getNext()->setPrev(actual->getPrev());
      }
      
      delete actual; // Borramos el nodo liberando su espacio
      size--;
      return;
    }
    actual = actual->getNext();
  }
}

// Mete un producto a la fila y lo acomoda en su sitio
void ListaEnlazadaOrdenada::insertar(Product *product)
{
    Nodo *nuevo = new Nodo(product);
    
    // Si no había nadie, toma el primer lugar
    if (isEmpty())
    {
        cabeza = nuevo;
        cola = nuevo;
    }
    else
    {
        Nodo *actual = cabeza;
        
        // Caminamos por la fila hasta encontrar a alguien "más grande" (alfabéticamente)
        while (actual != nullptr && actual->getValue()->getName() < product->getName())
        {
            actual = actual->getNext();
        }

        // Si nos detuvimos en el primerito, significa que este es el más pequeño de todos
        if (actual == cabeza) 
        {
            nuevo->setNext(cabeza);
            cabeza->setPrev(nuevo);
            cabeza = nuevo; // Pasa a ser el nuevo jefe de fila
        }
        // Si llegamos hasta el final sin detenernos, es el más grande de todos
        else if (actual == nullptr) 
        {
            cola->setNext(nuevo);
            nuevo->setPrev(cola);
            cola = nuevo; // Pasa a ser el último
        }
        // Si nos detuvimos a medias, lo metemos justo antes del que encontramos
        else 
        {
            Nodo *anterior = actual->getPrev();
            anterior->setNext(nuevo);
            nuevo->setPrev(anterior);
            nuevo->setNext(actual);
            actual->setPrev(nuevo);
        }
    }
    size++; // Llegó alguien nuevo, cuenta
}

Nodo *ListaEnlazadaOrdenada::getCabeza() const
{
  return cabeza;
}

Nodo *ListaEnlazadaOrdenada::getCola() const
{
  return cola;
}

// Avanza pasitos para darte a quien está en la posición que pediste
Nodo *ListaEnlazadaOrdenada::getAt(int position) const
{
  if (isEmpty() || position < 0 || position >= size)
    return nullptr;
    
  Nodo *current = cabeza;
  int currentPosition = 0;
  
  while (currentPosition < position && current != nullptr)
  {
    current = current->getNext();
    currentPosition++;
  }
  return current;
}

int ListaEnlazadaOrdenada::getSize() const
{
  return size;
}

bool ListaEnlazadaOrdenada::isEmpty() const
{
  // Si no hay cabeza, pues no hay nadie
  return cabeza == nullptr;
}

// Arma el código de dibujo para mandar a Graphviz
std::string ListaEnlazadaOrdenada::generarDOT() const {
    std::stringstream ss;
    ss << "digraph G {\n";
    ss << "  rankdir=LR;\n"; // Hacemos que la lista se dibuje en horizontal
    ss << "  node [shape=record, style=filled, fillcolor=\"#E8F5E9\", color=\"#2E7D32\"];\n"; // Diseño en tonos verdes
    Nodo* act = cabeza;
    int i = 0;
    while(act) {
        ss << "  node" << i << " [label=\"{ " << act->getValue()->getName() << " | " << act->getValue()->getBarcode() << " }\"];\n";
        if(i > 0) ss << "  node" << (i-1) << " -> node" << i << ";\n"; // Dibujamos la flecha
        act = act->getNext();
        i++;
    }
    ss << "}\n";
    return ss.str();
}

void ListaEnlazadaOrdenada::buscarPorNombre(const std::string& nombre, ListaGenerica<Product*>* resultados) const {
    Nodo* actual = cabeza;
    while (actual) {
        if (actual->getValue()->getName() == nombre) {
            Product* p = new Product(*(actual->getValue()));
            resultados->insertar(p);
        } else if (actual->getValue()->getName() > nombre) {
            break; // Como está ordenada, si ya nos pasamos en el abecedario, ya no hay más
        }
        actual = actual->getNext();
    }
}

void ListaEnlazadaOrdenada::eliminarPorCodigo(const std::string& barcode) {
    Nodo* actual = cabeza;
    while (actual) {
        if (actual->getValue()->getBarcode() == barcode) {
            eliminar(actual->getValue());
            return;
        }
        actual = actual->getNext();
    }
}
