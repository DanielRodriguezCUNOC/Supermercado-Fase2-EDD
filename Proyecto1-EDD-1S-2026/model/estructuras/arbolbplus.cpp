#include "arbolbplus.h"
#include <sstream>

BPlusNode::BPlusNode(int _grado, bool _hoja) : esHoja(_hoja), grado(_grado), numClaves(0), siguienteHoja(nullptr)
{
    // Reservamos espacio extra para poder meter un elemento de más temporamente antes de "partir" (split) el nodo a la mitad.
    claves = new std::string[grado + 1];
    hijos = new BPlusNode *[grado + 2];
    productos = new Nodo *[grado + 1];

    // Limpiamos los punteros para que no apunten a basura en la memoria.
    for (int i = 0; i < grado + 2; ++i) hijos[i] = nullptr;
    for (int i = 0; i < grado + 1; ++i) productos[i] = nullptr;
}

BPlusNode::~BPlusNode()
{
    // Cuando destruimos un nodo, si es hoja, tenemos la responsabilidad extra 
    // de borrar las listas enlazadas de productos que viven aquí.
    if (esHoja) {
        for (int i = 0; i < numClaves; ++i) {
            Nodo *actual = productos[i];
            while (actual) {
                Nodo *sig = actual->getNext(); // Guardamos el siguiente antes de destruir el actual
                delete actual->getValue();     // Borramos el producto
                delete actual;                 // Borramos el nodo de la lista
                actual = sig;                  // Avanzamos
            }
        }
    }
    // Liberamos los arreglos principales.
    delete[] claves;
    delete[] hijos;
    delete[] productos;
}

ArbolBPlus::ArbolBPlus(int _grado) : raiz(nullptr), grado(_grado) {}

ArbolBPlus::~ArbolBPlus()
{
    // Empezamos la destrucción masiva desde la raíz
    destruirRec(raiz);
}

void ArbolBPlus::destruirRec(BPlusNode *nodo)
{
    if (!nodo) return; // Caso base: si no hay nodo, no hacemos nada
    
    // Si es interno, primero mandamos a destruir a todos sus hijos
    if (!nodo->esHoja) {
        for (int i = 0; i <= nodo->numClaves; ++i) {
            destruirRec(nodo->hijos[i]);
        }
    }
    // Finalmente, lo borramos a él mismo
    delete nodo;
}

// Esta función hace la búsqueda, inserción y partición.
BPlusNode* ArbolBPlus::insertarRec(BPlusNode *nodo, const Product &producto, std::string &clavePromovida, std::string &error, bool &exito)
{
    std::string cat = producto.getCategory(); // La categoría es nuestra referencia

    if (nodo->esHoja) {
        // Llegamos a una hoja de destino
        
        // Buscamos dónde encaja esta categoría (orden alfabético)
        int i = 0;
        while (i < nodo->numClaves && nodo->claves[i] < cat) i++;
        
        // Si la categoría ya existía, solo metemos el producto en su lista
        if (i < nodo->numClaves && nodo->claves[i] == cat) {
            Nodo *actual = nodo->productos[i];
            while (actual) {
                // Revisamos que no haya un código de barras repetido
                if (actual->getValue()->getBarcode() == producto.getBarcode()) {
                    error = "Código de barras duplicado";
                    exito = false;
                    return nullptr;
                }
                actual = actual->getNext();
            }
            // Agregamos al inicio de la lista doblemente enlazada
            Nodo *nuevo = new Nodo(new Product(producto));
            nuevo->setNext(nodo->productos[i]);
            if (nodo->productos[i]) nodo->productos[i]->setPrev(nuevo); // Conectamos hacia atrás
            nodo->productos[i] = nuevo; // Nueva cabeza
            exito = true;
            return nullptr; // No hubo split, todo chill XD
        }

        // Si la categoría NO existía, movemos todo a la derecha
        for (int j = nodo->numClaves; j > i; j--) {
            nodo->claves[j] = nodo->claves[j-1];
            nodo->productos[j] = nodo->productos[j-1];
        }
        
        // Insertamos
        nodo->claves[i] = cat;
        nodo->productos[i] = new Nodo(new Product(producto)); // Iniciamos su lista
        nodo->numClaves++;
        exito = true;

        // Comprobamos si la hoja se llenó.
        if (nodo->numClaves >= grado) {
            // Toca dividir la hoja a la mitad
            BPlusNode *nuevoDer = new BPlusNode(grado, true); // El hermanito derecho :3
            int mitad = grado / 2;

            // Mudamos la mitad derecha de las claves al nuevo nodo
            for (int k = mitad; k < nodo->numClaves; k++) {
                nuevoDer->claves[nuevoDer->numClaves] = nodo->claves[k];
                nuevoDer->productos[nuevoDer->numClaves] = nodo->productos[k];
                nuevoDer->numClaves++;
            }
            nodo->numClaves = mitad; // El nodo original se queda con la primera mitad
            
            // Re-enlazamos los punteros para no romper la cadena de hojas
            nuevoDer->siguienteHoja = nodo->siguienteHoja;
            nodo->siguienteHoja = nuevoDer;

            // En los B+, la primera clave del nodo derecho se copia al padre
            clavePromovida = nuevoDer->claves[0];
            return nuevoDer; // Retornamos el hermano 
        }
        return nullptr; // Si no se llenó, regresamos null
    } else {
        // Somos un nodo interno. Aún no llegamos al final. ---
        
        int i = 0;
        // Buscamos qué camino tomar (por cuál hijo bajar)
        while (i < nodo->numClaves && cat >= nodo->claves[i]) i++;

        std::string subClaveProm;
        // Bajada RECURSIVA
        BPlusNode *nuevoHijo = insertarRec(nodo->hijos[i], producto, subClaveProm, error, exito);

        // Si nuevoHijo no es null, significa que abajo hubo una explosión (split)
        if (nuevoHijo) {
            // Hacemos hueco en nosotros mismos para la nueva clave
            for (int j = nodo->numClaves; j > i; j--) {
                nodo->claves[j] = nodo->claves[j-1];
                nodo->hijos[j+1] = nodo->hijos[j];
            }
            nodo->claves[i] = subClaveProm;
            nodo->hijos[i+1] = nuevoHijo; // Conectamos el nuevo hermanito
            nodo->numClaves++;

            // Si nos llenamos al aceptar este hijo
            if (nodo->numClaves >= grado) {
                // Split de nodo INTERNO
                BPlusNode *nuevoDer = new BPlusNode(grado, false);
                int mitad = grado / 2;
                
                // En los internos, la clave de en medio SUBE del todo y desaparece de aquí
                clavePromovida = nodo->claves[mitad];
                
                // Repartimos los hijos y claves de la derecha al nuevo nodo
                int idx = 0;
                int k = mitad + 1;
                for (; k < nodo->numClaves; k++) {
                    nuevoDer->claves[idx] = nodo->claves[k];
                    nuevoDer->hijos[idx] = nodo->hijos[k];
                    idx++;
                    nuevoDer->numClaves++;
                }
                nuevoDer->hijos[idx] = nodo->hijos[k]; // último hijo derecho
                nodo->numClaves = mitad; // Reducimos nuestro propio tamaño
                
                return nuevoDer; // Retornamos el hermano al padre
            }
        }
        return nullptr;
    }
}

