#include "Logica.h"

//Plantas que hay en el jardín: cada tipo tiene su arreglo (máximo 100) y su cantidad
static LanzaGuisantes lanzaguisantes[100];
static int cantidadLanzaguisantes = 0;
static Girasol girasoles[100];
static int cantidadGirasoles = 0;
static Nuez nueces[100];
static int cantidadNueces = 0;

//Coloca una planta en el jardín si la casilla es suelo libre ('.') y no hay un zombie encima
int plantar(struct Mapa *mapa, int fila, int columna, char planta, int* soles)
{
    if (fila < 0 || fila >= mapa->filas || columna < 0 || columna >= mapa->columnas){return 0;}
    if (mapa->matriz[fila][columna] != '.' || mapa->matrizLogica[fila][columna] == 'Z'){return 0;}

    if (planta == '2' && cantidadLanzaguisantes < 100 && *soles >= 100)
    {
        LanzaGuisantes nueva = {fila, columna, 50, 0.0f}; 
        lanzaguisantes[cantidadLanzaguisantes] = nueva;
        cantidadLanzaguisantes++;
        *soles = *soles - 100;
    }
    else if (planta == '1' && cantidadGirasoles < 100 && *soles >= 50)
    {
        Girasol nueva = {fila, columna, 50, 0.0f}; 
        girasoles[cantidadGirasoles] = nueva;
        cantidadGirasoles++;
        *soles = *soles - 50;
    }
    else if (planta == '3' && cantidadNueces < 100 && *soles >= 150)
    {
        Nuez nueva = {fila, columna, 400}; 
        nueces[cantidadNueces] = nueva;
        cantidadNueces++;
        *soles = *soles - 150;
    }
    else{return 0;} 

    mapa->matriz[fila][columna] = planta;
    return 1;
}

void eliminarPlantasMuertas(struct Mapa *mapa)
{
    for (int i = 0; i < cantidadLanzaguisantes;)
    {
        if (lanzaguisantes[i].vida <= 0)
        {
            mapa->matriz[lanzaguisantes[i].fila][lanzaguisantes[i].columna] = '.';
            lanzaguisantes[i] = lanzaguisantes[cantidadLanzaguisantes - 1];
            cantidadLanzaguisantes--;
        }
        else{i++;}
    }

    for (int i = 0; i < cantidadGirasoles;)
    {
        if (girasoles[i].vida <= 0)
        {
            mapa->matriz[girasoles[i].fila][girasoles[i].columna] = '.';
            girasoles[i] = girasoles[cantidadGirasoles - 1];
            cantidadGirasoles--;
        }
        else{i++;}
    }

    for (int i = 0; i < cantidadNueces; )
    {
        if (nueces[i].vida <= 0)
        {
            mapa->matriz[nueces[i].fila][nueces[i].columna] = '.';
            nueces[i] = nueces[cantidadNueces - 1];
            cantidadNueces--;
        }
        else{i++;}
    }
}


//Hay algún zombie en la misma fila, a la derecha de la columna dada
static int hayZombieAdelante(struct Mapa *mapa, int fila, int columna)
{
    for (int j = columna + 1; j < mapa->columnas; j++)
    {
        if (mapa->matrizLogica[fila][j] == 'Z'){return 1;}
    }
    return 0;
}

//Cada lanzaguisantes dispara (solo si hay un zombie en su fila): crea un proyectil '-'
//en la casilla de adelante, o daña directamente al zombie si ya está pegado a la planta.
//Los proyectiles viven en matrizLogica, por eso pasan "por encima" de otras plantas
void disparoPlantas(struct Mapa *mapa, Zombie* lista, int cantidad)
{
    for (int i = 0; i < mapa->filas; i++)
    {
        for (int j = 0; j < mapa->columnas - 1; j++)
        {
            if (mapa->matriz[i][j] != PLANTA_LANZAGUISANTES){continue;}
            if (!hayZombieAdelante(mapa, i, j)){continue;}

            char siguiente = mapa->matrizLogica[i][j + 1];
            if (siguiente == 'Z'){dañarZombie(lista, cantidad, i, j + 1, DAÑO_GUISANTE);}
            else if (siguiente == '.'){mapa->matrizLogica[i][j + 1] = '-';}
        }
    }
}

//Genera los soles de los girasoles
int generarSoles(void)
{
    int soles = 0;

    for (int i = 0; i < cantidadGirasoles; i++)
    {
        girasoles[i].movimientoSol += DT;
        if (girasoles[i].movimientoSol >= 10.0f)
        {
            girasoles[i].movimientoSol = 0.0f;
            soles += 25;
        }
    }
    return soles;
}

int dañarPlantas(int fila, int columna)
{
    int plantasTotales = cantidadGirasoles + cantidadLanzaguisantes + cantidadNueces;
    if (plantasTotales >= 100){plantasTotales = 100;}

    for (int i = 0; i < plantasTotales; i++)
    {
        if (girasoles[i].fila == fila && girasoles[i].columna == columna && girasoles[i].vida > 0)
        {
            girasoles[i].vida -= DAÑO_ZOMBIES_PLANTAS;
            return 1;
        }
        if (lanzaguisantes[i].fila == fila && lanzaguisantes[i].columna == columna && lanzaguisantes[i].vida > 0)
        {
            lanzaguisantes[i].vida -= DAÑO_ZOMBIES_PLANTAS;
            return 1;
        }
        if (nueces[i].fila == fila && nueces[i].columna == columna && nueces[i].vida > 0)
        {
            nueces[i].vida -= DAÑO_ZOMBIES_PLANTAS;
            return 1;
        }
    }
    return 0;
}