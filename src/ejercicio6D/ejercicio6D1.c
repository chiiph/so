#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

/* Defino la estructura del mensaje para la cola */
typedef struct messages{
   long mtype;
   char mtext[10];
}msg_t;

int main(){
   
   msg_t msg;
   int stat, msqid;

   msg.mtype = 1;
   strcpy(msg.mtext,"PING");


   /* Se intenta crear el espacio para la cola de mensajes, 
    * si ya esta creada, se hace el attach a la misma */
   printf("(%d) Creando o accediendo a la cola de mensajes\n", getpid());
   if ((msqid = msgget(1234, (IPC_CREAT | IPC_EXCL | 0666))) < 0){
      if ((msqid = msgget(1234, 0666)) < 0){
         printf("Error al intentar crear o acceder a la cola de mensajes\n");
         exit(EXIT_FAILURE);
      }
   }

   printf("(%d) Cola de mensajes accedida\n", getpid());

   printf("(%d) Enviando mensaje: %s\n", getpid(), msg.mtext);

   /* Se envia el mensaje por la cola de mensajes */
   stat = msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0);

   if (stat < 0){
      printf("Error al enviar mensaje\n");
      exit(EXIT_FAILURE);
   }

   printf("(%d) Mensaje enviado, esperando respuesta\n", getpid()); 
   
   /* El proceso recibe un mensaje, por defecto si no hay ningun mensaje en la cola
    * el proceso espera hasta que llegue un mensaje nuevo */
   stat = msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 2, 0);
   if (stat < 0){
      printf("Error al recibir mensaje\n");
      exit(EXIT_FAILURE);
   }

   printf("(%d) Mensaje recibido: %s\n", getpid(), msg.mtext);
   printf("(%d) Terminando\n",getpid());

   exit(EXIT_SUCCESS);
}
