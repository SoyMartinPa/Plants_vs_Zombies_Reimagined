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
    }; //Esto simplemente guarda dinamicamente memoria en el heap para la matriz jardin[][]


    //Este for crea el mapa base. '.' es el suelo y 'H' es la casa (House)
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            if (j == 0){mapa->matriz[i][j] = 'H';}
            else{mapa->matriz[i][j] = '.';}
        }
        mapa->matriz[i][columnas] = '\0'; //Se agregó el terminador '\0' para evitar posibles problemas de lectura y/o parseo
    }

    //Agrega obstaculos a la matriz.
    for (int i = 0; i < cantidadObstaculos; i++)
    {
        int fila = rand() % (filas); 
        int columna = rand() % (columnas - 3) + 3; // el +3 es para obligar que no se generen obstaculos en las primeras casillas
        mapa->matriz[fila][columna] = 'O'; 

    }

    //Esto guarda la matriz en el archivo Map.txt
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

//Esta es una función intermedia. Verifica si un archivo Map.txt es valido. Si no lo es
//retorna NULL, en caso contrario devuelve un struct Mapa con la información del archivo
static struct Mapa* validarArchivoMapa(FILE *file)
{
    char linea[128];
    int filas = 1, columnas = 0;

    if (fgets(linea,sizeof(linea),file) == NULL){return NULL;} //Si el archivo está vacio: NULL

    for (int i = 0; i < strlen(linea) - 1; i++){
        if (i != 0 && linea[i] == 'H'){return NULL;}
        if (linea[i] != 'H' && linea[i] != '.' && linea[i] != 'O'){return NULL;} 
    } //Si no tiene caracteres validos o en ubicaciones erroneas: NULL

    linea[strcspn(linea, "\n")] = '\0'; //Simplemente para evitar errores de lectura (ultimo caracter = '\0')
    columnas = strlen(linea);
    if (columnas < 5){return NULL;} //Se pide un minimo de 5 columans

    //Verifica que cada linea del archivo sea valida (similar al for de arriba)
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

    rewind(file); //Esto permite releer el archivo de nuevo.

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
    } //Se crea la matriz del mapa con memoria dinamica.
    mapa->columnas = columnas;
    mapa->filas = filas;
    char c;
    
    //Cada char extraido del archivo se coloca en su columna y fila correspondiente
    for (int i = 0; i < filas; i++)
    {
        int j = 0;
        while ((c = fgetc(file)) != EOF && c != '\n') //Si 'false' pasa a la siguiente fila
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

//Lee el archivo Map.txt y retorna la matriz del mapa leido, en caso que el archivo
//esté corrupto, se crea un nuevo mapa con parametros por defecto
struct Mapa* cargarMapa()
{
    const int filaDefecto = 5, columnaDefecto = 9, obstaculosDefecto = 5;

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
        //Ademas, 'O' se puede superponer a otras 'O' no obstante, podría ocurrir que no
        //hubiera espacio para los obstaculos, por eso dejamos que se superpongan simplemente.
    }
}

//Se ingresa un struct Mapa y el jardín contenido se reflea en un archivo Guardado.txt
//(Esta función es útil para guardar el estado actual de la partida)
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
