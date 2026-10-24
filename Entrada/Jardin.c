#include "Entrada.h"

char** crearMapa(int filas, int columnas,int cantidadObstaculos) 
{
    FILE *file = fopen("Archivos/Map.txt","w");
    srand(time(NULL)); // Generar semilla aleatoria


    char **jardin = malloc(filas * sizeof(char *));
    for (int i = 0; i < filas; i++) {
        jardin[i] = malloc((columnas + 1) * sizeof(char));
    }

    
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            if (j == 0){jardin[i][j] = 'H';}
            else{jardin[i][j] = '.';}
        }
        jardin[i][columnas] = '\0';
    }



    for (int i = 0; i < cantidadObstaculos; i++)
    {
        // el +3 es para obligar que no se generen obstaculos en las primeras casillas
        int fila = rand() % (filas); 
        int columna = rand() % (columnas -3) +3;
        jardin[fila][columna] = 'O';

    }

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            fprintf(file,"%c",jardin[i][j]);
        }
        fprintf(file,"\n");
    }

    fclose(file);
    return jardin;

}

char** cargarMapa()
{
    int filaDefecto = 5, columnaDefecto = 9, obstaculosDefecto = 5;
    return (crearMapa(filaDefecto,columnaDefecto,obstaculosDefecto));

}