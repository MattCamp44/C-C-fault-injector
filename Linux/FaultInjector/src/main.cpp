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
        

        
        waitpid(pid,nullptr,0);

        //sleep(1);
        
        vector<FunctionObject> functionObjects = extractObjects(pid,argv[1]);



        for(auto func: functionObjects){
            cout << func.getname() << endl;
            for(auto addr : func.getaddresses())
                cout << addr << endl;
        }
        ptrace(PTRACE_CONT, pid, nullptr, nullptr);
        

        //Wait for the golden run to finish
        //Get offsets, timer etc.
        waitpid(pid,nullptr,0);



        Debugger(functionObjects , argv[1]);

        
        
        //TODO: remove goldenoutput and injectedoutput files after comparing them
            // remove("goldenoutput.txt");

        
        

    }
             

    
    else{
        //child
        //personality(ADDR_NO_RANDOMIZE);
        //pause();
        FILE * fp;
        freopen("goldenoutput.txt", "w", stdout);
    
        ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
        execl(argv[1],argv[1],nullptr);

    }


    





}



