#ifndef PANTALLAELIMINARSUCURSAL_H
#define PANTALLAELIMINARSUCURSAL_H

#include <QWidget>

namespace Ui {
class PantallaEliminarSucursal;
}

class PantallaEliminarSucursal : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaEliminarSucursal(QWidget *parent = nullptr);
    ~PantallaEliminarSucursal();

private:
    Ui::PantallaEliminarSucursal *ui;
};

#endif // PANTALLAELIMINARSUCURSAL_H
