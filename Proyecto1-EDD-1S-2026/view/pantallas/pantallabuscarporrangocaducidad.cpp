#include "view/pantallas/pantallabuscarporrangocaducidad.h"
#include "ui_pantallabuscarporrangocaducidad.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

PantallaBuscarPorRangoCaducidad::PantallaBuscarPorRangoCaducidad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PantallaBuscarPorRangoCaducidad)
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

PantallaBuscarPorRangoCaducidad::~PantallaBuscarPorRangoCaducidad()
{
    delete ui;
}

void PantallaBuscarPorRangoCaducidad::on_pushButton_clicked()
{
    QString inicio = ui->leInicio->text();
    QString fin = ui->leFinal->text();
    
    if (inicio.isEmpty() || fin.isEmpty()) return;

    limpiarResultados();
    emit buscarSolicitado(inicio, fin);
}

void PantallaBuscarPorRangoCaducidad::limpiarResultados()
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

void PantallaBuscarPorRangoCaducidad::mostrarResultados(ListaGenerica<Product*>* resultados, long tiempo)
{
    if (!resultados) return;

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    if (resultados->isEmpty()) {
        QLabel* lblNo = new QLabel("No se encontraron productos en este rango.", this);
        lblNo->setStyleSheet("font-weight: bold; color: #E74C3C;");
        layout->addWidget(lblNo);
    } else {
        NodoGenerico<Product*>* actual = resultados->getCabeza();
        while (actual) {
            Product* p = actual->valor;
            if (p) {
                QFrame* card = new QFrame(this);
                card->setStyleSheet("background-color: #F8F9F9; color: Black; border: 1px solid #D5DBDB; border-radius: 5px; padding: 5px;");
                
                QVBoxLayout* cardLayout = new QVBoxLayout(card);
                QLabel* lblInfo = new QLabel(QString("<b>%1</b> (%2) - %3")
                                             .arg(QString::fromStdString(p->getName()))
                                             .arg(QString::fromStdString(p->getBarcode()))
                                             .arg(QString::fromStdString(p->getExpiryDate())));
                QLabel* lblDetails = new QLabel(QString("Precio: $%1 | Stock: %2")
                                                 .arg(p->getPrice())
                                                 .arg(p->getStock()));
                
                cardLayout->addWidget(lblInfo);
                cardLayout->addWidget(lblDetails);
                layout->addWidget(card);
            }
            actual = actual->siguiente;
        }
    }

    // Limpiar copias y lista
    NodoGenerico<Product*>* aux = resultados->getCabeza();
    while (aux) {
        delete aux->valor;
        aux = aux->siguiente;
    }
    delete resultados;
}
