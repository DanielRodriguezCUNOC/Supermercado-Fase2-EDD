#ifndef PANTALLAMOSTRARCSV_H
#define PANTALLAMOSTRARCSV_H

#include <QWidget>
#include <QTableWidget>
#include <QList>
#include "model/entidades/product.h"

namespace Ui {
class PantallaMostrarCSV;
}

class PantallaMostrarCSV : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaMostrarCSV(QWidget *parent = nullptr);
    ~PantallaMostrarCSV();

    void mostrarDatos(const QList<Product>& productos);

private:
    Ui::PantallaMostrarCSV *ui;
    QTableWidget *tablaProductos;
};

#endif // PANTALLAMOSTRARCSV_H
