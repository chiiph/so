CC = gcc
LIBS = -lpthread

all: ejercicio5B

ejercicio5B: ejercicio5B1.c ejercicio5B2.c ejercicio5B3.c
	$(CC) -o ejercicio5B1 ejercicio5B1.c $(LIBS)
	$(CC) -o ejercicio5B2 ejercicio5B2.c $(LIBS)
	$(CC) -o ejercicio5B3 ejercicio5B3.c $(LIBS)

clean:
	rm -rf ejercicio5B1 ejercicio5B2 ejercicio5B3
