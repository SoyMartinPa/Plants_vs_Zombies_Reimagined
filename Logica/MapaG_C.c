#include "Logica.h"

// Serializa y guarda en disco el estado completo de una partida (datos, matrices del mapa y zombis)
void guardarPartidaCompleta(const char *ruta, struct Mapa *mapa, Zombie *listaZombies, int cantidadZombies, int soles, int vidaCasa, int oleadaActual) {
    // Abre el archivo en modo escritura binaria ("wb")
    FILE *file = fopen(ruta, "wb");
    if (file == NULL) { return; } // Sale sin hacer nada si no se pudo abrir o crear el archivo

    // Empaqueta los valores generales de la partida en la estructura de cabecera
    DatosPartida_Guardada datos;
    datos.filas = mapa->filas;
    datos.columnas = mapa->columnas;
    datos.soles = soles;
    datos.vidaCasa = vidaCasa;
    datos.oleadaActual = oleadaActual;
    datos.cantidadZombies = cantidadZombies;

    // Guardar la cabecera con los datos generales
    fwrite(&datos, sizeof(DatosPartida_Guardada), 1, file);

    //  Guarda matrices del mapa (fisica y lógica) fila por fila
    for (int i = 0; i < mapa->filas; i++) {
        fwrite(mapa->matriz[i], sizeof(char), mapa->columnas, file);
        fwrite(mapa->matrizLogica[i], sizeof(char), mapa->columnas, file);
    }

    // Guardar el arreglo dinámico de zombis activos si existe al menos uno
    if (cantidadZombies > 0) {
        fwrite(listaZombies, sizeof(Zombie), cantidadZombies, file);
    }

    fclose(file); // Cierra el archivo 
}

// Lee un archivo de guardado binario reconstruye el mapa y restaura el estado de la partida
struct Mapa* cargarPartidaCompleta(const char *ruta, Zombie **listaZombies, int *cantidadZombies, int *capacidadZombies, int *soles, int *vidaCasa, int *oleadaActual) {
    // Abre el archivo en modo lectura binaria ("rb")
    FILE *file = fopen(ruta, "rb");
    if (file == NULL) { return NULL; } // Retorna NULL si el archivo no existe o no es accesible

    // Lee la cabecera inicial para conocer dimensiones del mapa y contadores de la partida
    DatosPartida_Guardada datos;
    fread(&datos, sizeof(DatosPartida_Guardada), 1, file);

    // Reservar el mapa utilizando la función estándar del proyecto (crearMapa)
    
    struct Mapa *mapa = crearMapa(datos.filas, datos.columnas, 5);

    // Restaura las variables de estado de la partida a través de sus punteros
    *soles = datos.soles;
    *vidaCasa = datos.vidaCasa;
    *oleadaActual = datos.oleadaActual;
    *cantidadZombies = datos.cantidadZombies;

    // Leer las matrices del mapa (fisica y lógica) fila por fila
    for (int i = 0; i < datos.filas; i++) {
        fread(mapa->matriz[i], sizeof(char), datos.columnas, file);
        fread(mapa->matrizLogica[i], sizeof(char), datos.columnas, file);
    }

    // Libera la lista previa de zombis y reserva memoria según la cantidad cargada (mínimo 4)
    free(*listaZombies);
    *capacidadZombies = (*cantidadZombies > 0) ? *cantidadZombies : 4;
    *listaZombies = malloc((*capacidadZombies) * sizeof(Zombie));

    // Copia los structs Zombie almacenados en el archivo hacia el nuevo arreglo dinámico
    if (*cantidadZombies > 0 && *listaZombies != NULL) {
        fread(*listaZombies, sizeof(Zombie), *cantidadZombies, file);
    }

    fclose(file); // Cierra el archivo tras completar la lectura
    return mapa;  // Devuelve el puntero al mapa reconstruido
}