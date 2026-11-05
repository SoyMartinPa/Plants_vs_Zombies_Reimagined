#include "Entrada.h"

// el struct 'Data' está definido en entrada.h

//Esta función crea un archivo Config.ini en la carpeta de archivos con los datos proporcionados por DATA
static void crearConfiguracion(struct Data *data) //Con un puntero evitamos tener que hacer una copia local (ineficiente)
{
    FILE *configuracion = fopen("Archivos/Config.ini","w");
    if (configuracion == NULL){return;} //Si no existe la carpeta Archivos/ simplemente no se guarda

    fprintf(configuracion,"NumeroOleadas=%d\n",data->numeroOleadas);
    fprintf(configuracion,"ResistenciaZombies=%f\n",data->resistenciaZombies);
    fprintf(configuracion,"ResistenciaCasa=%f\n",data->resistenciaCasa);
    fprintf(configuracion,"ResistenciaPlantas=%f\n",data->resistenciaPlantas); //antes guardaba resistenciaCasa
    fprintf(configuracion,"FrecuenciaDisparoPlantas=%f\n",data->frecuenciaDisparoPlantas);
    fprintf(configuracion,"FrecuenciaSoles=%f\n",data->frecuenciaSoles);
    fprintf(configuracion,"FrecuenciaSpawnZombies=%f\n",data->frecuenciaSpawnZombies);
    fprintf(configuracion,"VelocidadMovimientoZombies=%f\n",data->velocidadMovimientoZombies);

    fclose(configuracion);
}

//Esta función abre el archivo Config.ini para crear un struct Data con la información del archivo
//Por cada atributo que no se encuentre en el archivo pero sí sea requerido en data se ocupara una
//configuración por defecto (definida en la primera linea)
struct Data leerConfiguracion(void)
{
    struct Data data = {5,1.0f,1.0f,1.0f,1.0f,10.0f,5.0f,1.0f}; //configuración por defecto

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
        //Se exige que se lean los 2 campos (antes con "<= 0" una línea como "Atributo=abc"
        //pasaba y se reutilizaba el 'valor' de la línea anterior)
        if ( sscanf(linea, "%127[^=]=%f", atributo , &valor) != 2){continue;}
        //Luego se compara cada atributo con uno definido en el código y se le incorpora el valor en Data.<atributo>
        //Las resistencias y frecuencias deben ser > 0 (una resistencia 0 dejaría zombies sin vida)
        if (strcmp(atributo,"NumeroOleadas") == 0){data.numeroOleadas = ((int)valor > 0) ? (int)valor : 1;}
        else if (strcmp(atributo,"ResistenciaZombies") == 0){data.resistenciaZombies = (valor>0) ? valor : 1;}
        else if (strcmp(atributo,"ResistenciaCasa") == 0){data.resistenciaCasa = (valor>0) ? valor : 1;}
        else if (strcmp(atributo,"ResistenciaPlantas") == 0){data.resistenciaPlantas = (valor>0) ? valor : 1;}
        else if (strcmp(atributo,"FrecuenciaDisparoPlantas") == 0){data.frecuenciaDisparoPlantas = (valor>0) ? valor : 1;}
        else if (strcmp(atributo,"FrecuenciaSoles") == 0){data.frecuenciaSoles = (valor>0) ? valor: 1;} 
        else if (strcmp(atributo,"FrecuenciaSpawnZombies") == 0){data.frecuenciaSpawnZombies = (valor>0) ? valor : 1;}
        else if (strcmp(atributo,"VelocidadMovimientoZombies") == 0){data.velocidadMovimientoZombies = (valor>0) ? valor: 1;} 
    }

    fclose(file);
    crearConfiguracion(&data); //Si el archivo previo Config.ini es valido, se creará un archivo de igual contenido
    return data;
}
