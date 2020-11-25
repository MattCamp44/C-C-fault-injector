#include<iostream>
#include<vector>
#include "./FunctionObject/FunctionObject.h"
#include <unistd.h> 
#include "./Extractor/Extractor.h"
#include <sys/personality.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include "./Debugger/Debugger.h"
#include <sys/uio.h>
using namespace std;





int main(int argc, char ** argv){


    if(argc != 2){

        std::cout << "I need the debugee (compiled with -g)\n";
        return 1;

    }
    
    int pid;

    if(pid = fork()){
        //parent
        
        //ptrace(PTRACE_ATTACH,pid,nullptr,nullptr);

        

        

        vector<FunctionObject> functionObjects = extractObjects(pid,argv[1]);

        
        Debugger(pid, functionObjects);
        
        cout << "after Debugger\n"; 


        waitpid(pid,nullptr,0);

    }
             

    
    else{
        //child
        //personality(ADDR_NO_RANDOMIZE);
        ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
        execl(argv[1],argv[1],nullptr);
        

    }


    





}



