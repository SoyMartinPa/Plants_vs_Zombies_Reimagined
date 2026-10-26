#include "Entrada.h"

// el struct 'Data' está definido en entrada.h

//Esta función crea un archivo Config.ini en la carpeta de archivos con los datos proporcionados por DATA
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

//Esta función abre el archivo Config.ini para crear un struct Data con la información del archivo
//Por cada atributo que no se encuentre en el archivo pero sí sea requerido en data se ocupara una
//configuración por defecto (definida en la primera linea)
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

    //Este while extrae una linea del archivo hasta que no hayan más lineas que extraer.
    //Consiguiendo, con esto, el nombre del atributo y el valor de cada linea.
    while( ( fgets(linea, sizeof(linea), file) ) != NULL)
    {
        if (linea[0] == '\n' || linea[0] == ' '){continue;} //Manejo de errores

        //se parsea la linea del archivo con el sscanf, de manera quee lee la linea hasta
        //llegar al '=' (descrito en '[^=]') y luego se extrae el valor del otro lado del igual
        if ( (sscanf(linea, "%127[^=]=%f", atributo , &valor) ) <= 0){continue;}
        //Luego se compara cada atributo con uno definido en el código y se le incorpora el valor en Data.<atributo>
        if (strcmp(atributo,"NumeroOleadas") == 0){data.numeroOleadas = ((int)valor > 0) ? (int)valor : 1;}
        else if ((strcmp(atributo,"ResistenciaZombies") == 0)){data.resistenciaZombies = ((int)valor>=0) ? (int)valor : 0.1;}
        else if ((strcmp(atributo,"ResistenciaCasa") == 0)){data.resistenciaCasa = ((int)valor>=0) ? (int)valor : 0.1;}
        else if ((strcmp(atributo,"ResistenciaPlantas") == 0)){data.resistenciaPlantas = ((int)valor>=0) ? (int)valor : 0.1 ;}
        else if ((strcmp(atributo,"FrecuenciaDisparoPlantas") == 0)){data.frecuenciaDisparoPlantas = (valor>0) ? valor : 1;}
        else if ((strcmp(atributo,"FrecuenciaSoles") == 0)){data.frecuenciaSoles = (valor>0) ? valor: 1;} 
    }

    fclose(file);
    crearConfiguracion(&data); //Si el archivo previo Config.ini es valido, se creará un archivo de igual contenido
    return data;
}

