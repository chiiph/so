CC = gcc
LIBS = -lpthread

all: ejercicio6E1 ejercicio6E2

ejercicio6E1: ejercicio6E1_productor.c ejercicio6E1_consumidor.c
	$(CC) -o ejercicio6E1_productor ejercicio6E1_productor.c $(LIBS)
	$(CC) -o ejercicio6E1_consumidor ejercicio6E1_consumidor.c $(LIBS)

ejercicio6E2: ejercicio6E2_productor.c ejercicio6E2_consumidor.c
	$(CC) -o ejercicio6E2_productor ejercicio6E2_productor.c $(LIBS)
	$(CC) -o ejercicio6E2_consumidor ejercicio6E2_consumidor.c $(LIBS)

clean:
	rm -rf ejercicio6E1_productor ejercicio6E1_consumidor
	rm -rf ejercicio6E2_productor ejercicio6E2_consumidor
