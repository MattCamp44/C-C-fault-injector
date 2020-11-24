#include "./InjectionPoint.h"

#include<sys/ptrace.h>
#include<cstdint>

InjectionPoint::InjectionPoint(int pid, unsigned long int address){
    this->pid = pid;
    this->address = address;
}

void InjectionPoint::InjectFirstBit(){

    auto data = ptrace(PTRACE_PEEKDATA, pid, address, nullptr);
    data = data ^ 1;
    ptrace(PTRACE_POKEDATA, pid, address, data);

}













