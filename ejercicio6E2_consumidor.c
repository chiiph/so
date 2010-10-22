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
   int i,stat, msqid, shid;
   
   /* Se intenta crear el espacio para la cola de mensajes,
    * si ya esta creada, se hace el attach de la misma */
   printf("(%d) Creando o accediendo a la cola de mensajes\n", getpid());

   if ((msqid = msgget(1357, (IPC_CREAT | IPC_EXCL | 0666)))  < 0){
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
   if ((shid = shmget(2468, sizeof(full)*2, (IPC_CREAT | IPC_EXCL | 0666))) < 0){
      if ((shid = shmget(2468, sizeof(full)*2, 0666)) < 0){
         printf("Error al intentar crear o acceder a la memoria compartida\n");
         exit(EXIT_FAILURE);
      }
   }

   printf("(%d) Memoria compartida accedida\n", getpid());
   sleep(1);

   /* Se asignan los espacios de memoria de los semaforos al segmento de 
    * memoria compartida */
   full = (sem_t *)shmat(shid, 0, 0);
   empty = ((sem_t *)shmat(shid, 0, 0)) +1;

   
   /* Se espera un mensaje que confirme la inicializacion de los semaforos */
   printf("(%d) Esperando mensaje de inicializacion completada\n",getpid()); 

   stat = msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), BUFFSIZE+1, 0); 

      if (stat < 0){
         printf("Error receiving message\n");
         exit(EXIT_FAILURE);
      }

   printf("(%d) Mensaje de inicializacion completada recibido\n", getpid());
   printf("(%d) Comenzando el consumo de mensajes\n", getpid());

   sleep(1);

   i=1;
   for (;;){      
      
      /* Espero el semaforo full. Si hay algun mensaje en la cola full
       * tendra un valor entre 1 y BUFFSIZE y me dara el accesso */
      sem_wait(full);

      /* Leo el mensaje que corresponda en la cola */
      stat = msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), i, 0); 

      if (stat < 0){
         printf("Error recibiendo mensaje\n");
         exit(EXIT_FAILURE);
      }

      printf("(%d) Mensaje recibido: %s\n", getpid(), msg.mtext); 
      
      /* Aumento el valor del semaforo empty en 1, indicando que hay
       * un mensaje menos en la cola */
      sem_post(empty);

      /* espero 2 segundos */
      sleep(2);
      
      /* Aumento el indice que me indica de que lugar de la cola voy a leer
       * el proximo mensaje */
      i=(i+1)%(BUFFSIZE+1);
      if (i == 0)
         i++;
   }

   exit(EXIT_SUCCESS);
}
