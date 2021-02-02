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
#include <libexplain/ptrace.h>
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
        
        vector<FunctionObject> functionObjects = extractObjects(pid,argv[1]);
        // filesystem::exists("helloworld.txt");
        long long realaddress = static_cast<long long>(functionObjects[0].getaddresses()[0]);
        int64_t data = ptrace(PTRACE_PEEKTEXT, pid, realaddress,0);
        cout << data << endl;
        cout << errno << endl;
        //fprintf(stderr, "%s\n", explain_ptrace(PTRACE_PEEKTEXT, pid,realaddress,));
        if(data == EBUSY)
            cout << "Ebusy\n";
        if(data == EFAULT)
            cout << "EFAULT\n" ;
        if(data == EINVAL)
            cout << "EINVAL\n";
        return 1;

        for(auto func: functionObjects){
            cout << func.getname() << endl;
            for(auto addr : func.getaddresses())
                cout << hex << addr << endl;
        }

        cout << "Here\n" ;
        // return 1;
        int ptraceContReturnValue = ptrace(PTRACE_CONT, pid, nullptr, nullptr);
        cout << "ptraceContReturnValue: " << ptraceContReturnValue << endl;

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
        // freopen("goldenoutput.txt", "w", stdout);
    
        ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
        execl(argv[1],argv[1],nullptr);
        // execv(argv[1],newargv);

    }


    





}



