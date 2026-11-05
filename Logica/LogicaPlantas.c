#include "Logica.h"

//Coloca una planta en el jardín si la casilla es suelo libre ('.') y no hay un zombie encima
int Plantar(struct Mapa *mapa, int fila, int columna, char planta)
{
    if (fila < 0 || fila >= mapa->filas || columna < 0 || columna >= mapa->columnas){return 0;}

    if (mapa->matriz[fila][columna] == '.' && mapa->matrizLogica[fila][columna] != 'Z')
    {
        mapa->matriz[fila][columna] = planta;
        return 1;
    }
    return 0;
}

//Hay algún zombie en la misma fila, a la derecha de la columna dada
static int hayZombieAdelante(struct Mapa *mapa, int fila, int columna)
{
    for (int j = columna + 1; j < mapa->columnas; j++)
    {
        if (mapa->matrizLogica[fila][j] == 'Z'){return 1;}
    }
    return 0;
}

//Cada lanzaguisantes dispara (solo si hay un zombie en su fila): crea un proyectil '-'
//en la casilla de adelante, o daña directamente al zombie si ya está pegado a la planta.
//Los proyectiles viven en matrizLogica, por eso pasan "por encima" de otras plantas
void disparoPlantas(struct Mapa *mapa, Zombie* lista, int cantidad)
{
    for (int i = 0; i < mapa->filas; i++)
    {
        for (int j = 0; j < mapa->columnas - 1; j++)
        {
            if (mapa->matriz[i][j] != PLANTA_LANZAGUISANTES){continue;}
            if (!hayZombieAdelante(mapa, i, j)){continue;}

            char siguiente = mapa->matrizLogica[i][j + 1];
            if (siguiente == 'Z'){danarZombie(lista, cantidad, i, j + 1, DANIO_GUISANTE);}
            else if (siguiente == '.'){mapa->matrizLogica[i][j + 1] = '-';}
        }
    }
}
