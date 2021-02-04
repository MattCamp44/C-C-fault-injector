#include "./Debugger.h"
#include "sys/ptrace.h"
#include "sys/wait.h"
#include<vector>
#include "../FunctionObject/FunctionObject.h"
#include "../AddressSelector/AddressSelector.h"
#include "../BreakPoint/BreakPoint.h"
#include "../InjectionPoint/InjectionPoint.h"
#include <assert.h> 
#include<sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include<stdio.h>
#include "../Output_functions/comparefiles.h"
#include<fstream>
#include "../InstructionObject/InstructionObject.h"



using namespace std;

void continue_execution(int pid) {
    ptrace(PTRACE_CONT, pid, nullptr, nullptr);

    int wait_status;
    auto options = 0;
    waitpid(pid, &wait_status, options);

}


struct params {
    int pid; // pid of Deguee program
    int goldenExTime; // time golden ex
    int molt; // moltiplicator (input)
};


void * resetThread(void * p){
    // cout << "[Thread] i'm the killer thread" << endl;
    struct params * para;
    para = (struct params *) p;
    int exit;   
    // cout << "thread starts sleeping for " << para->goldenExTime * para->molt <<   "...\n";
    // sleep(para->goldenExTime * para->molt);
    // sleep(1);
    // usleep(1000);
    // cout << "thread woke up\n";

    if(kill(para->pid, SIGKILL) == 0){
        // cout << "[Thread] now i kill the child process" << endl;
        exit = 1;
    }else{
        // cout << "[Thread] child process is immortal" << endl;
        exit = -1;
    }

    pthread_exit((void *) &exit);

}




// void EnableInjectionPoints(int pid, vector<unsigned long int> addresses ){

//     vector<InjectionPoint> InjectionPoints;

//     for(auto a : addresses)
//         InjectionPoints.emplace_back(InjectionPoint(pid,a));

//     for(auto i : InjectionPoints)
//         i.InjectFirstBit();


// }
void EnableInjectionPoint(int pid, InstructionObject address ){

    

    
    InjectionPoint injPoint = InjectionPoint(pid,address.getAddress(),address.getLength());

    
    injPoint.InjectFirstBit();


}


void Debugger(vector<FunctionObject> FunctionObjects, char * prog, int NinjectionsPerAddress){





    // vector<unsigned long int> addresses;

    // for(auto addr : FunctionObjects[0].getaddresses()){
    //     addresses.emplace_back(addr);
    // }

    int pid;
    
    

    int index=0;
    // cout << FunctionObjects[0].getaddresses().size() << endl;
    for(auto FunctionObject : FunctionObjects)
    for(auto i : FunctionObject.getaddresses()){
        for( auto j = 0; j < NinjectionsPerAddress; j++ ){
    // for(auto i : myaddresses) {
        // cout << "Injecting " << hex << i << endl;
        pid = fork();
        if(pid){
            //Parent
            //Parte il thread
            waitpid(pid,nullptr,0);
            
            

            
            //addresses = AddressSelector(FunctionObjects);
            //addresses[0] = i;
            //addresses[1] = i;
            
            // addresses.emplace_back(myaddressesinstruction[index++]);
            // addresses.emplace_back(i);

            // assert(!addresses.empty());
            
            // cout << "Here\n";
            
            //Pop head
            unsigned long int breakpointAddress = i.getAddress();
            // addresses.erase(addresses.begin());
            EnableInjectionPoint(pid,i);
            BreakPoint breakpoint(pid,breakpointAddress);
            
            breakpoint.Enable();

            // sleep(1);
            //Da qui il programma continua con il breakpoint inserito -> breakpoint e injection point sono inseriti in due momenti diversi damn
            ptrace(PTRACE_CONT, pid, nullptr, nullptr);



            waitpid(pid,nullptr,0);
            // cout << "Breakpoint here\n";
            //sleep(1);
            

            user_regs_struct regs;


            breakpoint.Release();

            ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

            //reset program counter
            regs.rip = breakpointAddress ;

            ptrace(PTRACE_SETREGS, pid, nullptr, &regs);

            // cout << "Before continuing..." << endl;
            // sleep(1);
            ptrace(PTRACE_CONT, pid, nullptr, nullptr);

            //To get from main..
            int goldenExecutionTime = 100;
            int maxTimerMultiplicator = 4;

            struct params para;
            para.goldenExTime = 1;
            para.molt = 1;
            para.pid = pid;

            pthread_t t1;
            // cout << "Starting thread\n";
            if(pthread_create(&t1,NULL,resetThread,(void *) &para ) != 0){
                cout << "[Parent] error creating the thread, exit.." << endl;
                return ; //return error code
            }

            // Wait for program to exit
            // sleep(3);

            int status;

            int waitPidReturn = waitpid(pid,&status,0);
            // if(WIFSIGNALED(status)) cout << "Killed by signal " << WTERMSIG(status) << endl;
            // cout << "Waitpid return first time: " << waitPidReturn << endl;
            //Why two waitpid??
            

            //kill thread 
            // pthread_join(t1,NULL);

            
            pthread_cancel(t1); 

            errno = 0;


            // if(kill(pid, SIGKILL) == 0 ){

            //     cout << "process was not killed..." << endl; 


            // }
            // else if(errno == ESRCH) 
            // cout << "Process was already killed...\n"; 


            // waitPidReturn = waitpid(pid,nullptr,0);
            // cout << "Waitpid return second time: " << waitPidReturn << endl;

            errno = 0;


            

            // ptrace(PTRACE_CONT, pid, nullptr, nullptr);

            // waitpid(pid,nullptr,0);
            // ptrace(PTRACE_CONT, pid, nullptr, nullptr);

            //waitpid(pid,nullptr,0);
            //cout << "Program counter after continue: " << regs.rip << endl;
            //waitpid(pid,nullptr,0);
            //for(auto i : FunctionObjects[0].getaddresses())
                //cout << i << " : " <<ptrace(PTRACE_PEEKDATA, pid, i, nullptr) << endl;

            //continue_execution(pid);
            // addresses.erase(addresses.begin());

            //Compare the two files

            //popen("diff goldenoutput.txt injectedoutput.txt")
            // ....read output and put on csvfile
            // fstream objdumpfile;
            // objdumpfile.open("./Extractor/ObjectFiles/prova/objdump",ios::in);
            // fstream fgoldenoutput;
            // fgoldenoutput.open("../goldenoutput.txt",ios::in);            
            // fstream finjectedoutput;
            // finjectedoutput.open("./injectedoutput.txt",ios::in);

            // cout << fgoldenoutput << endl;
            // cout << finjectedoutput << endl;

            compareFiles();

            int errorGenerated = 0;

            ifstream ifile;
            ifile.open("injectedoutputstderr.txt");
            // If file exists and it is not empty -> error
            if(ifile && ifile.peek() != std::ifstream::traits_type::eof()) {
                cout << ifile.peek() ;
                errorGenerated = 1;
                cout<<"Errors\n";
            } else {
                // cout << ifile.peek() ;
                // cout<<"No errors\n";
            }
            ifile.close();
            //appendLineOutputFile(  );

            remove("injectedoutput.txt");
            remove("injectedoutputstderr.txt");
            // cout << "Done injecting\n";




        }

        else{
            //child
            freopen("injectedoutput.txt", "w", stdout);
            // freopen("injectedoutput.txt", "w", stderr);
            freopen("injectedoutputstderr.txt", "w", stderr);
            ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
            execl(prog,prog,nullptr);
            // execv(argv[1],newargv);

        }
    }

} //for

    return;

}



















