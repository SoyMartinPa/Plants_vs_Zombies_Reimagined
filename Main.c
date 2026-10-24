#include <stdio.h>
#include "Entrada.h"

int main(void)
{
    struct Data configuracion = leerConfiguracion(); //Ver info de DATA en Entrada.h
    char **jardin = cargarMapa(); //Jardin es una matriz Jardin[][]
    return 0;
}