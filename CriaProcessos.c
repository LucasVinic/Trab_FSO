#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

#define NUM_FILHOS 10

//signal handler dos filhos(o print eh mais pra teste)
void killTheChild(int sig){
    int pid = getpid();
    printf("sou o processo filho, pid = %d, e vou morrer\n", pid);
    
    exit(0);
}

int main() {

  bool isParent = true;
  int pids[10]; //lista de pids
  int ppid = getpid();

  for (int i = 0; i < NUM_FILHOS; i++) {
    int temp = fork();
    if (temp == 0) {
      isParent = false;  
      break;
    }
    else{
        pids[i] = temp;
    }
  }
   
  // só executa esse código se é filho
  if (!isParent) {
    int pid = getpid();

    // printar
    printf("sou o processo filho, pid = %d. Meu pai tem id = %d\n", pid, ppid);

    
    // avisa o pai que já imprimiu
    kill(ppid, SIGUSR1);

    // espera o ok do pai pra morrer:
    // signal(SIGUSR2, killTheChild);
    
    wait(NULL);

    exit(1);
  }

  // só executa esse código se é pai
  if(isParent){

    /* printf("processo pai tem id de %d\n", ppid);
    // espera 10 sinais dos filhos
    signal(SIGUSR1, checkTheChilds);
    while(1){
      sleep(0.5);
      if(counter == 10)
        break;
    }
    printf("O pai recebeu aviso de todos os filhos\n"); */

    // // manda mensagem pra filhos morrerem:
    // for (int i = 0; i < NUM_FILHOS; i++){
    //   if (kill(pids[i], SIGUSR2) == 0)
    //     printf("vou mandar o filho %d fazer a coisa dele\n", pids[i]);
    //   else
    //     printf("tentei mandar %d morrer mas não consegui\n", pids[i]);
    // }

    // esperar o ok dos 10 filhos (wait n conta creio eu que teremos que mandar msg do filhos pro pai de outra maneira)
    for (int i = 0; i < NUM_FILHOS; i++){
      int num = wait(NULL);
      printf("waited %d\n",num);
    }

    for (int i = 0; i < NUM_FILHOS; i++) {
      kill(pids[i], SIGUSR2);
    }

    // morre
    return 0;
  }
}  
    
    

