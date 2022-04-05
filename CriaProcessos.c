#include<stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include<signal.h>

#define NUM_FILHOS 10

//signal handler dos filhos(o print eh mais pra teste)
void killTheChild(int sig){
    int pid = getpid();
    printf("sou o processo filho, pid = %d,e vou morrer\n",pid);
    
    exit(0);
}
int main() {

  bool isParent = true;
  int pids[10]; //lista de pids

 
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
    printf("sou o processo filho, pid = %d\n",pid);
    // espera o ok do pai pra morrer:
    while(1){
      sleep(1);  
      signal(SIGUSR1, killTheChild);
    }   
    
    exit(1);
  }

  
    
    
    // só executa esse código se é pai
  if(isParent){
    //sleep(1);
    // manda mensagem pra filhos morrerem:
    for (int i = 0; i < NUM_FILHOS; i++){
    
      printf("it returns this: %d",kill(pids[i],SIGUSR1));

    }
    // esperar o ok dos 10 filhos (wait n conta creio eu que teremos que mandar msg do filhos pro pai de outra maneira)
    for (int i = 0; i < NUM_FILHOS; i++){
        int num = wait(NULL);
        printf("waited %d\n",num);
    }
    
    // morre
    return 0;
  }
  
}  
    
    

