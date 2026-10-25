#include "Entrada.h"

struct Mapa* crearMapa(int filas, int columnas,int cantidadObstaculos) 
{
    FILE *file = fopen("Archivos/Map.txt","w");
    srand(time(NULL)); // Generar semilla aleatoria

    struct Mapa* mapa = malloc(sizeof(struct Mapa));

    if (mapa == NULL) 
    {
        fclose(file);
        printf("Error en la asignación de memoria al Mapa");
        exit(1);
    }

    mapa->columnas = columnas;
    mapa->filas = filas;
    mapa->matriz = malloc(filas * sizeof(char *));
    for (int i = 0; i < filas; i++) {
        mapa->matriz[i] = malloc((columnas + 1) * sizeof(char));
    };

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            if (j == 0){mapa->matriz[i][j] = 'H';}
            else{mapa->matriz[i][j] = '.';}
        }
        mapa->matriz[i][columnas] = '\0';
    }

    for (int i = 0; i < cantidadObstaculos; i++)
    {
        // el +3 es para obligar que no se generen obstaculos en las primeras casillas
        int fila = rand() % (filas); 
        int columna = rand() % (columnas - 3) + 3;
        mapa->matriz[fila][columna] = 'O';

    }

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            fprintf(file,"%c",mapa->matriz[i][j]);
        }
        fprintf(file,"\n");
    }

    fclose(file);
    return mapa;
}

static struct Mapa* validarArchivoMapa(FILE *file)
{
    char linea[128];
    int filas = 1, columnas = 0;

    if (fgets(linea,sizeof(linea),file) == NULL){return NULL;}

    for (int i = 0; i < strlen(linea) - 1; i++){
        if (i != 0 && linea[i] == 'H'){return NULL;}
        if (linea[i] != 'H' && linea[i] != '.' && linea[i] != 'O'){return NULL;}
    }
    linea[strcspn(linea, "\n")] = '\0';
    columnas = strlen(linea);
    if (columnas < 5){return NULL;}


    while(fgets(linea,sizeof(linea),file) != NULL)
    {
        
        if (linea[0] == '\n' || linea[0] == ' '){continue;} 
        linea[strcspn(linea, "\n")] = '\0';
        if (strlen(linea) != columnas){return NULL;}

        for (int i = 0; i < columnas - 1; i++){
            if (i != 0 && linea[i] == 'H'){return NULL;}
            if (linea[i] != 'H'  && linea[i] != '.' && linea[i] != 'O'){return NULL;}
        }

        filas++;
    }
    if (filas < 5){return NULL;}

    rewind(file);

    struct Mapa* mapa = malloc(sizeof(struct Mapa));

    if (mapa == NULL) 
    {
        fclose(file);
        printf("Error en la asignación de memoria al Mapa");
        exit(1);
    }

    mapa->matriz = malloc(filas * sizeof(char *));
    for (int i = 0; i < filas; i++) {
        mapa->matriz[i] = malloc((columnas + 1) * sizeof(char));
    }
    mapa->columnas = columnas;
    mapa->filas = filas;
    char c;
    for (int i = 0; i < filas; i++)
    {
        int j = 0;
        while ((c = fgetc(file)) != EOF && c != '\n')
        {
            if (j < columnas) {
                mapa->matriz[i][j] = c;
                j++;
            }
        }
        mapa->matriz[i][columnas] = '\0'; 
    }

    return mapa;
}

void agregarObstaculos(struct Mapa *mapa, int cantidadObstaculos)
{
    int contador = 0;
    while (contador < cantidadObstaculos)
    {
        int fila = rand() % (mapa->filas);
        int columna = rand() % (mapa->columnas - 3) + 3;
        mapa->matriz[fila][columna] = 'O';
        contador++;
        //Habría que hacer una validación para no poner obstaculos encima de las plantas
    }
}
void guardarMapaEnArchivo(struct Mapa *mapa)
{
    FILE *file = fopen("Archivos/Guardado.txt","w");
    
    for (int i = 0; i < mapa->filas; i++)
    {
        for (int j = 0; j < mapa->columnas; j++)
        {
            fprintf(file,"%c",mapa->matriz[i][j]);
        }
        fprintf(file,"\n");
    }
    fclose(file);
}

struct Mapa* cargarMapa()
{
    int filaDefecto = 5, columnaDefecto = 9, obstaculosDefecto = 5;

    FILE *file = fopen("Archivos/Map.txt","r");
    if (file == NULL){return (crearMapa(filaDefecto,columnaDefecto,obstaculosDefecto));}
    
    struct Mapa* mapa = validarArchivoMapa(file);
    if (mapa == NULL){
        fclose(file);
        printf("El achivo del mapa se corrompió. Creando uno nuevo...");
        return crearMapa(filaDefecto,columnaDefecto,obstaculosDefecto);
    }

    fclose(file);
    return mapa;

}