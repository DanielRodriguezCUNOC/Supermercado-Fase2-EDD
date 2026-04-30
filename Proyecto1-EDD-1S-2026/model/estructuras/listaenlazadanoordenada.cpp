#include "listaenlazadanoordenada.h"

// Cuando creamos la lista, empieza vacía
ListaEnlazadaNoOrdenada::ListaEnlazadaNoOrdenada()
  : cabeza(nullptr), cola(nullptr), size(0)
{
}

// Agrega un producto a la fila
void ListaEnlazadaNoOrdenada::insertar(Product *product)
{
  Nodo *nuevoProducto = new Nodo(product);
  
  // Si no había nadie, este nuevo es el primero y también el último
  if (this->isEmpty())
  {
    this->cabeza = nuevoProducto;
    this->cola = nuevoProducto;
  }
  else
  {
    // Si ya había gente, lo metemos hasta adelante (al principio de la fila)
    nuevoProducto->setNext(this->cabeza);
    this->cabeza->setPrev(nuevoProducto);
    this->cabeza = nuevoProducto;
  }
  size++; // Sumamos uno al total
}

// Para sacar a un producto en específico
void ListaEnlazadaNoOrdenada::eliminar(Product *product)
{
  // Si no hay nadie, no hacemos nada
  if (this->isEmpty())
    return;

  Nodo *actual = this->cabeza;

  // Vamos revisando uno por uno
  while (actual != nullptr)
  {
    // Si lo encontramos...
    if (actual->getValue() == product)
    {
      // Caso 1: Era el primerito
      if (actual == this->cabeza)
      {
        this->cabeza = actual->getNext(); // El segundo pasa a ser el primero
        if (this->cabeza != nullptr) this->cabeza->setPrev(nullptr);
        else this->cola = nullptr; // Si era el único, ahora no hay nadie
      }
      // Caso 2: Era el último
      else if (actual == this->cola)
      {
        this->cola = actual->getPrev(); // El penúltimo pasa a ser el último
        if (this->cola != nullptr) this->cola->setNext(nullptr);
        else this->cabeza = nullptr; // Si era el único, ahora no hay nadie
      }
      // Caso 3: Estaba atrapado en medio
      else
      {
        // Conectamos al de atrás con el de adelante, saltándonos al que vamos a borrar
        actual->getPrev()->setNext(actual->getNext());
        actual->getNext()->setPrev(actual->getPrev());
      }
      
      delete actual; // Lo borramos
      size--;        // Restamos uno al total
      return;        // Terminamos
    }
    actual = actual->getNext(); // Pasamos al siguiente para seguir buscando
  }
}

Nodo *ListaEnlazadaNoOrdenada::getCabeza() const
{
  return this->cabeza;
}

Nodo *ListaEnlazadaNoOrdenada::getCola() const
{
  return this->cola;
}

// Para buscar quién está parado en x posición (0, 1, 2...)
Nodo *ListaEnlazadaNoOrdenada::getAt(int position) const
{
  if (this->isEmpty())
    return nullptr;

  if (this->getSize() <= position)
    return nullptr;

  Nodo *current = this->cabeza;
  int currentPosition = 1;

  // Caminamos en la fila hasta llegar a la posición que queremos
  while (currentPosition < position && current != nullptr)
  {
    current = current->getNext();
    currentPosition++;
  }

  return current;
}

int ListaEnlazadaNoOrdenada::getSize() const
{
  if (this->isEmpty())
    return 0;

  return size;
}

bool ListaEnlazadaNoOrdenada::isEmpty() const
{
  // Si no hay primero, es que está vacía
  return this->cabeza == nullptr;
}

#include <sstream>

// Prepara el código Graphviz para dibujar cajitas conectadas
std::string ListaEnlazadaNoOrdenada::generarDOT() const {
    std::stringstream ss;
    ss << "digraph G {\n";
    ss << "  rankdir=LR;\n"; // Se dibuja de izquierda a derecha
    ss << "  node [shape=record, style=filled, fillcolor=\"#E3F2FD\", color=\"#1565C0\"];\n";
    Nodo* act = cabeza;
    int i = 0;
    while(act) {
        ss << "  node" << i << " [label=\"{ " << act->getValue()->getName() << " | " << act->getValue()->getBarcode() << " }\"];\n";
        if(i > 0) ss << "  node" << (i-1) << " -> node" << i << ";\n"; // Unimos cajitas
        act = act->getNext();
        i++;
    }
    ss << "}\n";
    return ss.str();
}

void ListaEnlazadaNoOrdenada::buscarPorNombre(const std::string& nombre, ListaGenerica<Product*>* resultados) const {
    Nodo* actual = cabeza;
    while (actual) {
        if (actual->getValue()->getName() == nombre) {
            Product* p = new Product(*(actual->getValue()));
            resultados->insertar(p);
        }
        actual = actual->getNext();
    }
}
void ListaEnlazadaNoOrdenada::eliminarPorCodigo(const std::string& barcode) {
    Nodo* actual = cabeza;
    while (actual) {
        if (actual->getValue()->getBarcode() == barcode) {
            eliminar(actual->getValue());
            return;
        }
        actual = actual->getNext();
    }
}
