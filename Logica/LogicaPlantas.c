#include "Logica.h"

//Plantas que hay en el jardín: cada tipo tiene su arreglo (máximo 100) y su cantidad
static LanzaGuisantes lanzaguisantes[100];
static int cantidadLanzaguisantes = 0;
static Girasol girasoles[100];
static int cantidadGirasoles = 0;
static Nuez nueces[100];
static int cantidadNueces = 0;
static PetaCereza cerezas[100];
static int cantidadCerezas = 0;
static Ajo ajos[100];
static int cantidadAjos = 0;

// Coloca una planta en el jardin si la casilla es suelo libre '.' y no hay un zombi encima
int plantar(struct Mapa *mapa, int fila, int columna, char planta, int* soles)
{
    // Valida que las coordenadas estén dentro de los limites del mapa
    if (fila < 0 || fila >= mapa->filas || columna < 0 || columna >= mapa->columnas){return 0;}
    
    //Valida que la casilla sea un espacio libre '.' y que no haya un zombi 'Z' o 'F' ocupandola en matrizLogica
    if (mapa->matriz[fila][columna] != '.' ||mapa->matrizLogica[fila][columna] == 'Z' || mapa->matrizLogica[fila][columna] == 'F'){return 0;}

    //Evalua que tipo de planta se desea colocar ('2': Lanzaguisantes, '1': Girasol, '3': Nuez, '4': PetaCereza, '5': Ajo)
    
    //Si se cumple la condicion de que hay espacio en el arreglo y hay suficientes soles se crea la planta y se descuenta el costo de los soles
    if (planta == '2' && cantidadLanzaguisantes < 100 && *soles >= 100)
    {
        LanzaGuisantes nueva = {fila, columna, 100 , 0.0f}; 
        lanzaguisantes[cantidadLanzaguisantes] = nueva;
        cantidadLanzaguisantes++;
        *soles = *soles - 100; // Descuenta el costo de los soles
    }
    //Si se cumple la condicion de que hay espacio en el arreglo y hay suficientes soles se crea la planta y se descuenta el costo de los soles
    else if (planta == '1' && cantidadGirasoles < 100 && *soles >= 50)
    {
        Girasol nueva = {fila, columna, 150, 0.0f}; 
        girasoles[cantidadGirasoles] = nueva;
        cantidadGirasoles++;
        *soles = *soles - 50; // Descuenta el costo de los soles
    }
    //Si se cumple la condicion de que hay espacio en el arreglo y hay suficientes soles se crea la planta y se descuenta el costo de los soles
    else if (planta == '3' && cantidadNueces < 100 && *soles >= 50)
    {
        Nuez nueva = {fila, columna, 1000}; 
        nueces[cantidadNueces] = nueva;
        cantidadNueces++;
        *soles = *soles - 50; // Descuenta el costo de los soles
    }
    //Si se cumple la condicion de que hay espacio en el arreglo y hay suficientes soles se crea la planta y se descuenta el costo de los soles
    else if (planta == '4' && cantidadCerezas < 100 && *soles >= 150)
    {
        PetaCereza nueva = {fila, columna, 2.0f}; 
        cerezas[cantidadCerezas] = nueva;
        cantidadCerezas++;
        *soles = *soles - 150; // Descuenta el costo de los soles
    }
    //Si se cumple la condicion de que hay espacio en el arreglo y hay suficientes soles se crea la planta y se descuenta el costo de los soles
    else if (planta == '5' && cantidadAjos < 100 && *soles >= 50)
    {
        Ajo nueva = {fila, columna, 200}; 
        ajos[cantidadAjos] = nueva;
        cantidadAjos++;
        *soles = *soles - 50; // Descuenta el costo de los soles
    }
    else { 
        return 0; // Si no cumple ninguna condicion o faltan soles y/o espacio en el arreglo falla 
    } 

    //Actualiza la matriz del mapa con el carácter de la planta colocada y retorna éxito que corresponde a 1
    mapa->matriz[fila][columna] = planta;
    return 1;
}

// Recorre los arreglos de todas las plantas, elimina del mapa las que tienen 
// vida <= 0 y compacta los arreglos moviendo el ultimo elemento al hueco libre
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
    for (int i = 0; i < cantidadAjos; )
    {
        if (ajos[i].vida <= 0)
        {
            mapa->matriz[ajos[i].fila][ajos[i].columna] = '.';
            ajos[i] = ajos[cantidadAjos - 1];
            cantidadAjos--;
        }
        else{i++;}
    }
}

