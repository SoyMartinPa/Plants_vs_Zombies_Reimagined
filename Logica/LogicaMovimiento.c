#include <stdio.h>
#include "Logica.h"


int Movimiento_Z(struct Mapa *mapa, int fila, int columna)
{
    int siguiente_Columna = columna - 1;

    if (siguiente_Columna < 1)
    {
        return 2;
    }

    char Poscion_Proxima = mapa->matriz[fila][siguiente_Columna];

    if (Poscion_Proxima  == '1')
    {
        return 3;
    }

    if (Poscion_Proxima  == '.')
    {
        mapa->matriz[fila][Poscion_Proxima ] = 'Z';
        mapa->matriz[fila][Poscion_Proxima ] = '.';
        return 1;
    }

    return 0;
}