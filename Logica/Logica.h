#include <unistd.h>
#include "Entrada.h"

int Plantar(struct Mapa *mapa, int fila, int columna, char planta);
int SpawnZombie(struct Mapa *mapa, int fila, int columna, char zombie);
int Movimiento(struct Mapa *mapa, int fila, int columna);

typedef struct
{
    int fila;
    int columna;
    int vida; //Puntos de salud del zombie
    int armadura; //Si mayor o igual que 0, zombie perderá armadura en vez de vida (armadura es un factor a vida)
    int movimiento; //Si movimiento llega a 0, se mueve
} Zombie ;

