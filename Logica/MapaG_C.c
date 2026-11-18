#include "Logica.h"

void guardarPartidaCompleta(const char *ruta, struct Mapa *mapa, Zombie *listaZombies, int cantidadZombies, int soles, int vidaCasa, int oleadaActual) {
    FILE *file = fopen(ruta, "wb");
    if (file == NULL) { return; }

    DatosPartida_Guardada datos;
    datos.filas = mapa->filas;
    datos.columnas = mapa->columnas;
    datos.soles = soles;
    datos.vidaCasa = vidaCasa;
    datos.oleadaActual = oleadaActual;
    datos.cantidadZombies = cantidadZombies;

    // 1. Guardar la cabecera con los datos generales
    fwrite(&datos, sizeof(DatosPartida_Guardada), 1, file);

    // 2. Guardar matrices del mapa (física y lógica)
    for (int i = 0; i < mapa->filas; i++) {
        fwrite(mapa->matriz[i], sizeof(char), mapa->columnas, file);
        fwrite(mapa->matrizLogica[i], sizeof(char), mapa->columnas, file);
    }

    // 3. Guardar el arreglo dinámico de zombis activos
    if (cantidadZombies > 0) {
        fwrite(listaZombies, sizeof(Zombie), cantidadZombies, file);
    }

    fclose(file);
}

struct Mapa* cargarPartidaCompleta(const char *ruta, Zombie **listaZombies, int *cantidadZombies, int *capacidadZombies, int *soles, int *vidaCasa, int *oleadaActual) {
    FILE *file = fopen(ruta, "rb");
    if (file == NULL) { return NULL; }

    DatosPartida_Guardada datos;
    fread(&datos, sizeof(DatosPartida_Guardada), 1, file);

    // Reservar el mapa utilizando la función estándar del proyecto (crearMapa)
    // Ajusta los argumentos si tu función crearMapa recibe parámetros distintos.
    struct Mapa *mapa = crearMapa(datos.filas, datos.columnas, 5);

    *soles = datos.soles;
    *vidaCasa = datos.vidaCasa;
    *oleadaActual = datos.oleadaActual;
    *cantidadZombies = datos.cantidadZombies;

    // Leer las matrices del mapa
    for (int i = 0; i < datos.filas; i++) {
        fread(mapa->matriz[i], sizeof(char), datos.columnas, file);
        fread(mapa->matrizLogica[i], sizeof(char), datos.columnas, file);
    }

    // Reasignar memoria y restaurar la lista de zombis
    free(*listaZombies);
    *capacidadZombies = (*cantidadZombies > 0) ? *cantidadZombies : 4;
    *listaZombies = malloc((*capacidadZombies) * sizeof(Zombie));

    if (*cantidadZombies > 0 && *listaZombies != NULL) {
        fread(*listaZombies, sizeof(Zombie), *cantidadZombies, file);
    }

    fclose(file);
    return mapa;
}