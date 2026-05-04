#ifndef PANTALLAMODIFICARSUCURSAL_H
#define PANTALLAMODIFICARSUCURSAL_H

#include <QWidget>

namespace Ui {
class PantallaModificarSucursal;
}

class PantallaModificarSucursal : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaModificarSucursal(QWidget *parent = nullptr);
    ~PantallaModificarSucursal();

private:
    Ui::PantallaModificarSucursal *ui;
};

#endif // PANTALLAMODIFICARSUCURSAL_H
