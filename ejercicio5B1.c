#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* Declaro estas variables globales asi todos los threads
 * que se crean luego pueden acceder a las mismas
 * Se usan 5 semaforos, 1 para cada hilo */
sem_t sem[5];
char ids[5] = "ABCDE";

/* Imprime una letra correspondiente al arreglo "ids" dado el
 * entrero que se le pasa por parametro */
void *printid(void *id){

   int i = (int) id;

   for(;;){
      
      /* Espero el semaforo que me corresponde */
      sem_wait(&(sem[i]));

      /* Imprimo la letra identificatoria del hilo */
      printf("%c", ids[i]);
      fflush(stdout);

      /* Espero 1 segundo */
      sleep(1);

      /* Le señalo al proximo hilo que puede comenzara a ejecutar */
      sem_post(&(sem[(i+1)%5]));
   }
}


int main(){

   int i;
   pthread_t thr[5];
   
   /* Se inicializa el semaforo del hilo A en 1, y el resto de los 
    * semaforos en 0. De esta forma A siempre ejecutara primero */ 
   sem_init(&(sem[0]), 0, 1);

   for(i=1; i<5; i++){
      sem_init(&(sem[i]), 0, 0);
   }

   /* El proceso inicial crea los 5 hilos y les asigna la funcion
    * printid a cada uno de ellos */
   for(i=0; i<5; i++){
      pthread_create(&thr[i], NULL, printid, (void *) i);
   }

   for(i=0; i<5; i++){
      pthread_join(thr[i], NULL);
   }

   exit(EXIT_SUCCESS);

}
