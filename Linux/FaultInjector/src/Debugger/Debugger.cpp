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
    sleep(para->goldenExTime * para->molt);

    if(kill(para->pid, SIGKILL) == 0){
        cout << "[Thread] now i kill the child process" << endl;
        exit = 1;
    }else{
        cout << "[Thread] child process is immortal" << endl;
        exit = -1;
    }

    pthread_exit((void *) &exit);

}




void EnableInjectionPoints(int pid, vector<unsigned long int> addresses ){

    vector<InjectionPoint> InjectionPoints;

    for(auto a : addresses)
        InjectionPoints.emplace_back(InjectionPoint(pid,a));

    for(auto i : InjectionPoints)
        i.InjectFirstBit();


}
void EnableInjectionPoint(int pid, unsigned long int address ){

    

    
    InjectionPoint injPoint = InjectionPoint(pid,address);

    
    injPoint.InjectFirstBit();


}


void Debugger(vector<FunctionObject> FunctionObjects, char * prog, int NinjectionsPerAddress){





    vector<unsigned long int> addresses;

    for(auto addr : FunctionObjects[0].getaddresses()){
        addresses.emplace_back(addr);
    }

    int pid;
    
    

    int index=0;
    cout << FunctionObjects[0].getaddresses().size() << endl;
    for(auto i : FunctionObjects[0].getaddresses()){
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
            unsigned long int breakpointAddress = i;
            // addresses.erase(addresses.begin());
            EnableInjectionPoint(pid,i);
            BreakPoint breakpoint(pid,breakpointAddress);
            
            breakpoint.Enable();

            sleep(1);
            //Da qui il programma continua con il breakpoint inserito -> breakpoint e injection point sono inseriti in due momenti diversi damn
            ptrace(PTRACE_CONT, pid, nullptr, nullptr);



            waitpid(pid,nullptr,0);
            
            //sleep(1);
            

            user_regs_struct regs;


            breakpoint.Release();

            ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

            //reset program counter
            regs.rip = breakpointAddress ;

            ptrace(PTRACE_SETREGS, pid, nullptr, &regs);

            
            ptrace(PTRACE_CONT, pid, nullptr, nullptr);

            //To get from main..
            int goldenExecutionTime = 100;
            int maxTimerMultiplicator = 4;

            struct params para;
            para.goldenExTime = 3;
            para.molt = 1;
            para.pid = pid;

            pthread_t t1;

            if(pthread_create(&t1,NULL,resetThread,(void *) &para ) != 0){
                cout << "[Parent] error creating the thread, exit.." << endl;
                return ;
            }

            // Wait for program to exit
            waitpid(pid,nullptr,0);

            //kill thread 
            pthread_cancel(t1);

            ptrace(PTRACE_CONT, pid, nullptr, nullptr);

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
            if(ifile && ifile.peek() != std::ifstream::traits_type::eof()) {
                cout << ifile.peek() ;
                errorGenerated = 1;
                cout<<"Errors\n";
            } else {
                cout << ifile.peek() ;
                cout<<"No errors\n";
            }

            //appendLineOutputFile(  );

            remove("injectedoutput.txt");
            remove("injectedoutputstderr.txt");
            cout << "Done injecting\n";




        }

        else{
            //child
            // freopen("injectedoutput.txt", "w", stdout);
            // freopen("injectedoutput.txt", "w", stderr);
            // freopen("injectedoutputstderr.txt", "w", stderr);
            ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
            execl(prog,prog,nullptr);
            // execv(argv[1],newargv);

        }
    }

} //for

    return;

}



















