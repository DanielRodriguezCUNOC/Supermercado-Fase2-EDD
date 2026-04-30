#include "pantallabuscarporcategoria.h"
#include "ui_pantallabuscarporcategoria.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

PantallaBuscarPorCategoria::PantallaBuscarPorCategoria(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PantallaBuscarPorCategoria)
{
    ui->setupUi(this);

    // Configurar scroll area
    if (ui->scrollAreaWidgetContents->layout() == nullptr) {
        QVBoxLayout* layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
        layout->setAlignment(Qt::AlignTop);
        ui->scrollAreaWidgetContents->setLayout(layout);
    }
}

PantallaBuscarPorCategoria::~PantallaBuscarPorCategoria()
{
    delete ui;
}

void PantallaBuscarPorCategoria::on_pushButton_clicked()
{
    QString categoria = ui->lineEdit->text();
    if (categoria.isEmpty()) return;

    limpiarResultados();
    emit buscarSolicitado(categoria);
}

void PantallaBuscarPorCategoria::limpiarResultados()
{
    QLayout* layout = ui->scrollAreaWidgetContents->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            if (item->widget()) delete item->widget();
            delete item;
        }
    }
    ui->lineEdit->setText("");
}

void PantallaBuscarPorCategoria::mostrarResultados(ListaGenerica<Product*>* resultados, long tiempo)
{
    if (!resultados) return;

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    if (!layout) return;

    if (resultados->isEmpty()) {
        QLabel* lblNo = new QLabel("No se encontraron productos en esta categoría.", this);
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
                                             .arg(QString::fromStdString(p->getBrand())));
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

    // limpiar memoria porque si no se hace, se acumulan los productos y se traga la RAM
    NodoGenerico<Product*>* aux = resultados->getCabeza();
    while (aux) {
        delete aux->valor;
        aux = aux->siguiente;
    }
    delete resultados;
}
