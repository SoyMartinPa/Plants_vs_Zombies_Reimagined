#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Logica.h"

int main(void)
{


    srand(time(NULL)); // Semilla de inicializacion

    struct Configuraciones config;

    // Ciclo principal para que el juego vuelva al menu en lugar de cerrarse
    while (1) {
        config = menu();

        if (config.accion == SALIR)
         break; // Cierra el juego definitivamente si elige Exit
        
        struct Mapa* jardin = NULL;
        struct Data data = leerConfiguracion(config.dificultad);
        int soles = config.solesIniciales;

        // Variables generales para el control de la partida y guardado o carga
        Zombie *listaZombies = NULL;
        int cantidadZombies = 0;
        int capacidadZombies = 0;
        int vidaCasa = (int)(data.resistenciaCasa * 100);
        int oleadaActual = 1;
        int salirAlMenu = 0;
        // Dependiendo de la accion elegida en el menu se inicia una nueva partida se reinicia o se carga una partida guardada
        if (config.accion == NUEVO) {
            jardin = crearMapa(config.tamanoMapa * 4, config.tamanoMapa * 5 + 5, config.dificultad * 5);
            remove("Archivos/Config.ini");
            logica(data, jardin, soles, &listaZombies, &cantidadZombies, &capacidadZombies, &vidaCasa, &oleadaActual, &salirAlMenu);
        }
        else if (config.accion == REINICIAR) {
            jardin = cargarMapa();
            logica(data, jardin, soles, &listaZombies, &cantidadZombies, &capacidadZombies, &vidaCasa, &oleadaActual, &salirAlMenu);
        }
        else if (config.accion == CARGAR) {
            // Carga la partida guardada desde el archivo binario
            jardin = cargarPartidaCompleta("Archivos/Guardado.dat", &listaZombies, &cantidadZombies, &capacidadZombies, &soles, &vidaCasa, &oleadaActual);

            // Si el archivo no existe creamos el mapa segun la configuración del menu
            if (jardin == NULL) {
                jardin = crearMapa(config.tamanoMapa * 4, config.tamanoMapa * 5 + 5, config.dificultad * 5);
            }

            logica(data, jardin, soles, &listaZombies, &cantidadZombies, &capacidadZombies, &vidaCasa, &oleadaActual, &salirAlMenu);
        }

        // Si la bandera salirAlMenu se activó, el bucle externo 'while(1)' 
        // volvera a mostrar automáticamente el menu principal.
        if (salirAlMenu) {
            continue;
        }
    }

    return 0;
}
//siempre que se llama a logica es la funcion que controla todo el juego