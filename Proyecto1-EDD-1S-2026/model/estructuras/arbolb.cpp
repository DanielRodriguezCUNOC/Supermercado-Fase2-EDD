#include "arbolb.h"
#include "model/entidades/product.h"
#include <iostream>
#include <sstream>
using namespace std;

// =========================================================================
// Implementación de NodoB
// =========================================================================

NodoB::NodoB(int _t, bool _hoja) : t(_t), hoja(_hoja), n(0)
{
    /**
     * Se reserva memoria para 2t-1 claves y 2t hijos.
     * Esta es la capacidad máxima definida para un Árbol B de grado mínimo t.
     */
    claves = new Product[2 * t - 1];
    hijos = new NodoB *[2 * t];

    // Inicialización de los punteros
    for (int i = 0; i < 2 * t; i++)
        hijos[i] = nullptr;
}

NodoB::~NodoB()
{
    // Liberación de la memoria dinámica
    delete[] claves;
    delete[] hijos;
}

void NodoB::recorrer()
{
    int i;
    for (i = 0; i < n; i++)
    {
        // Si el nodo actual no es una hoja, se recorre primero el subárbol izquierdo de la clave i
        if (!hoja)
            hijos[i]->recorrer();

        // Se procesa la clave i en el recorrido InOrden
        cout << claves[i].getBarcode() << " (" << claves[i].getExpiryDate() << ") ";
    }

    // Al finalizar con las claves, se recorre el último subárbol
    if (!hoja)
        hijos[i]->recorrer();
}

int NodoB::buscarClave(const std::string &codigo)
{
    int idx = 0;
    // Iteramos hasta encontrar el primer índice cuya clave tenga un código mayor o igual
    while (idx < n && claves[idx].getBarcode() < codigo)
        ++idx;
    return idx;
}

void NodoB::eliminar(const std::string &codigo)
{
    int idx = buscarClave(codigo);

    // La clave a eliminar se encuentra en este nodo
    if (idx < n && claves[idx].getBarcode() == codigo)
    {
        if (hoja)
            removerDeHoja(idx);
        else
            removerDeNoHoja(idx);
    }
    // La clave no está en este nodo, debemos buscarla en los descendientes
    else
    {
        if (hoja)
        {
            // El elemento no existe en la estructura; se retorna sin acción.
            return;
        }

        bool flag = (idx == n); // Indica si bajaremos por el último hijo

        // Si el hijo donde descenderemos tiene menos de 't' claves, debemos garantizar que posea al menos 't'
        // claves mediante un préstamo o una fusión antes de descender.
        if (hijos[idx]->n < t)
            llenar(idx);

        // Descenso recursivo
        if (flag && idx > n)
            hijos[idx - 1]->eliminar(codigo);
        else
            hijos[idx]->eliminar(codigo);
    }
}

void NodoB::removerDeHoja(int idx)
{
    // Desplaza todas las claves posteriores una posición a la izquierda, sobreescribiendo la clave objetivo.
    for (int i = idx + 1; i < n; ++i)
        claves[i - 1] = claves[i];
    n--;
}

void NodoB::removerDeNoHoja(int idx)
{
    Product k = claves[idx];

    // Si el hijo anterior tiene suficientes claves, se utiliza su predecesor
    if (hijos[idx]->n >= t)
    {
        Product pred = obtenerPredecesor(idx);
        claves[idx] = pred;
        hijos[idx]->eliminar(pred.getBarcode());
    }
    // Alternativamente, si el hijo posterior tiene capacidad, se emplea su sucesor
    else if (hijos[idx + 1]->n >= t)
    {
        Product suc = obtenerSucesor(idx);
        claves[idx] = suc;
        hijos[idx + 1]->eliminar(suc.getBarcode());
    }
    // Si ambos hijos tienen solo t-1 claves, se fusionan
    else
    {
        fusionar(idx);
        hijos[idx]->eliminar(k.getBarcode());
    }
}

