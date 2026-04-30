#include "view/pantallas/pantallabuscarpornombre.h"
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include "ui_pantallabuscarpornombre.h"

PantallaBuscarPorNombre::PantallaBuscarPorNombre(QWidget *parent)
    : QWidget(parent), ui(new Ui::PantallaBuscarPorNombre)
{
    ui->setupUi(this);

    // Configurar el scroll area con un layout vertical
    if (ui->scrollAreaWidgetContents->layout() == nullptr)
    {
        QVBoxLayout *layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
        layout->setAlignment(Qt::AlignTop);
        ui->scrollAreaWidgetContents->setLayout(layout);
    }

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

PantallaBuscarPorNombre::~PantallaBuscarPorNombre()
{
    delete ui;
}

void PantallaBuscarPorNombre::on_pushButton_clicked()
{
    QString nombre = ui->lineEdit->text();
    if (nombre.isEmpty())
        return;

    limpiarResultados();
    emit buscarSolicitado(nombre);
}

void PantallaBuscarPorNombre::limpiarResultados()
{
    QLayout *layout = ui->scrollAreaWidgetContents->layout();
    if (layout)
    {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr)
        {
            if (item->widget())
            {
                delete item->widget();
            }
            delete item;
        }
    }
    ui->lblSearchAVL->setText("");
    ui->lblSearchOL->setText("");
    ui->lblSearchUL->setText("");
}

void PantallaBuscarPorNombre::mostrarResultados(ListaGenerica<Product *> *resultados, long tUL, long tOL, long tAVL)
{
    if (!resultados)
        return;

    ui->lblSearchUL->setText(QString("%1 µs").arg(tUL));
    ui->lblSearchOL->setText(QString("%1 µs").arg(tOL));
    ui->lblSearchAVL->setText(QString("%1 µs").arg(tAVL));

    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->scrollAreaWidgetContents->layout());
    if (!layout)
        return;

    if (resultados->isEmpty())
    {
        QLabel *lblNo = new QLabel("No se encontraron productos.", this);
        lblNo->setStyleSheet("font-weight: bold; color: #E74C3C;");
        layout->addWidget(lblNo);
    }
    else
    {
        // Recorremos la lista genérica usando NodoGenerico
        NodoGenerico<Product *> *actual = resultados->getCabeza();
        while (actual)
        {
            Product *p = actual->valor;
            if (p)
            {
                QFrame *card = new QFrame(this);
                card->setStyleSheet("background-color: #F8F9F9; color: Black; border: 1px solid #D5DBDB; border-radius: 5px; padding: 5px;");

                QVBoxLayout *cardLayout = new QVBoxLayout(card);
                QLabel *lblInfo = new QLabel(QString("<b>%1</b> (%2) - %3")
                                                 .arg(QString::fromStdString(p->getName()))
                                                 .arg(QString::fromStdString(p->getBarcode()))
                                                 .arg(QString::fromStdString(p->getBrand())));
                QLabel *lblDetails = new QLabel(QString("Precio: $%1 | Stock: %2 | Categoría: %3")
                                                    .arg(p->getPrice())
                                                    .arg(p->getStock())
                                                    .arg(QString::fromStdString(p->getCategory())));

                cardLayout->addWidget(lblInfo);
                cardLayout->addWidget(lblDetails);
                layout->addWidget(card);
            }
            actual = actual->siguiente;
        }
    }

    // Importante: Borrar la lista temporal de resultados y sus productos
    NodoGenerico<Product *> *aux = resultados->getCabeza();
    while (aux)
    {
        delete aux->valor;
        aux = aux->siguiente;
    }
    delete resultados;
}
