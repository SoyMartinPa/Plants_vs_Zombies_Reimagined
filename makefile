SRC = $(wildcard *.c Entrada/*.c Logica/*.c GUI/*.c)
HDR = $(wildcard Entrada/*.h Logica/*.h)

PVZR: $(SRC) $(HDR)
	gcc -Wall -Wextra -g $(SRC) -IEntrada -ILogica -lncurses -o PVZR

clean:
	rm -f PVZR
