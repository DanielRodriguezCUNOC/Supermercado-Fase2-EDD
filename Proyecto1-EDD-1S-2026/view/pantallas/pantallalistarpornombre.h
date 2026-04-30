#ifndef PANTALLALISTARPORNOMBRE_H
#define PANTALLALISTARPORNOMBRE_H

#include <QWidget>
#include "entidades/product.h"
#include "estructuras/listagenerica.h"

namespace Ui {
class PantallaListarPorNombre;
}

class PantallaListarPorNombre : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaListarPorNombre(QWidget *parent = nullptr);
    ~PantallaListarPorNombre();

public slots:
    void mostrarResultados(ListaGenerica<Product*>* resultados);
   void on_pushButton_clicked();

signals:
    void listarSolicitado();

private:
    void limpiarResultados();
    Ui::PantallaListarPorNombre *ui;
};

#endif // PANTALLALISTARPORNOMBRE_H
