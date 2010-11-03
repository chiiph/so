CC = gcc
LIBS = -lpthread

all: ejercicio4C

ejercicio4C: ejercicio4C.c job4c.c
	$(CC) -o ejercicio4C ejercicio4C.c $(LIBS)
	$(CC) -o job4c job4c.c $(LIBS)

clean:
	rm -rf ejercicio4C
	rm -rf salida*
