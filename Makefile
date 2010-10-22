CC = gcc
LIBS = -lpthread

all: ejercicio4A ejercicio4B ejercicio4C ejercicio5A ejercicio5B ejercicio6B ejercicio6D ejercicio6E1

ejercicio4A: ejercicio4A.c
	$(CC) -o ejercicio4A ejercicio4A.c $(LIBS)

ejercicio4B: ejercicio4B.c
	$(CC) -o ejercicio4B ejercicio4B.c $(LIBS)

ejercicio4C: ejercicio4C.c
	$(CC) -o ejercicio4C ejercicio4C.c $(LIBS)

ejercicio5A: ejercicio5A.c
	$(CC) -o ejercicio5A ejercicio5A.c $(LIBS) -std=c99

ejercicio5B: ejercicio5B1.c ejercicio5B2.c ejercicio5B3.c
	$(CC) -o ejercicio5B1 ejercicio5B1.c $(LIBS)
	$(CC) -o ejercicio5B2 ejercicio5B2.c $(LIBS)
	$(CC) -o ejercicio5B3 ejercicio5B3.c $(LIBS)

ejercicio6B: ejercicio6B.c ejercicio6B_2.c
	$(CC) -o ejercicio6B ejercicio6B.c $(LIBS)
	$(CC) -o ejercicio6B_2 ejercicio6B_2.c $(LIBS)
	
ejercicio6D: ejercicio6D1.c ejercicio6D2.c
	$(CC) -o ejercicio6D1 ejercicio6D1.c $(LIBS)
	$(CC) -o ejercicio6D2 ejercicio6D2.c $(LIBS)

ejercicio6E1: ejercicio6E1_productor.c ejercicio6E1_consumidor.c
	$(CC) -o ejercicio6E1_productor ejercicio6E1_productor.c $(LIBS)
	$(CC) -o ejercicio6E1_consumidor ejercicio6E1_consumidor.c $(LIBS)

clean:
	rm -rf ejercicio4A ejercicio5A ejercicio6B ejercicio6B_2
	rm -rf ejercicio6E1_productor ejercicio6E1_consumidor
	rm -rf output_*
