#include "./BreakPoint.h"
#include<sys/ptrace.h>
#include<cstdint>
#include<iostream>
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
    
    auto data = ptrace(PTRACE_PEEKDATA, pid, address, nullptr);
    // cout << "Breakpoint peekdata:" << data << endl;
    
    saved_data = data;
    
    unsigned long int int3 = 0xcc;
    unsigned long int data_with_int3 = ((data & ~0xff) | int3); //set bottom byte to 0xcc
    auto poke_exit_break = ptrace(PTRACE_POKEDATA, pid, address, data_with_int3);
    // cout << "Breakpoint pokedata:" << poke_exit_break << endl;
    
    //cout << "Breakpoint data: " << data << endl;
    //cout << "Breakpoint poke: " << poke_exit_break << endl;


}


void BreakPoint::Release(){

    
   

    auto poke_exit = ptrace(PTRACE_POKEDATA, pid, address, this->saved_data);

    
    

}




