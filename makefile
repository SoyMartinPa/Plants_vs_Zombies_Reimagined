PVZR: Main.c Entrada/Configuracion.c Entrada/Jardin.c Logica/Logica.c GUI/GUI.c Entrada/Entrada.h
	gcc -Wall Main.c Entrada/Configuracion.c Entrada/Jardin.c Logica/Logica.c GUI/GUI.c -IEntrada -o PVZR
clean:
	rm -f PVZR