#include "Entrada.h"
#include "Logica.h"

    
static void moverZombies(struct Mapa *mapa, int *vidaCasa)
{
    for (int i = 0; i < mapa->filas; i++)
    {
        for (int j = 0; j < mapa->columnas; j++)
        {
            if (mapa->matriz[i][j] == 'Z') // falta hacer la diferencia de tipo de zombies y todos se mover igual (lo mas probable)
            {
                int resultado = Movimiento(mapa, i, j);
 
                if (resultado == 2)   
                {
                    *vidaCasa = *vidaCasa - 1;
                }
            }
        }
    }
}

static void spawnAleatorio(struct Mapa *mapa,char TipoZombie){

    int fila = rand() % mapa->filas;
    SpawnZombie(mapa, fila, mapa->columnas - 1, TipoZombie);
}

static void pedirPlanta(struct Mapa *mapa){
    int fila, columna; // Nose hacerlo :(

}

static void DisparoPlantas(struct Mapa *mapa){

    for (int i = 0; i < mapa->filas; i++)
    {
        for (int j = 0; j < mapa->columnas - 1; j++){
        
            if (mapa->matriz[i][j] == '2'){
            
                if (mapa->matriz[i][j + 1] == 'Z'){
                
                    //funcion dañar zombie
                }
                else if (mapa->matriz[i][j + 1] == '.'){
                
                    mapa->matriz[i][j + 1] = '*';
                }
            }
        }
    }
}

static void moverProyectiles(struct Mapa *mapa)
{
    for (int i = 0; i < mapa->filas; i++)
    {
        
        for (int j = mapa->columnas - 1; j >= 0; j--)
        {
            if (mapa->matriz[i][j] == '*')
            {
                mapa->matriz[i][j] = '.';   

                if (j + 1 < mapa->columnas)
                {
                    char columna_S = mapa->matriz[i][j + 1];

                    if (columna_S == 'Z')
                    {
                        //dañar zombie
                    }
                    else if (columna_S == '.')
                    {
                        mapa->matriz[i][j + 1] = '*';  // sigue avanzando
                    }
                    
                }
            }
        }
    }
} // problemas cuando hay una planta adelante !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void logica()
{
    struct Data configuracion = leerConfiguracion(); //Ver info de DATA en Entrada.h
    struct Mapa* jardin = cargarMapa(); //Jardin es una matriz Jardin[][]}


    int vidaCasa= configuracion.resistenciaCasa;
    int Cantidad_soles=0;
    double intervalo_proyectil=0; // discutir!!!!!!!!!!!!!
    double intervalo_movimiento = 1.0; // se tiene que  cambiar?
    double intervalo_spawn = 5.0;
    double intervalo_disparo = configuracion.frecuenciaDisparoPlantas;
    double intervalo_sol = configuracion.frecuenciaSoles;
    double ultimo_movimiento = 0.0;
    double ultimo_spawn = 0.0;
    double ultimo_disparo = 0.0;
    double ultimo_sol = 0.0;
    double ultimo_proyectil = 0.0;






    while (vidaCasa > 0){
        double tiempoactual = (double)clock()/ CLOCKS_PER_SEC;

        if (tiempoactual-ultimo_movimiento>=intervalo_movimiento){
        
            moverZombies(jardin, &vidaCasa);
            ultimo_movimiento= tiempoactual;
        }
        if (tiempoactual-ultimo_spawn>=ultimo_spawn){
        
            spawnAleatorio(jardin,'Z');
            ultimo_spawn=tiempoactual;
        }
        if (tiempoactual - ultimo_disparo>= intervalo_disparo){
            DisparoPlantas(jardin);
            ultimo_disparo = tiempoactual;
        if (tiempoactual - ultimo_proyectil >= intervalo_proyectil)
        {
            moverProyectiles(jardin);
            ultimo_proyectil = tiempoactual;
        }
        }
        if (tiempoactual - ultimo_sol >= intervalo_sol){
            Cantidad_soles+=25;
            ultimo_sol = tiempoactual;
            
        }
    }
}
  

// problemas con Como dañar los zombies y plantas con el tema de la vida y hacer la diferencia entre plantar cada cosa 
// crear sunflower production planta
        





    












    

