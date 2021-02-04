


#ifndef FUNCOBJ_H
#define FUNCOBJ_H

#include<vector>
#include<string>
#include<iostream>
#include "../InstructionObject/InstructionObject.h"

using namespace std;

class FunctionObject {
    string FunctionName;

    //unsigned long addresses[2];  //beginning, end
    vector<InstructionObject> addresses;
    
    string linkageName;
    

    public:
    
    FunctionObject(string FunctionName, string linkageName);
    
    FunctionObject(string FunctionName, string linkageName, vector<InstructionObject> addresses);

    
    
    string getname();
    string getlinkagename();
    vector<InstructionObject> getaddresses();

    };







#endif