#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include<cstdint>


class BreakPoint{

    unsigned long int address;
    int pid;
    uint8_t saved_data;

    

    public:

    BreakPoint(int pid, unsigned long int address );
    
    void Enable();

};











#endif









