#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFSIZE 10

typedef struct messages{
   long mtype;
   char mtext[10];
}msg_t;

int main(){
   
   msg_t msg;
   sem_t *full;
   sem_t *empty;

   int i;

   strcpy(msg.mtext,"PING");

   int stat, msqid, shid;

   /* Se intenta crear el espacio para la cola de mensajes, 
    * si ya esta creada, se hace el attach a la misma 
    * */
   printf("(%d) Creating or accessing shared memory segment\n", getpid());

   if ((msqid = msgget(1234, (0666 | IPC_CREAT | IPC_EXCL))) < 0){
      if ((msqid = msgget(1234, 0666)) < 0){
         printf("Error al intentar crear o acceder a la memoria compartida\n");
         exit(EXIT_FAILURE);
      }
   }

   /* memoria compartida para el semaforo */
   if ((shid = shmget(4567, sizeof(full)*2, (0666 | IPC_CREAT | IPC_EXCL))) < 0){
      if ((shid = shmget(4567, sizeof(full)*2, 0666)) < 0){
         printf("Error al intentar crear o acceder a la memoria compartida\n");
         exit(EXIT_FAILURE);
      }
   }

   full = (sem_t *)shmat(shid, 0, 0);
   empty = ((sem_t *)shmat(shid, 0, 0)) + 1;

   if ((sem_init(empty, 1, BUFFSIZE) == -1) || (sem_init(full, 1, 0) == -1)){
      printf("Error al inicializar los semaforos\n");
      exit(EXIT_FAILURE);
   }


   printf("(%d) Shared memory accessed\n", getpid());

   printf("(%d) pings\n", getpid());
   i=1;
   msg.mtype = BUFFSIZE+1;
   sprintf(msg.mtext, "%d", msg.mtype);

   stat = msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0);
   if (stat < 0){
         printf("Error al enviar mensaje\n");
         exit(EXIT_FAILURE);
      }
   
   printf("(%d) Message %d sent: %s\n", getpid(), i, msg.mtext);
   sleep(3); 
   for(;;){
      
      sem_wait(empty);

      msg.mtype = i;
      sprintf(msg.mtext, "%d", i);

      stat = msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0);
      if (stat < 0){
         printf("Error al enviar mensaje\n");
         exit(EXIT_FAILURE);
      }
   
      printf("(%d) Message %d sent: %s\n", getpid(), i, msg.mtext);
      
      sem_post(full);

      sleep(1);

      i=(i + 1)%(BUFFSIZE+1);
      if (i == 0)
         i++;

   }
  
   exit(EXIT_SUCCESS);
   
}
