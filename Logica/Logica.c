#include "Logica.h"

int soles; // Variable global para compartir el contador de soles con las funciones de plantado y herramientas

// Actualiza la matriz lógica del mapa colocando 'Z' para zombis normales y 'F' para abanderados
// limpiando previamente las celdas para evitar "fantasmas" visuales en el tablero
static void actualizarMatrizLogica(struct Mapa *jardin, Zombie *lista, int cantidad)
{
    // Recorre todo el jardin restableciendo las posiciones a suelo vacío '.' (siempre que no sean obstáculos '-')
    for (int i = 0; i < jardin->filas; i++){
        for (int j = 0; j < jardin->columnas; j++){
            if (jardin->matrizLogica[i][j] != '-'){jardin->matrizLogica[i][j] = '.';}
        }
    }
    
    // Posiciona en la matriz logica a cada zombi activo segun su tipo
    for (int i = 0; i < cantidad; i++){
        char letra = 'Z'; // Carácter por defecto para zombis normales

        if (lista[i].tipo == ZOMBIE_FLAG) {
            letra = 'F';  // Zombi abanderado 
        } 
        else {
            letra = 'Z';  
        }

        jardin->matrizLogica[lista[i].fila][lista[i].columna] = letra;
    }
}

// Lee las entradas del teclado en tiempo real para mover el cursor seleccionar, plantas,
// plantar, usar la pala o abrir el menú de pausa mediante la tecla ESC.
// Lee la entrada del teclado en tiempo real: gestiona la pausa con ESC y,
// cuando el juego está activo, mueve el cursor, selecciona plantas/herramientas o planta/usa la pala.
static void leerTeclado(Movimiento_Usuario *movimiento, struct Mapa *jardin, Zombie **listaZombies, int *cantidadZombies,  int *vidaCasa, int *oleadaActual, int *salirAlMenu, int *pausado)
{
    
    int tecla = getch();// Captura la tecla presionada sin bloquear el bucle principal

    // Tecla 27 = ESC: activa la bandera de pausa y sale para congelar la simulacion
    if (tecla == 27) {
        *pausado = 1;
        return;
    }

    // Si esta pausado, muestra el menu bloqueante y procesa la opción elegida
    if (*pausado) {
        const char *opcionesPausa[] = {
            "Seguir jugando",
            "Guardar partida",
            "Salir al menu principal"
        };

        nodelay(stdscr, FALSE); // Espera a que el usuario elija una opcion en el menu
        int seleccionPausa = seleccionar("JUEGO PAUSADO", opcionesPausa, 3);
        nodelay(stdscr, TRUE);  // Restaura el modo en tiempo real para el bucle del juego
        keypad(stdscr, TRUE);   // Asegura que las teclas especiales (flechas) sigan activas
        flushinp();             // Limpia teclas residuales del búfer tras cerrar el menu

        // Guarda el estado actual de la partida en disco
        if (seleccionPausa == 1) {
            guardarPartidaCompleta("Archivos/Guardado.dat", jardin, *listaZombies, *cantidadZombies, soles, *vidaCasa, *oleadaActual);
        }
        // Marca la salida para romper el bucle principal y volver al menu inicial
        else if (seleccionPausa == 2) {
            *salirAlMenu = 1;
        }

        *pausado = 0; // Libera la pausa para reanudar o salir limpiamente
        return;
    }

    // Desplazamiento del cursor dentro de los limites válidos del jardín
    if (tecla == KEY_UP && movimiento->fila > 0){movimiento->fila--;}
    else if (tecla == KEY_DOWN && movimiento->fila < jardin->filas - 1){movimiento->fila++;}
    else if (tecla == KEY_LEFT && movimiento->columna > 1){movimiento->columna--;} 
    else if (tecla == KEY_RIGHT && movimiento->columna < jardin->columnas - 1){movimiento->columna++;}
    
    // Selección rapida de planta ('1'-'5') o pala ('0')
    else if (tecla >= '0' && tecla <= '5'){movimiento->planta = (char)tecla;}
    
    // Coloca la planta equipada en la celda actual con Espacio o Enter
    else if ((tecla == ' ' || tecla == '\n') && movimiento->planta != '0'){
        plantar(jardin, movimiento->fila, movimiento->columna, movimiento->planta, &soles);
    }
    // Usa la pala ('0') para quitar una planta u obstáculo en la celda actual
    else if ((tecla == ' ' || tecla == '\n') && movimiento->planta == '0'){
        pala(jardin, movimiento->fila, movimiento->columna, &soles);
    }
}

