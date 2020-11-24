#include<iostream>
#include<vector>
#include "./FunctionObject/FunctionObject.h"
#include <unistd.h> 
#include "./Extractor/Extractor.h"
#include <sys/personality.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

using namespace std;





int main(int argc, char ** argv){


    if(argc != 2){

        std::cout << "I need the debugee (compiled with -g)\n";
        return 1;

    }
    
    int pid;

    if(pid = fork()){
        //parent
        //vector<FunctionObject> extractObjects(pid);
        vector<FunctionObject> functionObjects = extractObjects(pid,argv[1]);

        for(auto a : functionObjects){
        cout << a.getname() << " " << a.getlinkagename() << " "  << endl;
        for(auto b : a.getaddresses())
            cout << b <<  endl;
        }
        waitpid(pid,nullptr,0);
    }
             

    
    else{
        //child
        //personality(ADDR_NO_RANDOMIZE);
        ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
        execl(argv[1],argv[1],nullptr);
        

    }


    





}



