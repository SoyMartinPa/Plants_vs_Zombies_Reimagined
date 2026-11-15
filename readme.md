# PvZ-Reimagined

Proyecto desarrollado para el curso **Programación de Computadores**.

## Avance y mecánicas actuales

Usando los números y la barra espaciadora se pueden utilizar las siguientes plantas y herramientas:

- `'0'`: Pala (Cuesta 200 soles quitar un obstaculo).
- `'1'`: Girasol (Vale 50 soles).
- `'2'`: Lanzaguisantes(Vale 100 soles).
- `'3'`: Nuez (Vale 50 soles).
- `'4'`: PetaCereza(Vale 150 soles).
- `'5'`: Ajo(Vale 50 soles).

El mapa también incorpora obstáculos (`'O'`), los cuales impiden colocar plantas en las casillas que ocupan. Se eliminan utilizando la pala.

## Funcionalidades pendientes (TODO)

A continuación, se presentan las principales funcionalidades que quedan por implementar para completar el proyecto. Esta lista puede ampliarse a medida que avance el desarrollo.

1. **Interfaz gráfica:** incorporar una interfaz que incluya un menú de inicio con opciones de configuración, selección de modos de juego y otras funcionalidades. Además, se deberán mejorar las representaciones visuales de las plantas, zombies y demás elementos del juego.
4. **Sistema de oleadas:** implementar diferentes oleadas de enemigos e incorporar zombies especiales asociados a ellas.
5. **Balance del juego:** ajustar las mecánicas, características y estadísticas de las plantas y zombies para lograr una experiencia de juego equilibrada.

## Compilación y ejecución

Para compilar y ejecutar el juego, se deben seguir los siguientes pasos:

1. Abrir una terminal en el directorio raíz del proyecto.
2. Ejecutar el comando `make` para compilar el programa.
3. Una vez finalizada la compilación, ejecutar el juego mediante:

   ```bash
   ./PVZR
   ```