bool ArbolBPlus::insertarProducto(const Product &producto, std::string &errorRollback)
{
    // Si esta pelado el arbol, lo creamos desde cero
    if (!raiz) {
        raiz = new BPlusNode(grado, true);
        raiz->claves[0] = producto.getCategory();
        raiz->productos[0] = new Nodo(new Product(producto));
        raiz->numClaves = 1;
        return true;
    }

    std::string clavePromovida;
    std::string error;
    bool exito = false;

    // Llamamos al motor principal recurviso
    BPlusNode *nuevoDer = insertarRec(raiz, producto, clavePromovida, error, exito);

    // Revisamos si reventó algo o se detectó duplicado
    if (!exito) {
        errorRollback = error;
        return false;
    }

    // Si la mismísima RAÍZ hizo un split, no tiene padre que le tienda la mano.
    // Se crea una nueva raiz
    if (nuevoDer) {
        BPlusNode *nuevaRaiz = new BPlusNode(grado, false); // Se crea una nueva raiz (interno)
        nuevaRaiz->claves[0] = clavePromovida;
        nuevaRaiz->hijos[0] = raiz;          // Antigua raiz pasa a ser hijo izquierdo
        nuevaRaiz->hijos[1] = nuevoDer;      // Y su hermano a la derecha
        nuevaRaiz->numClaves = 1;
        raiz = nuevaRaiz;                    // Nueva raiz
    }

    return true;
}

#include "listagenerica.h"

void ArbolBPlus::buscarPorCategoriaLista(const std::string &categoria, ListaGenerica<Product*>* resultados) const
{
    if (!raiz || !resultados) return;

    // Navegar hacia abajo hasta tocar fondo (la hoja base)
    BPlusNode *actual = raiz;
    while (!actual->esHoja) {
        int i = 0;

        // Caminamos hasta encontrar la ruta indicada (menor al hijo siguiente)
        // Usamos >= porque en insertarRec usamos >= para decidir por qué hijo bajar si la clave coincide
        while (i < actual->numClaves && categoria >= actual->claves[i]) i++;
        actual = actual->hijos[i];
    }

    // Ya en la hoja, buscamos a mano entre los vecinos de esa hoja
    // Como las categorías están ordenadas, si encontramos la categoría
    // recolectamos sus productos y terminamos.
    while (actual) {
        bool encontrada = false;
        for (int i = 0; i < actual->numClaves; ++i) {
            if (actual->claves[i] == categoria) {
                // Recolectamos todos los productos de esta lista
                Nodo *lista = actual->productos[i];
                while (lista) {
                    // Creamos una copia del producto para el resultado
                    resultados->insertar(new Product(*lista->getValue()));
                    lista = lista->getNext();
                }
                encontrada = true;
                break; // No hace falta buscar más en esta hoja
            } else if (actual->claves[i] > categoria) {
                // Como están ordenadas, si ya nos pasamos, no existe
                return;
            }
        }
        if (encontrada) return; // Ya recolectamos todo lo de esa categoría
        actual = actual->siguienteHoja;
    }
}

