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
#include "./InstructionObject/InstructionObject.h"
#include <time.h>

using namespace std;





int main(int argc, char ** argv){


    if(argc < 2){

        std::cout << "I need the debugee (compiled with -g) (and eventually its arguments )\n";
        return 1;

    }
    
    // char * const  argumentsVector[20];
    // if(argc >= 2){
    //     for(auto i = 1; i < argc; i++)
    //         argumentsVector[i] = argv[i];
    // }
    char * newargv[20];
    if(argc > 2)
        for(int iterator = 1; iterator < argc ; iterator++ )
            newargv[iterator-1] = argv[iterator];
    

    setbuf(stdout, 0);
    int pid;

    if(pid = fork()){
        //parent
        clock_t tStart = clock();

        
        waitpid(pid,nullptr,0);

        //sleep(1);
        
        vector<FunctionObject> functionObjects = extractObjects(pid,argv[1]);
        // filesystem::exists("helloworld.txt");
        // return 1;
        
        // return 1;
        // for(auto func: functionObjects){
        //     cout << func.getname() << endl;
        //     for(auto addr : func.getaddresses())
        //         cout << hex << addr.getAddress() << endl;
        // }
        // return 1;
        // cout << "Here\n" ;
        // return 1;
        int ptraceContReturnValue = ptrace(PTRACE_CONT, pid, nullptr, nullptr);
        // cout << "ptraceContReturnValue: " << ptraceContReturnValue << endl;

        //Wait for the golden run to finish
        //Get offsets, timer etc.
        waitpid(pid,nullptr,0);
        cout << "After waitpid\n";
        double glodenExecutionTime = (double)(clock() -tStart);

        int NinjectionsPerAddress = 2;

        Debugger(functionObjects , argv[1], NinjectionsPerAddress,glodenExecutionTime, newargv);

        
        
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



