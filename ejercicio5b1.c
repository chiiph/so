#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem[5];
char ids[5] = "ABCDE";

void *printid(void *id){

   int i = (int) id;

   for(;;){
      sem_wait(&(sem[i]));
      printf("%c", ids[i]);
      fflush(NULL);
      sleep(1);
      sem_post(&(sem[(i+1)%5]));
   }

   return 1;
}


int main(){

   int i;
   pthread_t thr[5];
   

   sem_init(&(sem[0]), 0, 1);

   for(i=1; i<5; i++){
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
