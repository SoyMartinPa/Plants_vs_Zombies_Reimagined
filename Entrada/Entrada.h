#pragma once

#include <stdio.h>
#include <string.h>

//Nota: Si se quiere ampliar la configuración, se debe modificar el struct y 
//la función "crearConfiguracion" y el while de "leerConfiguración"
//Esto claramente rompe los principios SOLID pero como como c no está orientado a objetos...
struct Data{
   int numeroOleadas;
   int resistenciaZombies;
   int resistenciaCasa;
   float frecuenciaDisparoPlantas; //frecuencia medida en segundos
   float frecuenciaSoles;
   int cantidadObstaculos; //Va en un intervalo de dificultad del 0 al 5

};

struct Data leerConfiguracion();
