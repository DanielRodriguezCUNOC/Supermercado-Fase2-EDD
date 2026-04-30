#include "pantallamostrarcsv.h"
#include "ui_pantallamostrarcsv.h"

PantallaMostrarCSV::PantallaMostrarCSV(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaMostrarCSV)
{
    ui->setupUi(this);
    tablaProductos = new QTableWidget(this);
    ui->scrollArea->setWidget(tablaProductos);
    ui->scrollArea->setWidgetResizable(true);

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

    tablaProductos->setStyleSheet(R"(
        QTableWidget {
            background-color: #2C313A;
            color: #ECF0F1;
            gridline-color: #353B45;
            font-size: 14px;
        }
        QHeaderView::section {
            background-color: #1A252F;
            color: #4DB0F2;
            font-weight: bold;
            font-size: 14px;
            padding: 4px;
            border: 1px solid #2C313A;
        }
        QTableWidget::item {
            color: #ECF0F1;
            background-color: #2C313A;
        }
    )");

}

PantallaMostrarCSV::~PantallaMostrarCSV()
{
    delete ui;
}

void PantallaMostrarCSV::mostrarDatos(const QList<Product>& productos)
{
    tablaProductos->clear();
    tablaProductos->setRowCount(productos.size());
    tablaProductos->setColumnCount(7);
    tablaProductos->setHorizontalHeaderLabels({"Nombre", "Código de Barras", "Categoría", "Fecha de Caducidad", "Marca", "Precio", "Stock"});

    for (int i = 0; i < productos.size(); ++i) {
        const Product& p = productos[i];
        tablaProductos->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(p.getName())));
        tablaProductos->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(p.getBarcode())));
        tablaProductos->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(p.getCategory())));
        tablaProductos->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(p.getExpiryDate())));
        tablaProductos->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(p.getBrand())));
        tablaProductos->setItem(i, 5, new QTableWidgetItem(QString::number(p.getPrice())));
        tablaProductos->setItem(i, 6, new QTableWidgetItem(QString::number(p.getStock())));
    }
}
