#include "panelaccionesgenerales.h"
#include "view/pantallas/ui_panelaccionesgenerales.h"
#include <QPushButton>

PanelAccionesGenerales::PanelAccionesGenerales(QWidget *parent)
    : QWidget(parent), ui(new Ui::PanelAccionesGenerales)
{
    ui->setupUi(this);

    this->setStyleSheet(R"(

QPushButton {
    background-color: #2ECC71;
    color: #ECF0F1;
    border-radius: 8px;
    padding: 5px 10px;
    font-size: 18px;
    font-weight: bold;

    min-width: 141px;
    max-width: 141px;

    min-height: 81px;
    max-height: 81px;
}

QPushButton:hover {
    background-color: #27AE60;
}

QPushButton:pressed {
    background-color: #1E8449;
}

QPushButton#btnEliminar {
    background-color: #E74C3C;
    font-size: 28px;
}

QPushButton#btnEliminar:hover {
    background-color: #ff3923;
}

QPushButton#btnEliminar:pressed {
    background-color: #D4210D;
}

QPushButton#btnAgregar {
    background-color:#2ECC71;
    font-size: 28px;
}

QPushButton#btnAgregar:hover {
    background-color: #2FA35F;
}

QPushButton#btnAgregar:pressed {
    background-color: #1E8449;
}

QPushButton#btnCargarArchivo {
    background-color: #FFAA4F;
    font-size: 18px;

    min-width: 160px;
    max-width: 180px;

    min-height: 65px;
    max-height: 70px;
}

QPushButton#btnCargarArchivo:hover {
    background-color: #F78B17;
}

QPushButton#btnCargarArchivo:pressed {
    background-color: #E67C0B;
}

QPushButton#btnVerArbol {
    background-color: #4DB0F2;
    font-size: 18px;

    min-width: 180px;
    max-width: 200px;

    min-height: 65px;
    max-height: 70px;
}

QPushButton#btnVerArbol:hover {
    background-color: #1DA2FA;
}

QPushButton#btnVerArbol:pressed {
    background-color: #0C88DB;
}





)");
    auto findBtn = [this](const QString &baseName)
    {
        QPushButton *b = this->findChild<QPushButton *>(baseName);
        if (!b)
            b = this->findChild<QPushButton *>(baseName + "_2");
        return b;
    };

    if (auto b = findBtn("btnBuscarPorNombre"))
        b->setText("Buscar\npor\nNombre");
    if (auto b = findBtn("btnBuscarPorCodigo"))
        b->setText("Buscar\npor\nCodigo");
    if (auto b = findBtn("btnBuscarPorCategoria"))
        b->setText("Buscar\npor\nCategoria");
    if (auto b = findBtn("btnBuscarPorCaducidad"))
        b->setText("Buscar\npor\nCaducidad");
    if (auto b = findBtn("btnListarPorNombre"))
        b->setText("Listar\npor\nNombre");
    if (auto b = findBtn("btnCompararBusquedas"))
        b->setText("Comparar\nBusquedas");
    if (auto b = findBtn("btnCargarArchivo"))
        b->setText("Cargar Archivo");
    if (auto b = findBtn("btnVerArbol"))
        b->setText("Visualizar Arboles");

    if (auto b = findBtn("btnRegresar"))
        b->setStyleSheet("background-color: #FFAA4F; color: #ECF0F1; border-radius: 8px; font-weight: bold;");
    if (auto b = findBtn("btnEliminar"))
        b->setStyleSheet("background-color: #E74C3C; color: #ECF0F1; border-radius: 8px; font-weight: bold;");
    if (auto b = findBtn("btnAgregar"))
        b->setStyleSheet("background-color: #2ECC71; color: #ECF0F1; border-radius: 8px; font-weight: bold;");

    this->setFixedSize(1920, 160);
    QStringList orden = {
        "btnRegresar",
        "btnAgregar",
        "btnEliminar",
        "btnBuscarPorNombre",
        "btnBuscarPorCodigo",
        "btnBuscarPorCategoria",
        "btnBuscarPorCaducidad",
        "btnListarPorNombre",
        "btnCompararBusquedas",
        "btnVerArbol",
        "btnCargarArchivo"};

    int x = 40;
    const int y = 40;
    const int w = 141;
    const int h = 81;
    const int gap = 30;

    for (const QString &base : orden)
    {
        QPushButton *b = findBtn(base);
        if (b)
        {
            b->setGeometry(x, y, w, h);
            b->setVisible(true);
            x += w + gap;
        }
    }

    if (auto b = findBtn("btnAgregar"))
        connect(b, &QPushButton::clicked, this, &PanelAccionesGenerales::agregarClicked);
    if (auto b = findBtn("btnEliminar"))
        connect(b, &QPushButton::clicked, this, &PanelAccionesGenerales::eliminarClicked);
    if (auto b = findBtn("btnBuscarPorNombre"))
        connect(b, &QPushButton::clicked, this, &PanelAccionesGenerales::buscarPorNombreRequested);
    if (auto b = findBtn("btnBuscarPorCodigo"))
        connect(b, &QPushButton::clicked, this, &PanelAccionesGenerales::buscarPorCodigoRequested);
    if (auto b = findBtn("btnBuscarPorCategoria"))
        connect(b, &QPushButton::clicked, this, &PanelAccionesGenerales::buscarPorCategoriaRequested);
    if (auto b = findBtn("btnBuscarPorCaducidad"))
        connect(b, &QPushButton::clicked, this, &PanelAccionesGenerales::buscarPorCaducidadRequested);
    if (auto b = findBtn("btnListarPorNombre"))
        connect(b, &QPushButton::clicked, this, &PanelAccionesGenerales::listarPorNombreRequested);
    if (auto b = findBtn("btnCompararBusquedas"))
        connect(b, &QPushButton::clicked, this, &PanelAccionesGenerales::compararBusquedasClicked);
    if (auto b = findBtn("btnVerArbol"))
        connect(b, &QPushButton::clicked, this, &PanelAccionesGenerales::verArbolClicked);
}

PanelAccionesGenerales::~PanelAccionesGenerales()
{
    delete ui;
}