Product NodoB::obtenerPredecesor(int idx)
{
    // Desciende a la rama izquierda y recupera el valor mas a la derecha.
    NodoB *cur = hijos[idx];
    while (!cur->hoja)
        cur = cur->hijos[cur->n];
    return cur->claves[cur->n - 1];
}

Product NodoB::obtenerSucesor(int idx)
{
    // Desciende a la rama derecha y recupera el valor mas a la izquierda.
    NodoB *cur = hijos[idx + 1];
    while (!cur->hoja)
        cur = cur->hijos[0];
    return cur->claves[0];
}

void NodoB::llenar(int idx)
{
    // Restaura la propiedad del Árbol B extrayendo claves vecinas o contrayendo niveles
    if (idx != 0 && hijos[idx - 1]->n >= t)
        pedirPrestadoAnterior(idx);
    else if (idx != n && hijos[idx + 1]->n >= t)
        pedirPrestadoSiguiente(idx);
    else
    {
        if (idx != n)
            fusionar(idx);
        else
            fusionar(idx - 1);
    }
}

void NodoB::pedirPrestadoAnterior(int idx)
{
    NodoB *hijo = hijos[idx];
    NodoB *hermano = hijos[idx - 1];

    // Desplazar claves y punteros del hijo hacia adelante
    for (int i = hijo->n - 1; i >= 0; --i)
        hijo->claves[i + 1] = hijo->claves[i];
    if (!hijo->hoja)
    {
        for (int i = hijo->n; i >= 0; --i)
            hijo->hijos[i + 1] = hijo->hijos[i];
    }

    // Transferir clave del padre al inicio del hijo
    hijo->claves[0] = claves[idx - 1];

    // Si no es hoja, se transfiere también el último puntero del hermano
    if (!hijo->hoja)
        hijo->hijos[0] = hermano->hijos[hermano->n];

    // Promover la última clave del hermano hacia el padre
    claves[idx - 1] = hermano->claves[hermano->n - 1];

    hijo->n += 1;
    hermano->n -= 1;
}

void NodoB::pedirPrestadoSiguiente(int idx)
{
    NodoB *hijo = hijos[idx];
    NodoB *hermano = hijos[idx + 1];

    // Mover clave del padre al final del hijo
    hijo->claves[hijo->n] = claves[idx];

    // Mover puntero del hermano al hijo
    if (!hijo->hoja)
        hijo->hijos[hijo->n + 1] = hermano->hijos[0];

    // Mover la primera clave del hermano al padre
    claves[idx] = hermano->claves[0];

    // Reorganizar el hermano desplazando claves hacia atrás
    for (int i = 1; i < hermano->n; ++i)
        hermano->claves[i - 1] = hermano->claves[i];
    if (!hermano->hoja)
    {
        for (int i = 1; i <= hermano->n; ++i)
            hermano->hijos[i - 1] = hermano->hijos[i];
    }

    hijo->n += 1;
    hermano->n -= 1;
}

void NodoB::fusionar(int idx)
{
    NodoB *hijo = hijos[idx];
    NodoB *hermano = hijos[idx + 1];

    // Mover la clave separadora del padre y adjuntarla al final del hijo
    hijo->claves[t - 1] = claves[idx];

    // Anexar las claves del hermano directamente al hijo
    for (int i = 0; i < hermano->n; ++i)
        hijo->claves[i + t] = hermano->claves[i];

    // Anexar los punteros
    if (!hijo->hoja)
    {
        for (int i = 0; i <= hermano->n; ++i)
            hijo->hijos[i + t] = hermano->hijos[i];
    }

    // Colapsar el arreglo del padre para rellenar el espacio vacío de la clave movida
    for (int i = idx + 1; i < n; ++i)
        claves[i - 1] = claves[i];
    for (int i = idx + 2; i <= n; ++i)
        hijos[i - 1] = hijos[i];

    hijo->n += hermano->n + 1;
    n--;
    delete hermano; // Liberar memoria del nodo hermano absorbido
}

