#ifndef PANELACCIONESLOGISTICA_H
#define PANELACCIONESLOGISTICA_H

#include <QWidget>

namespace Ui {
class PanelAccionesLogistica;
}

class PanelAccionesLogistica : public QWidget
{
    Q_OBJECT

public:
    explicit PanelAccionesLogistica(QWidget *parent = nullptr);
    ~PanelAccionesLogistica();

signals:
    void cargarConexionesRequested();

private:
    Ui::PanelAccionesLogistica *ui;
};

#endif // PANELACCIONESLOGISTICA_H
