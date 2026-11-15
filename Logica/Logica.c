#include "Logica.h"

//Vuelve a dibujar matrizLogica: borra los zombies de la posición anterior (los proyectiles '-' se
//conservan) y coloca los zombies de la lista en su posición actual ('Z').

int soles;//Como variable global para que pueda ser utilizada por las demás funciones

static void actualizarMatrizLogica(struct Mapa *jardin, Zombie *lista, int cantidad)
{
    for (int i = 0; i < jardin->filas; i++){
        for (int j = 0; j < jardin->columnas; j++){
            if (jardin->matrizLogica[i][j] != '-'){jardin->matrizLogica[i][j] = '.';}
        }
    }
    for (int i = 0; i < cantidad; i++){jardin->matrizLogica[lista[i].fila][lista[i].columna] = 'Z';}
}

//Lee una tecla si se pulsó alguna (sin detener el juego):
//flechas mueven el cursor, 1/2/3 eligen la planta y espacio o Enter planta
static void leerTeclado(Movimiento_Usuario *movimiento, struct Mapa *jardin)
{
    int tecla = getch(); //retorna ERR si no se pulsó nada

    if (tecla == KEY_UP && movimiento->fila > 0){movimiento->fila--;}
    else if (tecla == KEY_DOWN && movimiento->fila < jardin->filas - 1){movimiento->fila++;}
    else if (tecla == KEY_LEFT && movimiento->columna > 1){movimiento->columna--;} //la columna 0 es la casa
    else if (tecla == KEY_RIGHT && movimiento->columna < jardin->columnas - 1){movimiento->columna++;}
    else if (tecla >= '0' && tecla <= '5'){movimiento->planta = (char)tecla;}
    else if ( (tecla == ' ' || tecla == '\n') && movimiento->planta != '0'){plantar(jardin, movimiento->fila, movimiento->columna, movimiento->planta, &soles);}
    else if ( (tecla == ' ' || tecla == '\n') && movimiento->planta == '0'){pala(jardin,movimiento->fila,movimiento->columna, &soles);}
}

//TEMPORAL: dibuja el jardín en consola mientras no exista GUI.c (el cursor se ve con colores invertidos)
static void imprimirJardin(struct Mapa *jardin,int vidaCasa,int soles,Movimiento_Usuario movimiento,int oleada,int totalOleadas,Zombie *listaZombies,int cantidadZombies)
{
    erase();

    mvprintw(0, 0, "Casa: %d  Soles: %d  Planta: %c",vidaCasa, soles, movimiento.planta);

    mvprintw(1, 0, "Oleada: %d / %d", oleada, totalOleadas);

    for (int i = 0; i < jardin->filas; i++) {
        for (int j = 0; j < jardin->columnas; j++) {

            char logico = jardin->matrizLogica[i][j];
            char c = (logico != '.') ? logico : jardin->matriz[i][j];

            int color = 0;

            // Determinar si hay un zombie en esta posición.
            for (int k = 0; k < cantidadZombies; k++) {
                if (listaZombies[k].fila == i &&
                    listaZombies[k].columna == j) {

                    switch (listaZombies[k].armadura) {
                        case 0: color = 1; break; // Normal: verde
                        case 1: color = 7; break; // Cono: amarillo/naranjo
                        case 2: color = 6; break; // Casco: azul
                    }

                    break;
                }
            }

            // Si no es zombie, determinar el color de la planta.
            if (color == 0) {
                switch (c) {
                    case '2': color = 1; break; // Lanzaguisantes: verde
                    case '1': color = 2; break; // Girasol: amarillo
                    case '5': color = 3; break; // Ajo: blanco
                    case '3': color = 5; break; // Nuez: naranja
                    case '4': color = 4; break; // PetaCereza: rojo
                }
            }

            if (i == movimiento.fila && j == movimiento.columna)
                attron(A_REVERSE);

            if (color != 0)
                attron(COLOR_PAIR(color));

            mvaddch(i + 2, j, c);

            if (color != 0)
                attroff(COLOR_PAIR(color));

            if (i == movimiento.fila && j == movimiento.columna)
                attroff(A_REVERSE);
        }
    }
    refresh();
}