void NodoB::insertarNoLleno(const Product &k)
{
    int i = n - 1;

    // Se inserta directamente de forma secuencial en la hoja actual
    if (hoja)
    {
        // La política de inserción ordena los elementos primariamente por su fecha de caducidad.
        // Como criterio de desempate secundario, se utiliza el código de barras
        while (i >= 0 && (claves[i].getExpiryDate() > k.getExpiryDate() ||
                          (claves[i].getExpiryDate() == k.getExpiryDate() && claves[i].getBarcode() > k.getBarcode())))
        {
            claves[i + 1] = claves[i];
            i--;
        }
        claves[i + 1] = k;
        n++;
    }
    // Búsqueda del subárbol donde debe continuar la inserción
    else
    {
        while (i >= 0 && (claves[i].getExpiryDate() > k.getExpiryDate() ||
                          (claves[i].getExpiryDate() == k.getExpiryDate() && claves[i].getBarcode() > k.getBarcode())))
        {
            i--;
        }
        i++;

        // Si el nodo hijo determinó estar lleno, requiere ser particionado.
        if (hijos[i]->n == 2 * t - 1)
        {
            dividirHijo(i, hijos[i]);

            // Decidir la ruta entre los dos hijos resultantes post-partición
            if (claves[i].getExpiryDate() < k.getExpiryDate() ||
                (claves[i].getExpiryDate() == k.getExpiryDate() && claves[i].getBarcode() < k.getBarcode()))
                i++;
        }
        hijos[i]->insertarNoLleno(k);
    }
}

void NodoB::dividirHijo(int i, NodoB *y)
{
    // Creación dinámica del nuevo nodo derecho tras la ruptura del nodo objetivo 'y'
    NodoB *z = new NodoB(y->t, y->hoja);
    z->n = t - 1;

    // Transposición de claves y punteros excedentes hacia el nuevo hermano 'z'
    for (int j = 0; j < t - 1; j++)
        z->claves[j] = y->claves[j + t];
    if (!y->hoja)
    {
        for (int j = 0; j < t; j++)
            z->hijos[j] = y->hijos[j + t];
    }
    y->n = t - 1;

    // Creación del espacio en el padre respectivo
    for (int j = n; j >= i + 1; j--)
        hijos[j + 1] = hijos[j];
    hijos[i + 1] = z;

    // Promoción de la clave mediana desde el hijo hacia el contenedor actual
    for (int j = n - 1; j >= i; j--)
        claves[j + 1] = claves[j];
    claves[i] = y->claves[t - 1];
    n++;
}

// =========================================================================
// Implementación de ArbolB
// =========================================================================

ArbolB::ArbolB(int _t)
{
    raiz = nullptr;
    t = _t;
}

void ArbolB::destruirRec(NodoB *nodo)
{
    if (nodo)
    {
        // anulación del fondo al frente
        if (!nodo->hoja)
        {
            for (int i = 0; i <= nodo->n; i++)
            {
                destruirRec(nodo->hijos[i]);
            }
        }
        delete nodo;
    }
}

ArbolB::~ArbolB()
{
    destruirRec(raiz);
}

void ArbolB::insertar(const Product &k)
{
    if (!raiz)
    {
        // En escenarios nulos, se configura la memoria inicial instanciando la raíz base
        raiz = new NodoB(t, true);
        raiz->claves[0] = k;
        raiz->n = 1;
    }
    else
    {
        // Prevención de saturación en la raíz superior
        if (raiz->n == 2 * t - 1)
        {
            NodoB *s = new NodoB(t, false);
            s->hijos[0] = raiz;
            s->dividirHijo(0, raiz);

            int i = 0;
            if (s->claves[0].getExpiryDate() < k.getExpiryDate() ||
                (s->claves[0].getExpiryDate() == k.getExpiryDate() && s->claves[0].getBarcode() < k.getBarcode()))
                i++;

            s->hijos[i]->insertarNoLleno(k);
            raiz = s;
        }
        else
        {
            // Inserción normal en ausencia de saturación inminente local
            raiz->insertarNoLleno(k);
        }
    }
}

