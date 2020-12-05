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
    //cout << "Injectionpoint data: " << data << endl;
    data = data ^ 4;
    //cout << "Breakpoint data after xor: " << data << endl;
    auto ret = ptrace(PTRACE_POKEDATA, pid, address, data);
    //cout << "Return poke: " << ret << endl;
    //cout << "InjectionPoint enabled\n";
    //cout << "STO INIETTANDO\n";

}













