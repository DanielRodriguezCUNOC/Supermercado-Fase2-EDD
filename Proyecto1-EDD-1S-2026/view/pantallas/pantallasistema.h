#ifndef PANTALLASISTEMA_H
#define PANTALLASISTEMA_H
#include "view/pantallas/pantallaagregarproducto.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QList>
#include "model/entidades/product.h"

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
    void mostrarDatosCSV(const QList<Product>& productos);

    void setAppController(AppController *controller);
    
public slots:
    void actualizarTiempos(long ul, long ol, long b, long bp, long avl);

    // Getters para acceder a las QGraphicsViews desde ViewController
    QGraphicsView* getViewListaNoOrdenada();
    QGraphicsView* getViewListaOrdenada();
    QGraphicsView* getViewArbolB();
    QGraphicsView* getViewArbolBPlus();
    QGraphicsView* getViewArbolAVL();

private:
    void conectarPantallasConController();

    Ui::PantallaSistema *ui;
    QGraphicsScene *scene1;
    QGraphicsScene *scene2;
    QGraphicsScene *scene3;
    QGraphicsScene *scene4;
    AppController *appController = nullptr;
    PantallaAgregarProducto *agregarProducto = nullptr;
    PantallaMostrarCSV *mostrarCSV = nullptr;

signals:
    void addProducto();
    void archivoCSVSeleccionado(const QString &ruta);
};

#endif // PANTALLASISTEMA_H
