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
static void pedirPlanta(struct Mapa *mapa){
    int fila, columna; // Nose hacerlo :(
}
static void DisparoPlantas(struct Mapa *mapa){

    for (int i = 0; i < mapa->filas; i++)
    {
        for (int j = 0; j < mapa->columnas - 1; j++){
        
            if (mapa->matriz[i][j] == '2'){
            
                if (mapa->matriz[i][j + 1] == 'Z'){
                
                    //funcion dañar zombie
                }
                else if (mapa->matriz[i][j + 1] == '.'){
                
                    mapa->matriz[i][j + 1] = '*';
                }
            }
        }
    }
}
static void moverProyectiles(struct Mapa *mapa)
{
    for (int i = 0; i < mapa->filas; i++)
    {
        
        for (int j = mapa->columnas - 1; j >= 0; j--)
        {
            if (mapa->matriz[i][j] == '*')
            {
                mapa->matriz[i][j] = '.';   

                if (j + 1 < mapa->columnas)
                {
                    char columna_S = mapa->matriz[i][j + 1];

                    if (columna_S == 'Z')
                    {
                        //dañar zombie
                    }
                    else if (columna_S == '.')
                    {
                        mapa->matriz[i][j + 1] = '*';  // sigue avanzando
                    }
                    
                }
            }
        }
    }
} // problemas cuando hay una planta adelante !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!