#include "Logica.h"

//Avanza una casilla todos los proyectiles '-' de matrizLogica.
//Se recorre de derecha a izquierda para que un proyectil no se mueva dos veces en la misma llamada.
//Si la casilla siguiente tiene un zombie, el proyectil desaparece y daña al zombie.
void moverProyectiles(struct Mapa *mapa, Zombie* lista, int cantidad)
{
    for (int i = 0; i < mapa->filas; i++)
    {
        for (int j = mapa->columnas - 1; j >= 0; j--)
        {
            if (mapa->matrizLogica[i][j] != '-'){continue;}

            mapa->matrizLogica[i][j] = '.';
            if (j + 1 >= mapa->columnas){continue;} //salió del jardín

            if (mapa->matrizLogica[i][j + 1] == 'Z'){danarZombie(lista, cantidad, i, j + 1, DANIO_GUISANTE);}
            else{mapa->matrizLogica[i][j + 1] = '-';} //sigue avanzando
        }
    }
}
