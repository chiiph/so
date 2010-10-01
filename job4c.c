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
   
   if ((file = fopen(filename, "a+")) == NULL) {
      
      /* No se pudo abrir el archivo */
      return 0; 

   }
   else {

      /* Imprimo el estado por pantalla y en el archivo designado */
      //printf("ID: %c, Estoy realizando mi trabajo, PID: %d\n", argv[1], getpid());
      //fprintf("ID: %c, Estoy realizando mi trabajo, PID: %d\n", argv[1], getpid());
      printf(message);
      fprintf(file, message);

      fclose(file);

      return 1;
   }
}


int main(int argc, char *argv[]) {
   
   FILE *file;
   int i;
   char id[1];
   char pid[10];
   char message[45];

   ////////
   int shid;
   sem_t *mutex;
   shid = shmget(key, sizeof(mutex), 0);
   mutex = shmat(shid, 0, 0);
   ////////
   
/*   printf("HOOOOOLLLLAAAA\n");*/
/*   sprintf(pid, "%d", getpid());*/
/*   strcpy(id,argv[1]);*/
   
   sprintf(message, "ID: %s , Estoy realizando mi trabajo, PID: %d\n", argv[1], getpid());
/*   strcat(message, "ID: ");*/
/*   strcat(message, id);*/
/*   strcat(message, ", Estoy realizando mi trabajo, PID: ");*/
/*   strcat(message, pid);*/
/*   strcat(message, "\n");*/

   for(i=0; i<10; i++){
   
      /* chequear si puedo escribir y escribir */
      /* aca iria el tema de los semaforos */
      printf("Espero a tener el lock del mutex, soy el proceso %s\n", argv[1]);
      //sem_wait(mutex);
      if (sem_trywait(mutex) == 0){
      printf("Tengo el lock del mutex, soy el proceso %s\n",argv[1]);
      sleep(1);
      if (write(argv[2], message) == 0) {

         /* Hubo un error */
         printf("Error en el hijo %s",argv[1]);
         exit(EXIT_FAILURE);

      }
      sem_post(mutex);
      }
      else{
      sleep(2);
      printf("No obtuve el lock, soy el proceso %s\n", argv[1]);
      }
   }
}