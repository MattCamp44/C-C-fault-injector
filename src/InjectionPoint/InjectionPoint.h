#ifndef INJECTIONPOINT_H
#define INJECTIONPOINT_H

#include<cstdint>
#include "../InstructionObject/InstructionObject.h"

class InjectionPoint{

    unsigned long int address;
    int length;
    int pid;
    uint8_t saved_data;

    

    public:

    InjectionPoint(int pid, unsigned long int address, int length );

    int InjectFirstBit();
    
    void Enable();

};











#endif
