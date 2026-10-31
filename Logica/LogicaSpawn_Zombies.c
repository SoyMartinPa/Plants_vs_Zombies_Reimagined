#include <stdio.h>
#include "Logica.h"



int SpawnZombie(struct Mapa *mapa, int fila, int columna, char zombie)
{
    if (mapa->matriz[fila][columna] == '.')
    {
        mapa->matriz[fila][columna] = zombie;
        return 1;
    }
    return 0;
}