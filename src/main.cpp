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
#include "string.h"
#include "stdlib.h"
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
    
    for(int j = 0; j < 20; j++)
        newargv[j] = (char *) malloc( 1080*sizeof(char) );

    if(argc > 2){

        for(int iterator = 2; iterator < argc - 2 ; iterator++ )
            strcpy(newargv[iterator-2], argv[iterator]);
        newargv[argc -1] = NULL;
    }

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
        // return 1;
        int NinjectionsPerAddress = 2;

        Debugger(functionObjects , argv[1], NinjectionsPerAddress,glodenExecutionTime,newargv);

        
        
        //TODO: remove goldenoutput and injectedoutput files after comparing them
        remove("goldenoutput.txt");

        cout << endl;
        
        

    }
             

    
    else{
        //child
        //personality(ADDR_NO_RANDOMIZE);
        //pause();
        FILE * fp;
        freopen("goldenoutput.txt", "w", stdout);
    
        ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
        // execl(argv[1],argv[1],nullptr);
        execv(argv[1],newargv);
        
        pritnf("Execv failed\n");
        exit(1);
    }


    





}



