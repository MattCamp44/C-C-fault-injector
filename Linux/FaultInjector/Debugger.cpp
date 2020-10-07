// classe che fa da debugger 
#include "Debugger.h"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

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
                printf("i'm the child");
                execl(progName,progName,nullptr);
            }else{
                printf("i'm the father");
            }
            return 0;
};