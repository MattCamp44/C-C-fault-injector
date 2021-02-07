#include "./InjectionPoint.h"

#include<sys/ptrace.h>
#include<cstdint>
#include<iostream>
#include "../InstructionObject/InstructionObject.h"

using namespace std;

InjectionPoint::InjectionPoint(int pid, unsigned long int address, int length){
    this->pid = pid;
    this->address = address;
    this->length = length;
}

int InjectionPoint::InjectFirstBit(){

    
    auto data = ptrace(PTRACE_PEEKDATA, pid, address,0);
    if(data == -1){
        cout << "PEEKDATA gone wrong\n";
        return -1;
    }
    int bit = (std::rand() % (length*4) ) ;
    
    data = data ^ ( 1 << bit );
    
    auto ret = ptrace(PTRACE_POKEDATA, pid, address, data);
    
    if(ret == -1){
        cout << "POKEDATA gone wrong\n";
        return -1;
    }

        // cout << hex << "Return poke: " << ret << endl;
    // cout << "InjectionPoint enabled\n";
    // cout << "STO INIETTANDO\n";
    return bit;
}













