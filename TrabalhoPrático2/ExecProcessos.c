#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <sys/msg.h>
#include <time.h>

#define NUM_FILHOS 5
#define NUM_GRANDE 5000000000
                 //50000000000
#define RAND_CEILING 10
#define RAND_FLOOR 2

int main () {
  time_t start_t, end_t;
  bool isParent = true;
  int tempo, myPid, i;
  int pids[NUM_FILHOS];
  
  srand(time(0));

  for(i = 0; i < NUM_FILHOS; i++){
    tempo = rand() % (RAND_CEILING - RAND_FLOOR + 1) + RAND_FLOOR;
    int temp = fork();
    time(&start_t);
    //print("start t de pid: %d = a %ld",temp,start_t);
    if (temp == 0) {
      isParent = false;
      break;
    } else {
      printf("pid do filho i = %d é %d, tempo é %d\n", i, temp, tempo);
      pids[i] = temp;
    }
  }

  if(!isParent) {
    //imediatamente para a execução do filho
    
    sleep(tempo);

    myPid = getpid();
    for(int j = 0; j < NUM_GRANDE; j++); /* {
      if(j % 25000000 == 0) printf("foi uma vez\n");
    } */
    printf("meu pid é %d\n", myPid);

    time(&end_t);
    //print("end t de pid: %d = a %ld",myPid,end_t);
    double turnaround_time = difftime(end_t, start_t);
    printf("[%d] turnaround time: %lf\n", myPid, turnaround_time);
    exit(0);
  }
  
  if (isParent) {

    for (int i = 0; i < NUM_FILHOS; i++) {
      int tpid = wait(NULL);
      //time(&end_t);
      //ta = difftime(end_t, start_t);
      //printf("Filho com pid %d, teve turnaround de %f",tpid,ta)
    
    }
    return 0;
  }
}