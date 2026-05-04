#include "pantallamodificarsucursal.h"
#include "ui_pantallamodificarsucursal.h"

PantallaModificarSucursal::PantallaModificarSucursal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaModificarSucursal)
{
    ui->setupUi(this);

    QString labelStyle = "color: #ECF0F1; font-size: 18px; font-weight: bold;";
    QString buttonStyle = R"(
        QPushButton {
            background-color: #2ECC71;
            color: #ECF0F1;
            border-radius: 8px;
            padding: 8px 16px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover { background-color: #27AE60; }
        QPushButton:pressed { background-color: #1E8449; }
    )";
    QString lineEditStyle = "background-color: white; color: #2C3E50; border-radius: 4px; padding: 4px;";

    ui->label->setStyleSheet(labelStyle);
    ui->label_2->setStyleSheet(labelStyle);
    ui->label_3->setStyleSheet(labelStyle);
    ui->label_4->setStyleSheet(labelStyle);
    ui->label_5->setStyleSheet(labelStyle);
    ui->label_6->setStyleSheet(labelStyle);

    ui->btnActualizar->setStyleSheet(buttonStyle);

    ui->leSucursalObjetivo->setStyleSheet(lineEditStyle);
    ui->leNuevaUbicacion->setStyleSheet(lineEditStyle);
    ui->leNuevoTiempoIngreso->setStyleSheet(lineEditStyle);
    ui->leNuevoTiempoTraspaso->setStyleSheet(lineEditStyle);
    ui->leNuevoTiempoDespacho->setStyleSheet(lineEditStyle);
    ui->leNuevoNombre->setStyleSheet(lineEditStyle);
}

PantallaModificarSucursal::~PantallaModificarSucursal()
{
    delete ui;
}
