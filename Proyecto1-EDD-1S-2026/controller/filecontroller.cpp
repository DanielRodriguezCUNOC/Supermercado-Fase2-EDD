#include "filecontroller.h"
#include "estructurascontroller.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QStringList>

FileController::FileController(QObject *parent) : QObject(parent)
{
}

FileController::~FileController()
{
}

void FileController::logError(const QString &mensaje)
{
    // Escribir en errors.log en el directorio actual
    QFile file("errors.log");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        out << "[" << timestamp << "] ERROR: " << mensaje << "\n";
        file.close();
    } else {
        qDebug() << "No se pudo abrir errors.log para escribir el error:" << mensaje;
    }
}

QList<Product> FileController::cargarCSV(const QString &ruta)
{
    QList<Product> listaProductos;

    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        logError(QString("No se pudo abrir o encontrar el archivo CSV en la ruta: %1").arg(ruta));
        return listaProductos;
    }

    QTextStream in(&file);
    // Asumimos que la primera línea puede ser el encabezado, pero el enunciado no lo requiere eliminar obligatoriamente.
    // Verificamos si la primera línea contiene "Nombre" para saltarla si es un header.
    bool primeraLinea = true;
    int numeroLinea = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        numeroLinea++;

        if (line.isEmpty()) continue;

        if (primeraLinea) {
            primeraLinea = false;
            if (line.contains("\"Nombre\"") || line.startsWith("Nombre", Qt::CaseInsensitive)) {
                continue; // Es el header, lo saltamos
            }
        }

        // Dividir por comas asumiendo formato estricto: "Nombre","Codigo",...
        // Una manera sencilla es usar split(',') y luego limpiar comillas
        QStringList tokens = line.split(',');

        // 1. Validar que la estructura tenga 7 columnas
        if (tokens.size() != 7) {
            logError(QString("Línea %1 malformada (no tiene 7 columnas): %2").arg(numeroLinea).arg(line));
            continue;
        }

        // Limpiar comillas iniciales y finales de cada token
        for (int i = 0; i < tokens.size(); ++i) {
            tokens[i] = tokens[i].trimmed();
            if (tokens[i].startsWith('"')) {
                tokens[i] = tokens[i].mid(1);
            }
            if (tokens[i].endsWith('"')) {
                tokens[i] = tokens[i].chopped(1);
            }
        }

        QString nombre = tokens[0];
        QString codigoBarra = tokens[1];
        QString categoria = tokens[2];
        QString fechaCaducidad = tokens[3];
        QString marca = tokens[4];
        QString precioStr = tokens[5];
        QString stockStr = tokens[6];

        // 2. Validar que Precio y Stock sean numéricos
        bool precioOk;
        double precio = precioStr.toDouble(&precioOk);
        if (!precioOk) {
            logError(QString("Línea %1 malformada (Precio no es numérico): %2").arg(numeroLinea).arg(line));
            continue;
        }

        bool stockOk;
        int stock = stockStr.toInt(&stockOk);
        if (!stockOk) {
            logError(QString("Línea %1 malformada (Stock no es numérico): %2").arg(numeroLinea).arg(line));
            continue;
        }

        // Producto válido creado (La validación de duplicados se hará después o por el EstructurasController al insertar)
        Product nuevoProducto(
            nombre.toStdString(),
            codigoBarra.toStdString(),
            categoria.toStdString(),
            fechaCaducidad.toStdString(),
            marca.toStdString(),
            precio,
            stock
        );

        listaProductos.append(nuevoProducto);
    }

    file.close();
    return listaProductos;
}

void FileController::cargarSucursales(const QString &ruta, EstructurasController* ec) {
    if (!ec) return;
    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        logError("No se pudo abrir sucursales: " + ruta);
        return;
    }
    QTextStream in(&file);
    bool header = true;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (header) { 
            header = false; 
            if (line.contains("ID", Qt::CaseInsensitive)) continue; 
        }
        QStringList t = line.split(',');
        if (t.size() < 6) continue;
        // Limpiar comillas
        for (int i=0; i<t.size(); ++i) {
            t[i] = t[i].trimmed();
            if(t[i].startsWith('"')) t[i].remove(0,1);
            if(t[i].endsWith('"')) t[i].chop(1);
        }
        ec->agregarSucursal(t[0].toStdString(), t[1].toStdString(), t[2].toStdString(), 
                           t[3].toInt(), t[4].toInt(), t[5].toInt());
    }
    file.close();
}

void FileController::cargarConexiones(const QString &ruta, EstructurasController* ec) {
    if (!ec) return;
    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        logError("No se pudo abrir conexiones: " + ruta);
        return;
    }
    QTextStream in(&file);
    bool header = true;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (header) { 
            header = false; 
            if (line.contains("OrigenID", Qt::CaseInsensitive)) continue; 
        }
        QStringList t = line.split(',');
        if (t.size() < 4) continue;
        for (int i=0; i<t.size(); ++i) {
            t[i] = t[i].trimmed();
            if(t[i].startsWith('"')) t[i].remove(0,1);
            if(t[i].endsWith('"')) t[i].chop(1);
        }
        ec->conectarSucursales(t[0].toStdString(), t[1].toStdString(), t[2].toInt(), t[3].toInt());
    }
    file.close();
}

void FileController::cargarCatalogo(const QString &ruta, EstructurasController* ec) {
    if (!ec) return;
    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        logError("No se pudo abrir catalogo: " + ruta);
        return;
    }
    QTextStream in(&file);
    bool header = true;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (header) { 
            header = false; 
            if (line.contains("SucursalID", Qt::CaseInsensitive)) continue; 
        }
        QStringList t = line.split(',');
        if (t.size() < 8) continue;
        for (int i=0; i<t.size(); ++i) {
            t[i] = t[i].trimmed();
            if(t[i].startsWith('"')) t[i].remove(0,1);
            if(t[i].endsWith('"')) t[i].chop(1);
        }
        
        // SucursalID, Nombre, CodigoBarra, Categoria, Fecha, Marca, Precio, Stock
        ec->agregarProductoASucursal(t[0].toStdString(), t[1].toStdString(), t[2].toStdString(), 
                                   t[3].toStdString(), t[4].toStdString(), t[5].toStdString(), 
                                   t[6].toDouble(), t[7].toInt());
    }
    file.close();
}
