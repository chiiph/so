#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* Declaro estas variables globales asi todos los hilos
 * que se crean luego pueden acceder a las mismas.
 * Se usan 4 semaforos, si bien son 5 hilos, B y C comparten
 * el semaforo para la sincronizacion.
 * Se usa adicionalmente un entero que nos indica en que turno
 * se encuentra, es decir si hay que imprimir (B o C) una o 
 * dos veces */
sem_t sem[4];
char ids[5] = "ABCDE";
int turno = 2;

/* Imprime una letra correspondiente al arreglo "ids" dado el
 * entrero que se le pasa por parametro */
void *printid(void *id) {

   int i;
   char ch;
   
   /* El valor de i es el que indica que hilo es y que letra
    * debe imprimir */
   i = (int) id;
   ch = ids[i];

   /* En este caso como B y C comparten el mismo semaforo la
    * letra que debe imprimir fue guardada localmente e "i" es
    * decrementada para C, D y E */
   if (i>1)
      i--;

   for(;;) {

      /* Espero el semaforo que me corresponde */
      sem_wait(&(sem[i]));

      /* Imprimo la letra identificatoria del hilo */
      printf("%c", ch);
      fflush(NULL);

      /* Espero 1 segundo */
      sleep(1);
      
      /* Chequeo en que turno se encuentra la sincronizacion
       * y levanto el semaforo que corresponda.
       * Si es el hilo B o C... */
      if (i == 1) {
         
         /* ... y el turno esta en 2... */
         if (turno == 2) {
            
            /* ... significa que tengo que imprimir nuevamente (B o C).
             * Pongo el turno en 1, indicando que solo tiene que imprimir
             * una vez mas (B o C) */
            turno = 1;
            sem_post(&(sem[i]));
         }
         else {

            /* ... y el turno esta en 1... */
            if (turno == 1) {
   
               /* ... significa que acabo de imprimir el segundo (B o C).
                * Por lo tanto la proxima iteracion se debera imprimir una
                * sola  vez (B o C).
                * Pongo el turno en 0 y levanto el semaforo de D */
               turno = 0;
               sem_post(&(sem[(i+1)%4]));        
            }

            /* ... y el turno esta en 0... */
            else {
              
               /* ... significa que esta iteracion se imprimio una sola vez
                * (B o C) por lo tanto debo volver el turno a 2 para que se
                * repita el ciclo completo.
                * Luego levanto el semaforo de D para que todo continue */
               turno = 2;
               sem_post(&(sem[(i+1)%4]));
            }
         }
      }
      /* si no es (B o C) entonces levanto el semaforo siguiente al actual */
      else{

         sem_post(&(sem[(i+1)%4]));
      }
   }
}

int main(){

   int i;
   pthread_t thr[5];
   
   /* Se inicializa el semaforo del hilo A en 1, y el resto de los
    * semaforos en 0. De esta forma A siempre ejecutara primero */
   sem_init(&(sem[0]), 0, 1);

   for(i=1; i<4; i++){
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
