#include "pantallaagregarproducto.h"
#include "ui_pantallaagregarproducto.h"
#include <qdatetime.h>

PantallaAgregarProducto::PantallaAgregarProducto(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaAgregarProducto)
{
    ui->setupUi(this);
    this->setStyleSheet(this->styleSheet() + ";\n" + R"(
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

PantallaAgregarProducto::~PantallaAgregarProducto()
{
    delete ui;
}

void PantallaAgregarProducto::on_btnInsertar_clicked()
{
    QString nombre = ui->txtNombre->text();
    QString barcode = ui->txtBarcode->text();
    QString categoria = ui->txtCategoria->text();
    QString fechaStr = ui->txtFecha->text();
    QString marca = ui->txtMarca->text();
    double precio = ui->txtPrecio->text().toDouble();
    int stock = ui->txtStock->text().toInt();

    if (nombre.isEmpty() || barcode.isEmpty() || categoria.isEmpty()) return;

    QDate fecha = QDate::fromString(fechaStr, "yyyy-MM-dd");

    emit productoAgregado(nombre, barcode, categoria, fecha, marca, precio, stock);

    // Limpiar campos
    ui->txtNombre->clear();
    ui->txtBarcode->clear();
    ui->txtCategoria->clear();
    ui->txtFecha->clear();
    ui->txtMarca->clear();
    ui->txtPrecio->clear();
    ui->txtStock->clear();
}

void PantallaAgregarProducto::on_btnCancelar_clicked()
{
    ui->txtNombre->clear();
    ui->txtBarcode->clear();
    ui->txtCategoria->clear();
    ui->txtFecha->clear();
    ui->txtMarca->clear();
    ui->txtPrecio->clear();
    ui->txtStock->clear();
}
