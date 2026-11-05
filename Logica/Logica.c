#include "Logica.h"

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

//TEMPORAL: dibuja el jardín en consola mientras no exista GUI.c
static void imprimirJardin(struct Mapa *jardin, int vidaCasa, int soles)
{
    printf("\033[H\033[J"); //limpia la consola
    printf("Casa: %d  Soles: %d\n\n", vidaCasa, soles);
    for (int i = 0; i < jardin->filas; i++){
        for (int j = 0; j < jardin->columnas; j++){
            char logico = jardin->matrizLogica[i][j];
            putchar(logico != '.' ? logico : jardin->matriz[i][j]);
        }
        putchar('\n');
    }
    fflush(stdout);
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

    //Temporizadores: acumulan segundos y se reinician cuando llegan a su intervalo
    float tiempoSpawn = 0.0f;
    float tiempoSol = 0.0f;
    float tiempoDisparo = 0.0f;
    float tiempoProyectil = 0.0f;
    float tiempoImprimir = 0.0f;

    //PRUEBA: plantas de ejemplo. Borrar cuando exista pedirPlanta / la GUI
    Plantar(jardin, 2, 2, PLANTA_LANZAGUISANTES);
    Plantar(jardin, 0, 3, PLANTA_LANZAGUISANTES);

    while (vidaCasa > 0)
    {
        usleep(33333); //~30 frames por segundo (antes se usaba wait(), que NO es una pausa: espera hijos)

        eliminarZombiesMuertos(listaZombies, &cantidadZombies);
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
            //TODO: if (resultado == MOV_PLANTA) -> dañar la planta (falta guardar la vida de cada planta)
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
            imprimirJardin(jardin, vidaCasa, soles);
            tiempoImprimir = 0.0f;
        }
    }

    printf("\nLos zombies se comieron tu casa. Fin del juego.\n");
    free(listaZombies);
    liberarMapa(jardin);
}
