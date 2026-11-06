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
    #ifdef _WIN32
    activarANSI();
    #endif

    srand(time(NULL)); //Semilla de inicialización
    logica();
    
    return 0;
}



