#ifndef PANTALLASISTEMA_H
#define PANTALLASISTEMA_H
#include "view/pantallas/pantallaagregarproducto.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QList>
#include "model/entidades/product.h"
#include "view/pantallas/pantallabuscarporcodigo.h"
#include "view/pantallas/pantallamodificarsucursal.h"
#include "view/pantallas/pantallaeliminarsucursal.h"

namespace Ui
{
    class PantallaSistema;
}

class AppController;
class PantallaMostrarCSV;

class PantallaSistema : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaSistema(QWidget *parent = nullptr);
    ~PantallaSistema();

    void actualizarReloj();
    void mostrarArboles();
    void btnAgregarClicked();
    void inicializarPantallas();
    void mostrarDatosCSV(const QList<Product> &productos);

    void setAppController(AppController *controller);

public slots:
    void actualizarTiempos(long ul, long ol, long b, long bp, long avl, long hash);

    // Getters para acceder a las QGraphicsViews desde ViewController
    QGraphicsView *getViewListaNoOrdenada();
    QGraphicsView *getViewListaOrdenada();
    QGraphicsView *getViewArbolB();
    QGraphicsView *getViewArbolBPlus();
    QGraphicsView *getViewArbolAVL();
    QGraphicsView *getViewHashTable();
    QGraphicsView *getViewGrafo();

private:
    void conectarPantallasConController();

    Ui::PantallaSistema *ui;
    QGraphicsScene *scene1;
    QGraphicsScene *scene2;
    QGraphicsScene *scene3;
    QGraphicsScene *scene4;
    AppController *appController = nullptr;
    PantallaAgregarProducto *agregarProducto = nullptr;
    PantallaBuscarPorCodigo *buscarPorCodigo = nullptr;
    PantallaModificarSucursal *modificarSucursal = nullptr;
    PantallaEliminarSucursal *eliminarSucursal = nullptr;
    PantallaMostrarCSV *mostrarCSV = nullptr;
    class PanelAccionesGenerales *panelAccionesGenerales = nullptr;
    class PanelAccioneSucursales *panelAccionesSucursales = nullptr;
    class PanelAccionesLogistica *panelAccionesLogistica = nullptr;

signals:
    void addProducto();
    void archivoCSVSeleccionado(const QString &ruta);
    void sucursalesCSVSeleccionado(const QString &ruta);
    void conexionesCSVSeleccionado(const QString &ruta);
    void catalogoCSVSeleccionado(const QString &ruta);
};

#endif // PANTALLASISTEMA_H
