#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define LONGFOR(VAR, MAX) for(int VAR = 0; VAR < MAX; VAR++)

void *thread(void *param);

int main() {
	pthread_t tids[5];
	pthread_attr_t attrs[5];
	char *letter = "ABCDE";

	LONGFOR(i, 5) {
		pthread_attr_init(&attrs[i]);
		pthread_create(&tids[i], &attrs[i], thread, &letter[i]);
	}

	pid_t pid;
	pid = fork();
	if(pid == 0)
		printf("Se ha creado el proceso hijo correctamente con el PID=%d\n", getpid());
	else {
		LONGFOR(i, 5)
			pthread_join(tids[i], NULL);
	}
}

void *thread(void *param) {
	char letter = (*(char *)param);
	int variable_interna = 0;
	LONGFOR(i, 10000) {
		printf("%c(PID=%d - THREAD_ID=%lu)\n", letter, getpid(), pthread_self());
		LONGFOR(i, 1000)
			variable_interna++;
		variable_interna = 0;
	}
}
