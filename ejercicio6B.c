#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

// Macro para calcular el minimo entre dos numeros
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
// Maxima longitud del buffer de mensaje
#define MAXLEN 100

int create_or_attach(key_t key, size_t size, int *created) {
	int shmid;
	*created = 0;

	// Se trata de crear la memoria
	if((shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | (SHM_R | SHM_W))) < 0) {
		// Si falla, y el error es que la memoria ya esta creada
		if(errno == EEXIST) {
			// utiliza esa memoria
			if((shmid = shmget(key, size, (SHM_R | SHM_W))) < 0) {
				printf("\nERROR: No se puede ni crear ni attachear la shmem\n");
				exit(1);
			} else {
				*created = 0;
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
	// Se setean dos llaves arbitrarias
	key_t key_mutex = 4321;
	key_t key_msg = 8765;
	int shmid_mutex, shmid_msg;
	char *message;
	sem_t *full, *empty;
	int created;

	printf("Inicializando los semaforos...\t\t\t");
	fflush(stdout);
	// Se crea o attachea la shared memory de los mutex
	shmid_mutex = create_or_attach(key_mutex, sizeof(full)*2, &created);
	// Accedemos al array compartido
	void *mem = shmat(shmid_mutex, 0, 0);
	// Sabemos que el primer elemento es el semaforo full
	full = ((sem_t *)mem);
	// Y el segundo el empty
	empty = ((sem_t *)mem)+1;
	// Si creamos la memoria
	if(created) {
		// Entonces se inicializan los semaforos
		if(sem_init(full, 1, 0) == -1) {
			printf("ERROR: No se puede inicializar el semaforo full\n");
			exit(1);
		}
		// Empty tiene valor 1 porque se mandan de a un mensaje
		if(sem_init(empty, 1, 1) == -1) {
			printf("ERROR: No se puede inicializar el semaforo empty\n");
			exit(1);
		}
	}
	printf("[ OK ]\n");

	printf("Inicializando buffer...\t\t\t\t");
	fflush(stdout);
	// Se crea o attachea al buffer de mensaje
	shmid_msg = create_or_attach(key_msg, MAXLEN, &created);
	message = (char *)shmat(shmid_msg, 0, 0);
	printf("[ OK ]\n");

	int id;

	while(1) {
		// Seteamos un id random del mensaje
		id = random() % 100;
		// Esperamos que haya algun lugar vacio
		sem_wait(empty);
		// Se guarda el mensaje en la shmem
		sprintf(message, "(%d) Mensaje enviado por PID=%d\n", id, getpid());
		// Se avisa que se "envio" el mensaje
		printf("Enviado el mensaje con ID=%d por el PID=%d\n", id, getpid());
		// Avisamos que hay un mensaje nuevo
		sem_post(full);
		printf("***\n");
		// Esperamos 5 segundos para hacer mas simple la lectura del output
		sleep(5);
	}

	return 0;
}
