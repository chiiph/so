#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem[4];
char ids[5] = "ABCDE";
int turno = 0;
int turnoD = 0;
void *printid(void *id){

   int i;
   char letra;

   i = (int) id;
   letra = ids[i];

   if (i>0)
      i--;

   for(;;){
      sem_wait(&(sem[i]));
      printf("%c", letra);
      fflush(NULL);
      sleep(1);
      
      if (i == 0){
         if (turno == 0 && turnoD == 0){
            turnoD = 1;
            sem_post(&(sem[(i+1)%4]));        
         }
         else{
            if (turno == 0 && turnoD == 1){
               turno = 1;
               turnoD = 0;
               sem_post(&(sem[i]));
            }
            else{
               if (turno == 1 && turnoD == 0){
                  turno = 0;
                  turnoD = 0;
                  sem_post(&(sem[(i+2)%4]));
               }
            }
         }        
      }
      else{
         if (i == 1)
            sem_post(&(sem[(i+2)%4]));
         else
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
