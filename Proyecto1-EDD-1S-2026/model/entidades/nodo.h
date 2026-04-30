#ifndef NODO_H
#define NODO_H
#include "product.h"
class Nodo
{
private:
    Product *product;
    Nodo *next;
    Nodo *prev;

public:
    Nodo();
    Nodo(Product *product);
    Product *getValue() const;
    Nodo *getNext() const;
    Nodo *getPrev() const;
    void setValue(Product *product);
    void setNext(Nodo *next);
    void setPrev(Nodo *prev);
};

#endif // NODO_H
