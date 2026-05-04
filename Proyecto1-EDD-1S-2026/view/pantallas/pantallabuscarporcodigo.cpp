#include "pantallabuscarporcodigo.h"
#include "ui_pantallabuscarporcodigo.h"
#include "model/entidades/product.h"
#include <QLabel>
#include <QVBoxLayout>

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

    connect(ui->btnBuscar, &QPushButton::clicked, [this]() {
        emit buscarRequested(ui->leCodigo->text());
    });
}

PantallaBuscarPorCodigo::~PantallaBuscarPorCodigo()
{
    delete ui;
}


void PantallaBuscarPorCodigo::mostrarResultado(Product* p) {
    if (ui->scrollArea->widget()) {
        delete ui->scrollArea->widget();
    }

    QWidget* contenedor = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(contenedor);

    if (p == nullptr) {
        QLabel* label = new QLabel("No se encontró ningún producto con ese código.");
        label->setStyleSheet("color: red; font-weight: bold; font-size: 16px;");
        layout->addWidget(label);
    } else {
        QString info = QString(
            "<b>Nombre:</b> %1<br>"
            "<b>Código:</b> %2<br>"
            "<b>Categoría:</b> %3<br>"
            "<b>Marca:</b> %4<br>"
            "<b>Precio:</b> Q%5<br>"
            "<b>Stock:</b> %6<br>"
            "<b>Caducidad:</b> %7<br>"
        ).arg(QString::fromStdString(p->name))
         .arg(QString::fromStdString(p->barcode))
         .arg(QString::fromStdString(p->category))
         .arg(QString::fromStdString(p->brand))
         .arg(p->price)
         .arg(p->stock)
         .arg(QString::fromStdString(p->expiry_date));

        QLabel* label = new QLabel(info);
        label->setStyleSheet("color: #2C3E50; font-size: 16px;");
        layout->addWidget(label);
    }

    ui->scrollArea->setWidget(contenedor);
}
