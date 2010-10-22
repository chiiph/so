#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

typedef struct messages{
   long mtype;
   char mtext[10];
}msg_t;

int main(){
   
   msg_t msg;

   msg.mtype = 1;
   strcpy(msg.mtext,"PING");

   int stat, msqid;

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

   printf("(%d) Shared memory accessed\n", getpid());

   printf("(%d) pings\n", getpid());
   stat = msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0);

   if (stat < 0){
      printf("Error al enviar mensaje\n");
      exit(EXIT_FAILURE);
   }

   printf("(%d) PING sent, awaiting answer\n", getpid()); 
   
   stat = msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 2, 0);
   if (stat < 0){
      printf("Error al recibir mensaje\n");
      exit(EXIT_FAILURE);
   }

   printf("(%d) Message received: %s\n", getpid(), msg.mtext);
   exit(EXIT_SUCCESS);
   
}
