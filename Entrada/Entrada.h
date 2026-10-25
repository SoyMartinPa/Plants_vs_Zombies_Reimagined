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
   int resistenciaZombies;
   int resistenciaCasa;
   int resistenciaPlantas;
   float frecuenciaDisparoPlantas; //frecuencia medida en segundos
   float frecuenciaSoles;
};
struct Mapa{
   int filas;
   int columnas;
   char **matriz;
};



struct Data leerConfiguracion();
struct Mapa* crearMapa(int filas, int columnas,int cantidadObstaculos);
void agregarObstaculos(struct Mapa *mapa, int cantidadObstaculos);
void guardarMapaEnArchivo(struct Mapa *mapa);
struct Mapa* cargarMapa();
