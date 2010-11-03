#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAXLEN 100
#define BUFSIZE 10

int create_or_attach(key_t key, size_t size, int *created) {
	int shmid;
	*created = 0;

	if((shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | (SHM_R | SHM_W))) < 0) {
		if(errno == EEXIST) {
			if((shmid = shmget(key, size, (SHM_R | SHM_W))) < 0) {
				printf("\nERROR: No se puede ni crear ni attachear la shmem\n");
				exit(1);
			} else {
				created = 0;
				return shmid;
			}
		}
	} else if(shmid < 0) {
		printf("\nERROR: Ocurrio un problema al tratar de crear la shmem\n");
		exit(1);
	}

	*created = 1;
	return shmid;
}

int main(int argc, char** argv) {
	key_t key_mutex = 9928;
	key_t key_msg = 8903;
	int shmid_mutex, shmid_msg;
	char *message;
	sem_t *full, *empty;
	int created;

	printf("Inicializando los semaforos...\t\t\t");
	fflush(stdout);
	shmid_mutex = create_or_attach(key_mutex, sizeof(*full)*2, &created);
	void *mem = shmat(shmid_mutex, 0, 0);
	full = ((sem_t *)mem);
	empty = ((sem_t *)mem)+sizeof(full);
	if(created) {
		if(sem_init(full, 1, 0) == -1) {
			printf("ERROR: No se puede inicializar el semaforo full\n");
			exit(1);
		}
		if(sem_init(empty, 1, BUFSIZE) == -1) {
			printf("ERROR: No se puede inicializar el semaforo empty\n");
			exit(1);
		}
	}
	printf("[ OK ]\n");

	printf("Inicializando buffer...\t\t\t\t");
	fflush(stdout);
	shmid_msg = create_or_attach(key_msg, BUFSIZE*MAXLEN, &created);
	message = (char *)shmat(shmid_msg, 0, 0);
	printf("[ OK ]\n");

	int id, index;

	while(1) {
		id = random() % 100;
		sem_wait(empty);
		sem_getvalue(full, &index);
		sprintf(message+(index*MAXLEN), "(%d) Mensaje enviado por PID=%d\n", id, getpid());
		printf("Produciendo el mensaje con ID=%d por el PID=%d\n", id, getpid());
		sem_post(full);
		printf("***\n");
		sleep(5);
	}

	return 0;
}
