#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>

#define NUM_FILHOS 10

int children_not_yet_ready = NUM_FILHOS;
int myPid;

void recieved_ok_to_die(int signum) {
}

void child_is_ready(int signum) {
  children_not_yet_ready--;
}

int main() {
  bool isParent = true;
  int pids[NUM_FILHOS]; // lista de pids
  int parentPid = myPid = getpid();

  // setup signal callbacks
  signal(SIGUSR1, child_is_ready);
  signal(SIGUSR2, recieved_ok_to_die);

  // fork processes
  for (int i = 0; i < NUM_FILHOS; i++) {
    int temp = fork();
    if (temp == 0) {
      isParent = false;
      break;
    } else {
      pids[i] = temp;
    }
  }

  // só executa esse código se é filho
  if (!isParent) {
    myPid = getpid();
    
    printf("hi, i'm process %d and i did my thing. \n", myPid);
    
    // tell parent i did my thing
    kill(parentPid, SIGUSR1);

    // wait untill i have the ok to die
    pause();

    printf("#%d recieved the ok to go.\n", myPid);

    // die
    exit(0);
  }

  // só executa esse código se é pai
  if (isParent) {

    while (children_not_yet_ready > 0) {
      printf("i have %d children not yet ready\n", children_not_yet_ready);
      sleep(2);
    }

    printf("i'm the parent, and all of my children did their things.\n");

    // send all children the ok to die
    for (int i = 0; i < NUM_FILHOS; i++) {
      kill(pids[i], SIGUSR2);
    }

    printf("now i'll wait until they're all dead before i can go.\n");
    
    // wait for all children to stop before exiting
    for (int i = 0; i < NUM_FILHOS; i++) {
      int status;
      waitpid(pids[i], &status, 0);
    }

    // morre
    printf("all of my children have died, so now i'll end too.\n");
    return 0;
  }

}
