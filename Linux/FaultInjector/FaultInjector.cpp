#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/ptrace.h>
#include <sys/wait.h>


class Controller{
    // prende file eseguibile
// fa objdump per indirizzi
// crea pipe per raccogliere output
// comunica al Debugger dove metterre i BP e dove eseguire gli injecto e quanti
// prende output dal debugger 
// rilancia il debugger il numero di volte necessario
private :
    //Debugger dbg;

public:
Controller(){
    //dbg = Debugger();
};
void start(){
    fprintf(stdout,"Controller start \n");
    
    //dbg.start();
    return;
};
};

class Debugger{
// classe che fa da debugger e injetta gli errori
private:
    int pid;
    char * progName;
public:
Debugger(){
    this->pid = 0;
    progName = nullptr;
};
void start(){
    fprintf(stdout,"Debugger start \n");
    int pid = 0;
    pid = fork();
    if(pid == 0){
        printf("child \n");
        ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
        execl("Debugee1","Debugee1",nullptr);

    }else{
        int status;
        printf("father \n");
        wait(&status);
        int istr = 0;
        while(1){
                        
            istr++;
            if(ptrace(PTRACE_SINGLESTEP,pid,nullptr,nullptr)){
                perror("ptrace");
                break;
            }
            wait(&status);
            if(WIFEXITED(status)){
                printf("process exited \n");
                break;
            }
        }
        printf("tot istructions %d\n",istr);
    }

    return;
};
};

int main(){

    printf("Start \n");
    Debugger dbg;
    dbg.start();
    printf("Stop \n");
}