// Dibuja la interfaz visual completa del juego en la terminal utilizando ncurses
// aplica colores dinámicos a plantas zombis con armadura y destacando el cursor
static void imprimirJardin(struct Mapa *jardin, int vidaCasa, int soles, Movimiento_Usuario movimiento, int oleada, int totalOleadas, Zombie *listaZombies, int cantidadZombies)
{
    erase(); // Limpia la pantalla en cada ciclo

    // Muestra los marcadores superiores de estado (Vida de la casa, Soles actuales, Planta equipada y Oleada)
    mvprintw(0, 0, "Casa: %d  Soles: %d  Planta: %c", vidaCasa, soles, movimiento.planta);
    mvprintw(1, 0, "Oleada: %d / %d", oleada, totalOleadas);

    // Recorre la matriz del jardin celda por celda para renderizar el tablero
    for (int i = 0; i < jardin->filas; i++) {
        for (int j = 0; j < jardin->columnas; j++) {

            char logico = jardin->matrizLogica[i][j];
            char c = (logico != '.') ? logico : jardin->matriz[i][j];

            int color = 0;

            // Identifica si la celda contiene un zombi para asignarle un color según su tipo y armadura
            for (int k = 0; k < cantidadZombies; k++) {
                if (listaZombies[k].fila == i &&
                    listaZombies[k].columna == j) {

                    if (listaZombies[k].tipo == ZOMBIE_FLAG) {
                        color = 4; // Abanderado (Rojo)
                    } else {
                        switch (listaZombies[k].armadura) {
                            case 0: color = 1; break; // Normal (Verde)
                            case 1: color = 7; break; // Cono (Amarillo)
                            case 2: color = 6; break; // Casco (Azul)
                        }
                    }
                    break;
                }
            }

            // Si la celda no tiene zombis asigna color según el tipo de planta presente
            if (color == 0) {
                switch (c) {
                    case '2': color = 1; break; // Lanzaguisantes
                    case '1': color = 2; break; // Girasol
                    case '5': color = 3; break; // Ajo
                    case '3': color = 5; break; // Nuez
                    case '4': color = 4; break; // PetaCereza
                }
            }

            // Resalta con inversión de video si el cursor se encuentra en esta posicion
            if (i == movimiento.fila && j == movimiento.columna)
                attron(A_REVERSE);

            // Aplica el par de colores correspondiente si esta definido
            if (color != 0)
                attron(COLOR_PAIR(color));

            mvaddch(i + 2, j, c); // Dibuja el caracter en pantalla

            if (color != 0)
                attroff(COLOR_PAIR(color));

            if (i == movimiento.fila && j == movimiento.columna)
                attroff(A_REVERSE);
        }
    }
    refresh(); // Actualiza la pantalla con los cambios impresos
}

