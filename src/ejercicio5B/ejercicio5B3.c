#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* Declaro estas variables globales asi todos los hilos
 * que se crean luego pueden acceder a las mismas.
 * Se usan 4 semaforos, si bien son 5 hilos, A y B comparten
 * el semaforo para la sincronizacion.
 * Se usan adicionalmente dos enteros que nos indican en que turno
 * se encuentra, es decir, si hay que imprimir (A o B) 2 veces,
 * una vez e ir a C, o una vez e ir a D. */
sem_t sem[4];
char ids[5] = "ABCDE";
int turno = 0;
int turnoD = 0;

/* Imprime una letra correspondiente al arreglo "ids" dado el
 * entero que se le pasa por parametro */
void *printid(void *id){

   int i;
   char letra;

   /* El valor de i es el que indica que hilo es y que letra
    * debe imprimir */
   i = (int) id;
   letra = ids[i];

   /* En este caso c omo A y B comparten el mismo semaforo, la
    * letra que debe imprimir fue guardada localmente e "i" es
    * decrementada para B, C, D y E */
   if (i>0)
      i--;

   for(;;){

      /* Espero el semaforo que me corresponde */
      sem_wait(&(sem[i]));

      /* Imprimo la letra identificatoria del hilo */
      printf("%c", letra);
      fflush(NULL);

      /* Espero 1 segundo */
      sleep(1);
      
      /* Chequeo en que turno se encuentra la sincronizacion
       * y levanto el semaforo que corresponda.
       * Si el hilo es A o B... */
      if (i == 0) {

         /* ... y no es el turno de D, ni tengo que imprimir
          * dos veces (A o B) */
         if (turno == 0 && turnoD == 0) {
            
            /* ... significa que la proxima vez que sea el turno
             * de (A o B) voy a tener que imprimirlo 2 veces */
            turnoD = 1;
            sem_post(&(sem[(i+1)%4]));        
         }
         else {
            
            /* ... y no es el turno de D, pero si el turno doble */
            if (turno == 0 && turnoD == 1) {
               
               /* significa que tengo que imprimir nuevamente (A o B)
                * y luego sera el turno de D */
               turno = 1;
               turnoD = 1;
               sem_post(&(sem[i]));
            }
            else{
               
               /* ... y es el turno de D y fue el turno doble ... */
               if (turno == 1 && turnoD == 1){

                  /* significa que tengo que imprimir por ultima vez
                   * (A o B), pasar a imprimir D y dejar en el turno
                   * que la proxima vez no sera turno doble */
                  turno = 0;
                  turnoD = 0;
                  sem_post(&(sem[(i+2)%4]));
               }
            }
         }        
      }

      /* Si no es el turno de (A o B) */
      else{
         /* Y es el turno de C */
         if (i == 1)
            
            /* Levanto el semaforo de E */
            sem_post(&(sem[(i+2)%4]));
         /* O es el turno de D */
         else

            /* Levanto el semaforo de E */
            sem_post(&(sem[(i+1)%4]));
      }
   }
}

int main(){

   int i;
   pthread_t thr[5];
   
   /* Se inicializa el semaforo de (A o B) en 1, y el resto de los
    * semaforos en 0. De esta forma siempre ejecutara primero (A o B) */
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
