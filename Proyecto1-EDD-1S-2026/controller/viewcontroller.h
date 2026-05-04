#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QObject>
#include <QGraphicsView>
#include <string>

class ListaEnlazadaNoOrdenada;
class ListaEnlazadaOrdenada;
class ArbolB;
class ArbolBPlus;
class ArbolAVL;
class TablaHash;

class ViewController : public QObject
{
    Q_OBJECT
private:
    ListaEnlazadaNoOrdenada* listNoOrd;
    ListaEnlazadaOrdenada* listOrd;
    ArbolB* btree;
    ArbolBPlus* bplustree;
    ArbolAVL* avl;
    TablaHash* hashtable;

    QGraphicsView* viewListaNoOrd;
    QGraphicsView* viewListaOrd;
    QGraphicsView* viewArbolB;
    QGraphicsView* viewArbolBPlus;
    QGraphicsView* viewArbolAVL;
    QGraphicsView* viewHashTable;

    void renderizarYMostrar(const std::string& dotContent, const QString& baseName, QGraphicsView* view);

public:
    ViewController(ListaEnlazadaNoOrdenada* l1, ListaEnlazadaOrdenada* l2, ArbolB* b, ArbolBPlus* bp, ArbolAVL* a, TablaHash* h,
                   QGraphicsView* vl1 = nullptr, QGraphicsView* vl2 = nullptr, 
                   QGraphicsView* vb = nullptr, QGraphicsView* vbp = nullptr, QGraphicsView* va = nullptr, QGraphicsView* vh = nullptr);

    void mostrarListaDesordenada(QGraphicsView *view);
    void mostrarListaOrdenada(QGraphicsView *view);
    void mostrarArbolB(QGraphicsView *view);
    void mostrarArbolBPlus(QGraphicsView *view);
    void mostrarArbolAVL(QGraphicsView *view);
    void mostrarTablaHash(QGraphicsView *view);

public slots:
    void actualizarVista();
    void productoAgregado();
};

#endif // VIEWCONTROLLER_H
