#ifndef PANTALLAAGREGARPRODUCTO_H
#define PANTALLAAGREGARPRODUCTO_H

#include <QWidget>

namespace Ui {
class PantallaAgregarProducto;
}

class PantallaAgregarProducto : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaAgregarProducto(QWidget *parent = nullptr);
    ~PantallaAgregarProducto();

private:
    Ui::PantallaAgregarProducto *ui;

private slots:
    void on_btnInsertar_clicked();
    void on_btnCancelar_clicked();

signals:
    void productoAgregado(const QString& nombre, const QString& codigoBarra, const QString& categoria,
                          const QDate& fechaCaducidad, const QString& marca, double precio, int stock);
};

#endif // PANTALLAAGREGARPRODUCTO_H
