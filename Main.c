#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Logica.h"

#ifdef _WIN32
#include <windows.h>
void activarANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

int main(void)
{
    srand(time(NULL)); //Semilla de inicialización
    struct Configuraciones config = menu();

    if (config.accion == SALIR)
        return 0;

    if (config.accion == NUEVO){
        struct Mapa* jardin = crearMapa(config.tamanoMapa * 4, config.tamanoMapa * 5 + 5, config.dificultad * 5); //Jardin es una matriz Jardin[][]
        remove("Archivos/Config.ini");
        struct Data data = leerConfiguracion(config.dificultad); 
        logica(data, jardin, config.solesIniciales);
    }
    else if (config.accion == REINICIAR){
        struct Mapa* jardin = cargarMapa(); //Jardin es una matriz Jardin[][]
        struct Data data = leerConfiguracion(config.dificultad); 
        logica(data, jardin,config.solesIniciales);
    }
    else {return 0;}

    #ifdef _WIN32
    activarANSI();
    #endif
    return 0;
}



