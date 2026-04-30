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

class ViewController : public QObject
{
    Q_OBJECT
private:
    ListaEnlazadaNoOrdenada* listNoOrd;
    ListaEnlazadaOrdenada* listOrd;
    ArbolB* btree;
    ArbolBPlus* bplustree;
    ArbolAVL* avl;

    QGraphicsView* viewListaNoOrd;
    QGraphicsView* viewListaOrd;
    QGraphicsView* viewArbolB;
    QGraphicsView* viewArbolBPlus;
    QGraphicsView* viewArbolAVL;

    void renderizarYMostrar(const std::string& dotContent, const QString& baseName, QGraphicsView* view);

public:
    ViewController(ListaEnlazadaNoOrdenada* l1, ListaEnlazadaOrdenada* l2, ArbolB* b, ArbolBPlus* bp, ArbolAVL* a,
                   QGraphicsView* vl1 = nullptr, QGraphicsView* vl2 = nullptr, 
                   QGraphicsView* vb = nullptr, QGraphicsView* vbp = nullptr, QGraphicsView* va = nullptr);

    void mostrarListaDesordenada(QGraphicsView *view);
    void mostrarListaOrdenada(QGraphicsView *view);
    void mostrarArbolB(QGraphicsView *view);
    void mostrarArbolBPlus(QGraphicsView *view);
    void mostrarArbolAVL(QGraphicsView *view);

public slots:
    void actualizarVista();
    void productoAgregado();
};

#endif // VIEWCONTROLLER_H
