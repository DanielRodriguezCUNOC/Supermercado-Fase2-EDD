#include "pantallaeliminar.h"
#include "ui_pantallaeliminar.h"

PantallaEliminar::PantallaEliminar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaEliminar)
{
    ui->setupUi(this);
    this->setStyleSheet(this->styleSheet() + R"(
        QPushButton {
            background-color: #2ECC71;
            color: #ECF0F1;
            border-radius: 8px;
            padding: 5px 10px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #27AE60;
        }
        QPushButton:pressed {
            background-color: #1E8449;
        }
    )");

}

PantallaEliminar::~PantallaEliminar()
{
    delete ui;
}

void PantallaEliminar::on_btnEliminarConfirmar_clicked()
{
    QString barcode = ui->txtBarcodeEliminar->text();
    if (barcode.isEmpty()) return;

    emit productoEliminado(barcode);
    ui->txtBarcodeEliminar->clear();
}

void PantallaEliminar::on_btnEliminarCancelar_clicked()
{
    ui->txtBarcodeEliminar->clear();
}
