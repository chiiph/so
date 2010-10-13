#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem[4];
char ids[5] = "ABCDE";
int turno = 1;

void *printid(void *id){

   int i;
   char letra;

   i = (int) id;
   letra = ids[i];

   if (i>1)
      i--;

   for(;;){
      sem_wait(&(sem[i]));
      printf("%c", letra);
      fflush(NULL);
      sleep(1);
      
      if (i == 1){
         if (turno == 1){
            turno = 0;
            sem_post(&(sem[i]));        
         }
         else{
            turno = 1;
            sem_post(&(sem[(i+1)%4]));
         }        
      }
      else{
         sem_post(&(sem[(i+1)%4]));
      }
   }

   return 1;
}

int main(){

   int i;
   pthread_t thr[5];
   

   sem_init(&(sem[0]), 0, 1);

   for(i=1; i<4; i++){
      sem_init(&(sem[i]), 0, 0);
   }

   for(i=0; i<5; i++){
      pthread_create(&thr[i], NULL, printid, (void *) i);
   }

   for(i=0; i<5; i++){
      pthread_join(thr[i], NULL);
      //pthread_join(thr[i], (void **)&retval);
   }

   exit(EXIT_SUCCESS);

}
