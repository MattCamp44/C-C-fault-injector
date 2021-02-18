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
#include <fstream>

using namespace std;





int main(int argc, char ** argv){


    char * newargv[20];
    
    for(int j = 0; j < 20; j++)
        newargv[j] = (char *) malloc( 1080*sizeof(char) );

    
    if(argc < 2){

        std::cout << "I need the debugee (compiled with -g) (and eventually its arguments )\n";
        return 1;
        

    }
    
    // char * const  argumentsVector[20];
    // if(argc >= 2){
    //     for(auto i = 1; i < argc; i++)
    //         argumentsVector[i] = argv[i];
    // }
    
    if(argc == 2){
        newargv[0] = argv[1];
        newargv[1] = NULL;

        // cout << newargv[0] << "," << newargv[1] << endl;
        // return 1;
    }


    if(argc > 2){

        for(int iterator = 1; iterator < argc - 1 ; iterator++ )
            strcpy(newargv[iterator-1], argv[iterator]);
        newargv[argc -1] = NULL;
    }

    vector<int> inputParameters;
    fstream inputFile;
    inputFile.open("inputfile",ios::in);

    if (inputFile.is_open()){   
      string tp;
      while(getline(inputFile, tp)){  
        inputParameters.emplace_back(stoi(tp));
      }
      inputFile.close(); 

    }

    // if(inputParameters.size() < 1){

    //     cout << "Not enough parameters in inputfile\n";
    //     return 1;

    // }
    if(inputParameters.size() != 2){

        cout << "Not enough parameters in inputfile\n";
        return 1;

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
        int NinjectionsPerAddress = inputParameters[0];
        int goldenTimeMultiplicator = inputParameters[1];

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
        
        cout << "execv failed\n"; 
        exit(1);
    }


    





}



