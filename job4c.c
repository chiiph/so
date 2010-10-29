#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define key (key_t)1234

/* Escribe en el archivo filename, el mensaje message */
/* Devuelve 1 si la escritura fue exitosa, 0 si hubo algun error */
int write(char  *filename, char *message) {
   
   FILE *file;
   
   /* Intento abrir el archivo para escribir en el */
   if ((file = fopen(filename, "a+")) == NULL) {
      
      /* No se pudo abrir el archivo */
      return 0; 

   }
   else {

      /* Imprimo el estado por pantalla y en el archivo designado */
      printf("%s", message);
      fprintf(file, "%s", message);

      fclose(file);

      return 1;
   }
}


int main(int argc, char *argv[]) {
   
   FILE *file;
   int shid, i;
   char id[1];
   char pid[10];
   char message[45];
   sem_t *mutex;

   /* Intento acceder a la memoria compartida creada por el
    * proceso padre */
   if ((shid = shmget(key, sizeof(mutex), (SHM_R | SHM_W))) <0) {
      
      printf("Error al acceder la memoria compartida\n");
      exit(EXIT_FAILURE);
   }

   /* Asigno el espacio de memoria creado anteriormente al espacio
    * de memoria del mutex */
   mutex = shmat(shid, 0, 0);
   
   /* Creo el mensaje que voy a mostrar por consola y guardar en el
    * archivo */
   sprintf(pid, "%d", getpid());
   strcpy(id,argv[1]);
   strcat(message, "ID: ");
   strcat(message, id);
   strcat(message, ", Estoy realizando mi trabajo, PID: ");
   strcat(message, pid);
   strcat(message, "\n");

   /* Realizo las 10.000 iteraciones, escribiendo por pantalla y
    * en el archivo lo que estoy haciendo */
   for(i=0; i<10000; i++){

      /* Antes de poder escribir en el archivo me aseguro que no haya
       * proceso escribiendo en el para no generar inconsistencias */
      sem_wait(mutex);

      if (write(argv[2], message) == 0) {

         printf("Error en el hijo %s",id);
         exit(EXIT_FAILURE);

      }

      /* Cuando termino de escribir en el archivo libero el mutex
       * indicandole al resto de los procesos que el archivo esta
       * libre */
      sem_post(mutex);
   }
}
