#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Imprime el numero que se le pasa por parametro */
void *print(void *number) {

   int id = (int) number;

   printf("Soy el hilo numero %d\n", id);
} 


int main() {
   
   pthread_t threads[2];

   int i;

   for(i=0; i<2; i++) {
      
      pthread_create(&threads[i], NULL, print, (void *) i);

   }
   
   printf("Soy el proceso padre\n");

   for(i=0; i<2; i++) {
      pthread_join(threads[i], NULL);
   }

   exit(EXIT_SUCCESS);
}
