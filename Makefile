CC = gcc
LIBS = -lpthread

all: ejercicio4A ejercicio5A ejercicio6B ejercicio6D

ejercicio4A: ejercicio4A.c
	$(CC) -o ejercicio4A ejercicio4A.c $(LIBS)

ejercicio5A: ejercicio5A.c
	$(CC) -o ejercicio5A ejercicio5A.c $(LIBS) -std=c99

ejercicio6B: ejercicio6B.c ejercicio6B_2.c
	$(CC) -o ejercicio6B ejercicio6B.c $(LIBS)
	$(CC) -o ejercicio6B_2 ejercicio6B_2.c $(LIBS)
	
ejercicio6D: ejercicio6D_productor_shmem.c ejercicio6D_consumidor_shmem.c
	$(CC) -o ejercicio6D_productor_shmem ejercicio6D_productor_shmem.c $(LIBS)
	$(CC) -o ejercicio6D_consumidor_shmem ejercicio6D_consumidor_shmem.c $(LIBS)

clean:
	rm -rf ejercicio4A ejercicio5A ejercicio6B ejercicio6B_2
	rm -rf ejercicio6D_productor_shmem ejercicio6D_consumidor_shmem
	rm -rf output_*