// Bucle principal de la partida: controla temporizadores, fisicas, movimientosde zombis, 
// generación de soles, oleadas, transiciones y condiciones de victoria/derrota.
void logica(struct Data data, struct Mapa* jardin, int solesIniciales, Zombie **listaZombiesRef, int *cantidadZombiesRef, int *capacidadZombiesRef, int *vidaCasaRef, int *oleadaActualRef, int *salirAlMenuRef)
{
    (void)salirAlMenuRef; // Evita advertencias del compilador por parámetro no utilizado
    soles = solesIniciales; 
    
    // Inicializa o recupera el estado de los zombis (soporta partidas cargadas o nuevas)
    Zombie *listaZombies = (listaZombiesRef != NULL) ? *listaZombiesRef : NULL;
    int cantidadZombies = (cantidadZombiesRef != NULL) ? *cantidadZombiesRef : 0;
    int capacidadZombies = (capacidadZombiesRef != NULL) ? *capacidadZombiesRef : 0;

    int oleadaActual = (oleadaActualRef != NULL) ? *oleadaActualRef : 1;
    int zombiesPorOleada = 8 + (oleadaActual - 1) * 5;
    int zombiesGenerados = 0;

    int pausaEntreOleadas = 1;
    float tiempoPausa = 0.0f;

    int preparacionInicial = (oleadaActual == 1) ? 1 : 0;
    
    int vidaCasa = (vidaCasaRef != NULL) ? *vidaCasaRef : (int)(data.resistenciaCasa * 100);
    int salirAlMenu = 0;

    Movimiento_Usuario movimiento = {0, 0, '.'};
    
    // Configuración inicial de ncurses y paleta de colores para la terminal
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
              
    float tiempoSpawn = 0.0f;
    float tiempoSol = 0.0f;
    float tiempoDisparo = 0.0f;
    float tiempoProyectil = 0.0f;


    int pausado = 0; // Bandera para pausar el juego cuando se presiona ESC
    int bandera = 0; //indica si el zombie a generar debe poseer bandera (Un zombie bandera al inicio de cada ronda)

    // Bucle principal del juego en ejecución (se ejecuta mientras la casa tenga vida y no se pida salir)
    while (vidaCasa > 0 && !salirAlMenu)
    {
        usleep(33333); // Retardo para mantener una tasa constante de ~30 fotogramas por segundo
        
        // Lee las pulsaciones de teclado del usuario
        leerTeclado(&movimiento, jardin, &listaZombies, &cantidadZombies,&vidaCasa, &oleadaActual, &salirAlMenu, &pausado);
        
        if (salirAlMenu) break;
        
        if (!pausado)
        {
            // Limpieza de entidades destruidas y actualización de matrices y daño por explosivos
            eliminarZombiesMuertos(listaZombies, &cantidadZombies);
            eliminarPlantasMuertas(jardin);
            actualizarMatrizLogica(jardin, listaZombies, cantidadZombies);
            explosivosDañar(jardin, listaZombies, cantidadZombies);

            // Acumula el tiempo transcurrido en el frame actual (DT)
            tiempoSpawn += (DT * (1.0f + (oleadaActual - 1) * 0.05f));
            tiempoSol += DT;
            tiempoDisparo += DT;
            tiempoProyectil += DT;
            
            int permitirSpawn = 1; // Bandera para controlar si se permite generar un nuevo zombi en este ciclo

            if (preparacionInicial) {
                permitirSpawn = 0; 
            }

            // En la primera oleada, da un respiro inicial de 20 segundos al jugador antes de spawnear zombis
            if (oleadaActual == 1 && pausaEntreOleadas) {
                if (tiempoPausa < 20.0f) {
                    permitirSpawn = 0; 
                }
            }
            
            // Generacion automatica de zombis según la frecuencia y la oleada actual
            if (!pausaEntreOleadas && zombiesGenerados < zombiesPorOleada && permitirSpawn)
            {
                if (tiempoSpawn >= data.frecuenciaSpawnZombies)
                {
                    Zombie *nuevo = spawnZombieAleatoreo(jardin, data, oleadaActual, &bandera);
                    if (nuevo != NULL)
                    {
                        agregarZombie(&listaZombies, &cantidadZombies, &capacidadZombies, *nuevo);
                        zombiesGenerados++;
                        free(nuevo);
                    }
                    tiempoSpawn = 0.0f;
                }
            }

            // Control de fin de oleada: si ya se generaron todos y no quedan zombis vivos avanza o termina
            if (!pausaEntreOleadas && zombiesGenerados == zombiesPorOleada && cantidadZombies == 0)
            {
                if (oleadaActual >= data.numeroOleadas)
                {
                    break; // Si se completaron todas las oleadas
                }
                pausaEntreOleadas = 1;
                tiempoPausa = 0.0f;
            }
            
            soles += generarSoles(); // Genera soles periodicamente segun la frecuencia definida en la configuración del juego

            // Incremento periodico de soles automaticos
            if (tiempoSol >= data.frecuenciaSoles)
            {
                soles += 25;
                tiempoSol = 0.0f;
            }

            // Actualiza el movimiento y las acciones de ataque de cada zombi activo
            // Recorre la lista de zombis y mueve cada uno según su velocidad y tipo, aplicando daño a la casa o plantas si corresponde
            for (int i = 0; i < cantidadZombies; i++)
            {
                if (listaZombies[i].vida <= 0){continue;}

                listaZombies[i].movimiento += DT;
                float velocidad = data.velocidadMovimientoZombies;
                if (listaZombies[i].tipo == ZOMBIE_FLAG){velocidad *= FACTOR_VELOCIDAD_FLAG;}

                if (listaZombies[i].movimiento < velocidad){continue;}
                listaZombies[i].movimiento = 0.0f;

                int filaOriginal = listaZombies[i].fila; 
                int resultado = moverZombie(&listaZombies[i], jardin);
                if (resultado == MOV_CASA){vidaCasa -= DAÑO_ZOMBIE_CASA;}
                if (resultado == MOV_PLANTA){dañarPlantas(filaOriginal, listaZombies[i].columna - 1, DAÑO_ZOMBIES_PLANTAS);}
                if (resultado == MOV_AJO){dañarPlantas(filaOriginal, listaZombies[i].columna - 1, (int)(DAÑO_ZOMBIES_PLANTAS * 0.5));}
            }
            // Control de disparo de las plantas 
            if (tiempoDisparo >= data.frecuenciaDisparoPlantas)
            {
                disparoPlantas(jardin, listaZombies, cantidadZombies);
                tiempoDisparo = 0.0f;
            }

            // Actualiza el desplazamiento de los proyectiles en el mapa
            if (tiempoProyectil >= INTERVALO_PROYECTIL)
            {
                moverProyectiles(jardin, listaZombies, cantidadZombies);
                tiempoProyectil = 0.0f;
            }

            // Maneja el tiempo de preparación inicial o el descanso entre oleadas
            if (preparacionInicial)
            {
                tiempoPausa += DT;
                if (tiempoPausa >= 20.0f)
                {
                    preparacionInicial = 0;     // Termina la preparación inicial y permite que comiencen a salir los zombis
                    pausaEntreOleadas = 0;      
                    tiempoPausa = 0.0f;
                }
            }
            else if (pausaEntreOleadas)
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
                    bandera = 1;
                    agregarObstaculos(jardin, oleadaActual);
                }
            }
        }

        if (!pausado)
        {
            // Redibuja el jardin actualizado en la terminal
            imprimirJardin(jardin, vidaCasa, soles, movimiento, oleadaActual, data.numeroOleadas, listaZombies, cantidadZombies);
        }
    }
    endwin(); // Restaura el modo normal de la terminal al salir del entorno ncurses

    // Actualiza las referencias externas con el estado final al salir de la partida
    if (listaZombiesRef != NULL) *listaZombiesRef = listaZombies;
    if (cantidadZombiesRef != NULL) *cantidadZombiesRef = cantidadZombies;
    if (capacidadZombiesRef != NULL) *capacidadZombiesRef = capacidadZombies;
    if (vidaCasaRef != NULL) *vidaCasaRef = vidaCasa;
    if (oleadaActualRef != NULL) *oleadaActualRef = oleadaActual;

    // Despliega mensajes de termino de partida si no se regreso manualmente al menú principal si la casa fue destruida o si se completaron todas las oleadas
    if (!salirAlMenu)
    {
        nodelay(stdscr, FALSE);
        flushinp();
        erase();
        if (vidaCasa <= 0){mvprintw(LINES / 2, 2, "Los zombies se comieron tu casa. Fin del juego.");}
        else{mvprintw(LINES / 2, 2, "Felicidades! Completaste todas las oleadas.");}
        mvprintw(LINES / 2 + 2, 2, "Presiona una tecla para volver al menu...");
        refresh();
        getch();
    }
    endwin(); 

    
    free(listaZombies);
    reiniciarGirasoles(); // Reinicia el contador de girasoles para la siguiente partida
    liberarMapa(jardin);

}