#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#define PADRE "padre"
#define HIJO1 "hijo1"
#define HIJO2 "hijo2"

// Abre el archivo output_ejercicio4A y escribe en el
void toFile(pid_t pid, char *who) {
	FILE *f;
	f = fopen("output_ejercicio4A", "a+");
	if(f == NULL) {
		perror("Error al abrir el archivo output_ejercicio4A\n");
		return;
	}
	fprintf(f, "PID %d es el %s\n", pid, who);
	fclose(f);
}

// Funcion utilizada para manejar los semaforos y escribir al archivo
void writeIt(sem_t mutex, pid_t pid, char *who) {
	sem_wait(&mutex);
	toFile(getpid(), who);
	sem_post(&mutex);
	printf("El proceso %s(%d) esta terminando\n", who, getpid());
}

int main() {
	pid_t pid1, pid2;
	sem_t mutex;

	// Se inicializa el mutex
	if(sem_init(&mutex,1,1) < 0) {
		perror("Error inicializando el mutex\n");
		return 1;
	}

	// Se crea el primer hijo
	pid1 = fork();
	if(pid1 == 0) {
		// En el hijo, escribir al archivo que es el
		writeIt(mutex, getpid(), HIJO1);
	} else if(pid1 > 0) {
		// En el padre se crea el siguiente hijo
		pid2 = fork();

		if(pid2 == 0) {
		  // En el hijo, escribir al archivo que es el
			writeIt(mutex, getpid(), HIJO2);
		} else {
			// Y por ultimo en el padre escribe al archivo
			// su parte
			writeIt(mutex, getpid(), PADRE);
		}
	}

	return 0;
}
