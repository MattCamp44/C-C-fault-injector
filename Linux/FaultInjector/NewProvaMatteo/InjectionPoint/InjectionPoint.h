#ifndef INJECTIONPOINT_H
#define INJECTIONPOINT_H

#include<cstdint>


class InjectionPoint{

    unsigned long int address;
    int pid;
    uint8_t saved_data;

    

    public:

    InjectionPoint(int pid, unsigned long int address );

    void InjectFirstBit();
    
    void Enable();

};











#endif
