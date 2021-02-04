#include "./InstructionObject.h"

#include<sys/ptrace.h>
#include<cstdint>
#include<iostream>

using namespace std;

InstructionObject::InstructionObject(unsigned long int address, int length){
    this->address = address;
    this->length = length;
}


InstructionObject::InstructionObject( const InstructionObject & obj ){

    this->address = obj.address;
    this -> length = obj.length;


}

unsigned long int InstructionObject::getAddress(){
    return this->address;
};

int InstructionObject::getLength(){
    return this->length;
};