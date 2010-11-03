#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

/* Defino el tamaño maximo del buffer */
#define BUFFSIZE 10

/* Defino la estructura del mensaje para la cola de mensajes */
typedef struct messages{
   long mtype;
   char mtext[10];
}msg_t;

int main(){
   
   msg_t msg;
   sem_t *full;
   sem_t *empty;
   int i, stat, msqid, shid;

   /* Se intenta crear el espacio para la cola de mensajes, 
    * si ya esta creada, se hace el attach a la misma */
   printf("(%d) Creando o accediendo a la cola de mensajes\n", getpid());

   if ((msqid = msgget(1357, (IPC_CREAT | IPC_EXCL | 0666))) < 0){
      if ((msqid = msgget(1357, 0666)) < 0){
         printf("Error al intentar crear o acceder a la cola de mensajes\n");
         exit(EXIT_FAILURE);
      }
   }

   printf("(%d) Cola de mensajes accedida\n", getpid());
   sleep(1);

   /* Se intenta crear el espacio para la memoria compartida
    * para los semaforos. Si ya esta creada, se hace el attach
    * a la misma */
   printf("(%d) Creando o accediendo a la memoria compartida\n", getpid());

   if ((shid = shmget(2468, sizeof(full)*2, (0666 | IPC_CREAT | IPC_EXCL))) < 0){
      if ((shid = shmget(2468, sizeof(full)*2, 0666)) < 0){
         printf("Error al intentar crear o acceder a la memoria compartida\n");
         exit(EXIT_FAILURE);
      }
   }

   printf("(%d) Memoria compartida  accedida\n", getpid());
   sleep(1);

   /* Se asignan los espacios de memoria de los semaforos al segmento de
    * memoria compartida */
   full = (sem_t *)shmat(shid, 0, 0);
   empty = ((sem_t *)shmat(shid, 0, 0)) + 1;

   /* Se inicializa los semaforos.
    * empty se inicializa con el tamaño del buffer.
    * full se inicializa con 0 */
   printf("(%d) Inicializando los semaforos\n", getpid());
   
   if ((sem_init(empty, 1, BUFFSIZE) == -1) || (sem_init(full, 1, 0) == -1)){
      printf("Error al inicializar los semaforos\n");
      exit(EXIT_FAILURE);
   }



   /* Se envia un mensaje a la cola indicando que la inicializacion de
    * los semaforos se completo */
   printf("(%d) Enviando mensaje de inicializacion completada\n", getpid());
   sleep(1);

   i=1;
   msg.mtype = BUFFSIZE+1;
   sprintf(msg.mtext, "%d", msg.mtype);

   stat = msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0);
   if (stat < 0){
         printf("Error al enviar mensaje\n");
         exit(EXIT_FAILURE);
      }
    
   printf("(%d) Mensaje de inicializacion completada enviado\n", getpid());
   printf("(%d) Comenzando la produccion de mensajes\n", getpid());
   
   sleep(1); 

   for(;;){
      
      /* Espero el semaforo empty. Si hay lugar en la cola empty tendra un
       * valor entre 1 y BUFFSIZE y me dara el acceso */
      sem_wait(empty);

      /* Defino el mensaje que voy a enviar a la cola */
      msg.mtype = i;
      sprintf(msg.mtext, "%d", i);

      /* Envio el mensaje a la cola */
      stat = msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0);
      if (stat < 0){
         printf("Error al enviar mensaje\n");
         exit(EXIT_FAILURE);
      }
   
      printf("(%d) Mensaje enviado: %s\n", getpid(), msg.mtext);
      
      /* Aumento el valor del semaforo full en 1, indicando que hay un
       * mensaje nuevo para leer en la cola */
      sem_post(full);

      /* espero 1 segundo */
      sleep(1);

      /* Aumento el indice que me indica en que lugar de la cola se pondra
       * el proximo mensaje */
      i=(i + 1)%(BUFFSIZE+1);
      if (i == 0)
         i++;
   }
  
   exit(EXIT_SUCCESS);
}
