#include "pantallabuscarporcodigo.h"
#include "ui_pantallabuscarporcodigo.h"

PantallaBuscarPorCodigo::PantallaBuscarPorCodigo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaBuscarPorCodigo)
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

    ui->label->setStyleSheet(labelStyle);
    ui->btnBuscar->setStyleSheet(buttonStyle);
    ui->leCodigo->setStyleSheet("background-color: white; color: #2C3E50; border-radius: 4px; padding: 4px;");
}

PantallaBuscarPorCodigo::~PantallaBuscarPorCodigo()
{
    delete ui;
}
