#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/io.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define key (key_t)1234
int main() {
   FILE *file;

   if ((file = fopen("salida.txt", "w")) == NULL) {

      printf("Error al crear el archivo\n");
      exit(EXIT_FAILURE);
   }
   else {
      fclose(file);

      //////////
      int shid;
      sem_t *mutex;
      
      shid = shmget(key, sizeof(mutex), (IPC_CREAT|0666));
      mutex = shmat(shid, 0, 0);

      if (sem_init(mutex,1,1) == -1) {
         
         /* error al inicializar el semaforo */
         printf("Error al inicializar el semaforo\n");
         exit(EXIT_FAILURE);

      }

      else {

         /* creo el primer hijo */
         printf("Creo el primer hijo\n");
         char *cmd[] = { "./job4c", "A", "salida.txt", (char *) 0 };

         pid_t p = fork();
         
         if (p == 0) {
   
            /* es el hijo, carga la imagen ejecutable que realiza la tarea */
            if (execv("./job4c", cmd) == -1) {
               
               printf("Error al crear el primer proceso hijo\n");
            }
   
         }
         else {
            
            /* creo el segundo proceso hijo */
            printf("Creo el segundo hijo\n");
            char *cmd[] = { "./job4c", "B", "salida.txt", (char *) 0 };
            p = fork();
   
            if (p == 0) {
   
               /* es el segundo hijo, carga la imagen ejecutable que realiza la tarea */
               if (execv("./job4c", cmd) == -1) {
               
                  printf("Error al crear el segundo proceso hijo\n");
               }
   
            }
            else {
   
               /* creo el tercer proceso hijo */
               printf("Creo el tercer hijo\n");
               char *cmd[] = { "./job4c", "C", "salida.txt", (char *) 0 };
               p = fork();
   
               if (p == 0){
   
                  /* es el tercer hijo, carga la imagen ejecutable que realiza la tarea */
                  if (execv("./job4c", cmd) == -1) {
                  
                     printf("Error al crear el tercer proceso hijo\n");
                  }
   
               }
               else {
                  /* es el proceso padre */
                  /* espera que terminen todos los hijos */
                  printf("Espero a que terminen los hijos\n");
                  wait(NULL);
                  
                  /* abre el archivo que creo al inicio */
                  if ((file = fopen("salida.txt","a+")) == NULL) {
   
                     printf("Error al intentar abrir el archivo\n");
                     exit(EXIT_FAILURE);
                  }
                  else {
                     
                     /* si se abrio correctamente, escribe que se finalizo la actividad y termina */
                     fprintf(file, "Se ha finalizado la actividad");
                     fclose(file);
                     exit(EXIT_SUCCESS);
   
                  }
               }
            }
         }
      }
   }
}
