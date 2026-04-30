#ifndef PANTALLAELIMINAR_H
#define PANTALLAELIMINAR_H

#include <QWidget>

namespace Ui {
class PantallaEliminar;
}

class PantallaEliminar : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaEliminar(QWidget *parent = nullptr);
    ~PantallaEliminar();

private:
    Ui::PantallaEliminar *ui;

private slots:
    void on_btnEliminarConfirmar_clicked();
    void on_btnEliminarCancelar_clicked();

signals:
    void productoEliminado(const QString& barcode);
};

#endif // PANTALLAELIMINAR_H
