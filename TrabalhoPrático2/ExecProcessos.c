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
#define NUM_GRANDE 500 /* 50000000000 */

int main () {
  bool isParent = true;
  int tempo, myPid, i;
  int pids[NUM_FILHOS];
  
  srand(time(0));
  for(i = 0; i < NUM_FILHOS; i++){
    tempo = rand() % (180-60+1) + 60;
    int temp = fork();
    if (temp == 0) {
      isParent = false;
      break;
    } else {
      pids[i] = temp;
    }
  }

  if(!isParent) {
    myPid = getpid();
    printf("i é %d, pid éh %d, tempo é %d\n", i, myPid, tempo);
    sleep(tempo);

    for(int j = 0; j < NUM_GRANDE; j++);
    printf("meu pid é %d\n", myPid);

    exit(0);
  }
  
  if (isParent) {

    
    return 0;
  }
}