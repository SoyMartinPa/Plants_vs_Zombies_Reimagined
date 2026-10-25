#include "Entrada.h"

// 'Data' está definido en entrada.h
static void crearConfiguracion(struct Data *data) //Con un puntero evitamos tener que hacer una copia local (ineficiente)
{
    FILE *configuracion = fopen("Archivos/Config.ini","w");

    fprintf(configuracion,"NumeroOleadas=%d\n",data->numeroOleadas);
    fprintf(configuracion,"ResistenciaZombies=%d\n",data->resistenciaZombies);
    fprintf(configuracion,"ResistenciaCasa=%d\n",data->resistenciaCasa);
    fprintf(configuracion,"ResistenciaPlantas=%d\n",data->resistenciaCasa);
    fprintf(configuracion,"FrecuenciaDisparoPlantas=%f\n",data->frecuenciaDisparoPlantas);
    fprintf(configuracion,"FrecuenciaSoles=%f\n",data->frecuenciaSoles);

    fclose(configuracion);
}

struct Data leerConfiguracion()
{
    struct Data data = {5,1,1,1,1.0f,10.0f}; //configuración por defecto

    char linea[128];
    char atributo[128];
    float valor;
    //Abrimos el archivo Config.ini en modo lectura
    FILE *file = fopen("Archivos/Config.ini", "r"); 
    if (file == NULL)
    {
        crearConfiguracion(&data);
        return data;
    }


    while( ( fgets(linea, sizeof(linea), file) ) != NULL)
    {
        if (linea[0] == '\n' || linea[0] == ' '){continue;} 

        if ( (sscanf(linea, "%127[^=]=%f", atributo , &valor) ) <= 0){continue;}
        if (strcmp(atributo,"NumeroOleadas") == 0){data.numeroOleadas = (int)valor;}
        else if ((strcmp(atributo,"ResistenciaZombies") == 0)){data.resistenciaZombies = (int)valor;}
        else if ((strcmp(atributo,"ResistenciaCasa") == 0)){data.resistenciaCasa = (int)valor;}
        else if ((strcmp(atributo,"ResistenciaPlantas") == 0)){data.resistenciaPlantas = (int)valor;}
        else if ((strcmp(atributo,"FrecuenciaDisparoPlantas") == 0)){data.frecuenciaDisparoPlantas = valor;}
        else if ((strcmp(atributo,"FrecuenciaSoles") == 0)){data.frecuenciaSoles = valor;}
    }

    fclose(file);
    crearConfiguracion(&data);
    return data;
}

