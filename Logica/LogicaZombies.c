#include "Logica.h"

//Spawnea un zombie aleatorio (normal, con cono o con casco) en la última columna del jardin
//Si la última columna está ya ocupada por otros zombies en todas las filas, no se crea el zombie.
//Falta distingirlos con un 'char' para cada helment
Zombie* spawnZombieAleatoreo(struct Mapa *mapa, struct Data data,int oleadaActual)
{
    //Se juntan las filas cuya última casilla está libre y se elige una al azar
    //(reemplaza el while(1) con 'filasVisitadas', que hacía lo mismo de forma más larga)
    int libres[mapa->filas];
    int cantidadLibres = 0;
    for (int i = 0; i < mapa->filas; i++)
    {
        if (mapa->matrizLogica[i][mapa->columnas - 1] != 'Z'){libres[cantidadLibres++] = i;}
    }
    if (cantidadLibres == 0){return NULL;}

    Zombie *zombie = malloc(sizeof(Zombie));
    if (zombie == NULL)
    {
        printf("Error al alocar memoria");
        exit(1);
    }

    zombie->fila = libres[rand() % cantidadLibres];
    zombie->columna = mapa->columnas - 1;
    zombie->vida = (int)(100 * data.resistenciaZombies);
    zombie->movimiento = 0.0f; //antes: rand()%5 + 100 (sumaba 100 en vez de ser un valor entre 0 y 5)

    int probabilidadArmadura = rand() % 100;
    
    float probabilidadCasco = 10 * pow(1.10, oleadaActual - 1);
    float probabilidadCono = 30 * pow(1.15, oleadaActual - 1);

    if (probabilidadCasco > 80)
        probabilidadCasco = 80;

    if (probabilidadCono > 90)
        probabilidadCono = 90;

    if (probabilidadArmadura < probabilidadCasco) {
        zombie->armadura = 2;
        zombie->vida *= 2;
    }
    else if (probabilidadArmadura < probabilidadCono) {
        zombie->armadura = 1;
        zombie->vida = (int)(zombie->vida * 1.5f);
    }
    else {
        zombie->armadura = 0;
    }

    return zombie;
}


static int casillaDisponible(struct Mapa *mapa, int fila, int columna)
{
    return mapa->matriz[fila][columna] == '.' &&
           mapa->matrizLogica[fila][columna] == '.';
}

//Intenta avanzar el zombie una casilla hacia la izquierda. Retorna:
//  MOV_AVANZO    se movió
//  MOV_BLOQUEADO hay otro zombie adelante
//  MOV_CASA      la casilla de adelante es la casa 'H' (hay que dañar la casa)
//  MOV_PLANTA    la casilla de adelante tiene una planta (hay que dañar la planta)
int moverZombie(Zombie *zombie, struct Mapa *mapa)
{
    int fila = zombie->fila;
    int columna = zombie->columna;
    int siguiente = columna - 1;

    if (siguiente < 0){return MOV_BLOQUEADO;}

    char suelo = mapa->matriz[fila][siguiente];
    char logico = mapa->matrizLogica[fila][siguiente];

    if (suelo == 'H'){return MOV_CASA;}
    if (suelo>= '1' && suelo <= '4'){return MOV_PLANTA;}
    if (logico == 'Z'){return MOV_BLOQUEADO;}

    //Si caminó justo hacia un guisante, este lo golpea (si no, el guisante se perdería)
    if (logico == '-'){zombie->vida -= DAÑO_GUISANTE;}

    //Lógica del ajo
 if (suelo == '5')
    {
        int direccion;

        if (fila == 0)
        {
            direccion = 1;
        }
        else if (fila == mapa->filas - 1)
        {
            direccion = -1;
        }
        else
        {
            int azar = rand() % 2;
            direccion = (azar == 0) ? -1 : 1;
        }

        int nuevaFila = fila + direccion;

        // Si la primera dirección está ocupada, probamos la contraria
        if (!casillaDisponible(mapa, nuevaFila, columna))
        {
            if (fila == 0 || fila == mapa->filas - 1)
            {
                return MOV_PLANTA;
            }

            direccion *= -1;
            nuevaFila = fila + direccion;

            if (!casillaDisponible(mapa, nuevaFila, columna))
            {
                return MOV_PLANTA;
            }
        }

        zombie->fila = nuevaFila;
        return MOV_AJO;
    }
    //Se actualiza la matrizLogica al tiro para que el zombie de atrás vea la posición correcta
    zombie->columna = siguiente;
    return MOV_AVANZO;
}

//Resta 'daño al zombie que esté en (fila, columna), si hay alguno
void dañarZombie(Zombie* lista, int cantidad, int fila, int columna, int daño)
{
    for (int i = 0; i < cantidad; i++)
    {
        if (lista[i].fila == fila && lista[i].columna == columna && lista[i].vida > 0)
        {
            lista[i].vida -= daño;
            return;
        }
    }
}

//Agrega un zombie a la lista de zombies
void agregarZombie(Zombie** lista, int* cantidad, int* capacidad, Zombie nuevo)
{
    if (*cantidad == *capacidad)
    {
        int nuevaCapacidad = (*capacidad == 0) ? 4 : *capacidad * 2;

        Zombie *aux = realloc(*lista, nuevaCapacidad * sizeof(Zombie));
        if (aux == NULL)
        {
            printf("Error al alocar memoria");
            exit(1);
        }
        *lista = aux;
        *capacidad = nuevaCapacidad;
    }

    (*lista)[*cantidad] = nuevo;
    (*cantidad)++;
}

//Elimina todos los zombies muertos de la lista de zombies
void eliminarZombiesMuertos(Zombie* lista, int* cantidad)
{
    for (int i = 0; i < *cantidad;)
    {
        if (lista[i].vida <= 0)
        {
            for (int j = i; j < *cantidad - 1; j++){lista[j] = lista[j + 1];}
            (*cantidad)--;
        }
        else{i++;}
    }
}
