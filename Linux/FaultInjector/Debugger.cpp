// classe che fa da debugger 
#include "Debugger.h"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/ptrace.h>
#include <sys/wait.h>

Debugger::Debugger(){
        pid = 0;
};


int Debugger::start(char * progName){

            pid = fork();

            if(pid < 0) {
                return -1;
            }

            if(pid == 0){
                // indirizzare stdout e stderr al file di uscita

                //traceme
                printf("i'm the child \n");
                if(ptrace(PTRACE_TRACEME,0,nullptr,nullptr) < 0){
                    printf("ptrace error");
                    return 1;
                }
                execl("Debugee1","Debugee1",nullptr);

            }else{
                printf("i'm the father \n");
                
                int option = 0;
                int statusCode;
                int waitcode = waitpid(pid,&statusCode,option);

                if(waitcode == -1){
                    printf("error in waitpid");
                    return 0;
                }
                while(WIFSTOPPED(statusCode)){
                    if (WIFEXITED(statusCode)) {
                    printf("process exited, status=%d\n", WEXITSTATUS(statusCode));
                } else if (WIFCONTINUED(statusCode)) {
                    printf("continued\n");
                } else if(WIFSTOPPED(statusCode)){
                    printf("stopped \n");
                }
                printf("single step\n");
                if (ptrace(PTRACE_SINGLESTEP, pid, 0, 0) < 0) {
                    perror("ptrace error single step \n");
                     return 1;
                  }
                }
                wait(nullptr);
                printf("back to single step \n");
                
            }
            return 0;
};