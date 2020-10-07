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
                ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
                execl(progName,progName,nullptr);

            }else{
                printf("i'm the father \n");
                
                int option = WUNTRACED | WCONTINUED;
                int statusCode;
                int waitcode = waitpid(pid,&statusCode,option);

                if(waitcode == -1){
                    printf("error in waitpid");
                    return 0;
                }
                
                 if (WIFEXITED(statusCode)) {
                    printf("process exited, status=%d\n", WEXITSTATUS(statusCode));
                } else if (WIFCONTINUED(statusCode)) {
                    printf("continued\n");
                }

                printf("returned code : %d \n",statusCode);
            }
            return 0;
};