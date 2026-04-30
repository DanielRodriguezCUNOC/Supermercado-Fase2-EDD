#include "appcontroller.h"
#include <QDebug>
#include <qdatetime.h>

AppController::AppController(QObject *parent) : QObject(parent)
{
    // Instanciar Controladores secundarios
    estructurasController = new EstructurasController();
    fileController = new FileController();

    // Instanciar la Vista Principal
    vistaSistema = new PantallaSistema();
    vistaSistema->setAppController(this);

    // Inyección de dependencias al ViewController con las estructuras y las QGraphicsViews
    viewController = new ViewController(
        estructurasController->getUnorderedList(),
        estructurasController->getListaOrdenada(),
        estructurasController->getArbolB(),
        estructurasController->getArbolBPlus(),
        estructurasController->getArbolAVL(),
        vistaSistema->getViewListaNoOrdenada(),
        vistaSistema->getViewListaOrdenada(),
        vistaSistema->getViewArbolB(),
        vistaSistema->getViewArbolBPlus(),
        vistaSistema->getViewArbolAVL());

    connect(vistaSistema, &PantallaSistema::archivoCSVSeleccionado,
            this, &AppController::cargarArchivoCSV);

    // Conectar actualización de estructuras con viewController para renderizar
    connect(estructurasController, &EstructurasController::etructurasActualizadas,
            viewController, &ViewController::actualizarVista);

    // Conectar tiempos calculados con la vista principal
    connect(estructurasController, &EstructurasController::tiemposCalculados,
            vistaSistema, &PantallaSistema::actualizarTiempos);
}

AppController::~AppController()
{
    delete vistaSistema;
    delete estructurasController;
    delete fileController;
    delete viewController;
}

void AppController::iniciar()
{
    // Mostrar la pantalla principal
    if (vistaSistema)
    {
        vistaSistema->show();
    }
}

void AppController::onAgregarProducto()
{
    qDebug() << "AppController: Señal de agregarProducto recibida de la vista.";
}

void AppController::agregarProducto(const QString &nombre, const QString &codigoBarra, const QString &categoria,
                                    const QDate &fechaCaducidad, const QString &marca, double precio, int stock)
{
    estructurasController->agregarProducto(
        nombre.toStdString(),
        codigoBarra.toStdString(),
        categoria.toStdString(),
        fechaCaducidad.toString("yyyy-MM-dd").toStdString(),
        marca.toStdString(),
        precio,
        stock);
}

void AppController::eliminarProducto(const QString &barcode)
{
    estructurasController->eliminarProducto(barcode.toStdString());
}

void AppController::cargarArchivoCSV(const QString &ruta)
{
    const QList<Product> productos = fileController->cargarCSV(ruta);
    qDebug() << "CSV cargado:" << ruta << "Productos válidos:" << productos.size();

    estructurasController->reiniciarTiemposAcumulados();

    for (const Product &p : productos)
    {
        estructurasController->agregarProducto(
            p.getName(),
            p.getBarcode(),
            p.getCategory(),
            p.getExpiryDate(),
            p.getBrand(),
            p.getPrice(),
            p.getStock(),
            false); // No emitir señal por cada producto
    }

    estructurasController->emitirTiemposAcumulados();
    
    // Actualizar arboles de una sola vez XD
    estructurasController->actualizarVistas();
    
    // Pasar los datos cargados a la tabla qlera
    vistaSistema->mostrarDatosCSV(productos);
}

void AppController::buscarPorNombre(const QString& nombre)
{
    long tUL = 0, tOL = 0, tAVL = 0;
    ListaGenerica<Product*>* resultados = estructurasController->buscarPorNombre(nombre.toStdString(), tUL, tOL, tAVL);
    emit resultadosBusquedaNombre(resultados, tUL, tOL, tAVL);
}

void AppController::buscarPorCategoria(const QString& categoria)
{
    long tiempo = 0;
    ListaGenerica<Product*>* resultados = estructurasController->buscarPorCategoria(categoria.toStdString(), tiempo);
    emit resultadosBusquedaCategoria(resultados, tiempo);
}

void AppController::buscarPorRangoCaducidad(const QString& inicio, const QString& fin)
{
    long tiempo = 0;
    ListaGenerica<Product*>* resultados = estructurasController->buscarPorRangoCaducidad(
        inicio.toStdString(), fin.toStdString(), tiempo);
    emit resultadosBusquedaRango(resultados, tiempo);
}

void AppController::listarPorNombre()
{
    ListaGenerica<Product*>* resultados = new ListaGenerica<Product*>();
    estructurasController->listarPorNombre(resultados);
    emit resultadosListadoNombre(resultados);
}
