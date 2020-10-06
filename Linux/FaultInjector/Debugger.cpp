// classe che fa da debugger 
#include "./Debugger.h"
#include <unistd.h>
#include <stdlib.h>
/*
Debugger(){
        this->pid = 0;
    };
*/

int Debugger::start(char * progName){

            this.pid = fork();

            if(this->pid < 0) {
                return -1;
            }

            if(this->pid == 0){
                // indirizzare stdout e stderr al file di uscita

                //traceme
                execv(progName,progName,nullptr,nullptr);
            }
            return 0;
};