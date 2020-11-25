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

        struct iovec local[2];
        struct iovec remote[1];
        char buf1[10];
        char buf2[10];
        local[0].iov_base = buf1;
        local[0].iov_len = 10;
        local[1].iov_base = buf2;
        local[1].iov_len = 10;
        vector<unsigned long int> addrs = functionObjects[0].getaddresses();
        remote[0].iov_base = (void *)addrs[0] ;
        remote[0].iov_len = 20;

        process_vm_readv(pid,local,2,remote,1,0);

        cout << buf1 << " " << buf2 << endl;

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



