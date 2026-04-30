#!/bin/bash

BLUE='\033[0;34m'
YELLOW='\033[1;33m'
PURPLE='\033[0;35m'
NC='\033[0m'

echo -e "${BLUE}########################################${NC}"
echo -e "${PURPLE}     Iniciando proceso de ensamblado     ${NC}"
echo -e "${BLUE}########################################${NC}"

if [ ! -f "CMakeLists.txt" ]; then
    echo -e "${YELLOW}Advertencia: Archivo de configuración faltante en directorio actual.${NC}"
    exit 1
fi

echo "Eliminando residuos de sesiones anteriores..."
rm -rf build
mkdir build
cd build

echo "Preparando entorno de construcción..."

# Configuración estándar del generador
cmake ..

if [ $? -ne 0 ]; then
    echo -e "${YELLOW}Fallo en la preparación del entorno. Revisa las dependencias.${NC}"
    exit 1
fi

echo "Ejecutando tareas paralelas con $(nproc) hilos..."
cmake --build . --parallel $(nproc)

if [ $? -ne 0 ]; then
    echo -e "${YELLOW}Interrupción durante la fase de construcción.${NC}"
    exit 1
fi

echo -e "${BLUE}########################################${NC}"
echo -e "${PURPLE}     Ensamblado finalizado con éxito (〜￣▽￣)〜    ${NC}"
echo -e " Ubicación del artefacto: ${BLUE}$(pwd)${NC}"
echo -e "${BLUE}########################################${NC}"