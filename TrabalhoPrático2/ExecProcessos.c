/*
* TRABALHO 2 DE FSO - 04/2022
*
* AUTORES:
*   19/0017953 - Mateus de Paula Rodrigue
*   17/0061001 - Lucas Vinicius Magalhães Pinheiro
*   17/0043959 - Rafael Gonçalves de Paulo
*   16/0128331 - José Fortes Neto
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>

#define NUM_FILHOS 5
#define NUM_GRANDE 50000000000
                 //50000000000
#define RAND_CEILING 180
#define RAND_FLOOR 60

int main () {
  time_t start_t, end_t;
  bool isParent = true;
  int tempo, myPid, i;
  int pids[NUM_FILHOS];
  int tempos[NUM_FILHOS];
  

  srand(time(0));

  for(i = 0; i < NUM_FILHOS; i++){
    tempo = rand() % (RAND_CEILING - RAND_FLOOR + 1) + RAND_FLOOR;
    int temp = fork();
    
    time(&start_t);
    if (temp == 0) {
      isParent = false;
      break;
    } else {
      printf("pid do filho i = %d é %d, seu tempo_i é: %d\n", i, temp, tempo);
      pids[i] = temp;
      tempos[i] = tempo; 
    }
  }

  if(!isParent) {
    
    sleep(tempo);


    myPid = getpid();
    
       
    
    for(long long j = 0; j < NUM_GRANDE; j++);
    printf("Meu pid é %d\n", myPid);

    exit(0);
  }
  
  if (isParent) {
    
    
    for (int i = 0; i < NUM_FILHOS; i++) {
      int tpid = wait(NULL);
      int lt = 0;
      time(&end_t);
      for(int j = 0;j < NUM_FILHOS;j++){
          if(pids[j] == tpid){
              lt = tempos[j];
          }
      }
      double ta = difftime(end_t, start_t)-lt;
      printf("Turnaround time do filho %d: %lf\n", tpid, ta);
    
    }
    return 0;
  }
}
