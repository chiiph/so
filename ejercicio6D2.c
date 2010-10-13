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
   int stat, msqid;
   
   printf("(%d) Creating or accessing shared memory segment\n", getpid());

   if ((msqid = msgget(1234, (0666 | IPC_CREAT | IPC_EXCL)))  < 0){
      if ((msqid = msgget(1234, 0666)) < 0){
         printf("Error creating or accessing shared memory\n");
         exit(EXIT_FAILURE);
      }
   }
   printf("(%d) Shared Memory accessed\n",getpid()); 
   
   stat = msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 3, 0); 
   if (stat < 0){
      printf("Error receiving message\n");
      exit(EXIT_FAILURE);
   }
   printf("(%d) Message received: %s\n", getpid(), msg.mtext); 

   msg.mtype = 2;
   strcpy(msg.mtext,"PONG");

   printf("(%d) pongs\n", getpid());

   stat = msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0);
   
   if (stat < 0){
      printf("Error sending message\n");
      exit(EXIT_FAILURE);
   }

   printf("(%d) PONG sent\n", getpid());
   printf("(%d) Ending\n", getpid());

   exit(EXIT_SUCCESS);
}
