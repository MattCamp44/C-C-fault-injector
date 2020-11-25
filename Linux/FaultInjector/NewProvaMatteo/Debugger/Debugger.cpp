#include "./Debugger.h"
#include "sys/ptrace.h"
#include "sys/wait.h"
#include<vector>
#include "../FunctionObject/FunctionObject.h"
#include "../AddressSelector/AddressSelector.h"
#include "../BreakPoint/BreakPoint.h"
#include "../InjectionPoint/InjectionPoint.h"


using namespace std;

void continue_execution(int pid) {
    ptrace(PTRACE_CONT, pid, nullptr, nullptr);

    int wait_status;
    auto options = 0;
    waitpid(pid, &wait_status, options);

}




void EnableInjectionPointsAndBreakpoint(int pid, vector<unsigned long int> &addresses){
    
    BreakPoint breakpoint(pid,addresses[0]);

    InjectionPoint injectionpoint(pid,addresses[1]);

    breakpoint.Enable();

    injectionpoint.InjectFirstBit();


}




void Debugger(int pid, vector<FunctionObject> FunctionObjects){


    vector<unsigned long int> addresses;
    addresses = AddressSelector(FunctionObjects);

    EnableInjectionPointsAndBreakpoint(pid,addresses);
    // One continue for every peekdata I guess?
    continue_execution(pid);
    continue_execution(pid);
    cout << "Here" << endl;

    

    //for(auto i : FunctionObjects[0].getaddresses())
        //cout << i << " : " <<ptrace(PTRACE_PEEKDATA, pid, i, nullptr) << endl;

    //continue_execution(pid);

    return;

}



















