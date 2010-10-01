#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#define PADRE "padre"
#define HIJO1 "hijo1"
#define HIJO2 "hijo2"

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

void writeIt(sem_t mutex, pid_t pid, char *who) {
	sem_wait(&mutex);
	toFile(getpid(), who);
	sem_post(&mutex);
	printf("El proceso %s(%d) esta terminando\n", who, getpid());
}

int main() {
	pid_t pid1, pid2;
	sem_t mutex;

	if(sem_init(&mutex,1,1) < 0) {
		perror("Error inicializando el mutex\n");
		return 1;
	}

	pid1 = fork();
	if(pid1 == 0) {
		// hijo1
		writeIt(mutex, getpid(), HIJO1);
	} else if(pid1 > 0) {
		pid2 = fork();

		if(pid2 == 0) {
			// hijo2
			writeIt(mutex, getpid(), HIJO2);
		} else {
			// padre
			writeIt(mutex, getpid(), PADRE);
		}
	}

	return 0;
}
