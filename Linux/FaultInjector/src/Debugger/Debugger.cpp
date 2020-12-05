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
using namespace std;

void continue_execution(int pid) {
    ptrace(PTRACE_CONT, pid, nullptr, nullptr);

    int wait_status;
    auto options = 0;
    waitpid(pid, &wait_status, options);

}






void EnableInjectionPoints(int pid, vector<unsigned long int> addresses ){

    vector<InjectionPoint> InjectionPoints;

    for(auto a : addresses)
        InjectionPoints.emplace_back(InjectionPoint(pid,a));

    for(auto i : InjectionPoints)
        i.InjectFirstBit();


}


void Debugger(vector<FunctionObject> FunctionObjects, char * prog){


    for(auto f : FunctionObjects){
        cout << f.getname() << ":\n";
        for(auto a : f.getaddresses())
            cout << a << "\n";
    }


    vector<unsigned long int> addresses;

    int pid;
    
    vector<unsigned long int> myaddresses;
    //Check the campaign is working by modifying different lines in each run
    myaddresses.emplace_back(static_cast<unsigned long int>(0x4005f4));
    myaddresses.emplace_back(static_cast<unsigned long int>(0x4005f8));
    myaddresses.emplace_back(static_cast<unsigned long int>(0x4005fc));
    myaddresses.emplace_back(static_cast<unsigned long int>(0x400602));
    myaddresses.emplace_back(static_cast<unsigned long int>(0x400602));
    myaddresses.emplace_back(static_cast<unsigned long int>(0x400607));
    myaddresses.emplace_back(static_cast<unsigned long int>(0x40060c));


    //for(auto i : FunctionObjects[0].getaddresses()){
    for(auto i : myaddresses) {
        pid = fork();
        if(pid){
            //Parent
            waitpid(pid,nullptr,0);
            cout << "Segfault check lol\n"; 

            for(auto n : addresses)
                cout << "Addresses: " << n << endl; 

            
            //addresses = AddressSelector(FunctionObjects);
            //addresses[0] = i;
            //addresses[1] = i;

            addresses.emplace_back(i);
            addresses.emplace_back(i);

            assert(!addresses.empty());
            
            
            //Pop head
            unsigned long int breakpointAddress = addresses[0];
            //addresses.erase(addresses.begin());
            BreakPoint breakpoint(pid,breakpointAddress);
            
            breakpoint.Enable();
            
            //Da qui il programma continua con il breakpoint inserito -> breakpoint e injection point sono inseriti in due momenti diversi damn
            ptrace(PTRACE_CONT, pid, nullptr, nullptr);



            waitpid(pid,nullptr,0);
            
            //sleep(1);
            
            //EnableInjectionPoints(pid,addresses);

            user_regs_struct regs;


            breakpoint.Release();

            ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

            //regs.rip = 0x400543 ;
            regs.rip = breakpointAddress ;

            ptrace(PTRACE_SETREGS, pid, nullptr, &regs);

            //cout << "Program counter before singlestep: " << regs.rip << endl;
            sleep(1);
            ptrace(PTRACE_CONT, pid, nullptr, nullptr);

            //cout << "Program counter after singlestep: " << regs.rip << endl;


            waitpid(pid,nullptr,0);

            ptrace(PTRACE_CONT, pid, nullptr, nullptr);

            waitpid(pid,nullptr,0);

            ptrace(PTRACE_CONT, pid, nullptr, nullptr);

            waitpid(pid,nullptr,0);
            //cout << "Program counter after continue: " << regs.rip << endl;
            //waitpid(pid,nullptr,0);
            //for(auto i : FunctionObjects[0].getaddresses())
                //cout << i << " : " <<ptrace(PTRACE_PEEKDATA, pid, i, nullptr) << endl;

            //continue_execution(pid);
            addresses.erase(addresses.begin());
            addresses.erase(addresses.begin());
        }

        else{
            //child
            //freopen("newfile.txt", "w", stdout);
            //ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
            execl(prog,prog,nullptr);
        }
    }

    return;

}


















