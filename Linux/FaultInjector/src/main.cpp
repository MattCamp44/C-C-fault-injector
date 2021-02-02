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
#include<experimental/filesystem>

using namespace std;





int main(int argc, char ** argv){


    if(argc < 2){

        std::cout << "I need the debugee (compiled with -g)\n";
        return 1;

    }
    
    // char * const  argumentsVector[20];
    // if(argc >= 2){
    //     for(auto i = 1; i < argc; i++)
    //         argumentsVector[i] = argv[i];
    // }

    char * newargv[20];

    for( auto i=1; i < argc; i++ ){
        newargv[i-1] = argv[i];
    }

    setbuf(stdout, 0);
    int pid;

    if(pid = fork()){
        //parent
        

        
        waitpid(pid,nullptr,0);

        //sleep(1);
        
        // filesystem::exists("helloworld.txt");

        vector<FunctionObject> functionObjects = extractObjects(pid,argv[1]);



        // for(auto func: functionObjects){
        //     cout << func.getname() << endl;
        //     for(auto addr : func.getaddresses())
        //         cout << addr << endl;
        // }
        ptrace(PTRACE_CONT, pid, nullptr, nullptr);
        

        //Wait for the golden run to finish
        //Get offsets, timer etc.
        waitpid(pid,nullptr,0);

        int NinjectionsPerAddress = 2;

        Debugger(functionObjects , argv[1], NinjectionsPerAddress);

        
        
        //TODO: remove goldenoutput and injectedoutput files after comparing them
            remove("goldenoutput.txt");

        
        

    }
             

    
    else{
        //child
        //personality(ADDR_NO_RANDOMIZE);
        //pause();
        FILE * fp;
        freopen("goldenoutput.txt", "w", stdout);
    
        ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
        execl(argv[1],argv[1],nullptr);
        // execv(argv[1],newargv);

    }


    





}



