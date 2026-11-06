# PvZ-Reimagined

Proyecto desarrollado para el curso **Programación de Computadores**.

## Avance y mecánicas actuales

Actualmente, el proyecto cuenta con un prototipo funcional del juego que incorpora tres tipos de plantas:

- `'1'`: Girasol.
- `'2'`: Lanzaguisantes.
- `'3'`: Nuez.

El juego dispone de una única oleada infinita de zombies. Actualmente, existen tres tipos de enemigos: zombie normal, zombie con cono y zombie con cubeta. La única diferencia entre ellos es su cantidad de vida, ya que todavía no cuentan con representaciones gráficas diferenciadas.

El mapa también incorpora obstáculos (`'O'`), los cuales impiden colocar plantas en las casillas que ocupan. En futuras versiones, estos obstáculos podrán eliminarse utilizando una pala.

### Controles

La interacción con el mapa se realiza mediante los siguientes controles:

- **Flechas del teclado:** permiten mover la casilla seleccionada.
- **Teclas `1`, `2` y `3`:** permiten seleccionar el tipo de planta que se desea colocar.
- **Barra espaciadora:** permite colocar la planta seleccionada en la casilla actual.

## Funcionalidades pendientes (TODO)

A continuación, se presentan las principales funcionalidades que quedan por implementar para completar el proyecto. Esta lista puede ampliarse a medida que avance el desarrollo.

1. **Interfaz gráfica:** incorporar una interfaz que incluya un menú de inicio con opciones de configuración, selección de modos de juego y otras funcionalidades. Además, se deberán mejorar las representaciones visuales de las plantas, zombies y demás elementos del juego.
2. **Implementación de la pala:** permitir al jugador eliminar obstáculos del mapa.
3. **Incorporación de nuevas plantas:** agregar nuevos tipos de plantas, como la Cereza y la Patapum.
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