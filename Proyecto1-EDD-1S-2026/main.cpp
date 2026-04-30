#include "controller/appcontroller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // AppController es el coordinador general que orquesta e instancia toda la arquitectura MVC
    AppController appCoordinator;
    appCoordinator.iniciar();

    return a.exec();
}
