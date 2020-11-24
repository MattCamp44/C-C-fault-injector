#include "FunctionObject.h"
#include<vector>
#include<string>

using namespace std;

FunctionObject::FunctionObject(string FunctionName, string linkageName){
        
        this->FunctionName = FunctionName;
        this->linkageName = linkageName;

        }
    
FunctionObject::FunctionObject(string FunctionName, string linkageName, vector<unsigned long> addresses){
        
        this->FunctionName = FunctionName;
        this->linkageName = linkageName;
        this->addresses = addresses;

        }

    
    
    string  FunctionObject::getname(){return this->FunctionName;}
    string FunctionObject::getlinkagename(){return this->linkageName;}
    vector<unsigned long> FunctionObject::getaddresses(){return this->addresses;}