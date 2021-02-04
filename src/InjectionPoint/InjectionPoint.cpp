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

    // intptr_t realaddress = this->address;
    // cout << "address " << address << endl ;
    // cout << "realaddress " << realaddress << endl;
    auto data = ptrace(PTRACE_PEEKDATA, pid, address,0);
    if(data == -1)
        cout << "PEEKDATA gone wrong\n";
    //  cout << hex << "Injectionpoint data: " << data << endl;
    int bit = (std::rand() % (length*4) ) ;
    // cout << "Injecting " << bit << " bit\n";
    data = data ^ ( 1 << bit );
    // data = data ^ (1 << 12);
    // cout << hex << "Breakpoint data after xor: " << data << endl;
    auto ret = ptrace(PTRACE_POKEDATA, pid, address, data);
    if(ret == -1)
        cout << "POKEDATA gone wrong\n";

        // cout << hex << "Return poke: " << ret << endl;
    // cout << "InjectionPoint enabled\n";
    // cout << "STO INIETTANDO\n";
    return bit;
}













