#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <vector>
#include <iostream>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/types.h>

class Debugger{
// classe che fa da debugger e injetta gli errori
private:
    int pid;
    char * progName;
    int64_t WhereAddress = 0 ; // punto nel main

public:
Debugger(){
    this->pid = 0;
    progName = nullptr;
    WhereAddress = 0x0000000000001169 ; // punto nel main
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
        struct user_regs_struct regs;
        
        printf("father \n");
        wait(&status);
        int istr = 0;

        // INSERT BP
        ptrace(PTRACE_GETREGS,pid,0,&regs);
        // take the istructiono ad WhereAddress
        unsigned data = ptrace(PTRACE_PEEKTEXT,pid,(void *) WhereAddress,0);
        printf("IP reg : %d \n",regs.rip);
        printf("the original istruction is 0x%081x \n now writing 0xCC istruction \n",data);
        unsigned data_bp = (data & 0xFFFFFFFFFFFFFF00) | 0xCC; // only the last byte containt meaningfull istruction

        ptrace(PTRACE_POKETEXT,pid,(void *)WhereAddress,(void *)data_bp);


        ptrace(PTRACE_CONT,pid,0,0);


        while(1){
                        
            istr++;

            /*
            if(ptrace(PTRACE_SINGLESTEP,pid,nullptr,nullptr)){
                perror("ptrace");
                break;
            }
            */

            wait(&status);
            if(WIFEXITED(status)){
                printf("process exited \n");
                break;
            }
            if(WIFSTOPPED(status)){
                printf("process stopped\n");
                break;
            }
            ptrace(PTRACE_GETREGS,pid,0,regs);
            printf("child IP : 0x%081llx \n",regs.rip);
            ptrace(PTRACE_POKETEXT,pid,(void*)WhereAddress,(void *)data);
            printf("Process istruction restored \n now decrese IP \n");
            regs.rip -= 1;
            ptrace(PTRACE_SETREGS,pid,0,&regs);
            ptrace(PTRACE_CONT,pid,0,0);
        }
        printf("tot istructions %d\n",istr);
    }

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
