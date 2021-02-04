#ifndef INSTRUCTIONOBJECT_H
#define INSTRUCTIONOBJECT_H

#include<cstdint>


class InstructionObject{

    unsigned long int address;
    int length;

    

    public:
    InstructionObject( const InstructionObject & obj );
    InstructionObject(unsigned long int address, int length);
    unsigned long int getAddress();
    int getLength();

};











#endif
