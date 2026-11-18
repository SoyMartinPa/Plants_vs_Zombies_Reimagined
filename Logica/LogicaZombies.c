#include "Logica.h"

// Spawnea un zombi aleatorio (normal, con cono, con casco o abanderado) en la ultima columna del jardín
// Si la ultima columna esta ya ocupada por otros zombis (Z o F) en todas las filas no se crea el zombie
// Mientras aumentan las oleada aumenta la probabilidad de que el zombi sea abanderado y también de que tenga casco o cono
Zombie* spawnZombieAleatoreo(struct Mapa *mapa, struct Data data, int oleadaActual)
{
    // Arreglo temporal para almacenar los índices de las filas que están libres en la última columna
    int libres[mapa->filas];
    int cantidadLibres = 0;

    // Recorre todas las filas de la última columna del jardín para verificar espacios disponibles
    for (int i = 0; i < mapa->filas; i++)
    {
        char actual = mapa->matrizLogica[i][mapa->columnas - 1];
        
        // Si la casilla no tiene un zombi (Z o F) la fila se considera libre
        if (actual != 'Z' && actual != 'F')
        {
            libres[cantidadLibres++] = i;
        }
    }

    // Si no hay ninguna fila libre en la ultima columna no se puede generar el zombi
    if (cantidadLibres == 0) { return NULL; }

    // Reserva memoria dinamica para el nuevo zombi
    Zombie *zombie = malloc(sizeof(Zombie));
    if (zombie == NULL)
    {
        printf("Error al alocar memoria");
        exit(1); // Termina la ejecución si falla la asignación
    }

    // Configura la posición inicial, vida base y temporizador de movimiento del zombi
    zombie->fila = libres[rand() % cantidadLibres]; // Elige una fila libre al azar
    zombie->columna = mapa->columnas - 1;          // Aparece siempre en la ultima columna
    zombie->vida = (int)(100 * data.resistenciaZombies);
    zombie->movimiento = 0.0f; 

    // Calcula la probabilidad de que aparezca un zombi abanderado la cual aumenta con cada oleada
    int probabilidadBandera = 8 + (oleadaActual - 1);
    
    // Limita la probabilidad máxima del abanderado al 20%
    if (probabilidadBandera > 20) {
        probabilidadBandera = 20; 
    }

    // Genera un número aleatorio entre 0 y 99 para determinar si el zombi será abanderado
    int tirada = rand() % 100;
    // Determina si el zombi es abanderado comparando la tirada con la probabilidad calculada 
    int esBandera = (tirada < probabilidadBandera);

    // Si resulta ser abanderado, se configuran sus atributos especiales
    if (esBandera) {
        zombie->tipo = ZOMBIE_FLAG;
        zombie->armadura = 0;       
    }
    else {
        // Si no es abanderado se asigna un tipo normal y se determina si tendrá casco o cono según probabilidades escaladas por oleada
        zombie->tipo = ZOMBIE_NORMAL;
    
        int probabilidadArmadura = rand() % 100;
    
        // Calcula las probabilidades de equipar casco o cono escalando exponencialmente por oleada
        float probabilidadCasco = 10 * pow(1.10, oleadaActual - 1);
        float probabilidadCono = 30 * pow(1.15, oleadaActual - 1);

        // Establece los topes maximos de probabilidad para casco 80% y cono 90%
        if (probabilidadCasco > 80)
            probabilidadCasco = 80;

        if (probabilidadCono > 90)
            probabilidadCono = 90;

        // Asigna la armadura correspondiente y duplica o incrementa la vida según el caso
        if (probabilidadArmadura < probabilidadCasco) {
            zombie->armadura = 2; // Casco (mayor resistencia)
            zombie->vida *= 2;
        }
        else if (probabilidadArmadura < probabilidadCono) {
            zombie->armadura = 1; // Cono (resistencia media)
            zombie->vida = (int)(zombie->vida * 1.5f);
        }
        else {
            zombie->armadura = 0; // Sin armadura (normal)
        }
    }

    // Retorna el puntero con el zombi creado y configurado
    return zombie;
}

//Verifica si la casilla fila columna esta disponible para que el zombie se mueva a ella se usa en parte del ajo
static int casillaDisponible(struct Mapa *mapa, int fila, int columna)
{
    return mapa->matriz[fila][columna] == '.' &&
           mapa->matrizLogica[fila][columna] == '.';
}

//Intenta avanzar el zombie una casilla hacia la izquierda. Retorna:
//  MOV_AVANZO    se movió
//  MOV_BLOQUEADO hay otro zombie adelante (Z o F)
//  MOV_CASA      la casilla de adelante es la casa 'H' (hay que dañar la casa)
//  MOV_PLANTA    la casilla de adelante tiene una planta (hay que dañar la planta)
int moverZombie(Zombie *zombie, struct Mapa *mapa)
{
    // se obtiene la fila y columna actuales del zombie, y se calcula la columna siguiente a la izquierda y retorna el 
    // movimiento correspondiente según las condiciones del mapa y la lógica del juego.   
    int fila = zombie->fila;
    int columna = zombie->columna;
    int siguiente = columna - 1;

    if (siguiente < 0){return MOV_BLOQUEADO;}

    char suelo = mapa->matriz[fila][siguiente];
    char logico = mapa->matrizLogica[fila][siguiente];

    if (suelo == 'H'){return MOV_CASA;}
    if (suelo>= '1' && suelo <= '4'){return MOV_PLANTA;}
    
    
    if (logico == 'Z' || logico == 'F'){return MOV_BLOQUEADO;}

    //Si caminó justo hacia un guisante, este lo golpea
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
    
    zombie->columna = siguiente;
    return MOV_AVANZO;
}

//Resta 'daño' al zombie que esté en (fila, columna), si hay alguno
//Si el zombie muere no se elimina de la lista solo se le deja la vida en 0 se usa 
//la funcion eliminarZombiesMuertos() para eliminar los zombies muertos de la lista
void dañarZombie(Zombie* lista, int cantidad, int fila, int columna, int daño)
{   //recorre la lista de zombies y busca el zombie que esté en la posición (fila, columna) y 
    // resta el daño a su vida si tiene vida mayor a 0
    
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
    // verifica si la lista ha alcanzado su capacidad máxima actual
    if (*cantidad == *capacidad)
    {
        // Si la capacidad es 0 inicia con un tamaño base de 4; de lo contrario duplica la capacidad actual
        int nuevaCapacidad = (*capacidad == 0) ? 4 : *capacidad * 2;

        // Redimensiona el bloque de memoria del arreglo dinamico de zombis
        Zombie *aux = realloc(*lista, nuevaCapacidad * sizeof(Zombie));
        
        // Comprueba si la reasignación de memoria falló
        if (aux == NULL)
        {
            printf("Error al alocar memoria");
            exit(1); // Termina la ejecución si no hay memoria disponible
        }
        
        // Actualiza el puntero de la lista y la nueva capacidad maxima
        *lista = aux;
        *capacidad = nuevaCapacidad;
    }

    // Inserta el nuevo zombi en la posición disponible y aumenta el contador de cantidad
    (*lista)[*cantidad] = nuevo;
    (*cantidad)++;
}

//Elimina todos los zombies muertos de la lista de zombies
// busca en la lista de zombies aquellos que tengan vida <= 0 y los elimina 
// desplazando los elementos restantes hacia la izquierda para llenar el espacio vacio 
// Al final actualiza la cantidad de zombies vivos en la lista.
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
