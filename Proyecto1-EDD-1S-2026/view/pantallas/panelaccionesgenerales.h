#ifndef PANELACCIONESGENERALES_H
#define PANELACCIONESGENERALES_H

#include <QWidget>

namespace Ui {
class PanelAccionesGenerales;
}

class PanelAccionesGenerales : public QWidget
{
    Q_OBJECT

public:
    explicit PanelAccionesGenerales(QWidget *parent = nullptr);
    ~PanelAccionesGenerales();
    void configurePanelSize();

signals:
    void agregarClicked();
    void eliminarClicked();
    void buscarPorNombreRequested();
    void buscarPorCodigoRequested();
    void buscarPorCategoriaRequested();
    void buscarPorCaducidadRequested();
    void listarPorNombreRequested();
    void compararBusquedasClicked();
    void verArbolClicked();

private:
    Ui::PanelAccionesGenerales *ui;
};

#endif // PANELACCIONESGENERALES_H
