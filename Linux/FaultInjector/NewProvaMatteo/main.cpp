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
    
    setbuf(stdout, 0);
    int pid;

    if(pid = fork()){
        //parent
        //ptrace(PTRACE_ATTACH,pid,nullptr,nullptr);
        //ptrace(PTRACE_ATTACH,pid,nullptr,nullptr);

        
        waitpid(pid,nullptr,0);

        //sleep(1);
        

        vector<FunctionObject> functionObjects = extractObjects(pid,argv[1]);

        for(auto a : functionObjects){
        cout << a.getname() << " " << a.getlinkagename() << " "  << endl;
        for(auto b : a.getaddresses())
            cout << b  << endl;
        }
        
        Debugger(pid, functionObjects);
        ptrace(PTRACE_CONT, pid, nullptr, nullptr);
        sleep(2);
        
        cout << "after Debugger\n"; 
        cout << "Single step" << endl;
        //ptrace(PTRACE_SINGLESTEP, pid, nullptr, nullptr);
        

    }
             

    
    else{
        //child
        //personality(ADDR_NO_RANDOMIZE);
        //pause();
    
        ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
        execl(argv[1],argv[1],nullptr);

    }


    





}



