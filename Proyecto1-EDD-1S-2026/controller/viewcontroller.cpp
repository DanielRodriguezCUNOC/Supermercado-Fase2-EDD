#include "viewcontroller.h"
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDir>

#include "../model/estructuras/listaenlazadanoordenada.h"
#include "../model/estructuras/listaenlazadaordenada.h"
#include "../model/estructuras/arbolb.h"
#include "../model/estructuras/arbolbplus.h"
#include "../model/estructuras/arbolavl.h"

ViewController::ViewController(ListaEnlazadaNoOrdenada *l1, ListaEnlazadaOrdenada *l2, ArbolB *b, ArbolBPlus *bp, ArbolAVL *a,
                               QGraphicsView *vl1, QGraphicsView *vl2,
                               QGraphicsView *vb, QGraphicsView *vbp, QGraphicsView *va)
    : listNoOrd(l1), listOrd(l2), btree(b), bplustree(bp), avl(a),
      viewListaNoOrd(vl1), viewListaOrd(vl2), viewArbolB(vb), viewArbolBPlus(vbp), viewArbolAVL(va)
{
}

void ViewController::renderizarYMostrar(const std::string &dotContent, const QString &baseName, QGraphicsView *view)
{
    // Crear directorio de salida si no existe
    QDir outputDir("output/trees");
    if (!outputDir.exists())
    {
        QDir().mkpath("output/trees");
    }

    QString dotFile = "output/trees/" + baseName + ".dot";
    QString pngFile = "output/trees/" + baseName + ".png";

    // Guardar archivo .dot
    QFile file(dotFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << QString::fromStdString(dotContent);
        file.close();
    }

    // Compilar con graphviz
    QProcess process;
    process.start("dot", QStringList() << "-Tpng" << dotFile << "-o" << pngFile);
    process.waitForFinished();

    // Renderizar en el QGraphicsView
    QPixmap pixmap(pngFile);
    if (!pixmap.isNull())
    {
        QGraphicsScene *scene = new QGraphicsScene(this);
        scene->addPixmap(pixmap);
        view->setScene(scene);
        view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}

void ViewController::mostrarListaDesordenada(QGraphicsView *view)
{
    if (listNoOrd)
        renderizarYMostrar(listNoOrd->generarDOT(), "lista_no_ord", view);
}

void ViewController::mostrarListaOrdenada(QGraphicsView *view)
{
    if (listOrd)
        renderizarYMostrar(listOrd->generarDOT(), "lista_ord", view);
}

void ViewController::mostrarArbolB(QGraphicsView *view)
{
    if (btree)
        renderizarYMostrar(btree->generarDOT(), "arbol_b", view);
}

void ViewController::mostrarArbolBPlus(QGraphicsView *view)
{
    if (bplustree)
        renderizarYMostrar(bplustree->generarDOT(), "arbol_bplus", view);
}

void ViewController::mostrarArbolAVL(QGraphicsView *view)
{
    if (avl)
        renderizarYMostrar(avl->generarDOT(), "arbol_avl", view);
}

void ViewController::actualizarVista()
{
    if (viewListaNoOrd)
        mostrarListaDesordenada(viewListaNoOrd);
    if (viewListaOrd)
        mostrarListaOrdenada(viewListaOrd);
    if (viewArbolB)
        mostrarArbolB(viewArbolB);
    if (viewArbolBPlus)
        mostrarArbolBPlus(viewArbolBPlus);
    if (viewArbolAVL)
        mostrarArbolAVL(viewArbolAVL);
}

void ViewController::productoAgregado() {}
