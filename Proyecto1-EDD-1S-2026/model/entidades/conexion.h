#ifndef CONEXION_H
#define CONEXION_H

#include <string>

struct Conexion {
    std::string destinoID;
    int tiempo;
    int costo;

    Conexion(std::string dID, int t, int c) 
        : destinoID(dID), tiempo(t), costo(c) {}
};

#endif
