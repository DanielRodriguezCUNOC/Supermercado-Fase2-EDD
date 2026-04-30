#include "pantallasistema.h"
#include "ui_pantallasistema.h"
#include "view/pantallas/pantallaeliminar.h"
#include "view/pantallas/pantallabuscarporcategoria.h"
#include "view/pantallas/pantallabuscarpornombre.h"
#include "view/pantallas/pantallabuscarporrangocaducidad.h"
#include "view/pantallas/pantallalistarpornombre.h"
#include "view/pantallas/pantallamostrarcsv.h"
#include "controller/appcontroller.h"
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QFileDialog>

PantallaSistema::PantallaSistema(QWidget *parent)
    : QWidget(parent), ui(new Ui::PantallaSistema)
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

QFrame#header {
    background: qlineargradient(
        x1:0, y1:0,
        x2:1, y2:0,
        stop:0 #1A252F,
        stop:1 #22303C
    );
}

QLabel#tituloHeader {
    color: #ECF0F1;
    font-size: 28px;
    font-weight: bold;
}

QFrame#panelAcciones {
    background: qlineargradient(
        x1:0, y1:0,
        x2:1, y2:0,
        stop:0 #2C313A,
        stop:1 #353B45
    );
}

QFrame#panelContenido{
    background-color: #1E2228;
}

QLCDNumber {
    background-color: #1A252F;
    color: #ff9b2d;
    border: none;
}

QLabel#lblFecha{
    font-size: 28px;
    font-weight: bold;
}

)");
    ui->btnBuscarPorNombre->setText("Buscar\npor\nNombre");
    ui->btnBuscarPorCodigo->setText("Buscar\npor\nCodigo");
    ui->btnBuscarPorCategoria->setText("Buscar\npor\nCategoria");
    ui->btnBuscarPorCaducidad->setText("Buscar\npor\nCaducidad");
    ui->btnListarPorNombre->setText("Listar\npor\nNombre");
    ui->btnCompararBusquedas->setText("Comparar\nBusquedas");
    ui->btnCargarArchivo->setText("Cargar Archivo");
    ui->btnVerArbol->setText("Visualizar Arboles");

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

    // Agregar pantallas al stackedWidget
    ui->stackedWidget->addWidget(mostrarCSV);
    ui->stackedWidget->addWidget(agregarProducto);
    ui->stackedWidget->addWidget(eliminarProducto);
    ui->stackedWidget->addWidget(buscarNombre);
    ui->stackedWidget->addWidget(buscarCategoria);
    ui->stackedWidget->addWidget(buscarRango);
    ui->stackedWidget->addWidget(listarNombre);

        // Intenta conectar inmediatamente; si el controller aun no existe, se conecta al llamar setAppController().
        conectarPantallasConController();

    // Conectar botones a los cambios de pantalla

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
    connect(ui->btnAgregar, &QPushButton::clicked, [=]()
            { ui->stackedWidget->setCurrentWidget(agregarProducto); });
    connect(ui->btnEliminar, &QPushButton::clicked, [=]()
            { ui->stackedWidget->setCurrentWidget(eliminarProducto); });
    connect(ui->btnBuscarPorNombre, &QPushButton::clicked, [=]()
            { ui->stackedWidget->setCurrentWidget(buscarNombre); });
    connect(ui->btnBuscarPorCategoria, &QPushButton::clicked, [=]()
            { ui->stackedWidget->setCurrentWidget(buscarCategoria); });
    connect(ui->btnBuscarPorCaducidad, &QPushButton::clicked, [=]()
            { ui->stackedWidget->setCurrentWidget(buscarRango); });
    connect(ui->btnListarPorNombre, &QPushButton::clicked, [=]()
            { ui->stackedWidget->setCurrentWidget(listarNombre); });

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
                [this](ListaGenerica<Product*>* r, long ul, long ol, long avl) {
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
                [this](ListaGenerica<Product*>* r, long t) {
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
                [this](ListaGenerica<Product*>* r, long t) {
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

void PantallaSistema::mostrarDatosCSV(const QList<Product>& productos)
{
    if (mostrarCSV) {
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
    if (!ui) return;
    if (ul != -1) ui->lblTiempoUL->setText(QString("Tiempo: %1 µs").arg(ul));
    if (ol != -1) ui->lblTiempoOL->setText(QString("Tiempo: %1 µs").arg(ol));
    if (b != -1) ui->lblTiempoB->setText(QString("Tiempo: %1 µs").arg(b));
    if (bp != -1) ui->lblTiempoBPlus->setText(QString("Tiempo: %1 µs").arg(bp));
    if (avl != -1) ui->lblTiempoAVL->setText(QString("Tiempo: %1 µs").arg(avl));
}
