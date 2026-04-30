#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include <QList>
#include <QString>
#include "model/entidades/product.h"

class FileController : public QObject
{
    Q_OBJECT

public:
    FileController(QObject *parent = nullptr);
    ~FileController();

    // Método principal para cargar CSV
    QList<Product> cargarCSV(const QString &ruta);

private:
    // Helper para loggear errores en errors.log
    void logError(const QString &mensaje);
};

#endif // FILECONTROLLER_H