bool ArbolBPlus::eliminarProducto(const std::string &codigoBarra, std::string &errorRollback)
{
    if (!raiz) return false;


    // Como nuestra brújula es la categoría, pero solo sabemos el código de barras, 
    // tenemos que usar modo "escáner" en todas las hojas. 
    // Bajamos a la primera hoja (a la izquierda del todo).
    BPlusNode *hoja = raiz;
    while (!hoja->esHoja) hoja = hoja->hijos[0];

    bool encontrado = false;
    // Y empezamos a viajar hoja por hoja usando siguienteHoja
    while (hoja && !encontrado) {
        for (int i = 0; i < hoja->numClaves; ++i) {
            Nodo *actual = hoja->productos[i];
            Nodo *anterior = nullptr;
            while (actual) {
                if (actual->getValue()->getBarcode() == codigoBarra) {

                    // Lo sacamos de la lista doblemente enlazada
                    if(anterior) anterior->setNext(actual->getNext());
                    else hoja->productos[i] = actual->getNext();

                    if (actual->getNext()) actual->getNext()->setPrev(anterior);

                    delete actual->getValue();
                    delete actual;             
                    encontrado = true;

                    // Si la lista de la categoria qeudo vacia, se elimina la clave
                    if(hoja->productos[i]==nullptr){
                        for(int j=i; j < hoja->numClaves - 1; j++){
                            hoja->claves[j] = hoja->claves[j+1];
                            hoja->productos[j] = hoja->productos[j+1];
                        }
                        hoja->numClaves--;
                    }
                    break;
                }
                anterior = actual;
                actual = actual->getNext();
            }
            if(encontrado) break;
        }
        hoja = hoja->siguienteHoja; // Nos movemos hacia la derecha
    }
    return encontrado;
}

std::string ArbolBPlus::exportarCSV() const
{
    std::ostringstream oss;
    oss << "Nombre,CodigoBarra,Categoria,FechaCaducidad,Marca,Precio,Stock\n";

    if (!raiz) return oss.str();
    
    // Con los punteros leemos todas las hojas de izquierda a derecha.
    BPlusNode *hoja = raiz;
    while (!hoja->esHoja) hoja = hoja->hijos[0];

    while (hoja) {
        for (int i = 0; i < hoja->numClaves; ++i) {
            Nodo *actual = hoja->productos[i];
            while (actual) {
                Product* p = actual->getValue();
                oss << p->getName() << ","
                    << p->getBarcode() << ","
                    << p->getCategory() << ","
                    << p->getExpiryDate() << ","
                    << p->getBrand() << ","
                    << p->getPrice() << ","
                    << p->getStock() << "\n";
                actual = actual->getNext();
            }
        }
        hoja = hoja->siguienteHoja; 
    }
    return oss.str();
}

static void generarDOTArbolBPlusRec(BPlusNode* nodo, std::ostringstream& ss) {
    if (!nodo) return;
    long id = reinterpret_cast<long>(nodo);
    
    // Aquí construimos visualmente el nodo (como una tablita HTML)
    // <p0> es el slot visual para punteros en Graphviz, luego va un texto, luego otro slot <p1>...
    ss << "  node" << id << " [label=\"";
    for(int i=0; i<nodo->numClaves; i++) {
        ss << "<p" << i << "> | " << nodo->claves[i];
        
        // Si es hoja, muestra todos los productos
        if (nodo->esHoja && nodo->productos[i] != nullptr) {
            Nodo* act = nodo->productos[i];
            while (act) {
                ss << "\\n[" << act->getValue()->getBarcode() << "] " << act->getValue()->getName();
                act = act->getNext();
            }
        }
        
        ss << " | ";
    }
    ss << "<p" << nodo->numClaves << ">\"];\n";

    // Si tiene hijos, trazamos las flechitas de bajada
    if (!nodo->esHoja) {
        for(int i=0; i<=nodo->numClaves; i++) {
            if(nodo->hijos[i]) {
                long hid = reinterpret_cast<long>(nodo->hijos[i]);
                ss << "  node" << id << ":p" << i << " -> node" << hid << ";\n";
                generarDOTArbolBPlusRec(nodo->hijos[i], ss); // Recursividad para que dibuje a los nietos
            }
        }
    } else {
        // En las hojas, pintar esa flecha punteada que une directamente con la hoja vecina
        if (nodo->siguienteHoja) {
            long sigId = reinterpret_cast<long>(nodo->siguienteHoja);
            ss << "  node" << id << " -> node" << sigId << " [color=\"blue\", style=\"dashed\"];\n";
        }
    }
}

std::string ArbolBPlus::generarDOT() const {
    std::ostringstream ss;
    ss << "digraph BPlusTree {\n";
    // Pintamos los nodos de tonos rosadito ksksks
    ss << "  node [shape=record, style=filled, fillcolor=\"#E1BEE7\", color=\"#4A148C\"];\n";
    if (raiz) generarDOTArbolBPlusRec(raiz, ss);
    ss << "}\n";
    return ss.str();
}
