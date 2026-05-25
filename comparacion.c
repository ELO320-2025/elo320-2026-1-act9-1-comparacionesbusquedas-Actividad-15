#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOMBRE 20
#define TAM_HASH 101  // número primo cercano a 100

// ---------------------------------------------
// Estructura base
// ---------------------------------------------
typedef struct Mascota {
    char nombre[MAX_NOMBRE];
    char tipo[MAX_NOMBRE];
    struct Mascota *izq;
    struct Mascota *der;
    struct Mascota *sig;  // para lista enlazada
    int height;
} Mascota;

// ---------------------------------------------
// Estructuras globales
// ---------------------------------------------
Mascota *tabla_hash[TAM_HASH];
Mascota *lista = NULL;
Mascota *arbol = NULL;
Mascota **arreglo = NULL;
int totalMascotas = 0;

// ---------------------------------------------
// Funciones auxiliares
// ---------------------------------------------
unsigned int hash(const char *nombre) {
    unsigned int h = 0;
    for (int i = 0; nombre[i] != '\0'; i++)
        h = 31 * h + nombre[i];
    return h % TAM_HASH;
}

Mascota *crearMascota(const char *nombre, const char *tipo) {
    Mascota *nueva = malloc(sizeof(Mascota));
    strcpy(nueva->nombre, nombre);
    strcpy(nueva->tipo, tipo);
    nueva->izq = nueva->der = nueva->sig = NULL;
    nueva->height = 1;
    return nueva;
}

// ---------------------------------------------
// Inserción en estructuras
// ---------------------------------------------
void insertarHash(const char *nombre, const char *tipo) {
    unsigned int idx = hash(nombre);
    Mascota *nueva = crearMascota(nombre, tipo);
    nueva->sig = tabla_hash[idx];
    tabla_hash[idx] = nueva;
}

void insertarLista(const char *nombre, const char *tipo) {
    Mascota *nueva = crearMascota(nombre, tipo);
    nueva->sig = lista;
    lista = nueva;
}

Mascota *insertarABB(Mascota *raiz, const char *nombre, const char *tipo) {
    if (raiz == NULL) return crearMascota(nombre, tipo);
    if (strcmp(nombre, raiz->nombre) < 0)
        raiz->izq = insertarABB(raiz->izq, nombre, tipo);
    else
        raiz->der = insertarABB(raiz->der, nombre, tipo);
    return raiz;
}

// ToDo: implementar inserción balanceada AVL
Mascota *insertarABB_AVL(Mascota *raiz, const char *nombre, const char *tipo) {

    /* 1.  Realiza la inserción normal */
    if (raiz == NULL) return crearMascota(nombre, tipo);
    if (strcmp(nombre, raiz->nombre) < 0)
        raiz->izq = insertarABB_AVL(raiz->izq, nombre, tipo);
    else
        raiz->der = insertarABB_AVL(raiz->der, nombre, tipo);
    

    /* 2. Calcula la altura máxima de este nodo*/
    //ToDo implementar función que calcule altura 
    int 

    /* 3. Obtén el factor de balance del nodo  
        para verificar si está desbalanceado*/
    
    //ToDo implementar función que calcule balance
    int balance = 0;

    // Si el nodo está desbalanceado, tendremos 4 casos

    // Left Left Case
    
    // Right Right Case
    
    // Left Right Case
    
    // Right Left Case

    /* return el nodo raiz si no hay operación de balanceo*/
    return raiz;
}



// ---------------------------------------------
// Búsqueda
// ---------------------------------------------
Mascota *buscarHash(const char *nombre) {
    unsigned int idx = hash(nombre);
    Mascota *aux = tabla_hash[idx];
    while (aux != NULL) {
        if (strcmp(aux->nombre, nombre) == 0)
            return aux;
        aux = aux->sig;
    }
    return NULL;
}

Mascota *buscarLista(const char *nombre) {
    Mascota *aux = lista;
    while (aux != NULL) {
        if (strcmp(aux->nombre, nombre) == 0)
            return aux;
        aux = aux->sig;
    }
    return NULL;
}

Mascota *buscarABB(Mascota *raiz, const char *nombre) {
    if (raiz == NULL) return NULL;
    int cmp = strcmp(nombre, raiz->nombre);
    if (cmp == 0) return raiz;
    else if (cmp < 0) return buscarABB(raiz->izq, nombre);
    else return buscarABB(raiz->der, nombre);
}

Mascota *buscarArreglo(Mascota **arr, int n, const char *nombre) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i]->nombre, nombre) == 0)
            return arr[i];
    }
    return NULL;
}

// ---------------------------------------------
// Generar datos
// ---------------------------------------------
void generarDatos(int n) {
    arreglo = malloc(n * sizeof(Mascota *));
    char nombre[MAX_NOMBRE];
    char tipo[MAX_NOMBRE];

    for (int i = 0; i < n; i++) {
        sprintf(nombre, "Mascota%d", i);
        sprintf(tipo, "Tipo%d", i % 5);

        arreglo[i] = crearMascota(nombre, tipo);
        insertarHash(nombre, tipo);
        insertarLista(nombre, tipo);
        arbol = insertarABB(arbol, nombre, tipo);
    }
    totalMascotas = n;
}

// ---------------------------------------------
// Medición de tiempos
// ---------------------------------------------
double medirTiempo(Mascota *(*buscarFunc)(const char *), const char *nombre) {
    clock_t start = clock();
    Mascota *res = buscarFunc(nombre);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double medirTiempoLista(const char *nombre) {
    clock_t start = clock();
    Mascota *res = buscarLista(nombre);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double medirTiempoHash(const char *nombre) {
    clock_t start = clock();
    Mascota *res = buscarHash(nombre);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double medirTiempoABB(const char *nombre) {
    clock_t start = clock();
    Mascota *res = buscarABB(arbol, nombre);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double medirTiempoArreglo(const char *nombre) {
    clock_t start = clock();
    Mascota *res = buscarArreglo(arreglo, totalMascotas, nombre);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

// ---------------------------------------------
// Main
// ---------------------------------------------
int main() {
    srand(time(NULL));

    int tamanios[] = {1000, 5000, 10000, 50000};
    int numCasos = sizeof(tamanios) / sizeof(tamanios[0]);

    for (int i = 0; i < numCasos; i++) {
        int n = tamanios[i];
        printf("\n=== Prueba con %d mascotas ===\n", n);
        generarDatos(n);

        const char *buscada = "Mascota999";  // misma clave para comparar

        double tHash = medirTiempoHash(buscada);
        double tLista = medirTiempoLista(buscada);
        double tABB = medirTiempoABB(buscada);
        double tArr = medirTiempoArreglo(buscada);

        printf("Hash: %.8f s\n", tHash);
        printf("Lista: %.8f s\n", tLista);
        printf("Árbol: %.8f s\n", tABB);
        printf("Arreglo: %.8f s\n", tArr);

        // TODO: liberar estructuras dinámicas aquí (opcional)
    }

    return 0;
}
