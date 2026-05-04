#ifndef PANTALLABUSCARPORCODIGO_H
#define PANTALLABUSCARPORCODIGO_H

#include <QWidget>

namespace Ui {
class PantallaBuscarPorCodigo;
}

class PantallaBuscarPorCodigo : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaBuscarPorCodigo(QWidget *parent = nullptr);
    ~PantallaBuscarPorCodigo();

public slots:
    void mostrarResultado(class Product* p);

signals:
    void buscarRequested(const QString& barcode);

private:
    Ui::PantallaBuscarPorCodigo *ui;
};

#endif // PANTALLABUSCARPORCODIGO_H
