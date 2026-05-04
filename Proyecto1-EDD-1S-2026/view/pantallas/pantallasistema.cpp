#include "pantallasistema.h"
#include "ui_pantallasistema.h"
#include "view/pantallas/pantallaeliminar.h"
#include "view/pantallas/pantallabuscarporcategoria.h"
#include "view/pantallas/pantallabuscarpornombre.h"
#include "view/pantallas/pantallabuscarporrangocaducidad.h"
#include "view/pantallas/pantallalistarpornombre.h"
#include "view/pantallas/pantallamostrarcsv.h"
#include "controller/appcontroller.h"
#include "view/pantallas/panelaccionesgenerales.h"
#include "view/pantallas/panelaccionesucursales.h"
#include "view/pantallas/panelaccioneslogistica.h"
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QFileDialog>
#include <QPushButton>

PantallaSistema::PantallaSistema(QWidget *parent)
    : QWidget(parent), ui(new Ui::PantallaSistema)
{
    ui->setupUi(this);

    if (ui->panelAcciones)
    {
        ui->panelAcciones->setStyleSheet(R"(
QFrame#panelAcciones {
    background: qlineargradient(
        x1:0, y1:0,
        x2:1, y2:0,
        stop:0 #2C313A,
        stop:1 #353B45
    );
}

QFrame#panelAcciones QPushButton {
    background-color: #2ECC71;
    color: #ECF0F1;
    border-radius: 10px;
    padding: 6px 12px;
    font-size: 20px;
    font-weight: bold;
    min-width: 260px;
    max-width: 260px;
    min-height: 62px;
    max-height: 62px;
}

QFrame#panelAcciones QPushButton:hover {
    background-color: #27AE60;
}

QFrame#panelAcciones QPushButton:pressed {
    background-color: #1E8449;
}

QPushButton#btnAccionesGenerales { background-color: #4DB0F2; }
QPushButton#btnAccionesGenerales:hover { background-color: #1DA2FA; }
QPushButton#btnAccionesGenerales:pressed { background-color: #0C88DB; }

QPushButton#btnAdministrarSucursales { background-color: #FFAA4F; }
QPushButton#btnAdministrarSucursales:hover { background-color: #F78B17; }
QPushButton#btnAdministrarSucursales:pressed { background-color: #E67C0B; }

