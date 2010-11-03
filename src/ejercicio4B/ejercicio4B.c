#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Imprime por pantalla el numero de hilo que esta ejecutando la 
 * funcion.
 * El numero es pasado por parametro */
void *print(void *number) {

   int id = (int) number;

   printf("Soy el hilo numero %d\n", id);
} 


int main() {
   
   pthread_t threads[2];
   int i;
      
   /* El proceso crea 2 hilos que ejecutan la funcion print */
   for(i=0; i<2; i++) {
      
      pthread_create(&threads[i], NULL, print, (void *) i);

   }
   
   /* El proceso inicial muestra por pantalla quien es */
   printf("Soy el proceso inicial\n");
   
   /* Espera que terminen los 2 hilos para finalizar la ejecucion */
   for(i=0; i<2; i++) {
      pthread_join(threads[i], NULL);
   }

   exit(EXIT_SUCCESS);
}
