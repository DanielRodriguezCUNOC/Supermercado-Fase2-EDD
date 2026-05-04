#ifndef ESTRUCTURASCONTROLLER_H
#define ESTRUCTURASCONTROLLER_H
#include <QObject>
#include "estructuras/arbolavl.h"
#include "estructuras/arbolb.h"
#include "estructuras/arbolbplus.h"
#include "estructuras/listaenlazadanoordenada.h"
#include "estructuras/listaenlazadaordenada.h"
#include "estructuras/tablahash.h"
class EstructurasController : public QObject
{
    Q_OBJECT
private:
    ListaEnlazadaNoOrdenada *unorderedList;
    ListaEnlazadaOrdenada *listaOrdenada;
    ArbolB *arbolB;
    ArbolBPlus *arbolBPlus;
    ArbolAVL *arbolAVL;
    TablaHash *hashTable;
    long acumuladoUL = 0;
    long acumuladoOL = 0;
    long acumuladoB = 0;
    long acumuladoBPlus = 0;
    long acumuladoAVL = 0;
    long acumuladoHash = 0;

public:
    EstructurasController();
    EstructurasController(
        ListaEnlazadaNoOrdenada *unorderedList,
        ListaEnlazadaOrdenada *listaOrdenada,
        ArbolB *arbolB,
        ArbolBPlus *arbolBPlus,
        ArbolAVL *arbolAVL,
        TablaHash *hashTable);

    ListaEnlazadaNoOrdenada* getUnorderedList() const { return unorderedList; }
    ListaEnlazadaOrdenada* getListaOrdenada() const { return listaOrdenada; }
    ArbolB* getArbolB() const { return arbolB; }
    ArbolBPlus* getArbolBPlus() const { return arbolBPlus; }
    ArbolAVL* getArbolAVL() const { return arbolAVL; }
    TablaHash* getHashTable() const { return hashTable; }

public slots:
    void agregarProducto(std::string name,
                         std::string barcode,
                         std::string category,
                         std::string expiry_date,
                         std::string brand,
                         double price,
                         int stock,
                         bool emitirSenal = true);
    void actualizarVistas();
    void reiniciarTiemposAcumulados();
    void emitirTiemposAcumulados();
    void eliminarProducto(std::string barcode);
    ListaGenerica<Product*>* buscarPorNombre(const std::string& nombre, long& tUL, long& tOL, long& tAVL);
    ListaGenerica<Product*>* buscarPorCategoria(const std::string& categoria, long& tiempo);
    ListaGenerica<Product*>* buscarPorRangoCaducidad(const std::string& inicio, const std::string& fin, long& tiempo);
    Product* buscarPorCodigo(const std::string& barcode, long& tiempo);
    void listarPorNombre(ListaGenerica<Product*>* resultados);
signals:
    void etructurasActualizadas();
    void tiemposCalculados(long ul, long ol, long b, long bp, long avl, long hash);
};

#endif // ESTRUCTURASCONTROLLER_H
