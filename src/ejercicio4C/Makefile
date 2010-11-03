CC = gcc
LIBS = -lpthread

all: ejercicio4C

ejercicio4C: ejercicio4C.c job4c.c
	$(CC) -o ejercicio4C ejercicio4C.c $(LIBS) -std=c99
	$(CC) -o job4c job4c.c $(LIBS) -std=c99

clean:
	rm -rf ejercicio4C
