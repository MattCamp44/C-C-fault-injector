#include "./Debugger.h"
#include "sys/ptrace.h"
#include "sys/wait.h"
#include<vector>
#include "../FunctionObject/FunctionObject.h"
#include "../AddressSelector/AddressSelector.h"


void EnableInjectionPointsAndBreakpoint(int pid, vector<unsigned long int> addresses){
    
    



}




void Debugger(int pid, vector<FunctionObject> FunctionObjects){

    vector<unsigned long int> addresses = AddressSelector(FunctionObjects);

    EnableInjectionPointsAndBreakpoint(pid,addresses);


}








void continue_execution(int pid) {
    ptrace(PTRACE_CONT, pid, nullptr, nullptr);

    int wait_status;
    auto options = 0;
    waitpid(pid, &wait_status, options);

}










