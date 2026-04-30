#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include "estructurascontroller.h"
#include "filecontroller.h"
#include "viewcontroller.h"
#include "../view/pantallas/pantallasistema.h"

class AppController : public QObject
{
    Q_OBJECT

private:
    PantallaSistema* vistaSistema;
    EstructurasController* estructurasController;
    FileController* fileController;
    ViewController* viewController;

private slots:
    // Slot intermedio para manejar la interacción de la vista
    void onAgregarProducto();

public:
    AppController(QObject *parent = nullptr);
    ~AppController();

    // Método para arrancar la aplicación
    void iniciar();

public slots:
    void agregarProducto(const QString& nombre, const QString& codigoBarra, const QString& categoria,
                         const QDate& fechaCaducidad, const QString& marca, double precio, int stock);
    void eliminarProducto(const QString& barcode);
    void cargarArchivoCSV(const QString &ruta);
    void buscarPorNombre(const QString& nombre);
    void buscarPorCategoria(const QString& categoria);
    void buscarPorRangoCaducidad(const QString& inicio, const QString& fin);
    void listarPorNombre();

signals:
    void resultadosBusquedaNombre(ListaGenerica<Product*>* resultados, long tUL, long tOL, long tAVL);
    void resultadosBusquedaCategoria(ListaGenerica<Product*>* resultados, long tiempo);
    void resultadosBusquedaRango(ListaGenerica<Product*>* resultados, long tiempo);
    void resultadosListadoNombre(ListaGenerica<Product*>* resultados);
};

#endif // APPCONTROLLER_H
