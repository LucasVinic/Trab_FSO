#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>

#define NUM_FILHOS 10

//signal handler dos filhos(o print eh mais pra teste)
void killSelf(int sig) {
  int pid = getpid();
  printf("sou o processo filho, pid = %d,e vou morrer\n",pid);
  exit(0);
}

int main() {

  char okMsg[] = "tudo ok, pai!";
  char messageFromChildBuffer[100];

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

    exit(0);
  }

  // só executa esse código se é pai
  if (isParent) {

    // close all write pipes
    for (int i = 0; i < NUM_FILHOS; i++) {
      close(pipeFilhoTerminou[i][0]);
    }

    // read message from all children
    for (int i = 0; i < NUM_FILHOS; i++) {
      int bytesRead = read(pipeFilhoTerminou[i][1], messageFromChildBuffer, 5);
      if (bytesRead == -1) {
        printf("error when reading from pipe of child #%d\n", i);
        exit(-1);
      }
      printf("i read stuff from my child #%d. i read %d bytes, and the message was '%s'\n", i, bytesRead, messageFromChildBuffer);
    }

    // close all read pipes
    for (int i = 0; i < NUM_FILHOS; i++) {
      close(pipeFilhoTerminou[i][1]);
    }


    // wait for all children to stop before exiting
    // for()
    // waitpid()


    // morre
    return 0;
  }

}