QPushButton#btnAccionesLogistica { background-color: #E67E22; }
QPushButton#btnAccionesLogistica:hover { background-color: #D26A14; }
QPushButton#btnAccionesLogistica:pressed { background-color: #B85C0E; }
)");

        auto styleMainBtn = [this](const QString &name)
        {
            QPushButton *b = this->findChild<QPushButton *>(name);
            if (!b)
                b = this->findChild<QPushButton *>(name + "_2");
            return b;
        };

        const QStringList ordenPrincipal = {
            "btnAccionesGenerales",
            "btnAdministrarSucursales",
            "btnAccionesLogistica"};

        int x = 120;
        const int y = 48;
        const int w = 260;
        const int h = 62;
        const int gap = 90;
        for (const QString &name : ordenPrincipal)
        {
            if (QPushButton *b = styleMainBtn(name))
            {
                b->setGeometry(x, y, w, h);
                b->setVisible(true);
                x += w + gap;
            }
        }

        if (QPushButton *btnCargar = this->findChild<QPushButton *>("btnCargarArchivo"))
        {
            btnCargar->setText("Cargar Archivo");
            btnCargar->setGeometry(30, 10, 190, 40);
            btnCargar->setStyleSheet(R"(
QPushButton {
    background-color: #FFAA4F;
    color: #ECF0F1;
    border-radius: 10px;
    padding: 6px 12px;
    font-size: 18px;
    font-weight: bold;
    min-width: 190px;
    max-width: 190px;
}
QPushButton:hover { background-color: #F78B17; }
QPushButton:pressed { background-color: #E67C0B; }
)");
        }
    }

    //* Elimina los botones de minimizar, etc.
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    //* Evitar doble scroll en los graphics view
    ui->gvListaEnlazadaNoOrdenada->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->gvListaEnlazadaNoOrdenada->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->gvListaEnlazadaNoOrdenada->viewport()->setAttribute(Qt::WA_AcceptTouchEvents, false);
    ui->scrollArea->setWidgetResizable(true);
    actualizarReloj();

    scene1 = new QGraphicsScene(this);
    scene2 = new QGraphicsScene(this);
    scene3 = new QGraphicsScene(this);
    scene4 = new QGraphicsScene(this);

    ui->gvListaEnlazadaNoOrdenada->setScene(scene1);
    ui->gvListaEnlazadaOrdenada->setScene(scene2);
    panelAccionesGenerales = new PanelAccionesGenerales(this);
    panelAccionesGenerales->hide();

    panelAccionesSucursales = new PanelAccioneSucursales(this);
    panelAccionesSucursales->hide();

    // Crear y preparar panel Acciones Logistica
    panelAccionesLogistica = new PanelAccionesLogistica(this);
    panelAccionesLogistica->hide();

    auto syncPanelesPosicion = [this]()
    {
        const QRect r = ui->panelAcciones->geometry();
        if (panelAccionesGenerales)
            panelAccionesGenerales->setGeometry(r);
        if (panelAccionesSucursales)
            panelAccionesSucursales->setGeometry(r);
        if (panelAccionesLogistica)
            panelAccionesLogistica->setGeometry(r);
    };
    syncPanelesPosicion();
    QTimer::singleShot(0, this, [syncPanelesPosicion]()
                       { syncPanelesPosicion(); });

    auto findBtnLocal = [this](const QString &name)
    {
        QPushButton *b = this->findChild<QPushButton *>(name);
        if (!b)
            b = this->findChild<QPushButton *>(name + "_2");
        return b;
    };

    if (auto btnAcc = findBtnLocal("btnAccionesGenerales"))
    {
        connect(btnAcc, &QPushButton::clicked, this, [this]()
                {
            if (ui && panelAccionesGenerales) {
                const QRect r = ui->panelAcciones->geometry();
                panelAccionesGenerales->setGeometry(r);
                ui->panelAcciones->hide();
                if (panelAccionesSucursales) panelAccionesSucursales->hide();
                if (panelAccionesLogistica) panelAccionesLogistica->hide();
                panelAccionesGenerales->show();
            } });
    }

    if (panelAccionesGenerales)
    {
        QPushButton *btnReg = panelAccionesGenerales->findChild<QPushButton *>("btnRegresar");
        if (!btnReg)
            btnReg = panelAccionesGenerales->findChild<QPushButton *>("btnRegresar_2");
        if (btnReg)
        {
            connect(btnReg, &QPushButton::clicked, this, [this]()
                    {
                if (ui && panelAccionesGenerales) {
                    panelAccionesGenerales->hide();
                    if (panelAccionesSucursales) panelAccionesSucursales->hide();
                    if (panelAccionesLogistica) panelAccionesLogistica->hide();
                    ui->panelAcciones->show();
                } });
        }
    }
    if (panelAccionesSucursales)
    {
        QPushButton *btnRegS = panelAccionesSucursales->findChild<QPushButton *>("btnRegresar");
        if (!btnRegS)
            btnRegS = panelAccionesSucursales->findChild<QPushButton *>("btnRegresar_2");
        if (btnRegS)
            connect(btnRegS, &QPushButton::clicked, this, [this]()
                    { panelAccionesSucursales->hide(); if (panelAccionesGenerales) panelAccionesGenerales->hide(); if (panelAccionesLogistica) panelAccionesLogistica->hide(); ui->panelAcciones->show(); });
    }
    if (panelAccionesLogistica)
    {
        QPushButton *btnRegL = panelAccionesLogistica->findChild<QPushButton *>("btnRegresar");
        if (!btnRegL)
            btnRegL = panelAccionesLogistica->findChild<QPushButton *>("btnRegresar_2");
        if (btnRegL)
            connect(btnRegL, &QPushButton::clicked, this, [this]()
                    { panelAccionesLogistica->hide(); if (panelAccionesGenerales) panelAccionesGenerales->hide(); if (panelAccionesSucursales) panelAccionesSucursales->hide(); ui->panelAcciones->show(); });
    }
    ui->gvArbolB->setScene(scene3);
    ui->gvArbolBPlus->setScene(scene4);

    mostrarArboles();
    inicializarPantallas();
}

