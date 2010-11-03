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
	sem_t *full1, *full2;
	int created;

	printf("Inicializando los semaforos...\t\t\t");
	fflush(stdout);
	// Se crea o attachea la shared memory de los mutex
	shmid_mutex = create_or_attach(key_mutex, sizeof(*full1)*2, &created);
	// Accedemos al array compartido
	void *mem = shmat(shmid_mutex, 0, 0);
	// Sabemos que el primer elemento es el semaforo full1
	// Asignado para la escritura en el otro proceso
	// (a este semaforo se le hara wait para leer, y al otro
	// post para escribir)
	full1 = ((sem_t *)mem);
	// Y el segundo el full2
	full2 = ((sem_t *)mem)+sizeof(*full1);
	// Si creamos la memoria
	if(created) {
		// Entonces se inicializan los semaforos
		if(sem_init(full1, 1, 0) == -1) {
			printf("ERROR: No se puede inicializar el semaforo full\n");
			exit(1);
		}
		// Empty tiene valor 1 porque se mandan de a un mensaje
		if(sem_init(full2, 1, 1) == -1) {
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

	// Seteamos un id random del mensaje
	id = random() % 100;
	// Comienza esperando un mensaje
	sem_wait(full1);
	printf("Leyendo mensaje...\n");
	// Se imprime el mensaje
	printf("%s", message);
	sleep(1);
	// Se escribe el nuevo mensaje
	sprintf(message, "(%d) Mensaje enviado por PID=%d\n", id, getpid());
	// Se avisa que se "envio" el mensaje
	printf("Enviado el mensaje con ID=%d por el PID=%d\n", id, getpid());
	// Se avisa que hay un lugar vacio nuevo
	sem_post(full2);
	sleep(1);

	// Se marcan ambos segmentos para que se borren cuando
	// el ultimo proceso de dettachea
	shmctl(shmid_msg, IPC_RMID, 0);
	shmctl(shmid_mutex, IPC_RMID, 0);

	return 0;
}
