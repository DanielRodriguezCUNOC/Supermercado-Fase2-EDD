#include "pantallaeliminarsucursal.h"
#include "ui_pantallaeliminarsucursal.h"

PantallaEliminarSucursal::PantallaEliminarSucursal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaEliminarSucursal)
{
    ui->setupUi(this);

    QString labelStyle = "color: #ECF0F1; font-size: 18px; font-weight: bold;";
    QString buttonStyle = R"(
        QPushButton {
            background-color: #E74C3C;
            color: #ECF0F1;
            border-radius: 8px;
            padding: 8px 16px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover { background-color: #C0392B; }
        QPushButton:pressed { background-color: #A93226; }
    )";
    QString lineEditStyle = "background-color: white; color: #2C3E50; border-radius: 4px; padding: 4px;";

    ui->label->setStyleSheet(labelStyle);
    ui->btnEliminar->setStyleSheet(buttonStyle);
    ui->leSucursal->setStyleSheet(lineEditStyle);
}

PantallaEliminarSucursal::~PantallaEliminarSucursal()
{
    delete ui;
}