PantallaSistema::~PantallaSistema()
{
    delete ui;
}

void PantallaSistema::setAppController(AppController *controller)
{
    appController = controller;
    conectarPantallasConController();
}

void PantallaSistema::actualizarReloj()
{
    QTime hora = QTime::currentTime();
    QDate fecha = QDate::currentDate();

    ui->lcdNumber->display(hora.toString("hh:mm:ss"));
    ui->lblFecha->setText(fecha.toString("dddd dd MMMM yyyy"));

    int msRestantes = 1000 - hora.msec();
    QTimer::singleShot(msRestantes, this, &PantallaSistema::actualizarReloj);
}

void PantallaSistema::mostrarArboles()
{
    scene1->clear();
    scene2->clear();
    scene3->clear();
    scene4->clear();
}

void PantallaSistema::btnAgregarClicked()
{
    emit addProducto();
}

void PantallaSistema::inicializarPantallas()
{
    agregarProducto = new PantallaAgregarProducto(this);
    PantallaEliminar *eliminarProducto = new PantallaEliminar(this);
    PantallaBuscarPorCategoria *buscarCategoria = new PantallaBuscarPorCategoria(this);
    PantallaBuscarPorRangoCaducidad *buscarRango = new PantallaBuscarPorRangoCaducidad(this);
    PantallaBuscarPorNombre *buscarNombre = new PantallaBuscarPorNombre(this);
    PantallaListarPorNombre *listarNombre = new PantallaListarPorNombre(this);
    mostrarCSV = new PantallaMostrarCSV(this);
    buscarPorCodigo = new PantallaBuscarPorCodigo(this);
    modificarSucursal = new PantallaModificarSucursal(this);
    eliminarSucursal = new PantallaEliminarSucursal(this);

    // Aplicar estilos de fondo a las nuevas pantallas
    QString style = R"(
        background: qlineargradient(
            x1:0, y1:0,
            x2:1, y2:0,
            stop:0 #2C313A,
            stop:1 #353B45
        );
    )";
    buscarPorCodigo->setStyleSheet(style);
    modificarSucursal->setStyleSheet(style);
    eliminarSucursal->setStyleSheet(style);

    // Agregar pantallas al stackedWidget
    ui->stackedWidget->addWidget(mostrarCSV);
    ui->stackedWidget->addWidget(agregarProducto);
    ui->stackedWidget->addWidget(eliminarProducto);
    ui->stackedWidget->addWidget(buscarNombre);
    ui->stackedWidget->addWidget(buscarCategoria);
    ui->stackedWidget->addWidget(buscarRango);
    ui->stackedWidget->addWidget(listarNombre);
    ui->stackedWidget->addWidget(buscarPorCodigo);
    ui->stackedWidget->addWidget(modificarSucursal);
    ui->stackedWidget->addWidget(eliminarSucursal);

    // Intenta conectar inmediatamente; si el controller aun no existe, se conecta al llamar setAppController().
    conectarPantallasConController();

    // Conectar botones a los cambios de pantalla

    // Conectar señales del panel de acciones a los widgets correspondientes
    PanelAccionesGenerales *panel = findChild<PanelAccionesGenerales *>();
    if (panel)
    {
        connect(panel, &PanelAccionesGenerales::agregarClicked, [=]()
                { ui->stackedWidget->setCurrentWidget(agregarProducto); });
        connect(panel, &PanelAccionesGenerales::eliminarClicked, [=]()
                { ui->stackedWidget->setCurrentWidget(eliminarProducto); });
        connect(panel, &PanelAccionesGenerales::buscarPorNombreRequested, [=]()
                { ui->stackedWidget->setCurrentWidget(buscarNombre); });
        connect(panel, &PanelAccionesGenerales::buscarPorCategoriaRequested, [=]()
                { ui->stackedWidget->setCurrentWidget(buscarCategoria); });
        connect(panel, &PanelAccionesGenerales::buscarPorCaducidadRequested, [=]()
                { ui->stackedWidget->setCurrentWidget(buscarRango); });
        connect(panel, &PanelAccionesGenerales::listarPorNombreRequested, [=]()
                { ui->stackedWidget->setCurrentWidget(listarNombre); });
        connect(panel, &PanelAccionesGenerales::buscarPorCodigoRequested, [=]()
                { ui->stackedWidget->setCurrentWidget(buscarPorCodigo); });
    }

    if (panelAccionesSucursales)
    {
        connect(panelAccionesSucursales, &PanelAccioneSucursales::modifySucursalRequested, [=]()
                { ui->stackedWidget->setCurrentWidget(modificarSucursal); });
        connect(panelAccionesSucursales, &PanelAccioneSucursales::eliminarSucursalRequested, [=]()
                { ui->stackedWidget->setCurrentWidget(eliminarSucursal); });
    }

    connect(ui->btnCargarArchivo, &QPushButton::clicked, [=]()
            {
        const QString ruta = QFileDialog::getOpenFileName(
            this,
            "Seleccionar archivo CSV",
            QString(),
            "Archivos CSV (*.csv);;Todos los archivos (*)");

        if (ruta.isEmpty()) {
            return;
        }

        ui->stackedWidget->setCurrentWidget(mostrarCSV);
        emit archivoCSVSeleccionado(ruta); });

    auto findBtnLocal2 = [this](const QString &name)
    {
        QPushButton *b = this->findChild<QPushButton *>(name);
        if (!b)
            b = this->findChild<QPushButton *>(name + "_2");
        return b;
    };

    if (auto btnSuc = findBtnLocal2("btnAdministrarSucursales"))
    {
        connect(btnSuc, &QPushButton::clicked, this, [this]()
                { ui->panelAcciones->hide(); if (panelAccionesGenerales) panelAccionesGenerales->hide(); if (panelAccionesLogistica) panelAccionesLogistica->hide(); if (panelAccionesSucursales) { panelAccionesSucursales->setGeometry(ui->panelAcciones->geometry()); panelAccionesSucursales->show(); } });
    }
    if (auto btnLog = findBtnLocal2("btnAccionesLogistica"))
    {
        connect(btnLog, &QPushButton::clicked, this, [this]()
                { ui->panelAcciones->hide(); if (panelAccionesGenerales) panelAccionesGenerales->hide(); if (panelAccionesSucursales) panelAccionesSucursales->hide(); if (panelAccionesLogistica) { panelAccionesLogistica->setGeometry(ui->panelAcciones->geometry()); panelAccionesLogistica->show(); } });
    }

    // Pantalla inicial visible
    ui->stackedWidget->setCurrentWidget(mostrarCSV);
}

