#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/io.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* Defino la llave que voy a usar para la memoria compartida
 * en la cual comparto el semaforo para acceder al archivo */
#define key (key_t)1234

int main() {

   FILE *file;
   
   /* Creo el archivo salida.txt
    * El modo "w" significa que se crea el archivo o se
    * elimina todo el contenido */
   if ((file = fopen("salida.txt", "w")) == NULL) {
      
      /* Si se produjo algun error al crear el archivo el
       * programa no puede continuar */
      printf("Error al crear el archivo\n");
      exit(EXIT_FAILURE);

   }
   else {
      
      /* Si el archivo se creo o se elimino su contenido
       * procedo a cerrarlo ya que son los procesos hijos los
       * que lo van a usar */
      fclose(file);

      /* Una vez creado el archivo tengo que crear el segmento
       * de memoria compartida para el semaforo que me va a 
       * brindar la exclusion mutua entre los procesos hijos */
      int shid;
      sem_t *mutex;
      
      /* Intento crear la memoria compartida para el semaforo
       * En caso de que este creada intento accederla
       * Si fallan ambos intentos el programa reporta el error */
      if ((shid = shmget(key, sizeof(mutex), IPC_CREAT| (SHM_R | SHM_W))) < 0) {

         if ((shid = shmget(key, sizeof(mutex), (SHM_R | SHM_W))) < 0) {

            printf("Error al crear o acceder la memoria compartida\n");
            exit(EXIT_FAILURE);
         }
      }

      /* Asigno el espacio de memoria creado anteriormente al espacio
       * de memoria del mutex */
      mutex = shmat(shid, 0, 0);

      /* Inicializo el mutex
       * Si el mutex se inicializa correctamente se procede a crear
       * los hijos
       * En caso de fallar la inicializacion el programa reporta
       * el error */
      if (sem_init(mutex,1,1) == -1) {
         
         printf("Error al inicializar el semaforo\n");
         exit(EXIT_FAILURE);
      }
      else {

         char *ids[3] = { "A", "B", "C" };
         char *cmd[4]; 
         int i;
         pid_t p;
         
         cmd[0] = "job4c";
         cmd[2] = "salida.txt";
         cmd[3] = (char *) 0;
         
         /* El programa itera 3 veces, ya que crea 3 hijos */
         for (i=0; i<3; i++) { 

            cmd[1] = (char *) ids[i];
            
            /* Creo el proceso i, dependiendo de la iteracion en
             * la que se encuentre
             * No uso el else cuando pregunto si es el padre o el hijo
             * ya que el hijo carga una imagen ejecutable sobreescribiendo
             * la actual */
            printf("(%d): Creo el Hijo %d\n", getpid(), i+1);
            if (fork() == 0) {

               if (execv("job4c", cmd) == -1) {

                  printf("Error al crear el proceso hijo\n");
                  exit(EXIT_FAILURE);
               }
            }

            /* En caso de no ser el hijo (pid != 0) el padre volvera a 
             * iterar creando los hijos restantes.
             * El hijo nunca accedera a este codigo porque reemplazo la
             * imagen ejecutable por la propia */
         }
         
         /* Una vez creados los 3 hijos el padre procede a esperar
          * a que estos terminen */
         printf("(%d): Espero a que terminen los procesos hijos\n", getpid());

         wait();
         wait();
         wait();

         /* Una vez que sus hijos terminan abre el archivo "salida.txt"
          * para agregar la ultima linea, indicando que se finalizo la
          * actividad */
         if ((file = fopen("salida.txt", "a+")) == NULL) {
            
            printf("Error al abrir el archivo\n");
            exit(EXIT_FAILURE);
         }

         fprintf(file, "Se ha finalizado la actividad\n");
         printf("(%d): Se ha finalizado la actividad\n", getpid());

         fclose(file);
         
         exit(EXIT_SUCCESS);
      }
   }
}
