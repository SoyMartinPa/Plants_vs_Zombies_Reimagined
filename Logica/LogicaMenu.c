#include "Logica.h"

// Dibuja dinamicamente una lista de opciones centrada en la pantalla con ncurses 
// y destaca visualmente la opción actualmente seleccionada usando inversion de video
static void mostrarOpciones(const char *titulo, const char *opciones[], int cantidad, int seleccion)
{
    erase(); // Limpia la pantalla en cada cuadro/refresco
    
    // Nota: COLS y LINES están definidas por ncurses.h y representan el ancho y alto de la terminal
    int ancho = COLS;
    int alto = LINES;

    // Imprime el titulo centrado en la parte superior del menu
    mvprintw(alto / 2 - cantidad / 2 - 3, (ancho - (int)strlen(titulo)) / 2, "%s", titulo);

    // Recorre e imprime cada opción del menu
    for (int i = 0; i < cantidad; i++) {
        int x = (ancho - (int)strlen(opciones[i])) / 2; // Centra cada texto horizontalmente
        int y = alto / 2 - cantidad / 2 + i;           // Distribuye las opciones verticalmente

        // Si es la opción seleccionada  invierte los colores (resalta el texto)
        if (i == seleccion) attron(A_REVERSE);

        mvprintw(y, x, "%s", opciones[i]);

        if (i == seleccion) attroff(A_REVERSE); // Desactiva la inversión de color
    }

    // Muestra la barra de ayuda y controles en la parte inferior de la pantalla
    mvprintw(alto - 2, 2, "Flechas: navegar | Enter: seleccionar | ESC: volver");

    refresh(); // Actualiza la pantalla para mostrar los cambios
}

// Controla el bucle de interaccion por teclado en los menus
// Permite navegar con las flechas Arriba/Abajo y confirmar con Enter o salir con ESC 
int seleccionar(const char *titulo, const char *opciones[], int cantidad)
{
    int seleccion = 0;
    int tecla;

    while (1) {
        // Dibuja el menu actual con la selección actual resaltada
        mostrarOpciones(titulo, opciones, cantidad, seleccion);

        tecla = getch(); // Captura la tecla presionada por el usuario

        // Mueve la selección hacia arriba si está dentro de los limites
        if (tecla == KEY_UP && seleccion > 0) 
            seleccion--;

        // Mueve la selección hacia abajo si está dentro de los limites
        else if (tecla == KEY_DOWN && seleccion < cantidad - 1) 
            seleccion++;

        // Confirma la selección actual con Enter
        else if (tecla == '\n' || tecla == KEY_ENTER) 
            return seleccion;

        // Cancela o retrocede al presionar ESC
        else if (tecla == 27) 
            return -1;
    }
}

// Permite al usuario ingresar por teclado una cantidad personalizada de soles iniciales.
// Soporta escritura digito por digito y borrado con Backspace
static int ingresarSoles(int solesActuales)
{
    int soles = solesActuales;
    int tecla;

    while (1) {
        erase();

        // Muestra la interfaz interactiva para modificar los soles
        mvprintw(LINES / 2 - 2, COLS / 2 - 15, "INGRESAR SOLES INICIALES");
        mvprintw(LINES / 2, COLS / 2 - 15, "Cantidad: %d", soles);
        mvprintw(LINES - 2, 2, "Numeros: ingresar | Enter: confirmar | ESC: cancelar");

        refresh();

        tecla = getch();

        // Si presiona un número del '0' al '9' lo agrega al valor acumulado multiplicando por 10
        if (tecla >= '0' && tecla <= '9') {
            if (soles <= 99999999) {
                soles = soles * 10 + (tecla - '0');
            }
        }
        // Si presiona borrar (Backspace) elimina el ultimo digito dividiendo entre 10
        else if (tecla == KEY_BACKSPACE || tecla == 127 || tecla == 8) {
            soles /= 10;
        }
        // Confirma el nuevo valor con Enter
        else if (tecla == '\n' || tecla == KEY_ENTER) {
            return soles;
        }
        // Cancela y retorna el valor que tenía originalmente con ESC
        else if (tecla == 27) {
            return solesActuales;
        }
    }
}

