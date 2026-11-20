# PvZ-Reimagined

Proyecto desarrollado para el curso **Programación de Computadores**.

## Selección de utilidades

Usando los números y la barra espaciadora se pueden utilizar las siguientes plantas y herramientas:

- `'0'`: Pala (Cuesta 200 soles quitar un obstaculo).
- `'1'`: Girasol (Vale 50 soles).
- `'2'`: Lanzaguisantes(Vale 100 soles).
- `'3'`: Nuez (Vale 50 soles).
- `'4'`: PetaCereza(Vale 150 soles).
- `'5'`: Ajo(Vale 50 soles).

El mapa también incorpora obstáculos (`'O'`), los cuales impiden colocar plantas en las casillas que ocupan. Se eliminan utilizando la pala.

## Funcionamiento de las oleadas
Al iniciar una partida, se otorgará al usuario 20 segundos para preparar su defensa contra los zombies.
Cada oleada es más larga y dificil que la anterior, aumentando la posibilidad de otorgarle armadura más poderosa
a los zombies y más vida, como también aumentar su cantidad de aparición por ronda.
Una vez terminada una ronda, se otorgarán 10 segundos de preparación para la siguiente ronda, que será empezada
por un zombie bandera 'F' que será más rápido que un zombie normal. Adicionalmente, obstaculos serán agregados
al inicio de cada ronda.


## Compilación y ejecución

Para compilar y ejecutar el juego, se deben seguir los siguientes pasos:

1. Abrir una terminal en el directorio raíz del proyecto.
2. Ejecutar el comando `make` para compilar el programa.
3. Una vez finalizada la compilación, ejecutar el juego mediante:

   ```bash
   ./PVZR
   ```