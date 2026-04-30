#ifndef PANTALLABUSCARPORRANGOCADUCIDAD_H
#define PANTALLABUSCARPORRANGOCADUCIDAD_H

#include <QWidget>
#include "model/entidades/product.h"
#include "model/estructuras/listagenerica.h"

namespace Ui {
class PantallaBuscarPorRangoCaducidad;
}

class PantallaBuscarPorRangoCaducidad : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaBuscarPorRangoCaducidad(QWidget *parent = nullptr);
    ~PantallaBuscarPorRangoCaducidad();

public slots:
    void mostrarResultados(ListaGenerica<Product*>* resultados, long tiempo);

signals:
    void buscarSolicitado(const QString& inicio, const QString& fin);

private slots:
    void on_pushButton_clicked();
    void limpiarResultados();

private:
    Ui::PantallaBuscarPorRangoCaducidad *ui;
};

#endif // PANTALLABUSCARPORRANGOCADUCIDAD_H
