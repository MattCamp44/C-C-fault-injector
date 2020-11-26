#include "./BreakPoint.h"
#include<sys/ptrace.h>
#include<cstdint>
#include<iostream>
#include <libexplain/ptrace.h>
#include<errno.h>
#include<sys/wait.h>

using namespace std;

BreakPoint::BreakPoint(int pid, unsigned long int address){
    this->pid = pid;
    this->address = address;
}

BreakPoint::BreakPoint(const BreakPoint& br){

    this->pid = br.pid;
    this->address = br.address;
    this->saved_data = br.saved_data;


}


void BreakPoint::Enable(){
    
    errno = 0;
    cout << "Breakpoint at: "  << address << endl;
    auto data = ptrace(PTRACE_PEEKDATA, pid, address, nullptr);
    cout << "Breakpoint data: " << hex << data <<  endl;
    //waitpid(pid,nullptr,0);
    saved_data = data;
    cout << "Saved data before: " << saved_data << endl;
    unsigned long int int3 = 0xcc;
    unsigned long int data_with_int3 = ((data & ~0xff) | int3); //set bottom byte to 0xcc
    ptrace(PTRACE_POKEDATA, pid, address, data_with_int3);
    //data = ptrace(PTRACE_PEEKDATA, pid, address, nullptr);
    //waitpid(pid,nullptr,0);

    //Inject();

    //DisableBreakpoint();
    

    cout << "Breakpoint enabled\n";
}


void BreakPoint::Release(){

    cout << "Peek at release: " << ptrace(PTRACE_PEEKDATA, pid, address, nullptr) << endl;


    ptrace(PTRACE_POKEDATA, pid, address, this->saved_data);

    cout << "Address=" << address  << " \nSaved_data=" << saved_data <<"\n Breakpoint released: peek :" << hex << ptrace(PTRACE_PEEKDATA, pid, address, nullptr) << endl;

    cout << "Peek after release: " << ptrace(PTRACE_PEEKDATA, pid, address, nullptr) << endl;

}




