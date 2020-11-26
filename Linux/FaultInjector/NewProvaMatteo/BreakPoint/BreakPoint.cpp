#include "./BreakPoint.h"
#include<sys/ptrace.h>
#include<cstdint>
#include<iostream>
#include <libexplain/ptrace.h>
#include<errno.h>

using namespace std;

BreakPoint::BreakPoint(int pid, unsigned long int address){
    this->pid = pid;
    this->address = address;
}

void BreakPoint::Enable(){
    
    errno = 0;
    cout << "Breakpoint at: " << hex << address << endl;
    auto data = ptrace(PTRACE_PEEKDATA, pid, address, nullptr);
    cout << "Breakpoint data: " << hex << data <<  endl;
    
    saved_data = static_cast<uint8_t>(data & 0xff); //save bottom byte
    unsigned long int int3 = 0xcc;
    unsigned long int data_with_int3 = ((data & ~0xff) | int3); //set bottom byte to 0xcc
    ptrace(PTRACE_POKEDATA, pid, address, data_with_int3);
    //data = ptrace(PTRACE_PEEKDATA, pid, address, nullptr);
    
    

    cout << "Breakpoint enabled\n";
}




