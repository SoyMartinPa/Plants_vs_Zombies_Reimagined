#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <math.h>
#include "Entrada.h"

// Estructuras de datos para guardar el estado de la partida
typedef struct {
    int filas;
    int columnas;
    int soles;
    int vidaCasa;
    int oleadaActual;
    int cantidadZombies;
} DatosPartida_Guardada;

//Casilla marcada por el jugador y planta elegida
typedef struct
{
    int fila;
    int columna;
    char planta; //'0', '1', '2', '3', '4' o '5'
} Movimiento_Usuario;

//Tiempo que dura un "frame" (30 por segundo). Todos los temporizadores avanzan en DT por vuelta.
#define DT 0.0333f
#define DAÑO_GUISANTE 20
#define DAÑO_ZOMBIE_CASA 10        //daño que hace un zombie a la casa por cada "mordida"
#define DAÑO_ZOMBIES_PLANTAS 150      
#define INTERVALO_PROYECTIL 0.25f   //segundos que tarda un proyectil en avanzar una casilla
#define PLANTA_LANZAGUISANTES '2'
#define DAÑO_PETACEREZA 300

//Acciones del menu principal
#define NUEVO 1
#define REINICIAR 2
#define CARGAR 3
#define SALIR 4
//Tamaños de mapa
#define PEQUENO 1
#define MEDIANO 2
#define GRANDE 3
//Dificultades
#define FACIL 1
#define NORMAL 2
#define DIFICIL 3
//Tipos de zombies
#define ZOMBIE_NORMAL 0
#define ZOMBIE_FLAG   1
#define FACTOR_VELOCIDAD_FLAG 0.5f  //El zombie abanderado se mueve el doble de rápido que un zombie normal

//Resultados de moverZombie
enum { MOV_BLOQUEADO = 0, MOV_AVANZO = 1, MOV_CASA = 2, MOV_PLANTA = 3, MOV_AJO = 4};
// Estructura de configuraciones del juego
struct Configuraciones {
    int accion;       // NUEVO, CONTINUAR o SALIR
    int tamanoMapa;   // PEQUENO, MEDIANO o GRANDE
    int dificultad;  // FACIL, NORMAL o DIFICIL
    int solesIniciales;
};
// Todas lac caracteristicas de un zombie se guardan en esta estructura
typedef struct
{
    int fila;
    int columna;
    int vida; //Puntos de salud del zombie
    int armadura; //0: sin armadura, 1: cono (vida x1.5), 2: casco (vida x2)
    int tipo;
    float movimiento; //Segundos acumulados desde el último movimiento (float: con int el += 0.0333 nunca sumaba)
} Zombie ;
// Todas las caracteristicas de un lanzaguisante se guardan en esta estructura
typedef struct
{
    int fila;
    int columna;
    int vida; 
    float movimientoDisparo;
} LanzaGuisantes ;
// Todas las caracteristicas de un girasol se guardan en esta estructura
typedef struct
{
    int fila;
    int columna;
    int vida; 
    float movimientoSol;
} Girasol ;
// Todas las caracteristicas de una nuez se guardan en esta estructura
typedef struct
{
    int fila;
    int columna;
    int vida; 
} Nuez ;
// Todas las caracteristicas de un petacereza se guardan en esta estructura
typedef struct
{
    int fila;
    int columna;
    float tiempoExplosion; 
} PetaCereza ;
// Todas las caracteristicas de un ajo se guardan en esta estructura
typedef struct
{
    int fila;
    int columna;
    int vida; 
} Ajo ;

//Las plantas se guardan en 'matriz' como un dígito '1'..'5'
static inline int esPlanta(char c){return c >= '1' && c <= '5';}

//Logica.c
void logica(struct Data data, struct Mapa* jardin, int solesIniciales, Zombie **listaZombiesRef, int *cantidadZombiesRef, int *capacidadZombiesRef, int *vidaCasaRef, int *oleadaActualRef, int *salirAlMenuRef);

//LogicaZombies.c
Zombie* spawnZombieAleatoreo(struct Mapa *mapa, struct Data data, int oleadaActual, int *bandera);
int moverZombie(Zombie *zombie, struct Mapa *mapa);
void agregarZombie(Zombie** lista, int* cantidad, int* capacidad, Zombie nuevo);
void eliminarZombiesMuertos(Zombie* lista, int* cantidad);
void dañarZombie(Zombie* lista, int cantidad, int fila, int columna, int danio);

//LogicaPlantas.c
int plantar(struct Mapa *mapa, int fila, int columna, char planta, int* soles);
int pala(struct Mapa *mapa, int fila, int columna, int* soles);
int dañarPlantas(int fila, int columna, int daño);
void eliminarPlantasMuertas(struct Mapa *mapa);
int generarSoles(void);
void disparoPlantas(struct Mapa *mapa, Zombie* lista, int cantidad);
void explosivosDañar(struct Mapa *mapa, Zombie* lista, int cantidad);

//LogicaProyectil.c
void moverProyectiles(struct Mapa *mapa, Zombie* lista, int cantidad);

//configuraciones
struct Configuraciones menu(void);

//Funciones para guardar y cargar partidas
void guardarPartidaCompleta(const char *ruta, struct Mapa *mapa, Zombie *listaZombies, int cantidadZombies, int soles, int vidaCasa, int oleadaActual);
struct Mapa* cargarPartidaCompleta(const char *ruta, Zombie **listaZombies, int *cantidadZombies, int *capacidadZombies, int *soles, int *vidaCasa, int *oleadaActual);


// Función para mostrar un menu de selección y devolver la opción elegida
int seleccionar(const char *titulo, const char *opciones[], int n);