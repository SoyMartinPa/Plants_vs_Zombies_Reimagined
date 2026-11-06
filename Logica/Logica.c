#include "Logica.h"
#include <ncurses.h>

//Vuelve a dibujar matrizLogica: borra los zombies de la posición anterior (los proyectiles '-' se
//conservan) y coloca los zombies de la lista en su posición actual ('Z').
static void actualizarMatrizLogica(struct Mapa *jardin, Zombie *lista, int cantidad)
{
    for (int i = 0; i < jardin->filas; i++){
        for (int j = 0; j < jardin->columnas; j++){
            if (jardin->matrizLogica[i][j] != '-'){jardin->matrizLogica[i][j] = '.';}
        }
    }
    for (int i = 0; i < cantidad; i++){jardin->matrizLogica[lista[i].fila][lista[i].columna] = 'Z';}
}
//Casilla marcada por el jugador y planta elegida
typedef struct
{
    int fila;
    int columna;
    char planta; //'1', '2' o '3'
} Movimiento_Usuario;

//Lee una tecla si se pulsó alguna (sin detener el juego):
//flechas mueven el cursor, 1/2/3 eligen la planta y espacio o Enter planta
static void leerTeclado(Movimiento_Usuario *movimiento, struct Mapa *jardin)
{
    int tecla = getch(); //retorna ERR si no se pulsó nada

    if (tecla == KEY_UP && movimiento->fila > 0){movimiento->fila--;}
    else if (tecla == KEY_DOWN && movimiento->fila < jardin->filas - 1){movimiento->fila++;}
    else if (tecla == KEY_LEFT && movimiento->columna > 1){movimiento->columna--;} //la columna 0 es la casa
    else if (tecla == KEY_RIGHT && movimiento->columna < jardin->columnas - 1){movimiento->columna++;}
    else if (tecla == '1' || tecla == '2' || tecla == '3'){movimiento->planta = (char)tecla;}
    else if (tecla == ' ' || tecla == '\n'){Plantar(jardin, movimiento->fila, movimiento->columna, movimiento->planta);}
}

//TEMPORAL: dibuja el jardín en consola mientras no exista GUI.c (el cursor se ve con colores invertidos)
static void imprimirJardin(struct Mapa *jardin, int vidaCasa, int soles, Movimiento_Usuario movimiento)
{
    erase(); 
    mvprintw(0, 0, "Casa: %d  Soles: %d  Planta: %c", vidaCasa, soles, movimiento.planta);
    for (int i = 0; i < jardin->filas; i++){
        for (int j = 0; j < jardin->columnas; j++){
            char logico = jardin->matrizLogica[i][j];
            char c = (logico != '.') ? logico : jardin->matriz[i][j];

            if (i == movimiento.fila && j == movimiento.columna){attron(A_REVERSE);}
            mvaddch(i + 2, j, c);
            attroff(A_REVERSE);
        }
    }
    refresh(); //muestra todo junto
}

void logica(void)
{
    struct Data data = leerConfiguracion(); //Ver info de DATA en Entrada.h
    struct Mapa* jardin = cargarMapa(); //Jardin es una matriz Jardin[][]

    Zombie* listaZombies = NULL;
    int cantidadZombies = 0;
    int capacidadZombies = 0;

    int soles = 0;
    int vidaCasa = (int)(data.resistenciaCasa * 100);

    Movimiento_Usuario movimiento = {0, 0, '.'};
    initscr();              
    cbreak();               
    noecho();               
    keypad(stdscr, TRUE);  
    nodelay(stdscr, TRUE);  
              


    //Temporizadores: acumulan segundos y se reinician cuando llegan a su intervalo
    float tiempoSpawn = 0.0f;
    float tiempoSol = 0.0f;
    float tiempoDisparo = 0.0f;
    float tiempoProyectil = 0.0f;
    float tiempoImprimir = 0.0f;


    while (vidaCasa > 0)
    {
        usleep(33333); //~30 frames por segundo (antes se usaba wait(), que NO es una pausa: espera hijos)
        leerTeclado(&movimiento, jardin);
        eliminarZombiesMuertos(listaZombies, &cantidadZombies);
        eliminarPlantasMuertas(jardin);
        actualizarMatrizLogica(jardin, listaZombies, cantidadZombies);

        tiempoSpawn += DT;
        tiempoSol += DT;
        tiempoDisparo += DT;
        tiempoProyectil += DT;
        tiempoImprimir += DT;

        //Spawn de zombies (usa la frecuencia del Config.ini)
        if (tiempoSpawn >= data.frecuenciaSpawnZombies)
        {
            Zombie *nuevo = SpawnZombieAleatoreo(jardin, data);
            if (nuevo != NULL)
            {
                agregarZombie(&listaZombies, &cantidadZombies, &capacidadZombies, *nuevo);
                jardin->matrizLogica[nuevo->fila][nuevo->columna] = 'Z';
                free(nuevo);
            }
            tiempoSpawn = 0.0f;
        }
        

        soles += generarSoles();


        //Aparece un sol
        if (tiempoSol >= data.frecuenciaSoles)
        {
            soles += 25;
            tiempoSol = 0.0f;
        }

        //Movimiento de zombies (usa la velocidad del Config.ini)
        for (int i = 0; i < cantidadZombies; i++)
        {
            if (listaZombies[i].vida <= 0){continue;}

            listaZombies[i].movimiento += DT;
            if (listaZombies[i].movimiento < data.velocidadMovimientoZombies){continue;}
            listaZombies[i].movimiento = 0.0f;

            int resultado = moverZombie(&listaZombies[i], jardin);
            if (resultado == MOV_CASA){vidaCasa -= DANIO_ZOMBIE_CASA;}
            if (resultado == MOV_PLANTA){danarPlanta(jardin, listaZombies[i].fila, listaZombies[i].columna - 1);}
        }

        //Las plantas disparan
        if (tiempoDisparo >= data.frecuenciaDisparoPlantas)
        {
            disparoPlantas(jardin, listaZombies, cantidadZombies);
            tiempoDisparo = 0.0f;
        }

        //Los proyectiles avanzan (antes este if estaba DENTRO del de disparo)
        if (tiempoProyectil >= INTERVALO_PROYECTIL)
        {
            moverProyectiles(jardin, listaZombies, cantidadZombies);
            tiempoProyectil = 0.0f;
        }

        if (tiempoImprimir >= 0.2f)
        {
            imprimirJardin(jardin, vidaCasa, soles, movimiento);
            tiempoImprimir = 0.0f;
        }
    }


    endwin();
    printf("\nLos zombies se comieron tu casa. Fin del juego.\n");
    free(listaZombies);
    liberarMapa(jardin);
}