void logica(struct Data data, struct Mapa* jardin, int solesIniciales)
{
    soles = solesIniciales;
    Zombie* listaZombies = NULL;
    int cantidadZombies = 0;
    int capacidadZombies = 0;

    // Sistema de oleadas
    int oleadaActual = 1;
    int zombiesPorOleada = 10;
    int zombiesGenerados = 0;

    int pausaEntreOleadas = 0;
    float tiempoPausa = 0.0f;


    int vidaCasa = (int)(data.resistenciaCasa * 100);

    Movimiento_Usuario movimiento = {0, 0, '.'};
    initscr();            
    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_WHITE, -1);
    init_pair(4, COLOR_RED, -1);
    init_pair(5, COLOR_YELLOW, -1);
    init_pair(6, COLOR_BLUE, -1);
    init_pair(7, COLOR_YELLOW, -1);  

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
        usleep(33333); //~30 frames por segundo
        leerTeclado(&movimiento, jardin);
        eliminarZombiesMuertos(listaZombies, &cantidadZombies);
        eliminarPlantasMuertas(jardin);
        actualizarMatrizLogica(jardin, listaZombies, cantidadZombies);
        explosivosDañar(jardin, listaZombies,cantidadZombies);

        tiempoSpawn += ( DT * (1.0f + (oleadaActual - 1) * 0.05f) );
        tiempoSol += DT;
        tiempoDisparo += DT;
        tiempoProyectil += DT;
        tiempoImprimir += DT;

        //Spawn de zombies (usa la frecuencia del Config.ini)
        if (!pausaEntreOleadas && zombiesGenerados < zombiesPorOleada)
        {
            if (tiempoSpawn >= data.frecuenciaSpawnZombies)
            {
                Zombie *nuevo = spawnZombieAleatoreo(jardin, data, oleadaActual);
                if (nuevo != NULL)
                {
                    agregarZombie(&listaZombies, &cantidadZombies, &capacidadZombies, *nuevo);
                    jardin->matrizLogica[nuevo->fila][nuevo->columna] = 'Z';
                    zombiesGenerados++;
                    free(nuevo);
                }
                tiempoSpawn = 0.0f;
            }
        }
        // Comprobar si terminó la oleada
        if (!pausaEntreOleadas && zombiesGenerados == zombiesPorOleada && cantidadZombies == 0)
        {
            if (oleadaActual >= data.numeroOleadas)
            {
                // Se completaron todas las oleadas
                break;
            }
            pausaEntreOleadas = 1;
            tiempoPausa = 0.0f;
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
            if (resultado == MOV_CASA){vidaCasa -= DAÑO_ZOMBIE_CASA;}
            if (resultado == MOV_PLANTA){dañarPlantas(listaZombies[i].fila, listaZombies[i].columna - 1, DAÑO_ZOMBIES_PLANTAS);}
            if (resultado == MOV_AJO){dañarPlantas(listaZombies[i].fila, listaZombies[i].columna - 1, DAÑO_ZOMBIES_PLANTAS*0.5);}
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
            imprimirJardin(jardin, vidaCasa, soles, movimiento,oleadaActual,data.numeroOleadas,listaZombies,cantidadZombies);
            tiempoImprimir = 0.0f;
        }
        
        if (pausaEntreOleadas)
        {
            tiempoPausa += DT;
            if (tiempoPausa >= 10.0f)
            {
                oleadaActual++;
                zombiesPorOleada += 5;
                zombiesGenerados = 0;

                tiempoSpawn = 0.0f;
                tiempoPausa = 0.0f;

                pausaEntreOleadas = 0;
            }
        }
    }

    endwin();
    printf("\nLos zombies se comieron tu casa. Fin del juego.\n");
    free(listaZombies);
    liberarMapa(jardin);
}

