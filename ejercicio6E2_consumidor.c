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
   int i,stat, msqid, shid;
   sem_t *full;
   sem_t *empty;
   
   printf("(%d) Creating or accessing shared memory segment\n", getpid());

   if ((msqid = msgget(1234, (0666 | IPC_CREAT | IPC_EXCL)))  < 0){
      if ((msqid = msgget(1234, 0666)) < 0){
         printf("Error creating or accessing shared memory\n");
         exit(EXIT_FAILURE);
      }
   }

   /* memoria compartida para los semaforos */
   if ((shid = shmget(4567, sizeof(full)*2, (0666 | IPC_CREAT | IPC_EXCL))) < 0){
      if ((shid = shmget(4567, sizeof(full)*2, 0666)) < 0){
         printf("Error al intentar crear o acceder a la memoria compartida\n");
         exit(EXIT_FAILURE);
      }
   }

   full = (sem_t *)shmat(shid, 0, 0);
   empty = ((sem_t *)shmat(shid, 0, 0)) +1;

   
   printf("(%d) Shared Memory accessed\n",getpid()); 
   printf("(%d) Awaiting initialization message\n",getpid()); 
   stat = msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), BUFFSIZE+1, 0); 

      if (stat < 0){
         printf("Error receiving message\n");
         exit(EXIT_FAILURE);
      }

   i=1;
   for (;;){      

      sem_wait(full);

      stat = msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), i, 0); 

      if (stat < 0){
         printf("Error receiving message\n");
         exit(EXIT_FAILURE);
      }

      printf("(%d) Message received: %s\n", getpid(), msg.mtext); 
      
      sem_post(empty);

      sleep(2);

      i=(i+1)%(BUFFSIZE+1);
      if (i == 0)
         i++;
   }

   exit(EXIT_SUCCESS);
}
