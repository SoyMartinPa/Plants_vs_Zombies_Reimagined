#include "Logica.h"

// Dibuja una lista de opciones y destaca la seleccionada.
static void mostrarOpciones(const char *titulo, const char *opciones[],int cantidad, int seleccion)
{
    erase();
    //Nota: COLS y LINES están definidas por <ncurses.h>
    int ancho = COLS;
    int alto = LINES;

    mvprintw(alto / 2 - cantidad / 2 - 3, (ancho - (int)strlen(titulo)) / 2, "%s", titulo);

    for (int i = 0; i < cantidad; i++) {
        int x = (ancho - (int)strlen(opciones[i])) / 2;
        int y = alto / 2 - cantidad / 2 + i;

        if (i == seleccion) attron(A_REVERSE);

        mvprintw(y, x, "%s", opciones[i]);

        if (i == seleccion) attroff(A_REVERSE);
    }

    mvprintw(alto - 2, 2, "Flechas: navegar | Enter: seleccionar | ESC: volver");

    refresh();
}

// Espera una seleccion mediante teclado.
static int seleccionar(const char *titulo, const char *opciones[],int cantidad)
{
    int seleccion = 0;
    int tecla;

    while (1) {
        mostrarOpciones(titulo, opciones, cantidad, seleccion);

        tecla = getch();

        if (tecla == KEY_UP && seleccion > 0) seleccion--;

        else if (tecla == KEY_DOWN && seleccion < cantidad - 1) seleccion++;

        else if (tecla == '\n' || tecla == KEY_ENTER) return seleccion;

        else if (tecla == 27) return -1;
    }
}
static int ingresarSoles(int solesActuales)
{
    int soles = solesActuales;
    int tecla;

    while (1) {
        erase();

        mvprintw(LINES / 2 - 2, COLS / 2 - 15,
                 "INGRESAR SOLES INICIALES");

        mvprintw(LINES / 2, COLS / 2 - 15,
                 "Cantidad: %d", soles);

        mvprintw(LINES - 2, 2,
                 "Numeros: ingresar | Enter: confirmar | ESC: cancelar");

        refresh();

        tecla = getch();

        if (tecla >= '0' && tecla <= '9') {
            if (soles <= 99999999) {
                soles = soles * 10 + (tecla - '0');
            }
        }

        else if (tecla == KEY_BACKSPACE || tecla == 127 || tecla == 8) {
            soles /= 10;
        }

        else if (tecla == '\n' || tecla == KEY_ENTER) {
            return soles;
        }

        else if (tecla == 27) {
            return solesActuales;
        }
    }
}

struct Configuraciones menu(void)
{
    struct Configuraciones config = {0, MEDIANO, NORMAL,500};

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

    //const char *volver[] = {"Volver"}; No se ocupa, pero para mantener orden

    int opcion;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    while (1) {
        opcion = seleccionar("PLANTS VS ZOMBIES REIMAGINED",principal, 4);

        if (opcion == 0) {opcion = seleccionar("JUGAR", partida, 3);

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

        else if (opcion == 1) {
            int subopcion = seleccionar("CONFIGURACION",
                         (const char *[]) {
                                             "Tamano de matriz",
                                             "Dificultad",
                                            "Soles iniciales",
                                             "Volver"
                                         }, 4);

            if (subopcion == 0) {int tamano = seleccionar("TAMANO DE MATRIZ",tamanos, 3);

                if (tamano != -1) config.tamanoMapa = tamano + 1;
            }

            else if (subopcion == 1) {
                int dificultad = seleccionar("DIFICULTAD", dificultades, 3);

                if (dificultad != -1) config.dificultad = dificultad + 1;
            }
            else if (subopcion == 2) {config.solesIniciales = ingresarSoles(config.solesIniciales);}
        }

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
            getch();
        }
        
        else if (opcion == 3 || opcion == -1) {
            config.accion = SALIR;
            break;
        }
    }
    endwin();
    return config;
}