void PantallaSistema::conectarPantallasConController()
{
    if (!appController)
    {
        return;
    }

    PantallaEliminar *eliminarProducto = findChild<PantallaEliminar *>();
    PantallaBuscarPorNombre *buscarNombre = findChild<PantallaBuscarPorNombre *>();
    PantallaBuscarPorCategoria *buscarCategoria = findChild<PantallaBuscarPorCategoria *>();
    PantallaBuscarPorRangoCaducidad *buscarRango = findChild<PantallaBuscarPorRangoCaducidad *>();

    if (agregarProducto)
    {
        connect(agregarProducto, &PantallaAgregarProducto::productoAgregado,
                appController, &AppController::agregarProducto,
                Qt::UniqueConnection);
    }

    if (eliminarProducto)
    {
        connect(eliminarProducto, &PantallaEliminar::productoEliminado,
                appController, &AppController::eliminarProducto,
                Qt::UniqueConnection);
    }

    if (buscarNombre)
    {
        connect(buscarNombre, &PantallaBuscarPorNombre::buscarSolicitado,
                appController, &AppController::buscarPorNombre,
                Qt::UniqueConnection);
        connect(appController, &AppController::resultadosBusquedaNombre,
                buscarNombre, &PantallaBuscarPorNombre::mostrarResultados,
                Qt::UniqueConnection);
        // Connect name search times to system labels
        connect(appController, &AppController::resultadosBusquedaNombre,
                [this](ListaGenerica<Product *> *r, long ul, long ol, long avl)
                {
                    this->actualizarTiempos(ul, ol, -1, -1, avl);
                });
    }

    if (buscarCategoria)
    {
        connect(buscarCategoria, &PantallaBuscarPorCategoria::buscarSolicitado,
                appController, &AppController::buscarPorCategoria,
                Qt::UniqueConnection);
        connect(appController, &AppController::resultadosBusquedaCategoria,
                buscarCategoria, &PantallaBuscarPorCategoria::mostrarResultados,
                Qt::UniqueConnection);
        // Connect category search time (B+)
        connect(appController, &AppController::resultadosBusquedaCategoria,
                [this](ListaGenerica<Product *> *r, long t)
                {
                    this->actualizarTiempos(-1, -1, -1, t, -1);
                });
    }

    if (buscarRango)
    {
        connect(buscarRango, &PantallaBuscarPorRangoCaducidad::buscarSolicitado,
                appController, &AppController::buscarPorRangoCaducidad,
                Qt::UniqueConnection);
        connect(appController, &AppController::resultadosBusquedaRango,
                buscarRango, &PantallaBuscarPorRangoCaducidad::mostrarResultados,
                Qt::UniqueConnection);
        // Connect range search time (B)
        connect(appController, &AppController::resultadosBusquedaRango,
                [this](ListaGenerica<Product *> *r, long t)
                {
                    this->actualizarTiempos(-1, -1, t, -1, -1);
                });
    }

    PantallaListarPorNombre *listarNombre = findChild<PantallaListarPorNombre *>();
    if (listarNombre)
    {
        connect(listarNombre, &PantallaListarPorNombre::listarSolicitado,
                appController, &AppController::listarPorNombre,
                Qt::UniqueConnection);
        connect(appController, &AppController::resultadosListadoNombre,
                listarNombre, &PantallaListarPorNombre::mostrarResultados,
                Qt::UniqueConnection);
    }
}

