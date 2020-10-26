#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <vector>
#include <iostream>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/user.h>

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
            if(istr % 1000 == 0  ) readRegs(istr); // 1/1000 delle volte
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
void readRegs(int istr){
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS,pid,0,&regs);
    unsigned long peeked_istr = ptrace(PTRACE_PEEKTEXT,pid,regs.rip,0);
    printf("istruction [%d] \n RIP : 0x%lld \n istruction : 0x%ld \n rax : %lld \n",istr,regs.rip,peeked_istr,regs.rax);
    return;
};
};

class Controller{
    // prende file eseguibile
// fa objdump per indirizzi
// crea pipe per raccogliere output
// comunica al Debugger dove metterre i BP e dove eseguire gli injecto e quanti
// prende output dal debugger 
// rilancia il debugger il numero di volte necessario
private :
    //Debugger dbg;
    char * progName;

public:
Controller(){
    //dbg = Debugger();
    progName = (char *) "Debugee1";
};
void start(){
    fprintf(stdout,"Controller start \n");
    
    Debugger dbg;

    dbg.start();
    
    return;
};
};

int main(){



    printf("Start \n");

    Controller controller;

    controller.start();

    printf("Stop \n");
}
