#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Nota: Si se quiere ampliar la configuración, se debe modificar el struct y 
//la función "crearConfiguracion" y el while de "leerConfiguración"
//Esto claramente rompe los principios SOLID pero como como c no está orientado a objetos...
struct Data{
   int numeroOleadas;
   float resistenciaZombies; //La resistencia es un factor, si 1 es valor por defecto. Si 1.5, vidaZombie *1.5
   float resistenciaCasa;    //(float: con int, un 1.5 del Config.ini se truncaba a 1)
   float resistenciaPlantas; 
   float frecuenciaDisparoPlantas; //frecuencia medida en segundos
   float frecuenciaSoles;
   float frecuenciaSpawnZombies; //5.0 segundos por defecto
   float velocidadMovimientoZombies; // 1.0 segundos por casilla por defecto

};

struct Mapa{
   int filas; 
   int columnas;
   char **matriz; //Matriz del mapa o jardín (suelo 'H' '.' 'O' y plantas)
   char **matrizLogica; //mapa de los zombies ('Z') y proyectiles ('-')
};

struct Data leerConfiguracion(void);
struct Mapa* crearMapa(int filas, int columnas,int cantidadObstaculos);
void agregarObstaculos(struct Mapa *mapa, int cantidadObstaculos);
void guardarMapaEnArchivo(struct Mapa *mapa);
struct Mapa* cargarMapa(void);
void liberarMapa(struct Mapa *mapa);
