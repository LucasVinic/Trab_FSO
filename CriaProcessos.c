#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <sys/msg.h>

#define NUM_FILHOS 10

int children_not_yet_ready = NUM_FILHOS;
int myPid;

void recieved_ok_to_die(int signum) {
}


int main() {
  bool isParent = true;
  int pids[NUM_FILHOS]; // lista de pids
  int parentPid = myPid = getpid();
  int  idfila;

  struct mensagem
  {
    int pid;
    char msg[30];
  };

  struct mensagem mensagem_env, mensagem_rec; 

  /* cria */
  if ((idfila = msgget(0x1223, IPC_CREAT|0x1B6)) < 0)
  {
    printf("erro na criacao da fila\n");
    exit(1);
  } 
 
 
  // setup signal callback
  signal(SIGUSR2, recieved_ok_to_die);

  // cria 10 processos filhos
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
    // imprime
    printf("sou o processo filho, pid =  %d  \n", myPid);
    
    // avisa o pai
    mensagem_env.pid = myPid;
    strcpy(mensagem_env.msg, "dad I printed plz kill me \n");
    msgsnd(idfila, &mensagem_env, sizeof(mensagem_env)-sizeof(long), 0);

    

    // espera a notificacao do processo pai
    pause();

    //printf("#%d recieved the ok to go.\n", myPid);

    // termina execucao
    exit(0);
  }

  // só executa esse código se é pai
  if (isParent) {
    //espera o aviso dos filhos
    while (children_not_yet_ready > 0) {
      msgrcv(idfila, &mensagem_rec, sizeof(mensagem_rec)-sizeof(long), 0, 0);
      ////printf("mensagem recebida = %d %s\n", mensagem_rec.pid, mensagem_rec.msg);
      ////printf("i have %d children not yet ready\n", children_not_yet_ready);
      children_not_yet_ready--;
      
    }

    ////printf("i'm the parent, and all of my children did their things.\n");

    // send all children the ok to die
    for (int i = 0; i < NUM_FILHOS; i++) {
      kill(pids[i], SIGUSR2);
    }

    ////printf("now i'll wait until they're all dead before i can go.\n");
    
    // esperando filhos avisarem seus terminos
    for (int i = 0; i < NUM_FILHOS; i++) {
      int status;
      waitpid(pids[i], &status, 0);
      
    }

    // morre
    ////printf("all of my children have died, so now i'll end too.\n");
    return 0;
  }

}