void ArbolB::eliminarPorCodigo(const std::string &codigo)
{
    if (!raiz)
        return;
    raiz->eliminar(codigo);

    // De requerirse, se anula la raíz vacía para asignar al descendiente unico como nueva cabecera.
    if (raiz->n == 0)
    {
        NodoB *tmp = raiz;
        if (raiz->hoja)
            raiz = nullptr;
        else
            raiz = raiz->hijos[0];
        delete[] tmp->claves;
        delete[] tmp->hijos;
        ::operator delete(tmp);
    }
}

void ArbolB::imprimir()
{
    if (raiz)
        raiz->recorrer();
    cout << endl;
}

void NodoB::buscarPorRangoFechaRec(const std::string &inicio, const std::string &fin, ListaGenerica<Product *> *resultados)
{
    int i;
    for (i = 0; i < n; i++)
    {
        // Recursión a la izquierda de la clave i
        if (!hoja)
            hijos[i]->buscarPorRangoFechaRec(inicio, fin, resultados);

        // Verificar si la clave i está en el rango
        // Comparamos por fecha de expiración
        if (claves[i].getExpiryDate() >= inicio && claves[i].getExpiryDate() <= fin)
        {
            // Insertamos una copia del producto
            resultados->insertar(new Product(claves[i]));
        }
        else if (claves[i].getExpiryDate() > fin)
        {
            // Como las claves están ordenadas por fecha, si ya nos pasamos de 'fin',
            // no hay necesidad de procesar más claves ni el hijo derecho de esta clave.
            return;
        }
    }

    // Al finalizar con las claves, se recorre el último subárbol si no nos pasamos del rango
    if (!hoja)
        hijos[i]->buscarPorRangoFechaRec(inicio, fin, resultados);
}

void ArbolB::buscarPorRangoFecha(const std::string &inicio, const std::string &fin, ListaGenerica<Product *> *resultados)
{
    if (raiz && resultados)
    {
        raiz->buscarPorRangoFechaRec(inicio, fin, resultados);
    }
}

// =========================================================================
// Generación DOT - Gráfico de la estructura visual - Grafo
// =========================================================================

static void generarDOTArbolBRec(NodoB *nodo, std::stringstream &ss)
{
    if (!nodo)
        return;
    long id = reinterpret_cast<long>(nodo);

    // Proyección de atributos al formato matricial Graphviz mediante sintaxis HTML-like <p> records.
    ss << "  node" << id << " [label=\"";
    for (int i = 0; i < nodo->n; i++)
    {
        ss << "<p" << i << "> | " << nodo->claves[i].getExpiryDate() << " | ";
    }
    ss << "<p" << nodo->n << ">\"];\n";

    // Revestimiento de flujos direccionados hacia las memorias de estancias inferiores.
    if (!nodo->hoja)
    {
        for (int i = 0; i <= nodo->n; i++)
        {
            if (nodo->hijos[i])
            {
                long hid = reinterpret_cast<long>(nodo->hijos[i]);
                ss << "  node" << id << ":p" << i << " -> node" << hid << ";\n";
                generarDOTArbolBRec(nodo->hijos[i], ss);
            }
        }
    }
}

std::string ArbolB::generarDOT() const
{
    std::stringstream ss;
    ss << "digraph BTree {\n";
    // Definición formal con configuraciones estilísticas representativas.
    ss << "  node [shape=record, style=filled, fillcolor=\"#FFF3E0\", color=\"#E65100\"];\n";
    if (raiz)
        generarDOTArbolBRec(raiz, ss);
    ss << "}\n";
    return ss.str();
}
