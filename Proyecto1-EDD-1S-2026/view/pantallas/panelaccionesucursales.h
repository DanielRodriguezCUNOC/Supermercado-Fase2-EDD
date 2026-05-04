#ifndef PANELACCIONESUCURSALES_H
#define PANELACCIONESUCURSALES_H

#include <QWidget>

namespace Ui {
class PanelAccioneSucursales;
}

class PanelAccioneSucursales : public QWidget
{
    Q_OBJECT

public:
    explicit PanelAccioneSucursales(QWidget *parent = nullptr);
    ~PanelAccioneSucursales();

signals:
    void modifySucursalRequested();
    void eliminarSucursalRequested();
    void cargarSucursalesRequested();

private:
    Ui::PanelAccioneSucursales *ui;
};

#endif // PANELACCIONESUCURSALES_H
