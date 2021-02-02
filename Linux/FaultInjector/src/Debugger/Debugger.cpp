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
void EnableInjectionPoint(int pid, unsigned long int address ){

    

    

    InjectionPoint injPoint = InjectionPoint(pid,address);
    injPoint.InjectFirstBit();
    


}


void Debugger(vector<FunctionObject> FunctionObjects, char * prog){


    


    vector<unsigned long int> addresses;

    for(auto addr : FunctionObjects[0].getaddresses()){
        addresses.emplace_back(addr);
    }

    int pid;
    
    

    int index=0;
    for(auto i : FunctionObjects[0].getaddresses()){
        for( auto j = 0; j < 2; j++ ){
    // for(auto i : myaddresses) {
        cout << "Injecting " << hex << i << endl;
        pid = fork();
        if(pid){
            //Parent
            waitpid(pid,nullptr,0);
            
            

            
            //addresses = AddressSelector(FunctionObjects);
            //addresses[0] = i;
            //addresses[1] = i;
            
            // addresses.emplace_back(myaddressesinstruction[index++]);
            // addresses.emplace_back(i);

            // assert(!addresses.empty());
            
            cout << "Here\n";
            
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

            //regs.rip = 0x400543 ;
            regs.rip = breakpointAddress ;

            ptrace(PTRACE_SETREGS, pid, nullptr, &regs);

            //cout << "Program counter before singlestep: " << regs.rip << endl;
            //sleep(1);
            ptrace(PTRACE_CONT, pid, nullptr, nullptr);

            //cout << "Program counter after singlestep: " << regs.rip << endl;


            waitpid(pid,nullptr,0);

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


            //remove("injectedoutput.txt");
            cout << "Done injecting\n";
        }

        else{
            //child
            // freopen("injectedoutput.txt", "w", stdout);
            ptrace(PTRACE_TRACEME,0,nullptr,nullptr);
            execl(prog,prog,nullptr);
        }
    }

} //for

    return;

}



















