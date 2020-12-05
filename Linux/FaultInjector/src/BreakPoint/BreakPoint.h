#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include<cstdint>


class BreakPoint{

    unsigned long int address;
    int pid;
    unsigned long long int saved_data;

    

    public:

    BreakPoint(int pid, unsigned long int address );

    BreakPoint(const BreakPoint& br);
    
    void Enable();

    void Release();

    void SetSavedData(unsigned long long int data){
        this->saved_data = data;
    }

    unsigned long long int GetSavedData(){
        return this->saved_data;
    }


};











#endif









