#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// Macro para comodidad al declarar los ciclos for
#define LONGFOR(VAR, MAX) for(int VAR = 0; VAR < MAX; VAR++)

// Prototipo de funcion usada para pasarle a los hilos
void *thread(void *param);

int main() {
	pthread_t tids[5];
	pthread_attr_t attrs[5];
	char *letter = "ABCDE";

	// Se inicializan los hilos y se les asigna la letra que les 
	// corresponde
	LONGFOR(i, 5) {
		pthread_attr_init(&attrs[i]);
		pthread_create(&tids[i], &attrs[i], thread, &letter[i]);
	}

	pid_t pid;
	// Se crea un proceso hijo
	pid = fork();
	if(pid == 0)
		// En el hijo se imprime un cartel para mostrar que se creo correctamente
		printf("Se ha creado el proceso hijo correctamente con el PID=%d\n", getpid());
	else {
		// El padre espera por cada uno de sus hijos
		LONGFOR(i, 5)
			pthread_join(tids[i], NULL);
	}
}

void *thread(void *param) {
	char letter = (*(char *)param);
	int variable_interna = 0;
	// Se imprime 10K veces la letra pasada por parametro
	LONGFOR(i, 10000) {
		printf("%c(PID=%d - THREAD_ID=%lu)\n", letter, getpid(), pthread_self());
		// Se incremente 1K veces la variable interna
		LONGFOR(i, 1000)
			variable_interna++;
		// Se resetea la variable
		variable_interna = 0;
	}
}
