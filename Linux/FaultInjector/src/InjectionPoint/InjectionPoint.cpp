#include "./InjectionPoint.h"

#include<sys/ptrace.h>
#include<cstdint>
#include<iostream>

using namespace std;

InjectionPoint::InjectionPoint(int pid, unsigned long int address){
    this->pid = pid;
    this->address = address;
}

void InjectionPoint::InjectFirstBit(){

    auto data = ptrace(PTRACE_PEEKDATA, pid, address,0);
    if(data == -1)
        cout << "PEEKDATA gone wrong\n";
    // cout << hex << "Injectionpoint data: " << data << endl;
    // data = data ^ ( 1 << (std::rand() % length )  )
    data = data ^ (1 << 16);
    // cout << hex << "Breakpoint data after xor: " << data << endl;
    auto ret = ptrace(PTRACE_POKEDATA, pid, address, data);
    if(ret == -1)
        cout << "POKEDATA gone wrong\n";

        // cout << hex << "Return poke: " << ret << endl;
    // cout << "InjectionPoint enabled\n";
    // cout << "STO INIETTANDO\n";

}













