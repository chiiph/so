CC = gcc
LIBS = -lpthread

all: ejercicio4A ejercicio5A

ejercicio4A: ejercicio4A.c
	$(CC) -o ejercicio4A ejercicio4A.c $(LIBS)

ejercicio5A: ejercicio5A.c
	$(CC) -o ejercicio5A ejercicio5A.c $(LIBS) -std=c99

clean:
	rm -rf ejercicio4A ejercicio5A
	rm -rf output_*
