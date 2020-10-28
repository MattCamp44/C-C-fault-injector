#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>
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
    unsigned int peeked_istr = ptrace(PTRACE_PEEKDATA,pid,(void *)regs.rip,0);
    printf("istruction [%d] \n RIP : 0x%08llx \n istruction : 0x%08x \n rax : %08llx \n",istr,regs.rip,peeked_istr,regs.rax);
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
