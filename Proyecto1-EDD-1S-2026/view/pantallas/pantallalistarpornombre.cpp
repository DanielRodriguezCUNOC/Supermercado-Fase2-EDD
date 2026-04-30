#include "pantallalistarpornombre.h"
#include "ui_pantallalistarpornombre.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

PantallaListarPorNombre::PantallaListarPorNombre(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaListarPorNombre)
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

    if (ui->scrollAreaWidgetContents->layout() == nullptr) {
        QVBoxLayout* layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
        layout->setAlignment(Qt::AlignTop);
        ui->scrollAreaWidgetContents->setLayout(layout);
    }
}

PantallaListarPorNombre::~PantallaListarPorNombre()
{
    delete ui;
}

void PantallaListarPorNombre::limpiarResultados()
{
    QLayout* layout = ui->scrollAreaWidgetContents->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            if (item->widget()) delete item->widget();
            delete item;
        }
    }
}

void PantallaListarPorNombre::mostrarResultados(ListaGenerica<Product*>* resultados)
{
    if (!resultados) return;

    limpiarResultados();

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    if (resultados->isEmpty()) {
        QLabel* lblNo = new QLabel("No hay productos registrados.", this);
        lblNo->setStyleSheet("font-weight: bold; color: #E74C3C;");
        layout->addWidget(lblNo);
    } else {
        NodoGenerico<Product*>* actual = resultados->getCabeza();
        while (actual) {
            Product* p = actual->valor;
            if (p) {
                QFrame* card = new QFrame(this);
                card->setStyleSheet("background-color: #F8F9F9; border: 1px solid #D5DBDB; border-radius: 5px; padding: 5px;");
                
                QVBoxLayout* cardLayout = new QVBoxLayout(card);
                QLabel* lblInfo = new QLabel(QString("<b>%1</b> (%2) - %3")
                                             .arg(QString::fromStdString(p->getName()))
                                             .arg(QString::fromStdString(p->getBarcode()))
                                             .arg(QString::fromStdString(p->getBrand())));
                QLabel* lblDetails = new QLabel(QString("Precio: $%1 | Stock: %2 | Caducidad: %3")
                                                 .arg(p->getPrice())
                                                 .arg(p->getStock())
                                                 .arg(QString::fromStdString(p->getExpiryDate())));
                
                cardLayout->addWidget(lblInfo);
                cardLayout->addWidget(lblDetails);
                layout->addWidget(card);
            }
            actual = actual->siguiente;
        }
    }

    // Limpiar copias
    NodoGenerico<Product*>* aux = resultados->getCabeza();
    while (aux) {
        delete aux->valor;
        aux = aux->siguiente;
    }
    delete resultados;
}

void PantallaListarPorNombre::on_pushButton_clicked()
{
    emit listarSolicitado();
}
