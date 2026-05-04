#include "panelaccioneslogistica.h"
#include "ui_panelaccioneslogistica.h"
#include <QPushButton>

PanelAccionesLogistica::PanelAccionesLogistica(QWidget *parent)
    : QWidget(parent), ui(new Ui::PanelAccionesLogistica)
{
    ui->setupUi(this);
    connect(ui->btnCargarConexiones, &QPushButton::clicked, this, &PanelAccionesLogistica::cargarConexionesRequested);

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

QPushButton#btnRegresar { background-color: #FFAA4F; }
)");

    this->setFixedSize(1920, 160);

    auto findBtn = [this](const QString &baseName)
    {
        QPushButton *b = this->findChild<QPushButton *>(baseName);
        if (!b)
            b = this->findChild<QPushButton *>(baseName + "_2");
        return b;
    };

    QStringList orden = {"btnRegresar", "btnReporteColision", "pushButton", "btnSimularTraslado"};
    int x = 40;
    const int y = 40;
    const int w = 141;
    const int h = 81;
    const int gap = 40;
    for (const QString &name : orden)
    {
        if (QPushButton *b = findBtn(name))
        {
            b->setGeometry(x, y, w, h);
            b->setVisible(true);
            if (name == "btnRegresar") {
                b->setStyleSheet("background-color: #FFAA4F; color: #ECF0F1; border-radius: 8px; font-weight: bold;");
            }
            x += w + gap;
        }
    }
}

PanelAccionesLogistica::~PanelAccionesLogistica()
{
    delete ui;
}
