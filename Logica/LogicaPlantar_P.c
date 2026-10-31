#include <stdio.h>
#include "Logica.h"

int Plantar(struct Mapa *mapa, int fila, int columna, char planta)
{
    if (mapa->matriz[fila][columna] == '.')
    {
        mapa->matriz[fila][columna] = planta;
        return 1;
    }
    return 0;
}