// Funcion principal que gestiona el menu de navegacion del juego
struct Configuraciones menu(void)
{
    // Configuracion por defecto inicial del juego
    struct Configuraciones config = {0, MEDIANO, NORMAL, 500};

    // Declaracion de opciones para los diferentes submenus
    const char *principal[] = {
        "Jugar",
        "Configuracion",
        "Readme",
        "Exit"
    };

    const char *partida[] = {
        "Reiniciar",
        "Nuevo juego",
        "Cargar partida"
    };

    const char *tamanos[] = {
        "Pequeno",
        "Mediano",
        "Grande"
    };

    const char *dificultades[] = {
        "Facil",
        "Normal",
        "Dificil"
    };

    int opcion;

    // Inicializa el entorno de la terminal para ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Bucle principal del menú de navegación
    while (1) {
        // Muestra el menú principal
        opcion = seleccionar("PLANTS VS ZOMBIES REIMAGINED", principal, 4);

        // Opcion 0: JUGAR (Submenu de modos de partida)
        if (opcion == 0) {
            opcion = seleccionar("JUGAR", partida, 3);

            if (opcion == 0) {
                config.accion = REINICIAR;
                break;
            }
            else if (opcion == 1) {
                config.accion = NUEVO;
                break;
            }
            else if (opcion == 2) {
                config.accion = CARGAR;
                break;
            }
        }
        // Opcion 1: CONFIGURACION (Tamaño, Dificultad, Soles iniciales)
        else if (opcion == 1) {
            int subopcion = seleccionar("CONFIGURACION",
                         (const char *[]) {
                                             "Tamano de matriz",
                                             "Dificultad",
                                             "Soles iniciales",
                                             "Volver"
                                         }, 4);

            if (subopcion == 0) {
                int tamano = seleccionar("TAMANO DE MATRIZ", tamanos, 3);
                if (tamano != -1) config.tamanoMapa = tamano + 1;
            }
            else if (subopcion == 1) {
                int dificultad = seleccionar("DIFICULTAD", dificultades, 3);
                if (dificultad != -1) config.dificultad = dificultad + 1;
            }
            else if (subopcion == 2) {
                config.solesIniciales = ingresarSoles(config.solesIniciales);
            }
        }
        // Opcion 2: README (Instrucciones y controles del juego)
        else if (opcion == 2) {
            erase();

            mvprintw(2, 4, "README - CONTROLES DEL JUEGO");
            mvprintw(4, 4, "Usando los numeros y la barra espaciadora se pueden");
            mvprintw(5, 4, "utilizar las siguientes plantas y herramientas:");

            mvprintw(7, 6, "'0': Pala (Cuesta 200 soles quitar un obstaculo).");
            mvprintw(8, 6, "'1': Girasol (Vale 50 soles).");
            mvprintw(9, 6, "'2': Lanzaguisantes (Vale 100 soles).");
            mvprintw(10, 6, "'3': Nuez (Vale 50 soles).");
            mvprintw(11, 6, "'4': PetaCereza (Vale 150 soles).");
            mvprintw(12, 6, "'5': Ajo (Vale 50 soles).");

            mvprintw(14, 4, "El mapa tambien incorpora obstaculos ('O'),");
            mvprintw(15, 4, "los cuales impiden colocar plantas en las casillas");
            mvprintw(16, 4, "que ocupan. Se eliminan utilizando la pala.");

            mvprintw(LINES - 2, 2, "Presiona cualquier tecla para volver...");

            refresh();
            getch(); // Espera una tecla cualquiera para regresar al menú
        }
        // Opcion 3 o ESC: Salir del programa definitivamente
        else if (opcion == 3 || opcion == -1) {
            config.accion = SALIR;
            break;
        }
    }

    endwin(); // Finaliza el modo ncurses y restaura la terminal normal
    return config;
}