#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Entrada.h"

//Tiempo que dura un "frame" (30 por segundo). Todos los temporizadores avanzan en DT por vuelta.
#define DT 0.0333f
#define DAÑO_GUISANTE 20
#define DANIO_ZOMBIE_CASA 10        //daño que hace un zombie a la casa por cada "mordida"
#define INTERVALO_PROYECTIL 0.25f   //segundos que tarda un guisante en avanzar una casilla
#define PLANTA_LANZAGUISANTES '2'

//Resultados de moverZombie
enum { MOV_BLOQUEADO = 0, MOV_AVANZO = 1, MOV_CASA = 2, MOV_PLANTA = 3 };

typedef struct
{
    int fila;
    int columna;
    int vida; //Puntos de salud del zombie
    int armadura; //0: sin armadura, 1: cono (vida x1.5), 2: casco (vida x2)
    float movimiento; //Segundos acumulados desde el último movimiento (float: con int el += 0.0333 nunca sumaba)
} Zombie ;

typedef struct
{
    int fila;
    int columna;
    int vida; 
    float movimientoDisparo;
} LanzaGuisantes ;

typedef struct
{
    int fila;
    int columna;
    int vida; 
    float movimientoSol;
} Girasol ;

typedef struct
{
    int fila;
    int columna;
    int vida; 
} Nuez ;

//Las plantas se guardan en 'matriz' como un dígito '1'..'9'
static inline int esPlanta(char c){return c >= '1' && c <= '9';}

//Logica.c
void logica(void);

//LogicaZombies.c
Zombie* SpawnZombieAleatoreo(struct Mapa *mapa, struct Data data);
int moverZombie(Zombie *zombie, struct Mapa *mapa);
void agregarZombie(Zombie** lista, int* cantidad, int* capacidad, Zombie nuevo);
void eliminarZombiesMuertos(Zombie* lista, int* cantidad);
void danarZombie(Zombie* lista, int cantidad, int fila, int columna, int danio);

//LogicaPlantas.c
int Plantar(struct Mapa *mapa, int fila, int columna, char planta);
void disparoPlantas(struct Mapa *mapa, Zombie* lista, int cantidad);

//LogicaProyectil.c
void moverProyectiles(struct Mapa *mapa, Zombie* lista, int cantidad);
