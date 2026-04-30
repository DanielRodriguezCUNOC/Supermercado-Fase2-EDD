#ifndef PANTALLABUSCARPORCATEGORIA_H
#define PANTALLABUSCARPORCATEGORIA_H

#include <QWidget>
#include "model/entidades/product.h"
#include "model/estructuras/listagenerica.h"

namespace Ui {
class PantallaBuscarPorCategoria;
}

class PantallaBuscarPorCategoria : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaBuscarPorCategoria(QWidget *parent = nullptr);
    ~PantallaBuscarPorCategoria();

public slots:
    void mostrarResultados(ListaGenerica<Product*>* resultados, long tiempo);

signals:
    void buscarSolicitado(const QString& categoria);

private slots:
    void on_pushButton_clicked();
    void limpiarResultados();

private:
    Ui::PantallaBuscarPorCategoria *ui;
};

#endif // PANTALLABUSCARPORCATEGORIA_H
