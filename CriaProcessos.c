#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

#define NUM_FILHOS 10

//signal handler dos filhos(o print eh mais pra teste)
void killSelf(int sig) {
  int pid = getpid();
  printf("sou o processo filho, pid = %d,e vou morrer\n",pid);
  exit(0);
}

int main() {

  char okMsg[] = "tudo ok, pai!";

  bool isParent = true;
  int pids[NUM_FILHOS]; //lista de pids
  int parentPid = getpid();
  // array de pipes para cada filho falar que terminou
  int pipeFilhoTerminou[NUM_FILHOS][2];
  for (int i = 0; i < NUM_FILHOS; i++) {
    if (pipe(pipeFilhoTerminou[i]) == -1) {
      printf("error when creating pipe\n");
      exit(-1);
    }
    printf("pipe %d: read: %d write: %d\n", i, pipeFilhoTerminou[i][0], pipeFilhoTerminou[i][1]);
  }

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
    int myPid = getpid();
    int myIndex;
    // close all pipes this child isn't going to use
    for (int i = 0; i < NUM_FILHOS; i++) {
      // if it's another child's pipe
      if (myPid != pids[i]) {
        close(pipeFilhoTerminou[i][0]);
        close(pipeFilhoTerminou[i][1]);
      // if it's the child's pipe, close the read, keep the write open
      } else {
        myIndex = i;
        close(pipeFilhoTerminou[i][1]);
      }
    }



    printf("hi, im #%d. and i'm sending '%s' to my daddy\n", myIndex, okMsg);
    write(pipeFilhoTerminou[myIndex][0], okMsg, strlen(okMsg)+1);
    close(pipeFilhoTerminou[myIndex][0]);



    // int pid = getpid();
    // // printar
    // // printf("sou o processo filho, pid = %d. meu pai eh %d\n", pid, parentPid);
    // // espera o ok do pai pra morrer:
    // while (1) {
    //   sleep(1);
    //   signal(SIGUSR1, killSelf);
    // }
    // exit(1);
  }

  // só executa esse código se é pai
  if (isParent) {

    // close all write pipes
    for (int i = 0; i < NUM_FILHOS; i++) {
      close(pipeFilhoTerminou[i][0]);
    }

    // read message from all children
    for (int i = 0; i < NUM_FILHOS; i++) {
      char* messageFromChild;
      int bytesRead = read(pipeFilhoTerminou[i][1], messageFromChild, sizeof(okMsg));
      printf("i read stuff from my child #%d. it was %s\n", i, messageFromChild);
    }

    // close all read pipes
    for (int i = 0; i < NUM_FILHOS; i++) {
      close(pipeFilhoTerminou[i][1]);
    }






    // //sleep(1);
    // // manda mensagem pra filhos morrerem:
    // for (int i = 0; i < NUM_FILHOS; i++) {
    //   // printf("it returns this: %d\n", kill(pids[i],SIGUSR1));
    //   kill(pids[i],SIGUSR1);
    // }
    // // esperar o ok dos 10 filhos (wait n conta creio eu que teremos que mandar msg do filhos pro pai de outra maneira)
    // for (int i = 0; i < NUM_FILHOS; i++) {
    //   int num = wait(NULL);
    //   // printf("waited %d\n",num);
    // }

    // morre
    return 0;
  }

}