void PantallaSistema::mostrarDatosCSV(const QList<Product> &productos)
{
    if (mostrarCSV)
    {
        mostrarCSV->mostrarDatos(productos);
    }
}

QGraphicsView *PantallaSistema::getViewListaNoOrdenada()
{
    return ui ? ui->gvListaEnlazadaNoOrdenada : nullptr;
}

QGraphicsView *PantallaSistema::getViewListaOrdenada()
{
    return ui ? ui->gvListaEnlazadaOrdenada : nullptr;
}

QGraphicsView *PantallaSistema::getViewArbolB()
{
    return ui ? ui->gvArbolB : nullptr;
}

QGraphicsView *PantallaSistema::getViewArbolBPlus()
{
    return ui ? ui->gvArbolBPlus : nullptr;
}

QGraphicsView *PantallaSistema::getViewArbolAVL()
{

    return ui ? ui->gvArbolAVL : nullptr;
    // Si da clavos descomentar esta línea y comentar la de arriba, es un parche temporal para evitar que se caiga la aplicación al no tener implementada la vista del AVL
    // return nullptr;
}

void PantallaSistema::actualizarTiempos(long ul, long ol, long b, long bp, long avl)
{
    if (!ui)
        return;
    if (ul != -1)
        ui->lblTiempoUL->setText(QString("Tiempo: %1 µs").arg(ul));
    if (ol != -1)
        ui->lblTiempoOL->setText(QString("Tiempo: %1 µs").arg(ol));
    if (b != -1)
        ui->lblTiempoB->setText(QString("Tiempo: %1 µs").arg(b));
    if (bp != -1)
        ui->lblTiempoBPlus->setText(QString("Tiempo: %1 µs").arg(bp));
    if (avl != -1)
        ui->lblTiempoAVL->setText(QString("Tiempo: %1 µs").arg(avl));
}
