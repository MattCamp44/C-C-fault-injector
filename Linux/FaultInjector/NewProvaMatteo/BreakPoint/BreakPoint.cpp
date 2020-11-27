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
    
    auto data = ptrace(PTRACE_PEEKDATA, pid, address, nullptr);
    
    
    saved_data = data;
    
    unsigned long int int3 = 0xcc;
    unsigned long int data_with_int3 = ((data & ~0xff) | int3); //set bottom byte to 0xcc
    ptrace(PTRACE_POKEDATA, pid, address, data_with_int3);
    
}


void BreakPoint::Release(){

    


    ptrace(PTRACE_POKEDATA, pid, address, this->saved_data);

    

}




