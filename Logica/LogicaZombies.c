#include <stdio.h>
#include "Logica.h"
#include "Entrada.h"

Zombie* SpawnZombieAleatoreo(struct Mapa *mapa, struct Data data)
{
    int fila, contador = 0;
    int filasVisitadas[mapa->filas];
    for (int i = 0; i < mapa->filas; i++){filasVisitadas[i] = 0;}

    
    while(1){

        fila = rand() % mapa->filas;
        if (mapa->matrizLogica[fila][mapa->columnas - 1] != 'Z' && filasVisitadas[fila] == 0){break;}
        if (filasVisitadas[fila] == 0){filasVisitadas[fila] = 1 ; contador++;};

        if (contador == mapa->filas){return NULL;}
    }

    Zombie *zombie;
    zombie->filafila = fila;
    
    zombie->columna = mapa->columnas - 1;
    zombie->vida = 100 * data.resistenciaZombies;
    zombie->movimiento = rand()%5 + 100;

    zombie->armadura = rand() % 3;

    switch (zombie->armadura){
    case  0:
        break;
    case 1:
        zombie->vida = zombie->vida * 1.5;
        break;
    case 2:
        zombie->vida = zombie->vida * 2;
        break;
    
    return zombie;
    }
}

static void moverZombies(struct Mapa *mapa, int *vidaCasa)
{
    for (int i = 0; i < mapa->filas; i++)
    {
        for (int j = 0; j < mapa->columnas; j++)
        {
            if (mapa->matriz[i][j] == 'Z') // falta hacer la diferencia de tipo de zombies y todos se mover igual (lo mas probable)
            {
                int resultado = Movimiento(mapa, i, j);
                if (resultado == 2)   
                {
                    *vidaCasa = *vidaCasa - 1;
                }
            }
        }
    }
}