//Funcion de la petacereza que recorre el arreglo de cerezas y si alguna tiene tiempoExplosion <= 0 explota y daña a los zombies en un area de 3x3
void explosivosDañar(struct Mapa *mapa, Zombie* lista, int cantidad)
{
    for (int i = 0; i < cantidadCerezas;)
    {
        if (cerezas[i].tiempoExplosion <= 0)
        {
            // Guardamos las coordenadas antes de eliminar la cereza
            int fila = cerezas[i].fila;
            int columna = cerezas[i].columna;

            // Liberamos su casilla
            mapa->matriz[fila][columna] = '.';

            // Eliminamos la cereza del arreglo
            cerezas[i] = cerezas[cantidadCerezas - 1];
            cantidadCerezas--;

            // Dañamos a los zombies en el área 3x3
            for (int f = fila - 1; f <= fila + 1; f++)
            {
                for (int c = columna - 1; c <= columna + 1; c++)
                {
                    // Comprobamos los límites del mapa
                    if (f < 0 || f >= mapa->filas ||
                        c < 0 || c >= mapa->columnas)
                    {
                        continue;
                    }

                    if (mapa->matrizLogica[f][c] != 'Z' && mapa->matrizLogica[f][c] != 'F')
                    {
                        continue;
                    }

                    dañarZombie(lista, cantidad, f, c, DAÑO_PETACEREZA);
                }
            }
        }
        else
        {
            cerezas[i].tiempoExplosion -= DT;
            i++;
        }
    }
}



//Hay algún zombie en la misma fila, a la derecha de la columna dada si hay un zombie devuelve 1, si no hay devuelve 0
static int hayZombieAdelante(struct Mapa *mapa, int fila, int columna)
{
    for (int j = columna + 1; j < mapa->columnas; j++)
    {
        if (mapa->matrizLogica[fila][j] == 'Z' || mapa->matrizLogica[fila][j] == 'F'){return 1;}
    }
    return 0;
}

//Cada lanzaguisantes dispara (solo si hay un zombie en su fila): crea un proyectil '-'
//en la casilla de adelante, o daña directamente al zombie si ya está pegado a la planta.
//Los proyectiles viven en matrizLogica por eso pasan por encima de otras plantas
void disparoPlantas(struct Mapa *mapa, Zombie* lista, int cantidad)
{
    for (int i = 0; i < mapa->filas; i++)
    {
        for (int j = 0; j < mapa->columnas - 1; j++)
        {
            if (mapa->matriz[i][j] != PLANTA_LANZAGUISANTES){continue;}
            if (!hayZombieAdelante(mapa, i, j)){continue;}

            char siguiente = mapa->matrizLogica[i][j + 1];
            if (siguiente == 'Z' || siguiente == 'F'){dañarZombie(lista, cantidad, i, j + 1, DAÑO_GUISANTE);}
            else if (siguiente == '.'){mapa->matrizLogica[i][j + 1] = '-';}
        }
    }
}

//Genera los soles de los girasoles
// Cada girasol genera 25 soles cada 10 segundos se acumula el tiempo de cada girasol y cuando llega a 10 segundos se reinicia y se suman 25 soles al total
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

// Daña a las plantas en la casilla (fila, columna) si hay alguna y devuelve 1 si dañó alguna planta, 0 si no había ninguna planta
int dañarPlantas(int fila, int columna, int daño)
{
    int plantasTotales = cantidadGirasoles + cantidadLanzaguisantes + cantidadNueces + cantidadAjos;
    if (plantasTotales >= 100){plantasTotales = 100;}

    for (int i = 0; i < plantasTotales; i++)
    {
        if (girasoles[i].fila == fila && girasoles[i].columna == columna && girasoles[i].vida > 0)
        {
            girasoles[i].vida -= daño;
            return 1;
        }
        if (lanzaguisantes[i].fila == fila && lanzaguisantes[i].columna == columna && lanzaguisantes[i].vida > 0)
        {
            lanzaguisantes[i].vida -= daño;
            return 1;
        }
        if (nueces[i].fila == fila && nueces[i].columna == columna && nueces[i].vida > 0)
        {
            nueces[i].vida -= daño;
            return 1;
        }
        if (ajos[i].fila == fila && ajos[i].columna == columna && ajos[i].vida > 0)
        {
            ajos[i].vida -= daño;
            return 1;
        }
    }
    return 0;
}


// Quita un obstáculo 'O'  o planta de la casilla fila y columna si hay alguno, para el obstaculo solo si hay
// suficientes soles 
int pala(struct Mapa *mapa, int fila, int columna, int* soles)
{
    if (fila < 0 || fila >= mapa->filas || columna < 0 || columna >= mapa->columnas){return 1;}

    if (mapa->matriz[fila][columna] >= '1'  && mapa->matriz[fila][columna] <= '5'){
        dañarPlantas(fila, columna,99999999);
    }
    else if(mapa->matriz[fila][columna] == 'O' && *soles >= 200){
        mapa->matriz[fila][columna] = '.';
        *soles -= 200;
    }
    return 0;
}

void reiniciarGirasoles(void)
{
    cantidadGirasoles = 0; // Reinicia la cantidad de girasoles a 0 para la siguiente partida
}

