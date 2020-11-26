#include "./Debugger.h"
#include "sys/ptrace.h"
#include "sys/wait.h"
#include<vector>
#include "../FunctionObject/FunctionObject.h"
#include "../AddressSelector/AddressSelector.h"
#include "../BreakPoint/BreakPoint.h"
#include "../InjectionPoint/InjectionPoint.h"
#include <assert.h> 

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


void Debugger(int pid, vector<FunctionObject> FunctionObjects){


    vector<unsigned long int> addresses;
    addresses = AddressSelector(FunctionObjects);
    cout << "Addresses: " << endl;
    for(auto a : addresses)
        cout << hex << a << endl;
    assert(!addresses.empty());
    //assert(!(addresses.size() >= 2));
    
    //Pop head
    unsigned long int breakpointAddress = addresses[0];
    addresses.erase(addresses.begin());
    BreakPoint breakpoint(pid,breakpointAddress);
    
    breakpoint.Enable();
    
    
    //BreakPoint breakpoint = EnableBreakpoint(pid,breakpointAddress);

    cout << "Saved data in debugger:" << breakpoint.GetSavedData() << endl;
    // One continue for every peekdata I guess?
    //continue_execution(pid);
    //continue_execution(pid);
    cout << "Here" << endl;
    //Da qui il programma continua con il breakpoint inserito -> breakpoint e injection point sono inseriti in due momenti diversi damn
    ptrace(PTRACE_CONT, pid, nullptr, nullptr);


    waitpid(pid,nullptr,0);
    
    EnableInjectionPoints(pid,addresses);


    breakpoint.Release();

    ptrace(PTRACE_SINGLESTEP, pid, nullptr, nullptr);

    waitpid(pid,nullptr,0);

    ptrace(PTRACE_CONT, pid, nullptr, nullptr);

    waitpid(pid,nullptr,0);
    //for(auto i : FunctionObjects[0].getaddresses())
        //cout << i << " : " <<ptrace(PTRACE_PEEKDATA, pid, i, nullptr) << endl;

    //continue_execution(pid);

    return;

}



















