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
            int nistr = 0;
            pid = fork();

            if(pid < 0) {
                return -1;
            }

            if(pid == 0){
                // indirizzare stdout e stderr al file di uscita
                printf("my pid %d \n",getpid());
                //traceme
                printf("i'm the child \n");
                if(ptrace(PTRACE_TRACEME,0,nullptr,nullptr) < 0){
                    printf("ptrace error");
                    return 1;
                }
                execl("Debugee1","",nullptr);

            }else{
                printf("i'm the father of PID : %d \n",pid);
                
                int option = 0;
                int statusCode;
                int waitcode = waitpid(pid,&statusCode,option);
                

                if(waitcode == -1){
                    printf("error in waitpid");
                    return 0;
                }

                while(WIFSTOPPED(statusCode)){
                    nistr++;
                    if (WIFEXITED(statusCode)) {
                    printf("process exited, status=%d\n", WEXITSTATUS(statusCode));
                } else if (WIFCONTINUED(statusCode)) {
                    printf("continued\n");
                } else if(WIFSTOPPED(statusCode)){
                    printf("stopped \n");
                } else if (WIFSIGNALED(statusCode)){
                    printf("signaled");
                } 
                
                if (ptrace(PTRACE_CONT, pid, nullptr, nullptr) < 0) {
                    perror("ptrace CONT  \n");
                    sleep(5);
                    //break;
                    if(ptrace(PTRACE_ATTACH,pid,nullptr,nullptr) < 0){
                        perror("Attach failed :");
                        break;
                    }
                  }
                }
                
                wait(&statusCode);
                
                
            }
            printf("eseguite %d istruzioni \n",nistr);
            return 0;
};