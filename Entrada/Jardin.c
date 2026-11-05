#include "Entrada.h"

//Reserva en el heap una matriz filas x columnas llena con el caracter 'relleno'
static char** reservarMatriz(int filas, int columnas, char relleno)
{
    char **m = malloc(filas * sizeof(char *));
    if (m == NULL){printf("Error en la asignación de memoria\n"); exit(1);}

    for (int i = 0; i < filas; i++)
    {
        m[i] = malloc((columnas + 1) * sizeof(char));
        if (m[i] == NULL){printf("Error en la asignación de memoria\n"); exit(1);}
        memset(m[i], relleno, columnas);
        m[i][columnas] = '\0'; //Se agregó el terminador '\0' para evitar posibles problemas de lectura y/o parseo
    }
    return m;
}

//Crea un struct Mapa con las dos matrices ya reservadas (jardín y lógica) y llenas de '.'
//Se usa tanto al crear un mapa nuevo como al cargarlo desde archivo
static struct Mapa* reservarMapa(int filas, int columnas)
{
    struct Mapa* mapa = malloc(sizeof(struct Mapa));
    if (mapa == NULL){printf("Error en la asignación de memoria al Mapa\n"); exit(1);}

    mapa->filas = filas;
    mapa->columnas = columnas;
    mapa->matriz = reservarMatriz(filas, columnas, '.');
    mapa->matrizLogica = reservarMatriz(filas, columnas, '.');
    return mapa;
}

//Libera la memoria dinamica utilizada por el mapa
void liberarMapa(struct Mapa *mapa)
{
    if (mapa == NULL){return;}
    for (int i = 0; i < mapa->filas; i++)
    {
        free(mapa->matriz[i]);
        free(mapa->matrizLogica[i]);
    }
    free(mapa->matriz);
    free(mapa->matrizLogica);
    free(mapa);
}

//Escribe la matriz del jardín en el archivo indicado
static void guardarMatriz(struct Mapa *mapa, const char *ruta)
{
    FILE *file = fopen(ruta,"w");
    if (file == NULL){return;}

    for (int i = 0; i < mapa->filas; i++)
    {
        fprintf(file,"%s\n",mapa->matriz[i]);
    }
    fclose(file);
}

struct Mapa* crearMapa(int filas, int columnas,int cantidadObstaculos) 
{
    struct Mapa* mapa = reservarMapa(filas, columnas);

    //Mapa base. '.' es el suelo y 'H' es la casa (House)
    for (int i = 0; i < filas; i++){mapa->matriz[i][0] = 'H';}

    //Agrega obstaculos a la matriz.
    if (columnas > 3){agregarObstaculos(mapa, cantidadObstaculos);}

    guardarMatriz(mapa, "Archivos/Map.txt");
    return mapa;
}

//Lee la siguiente línea NO vacía del archivo y le quita el '\n' y el '\r'. Retorna 0 si no quedan.
static int leerLineaMapa(FILE *file, char *linea, int tamano)
{
    while (fgets(linea, tamano, file) != NULL)
    {
        linea[strcspn(linea, "\r\n")] = '\0';
        if (linea[0] != '\0' && linea[0] != ' '){return 1;}
    }
    return 0;
}

//Una fila válida empieza con 'H' (la casa) y el resto solo puede ser '.' u 'O'
static int lineaValida(const char *linea)
{
    if (linea[0] != 'H'){return 0;}
    for (int i = 1; linea[i] != '\0'; i++)
    {
        if (linea[i] != '.' && linea[i] != 'O'){return 0;}
    }
    return 1;
}

//Esta es una funcion intermedia. Verifica si un archivo Map.txt es valido. Si no lo es
//retorna NULL, en caso contrario devuelve un struct Mapa con la información del archivo
static struct Mapa* validarArchivoMapa(FILE *file)
{
    char linea[256];
    int filas = 0, columnas = 0;

    while (leerLineaMapa(file, linea, sizeof(linea)))
    {
        if (filas == 0)
        {
            columnas = (int)strlen(linea);
            if (columnas < 5){return NULL;} //Se pide un minimo de 5 columnas
        }
        else if ((int)strlen(linea) != columnas){return NULL;} //todas las filas del mismo largo

        if (!lineaValida(linea)){return NULL;}
        filas++;
    }
    if (filas < 5){return NULL;} //Si el archivo está vacío también cae aquí

    rewind(file); //Esto permite releer el archivo de nuevo.

    struct Mapa* mapa = reservarMapa(filas, columnas);
    for (int i = 0; i < filas; i++)
    {
        leerLineaMapa(file, linea, sizeof(linea));
        memcpy(mapa->matriz[i], linea, columnas);
    }
    return mapa;
}

//Lee el archivo Map.txt y retorna la matriz del mapa leido, en caso que el archivo
//esté corrupto, se crea un nuevo mapa con parametros por defecto
struct Mapa* cargarMapa(void)
{
    const int filaDefecto = 5, columnaDefecto = 9, obstaculosDefecto = 5;

    FILE *file = fopen("Archivos/Map.txt","r");
    if (file == NULL){return (crearMapa(filaDefecto,columnaDefecto,obstaculosDefecto));}
    
    struct Mapa* mapa = validarArchivoMapa(file);
    fclose(file);

    if (mapa == NULL){
        printf("El archivo del mapa se corrompió. Creando uno nuevo...\n");
        return crearMapa(filaDefecto,columnaDefecto,obstaculosDefecto);
    }
    return mapa;
}

void agregarObstaculos(struct Mapa *mapa, int cantidadObstaculos)
{
    int contador = 0;
    while (contador < cantidadObstaculos)
    {
        int fila = rand() % (mapa->filas);
        int columna = rand() % (mapa->columnas - 3) + 3; // el +3 evita obstaculos en las primeras casillas
        mapa->matriz[fila][columna] = 'O';
        contador++;
        //Habría que hacer una validación para no poner obstaculos encima de las plantas
        //Ademas, 'O' se puede superponer a otras 'O' no obstante, podría ocurrir que no
        //hubiera espacio para los obstaculos, por eso dejamos que se superpongan simplemente.
    }
}

//Se ingresa un struct Mapa y el jardín contenido se reflea en un archivo Guardado.txt
//(Esta función es útil para guardar el estado actual de la partida)
void guardarMapaEnArchivo(struct Mapa *mapa)
{
    guardarMatriz(mapa, "Archivos/Guardado.txt");
}
