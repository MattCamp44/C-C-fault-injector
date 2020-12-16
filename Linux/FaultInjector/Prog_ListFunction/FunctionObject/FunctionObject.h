


#ifndef FUNCOBJ_H
#define FUNCOBJ_H

#include<vector>
#include<string>
#include<iostream>


using namespace std;

class FunctionObject {
    string FunctionName;

    //unsigned long addresses[2];  //beginning, end
    vector<unsigned long> addresses;
    
    string linkageName;
    

    public:
    
    FunctionObject(string FunctionName, string linkageName);
    
    FunctionObject(string FunctionName, string linkageName, vector<unsigned long> addresses);

    
    
    string getname();
    string getlinkagename();
    vector<unsigned long> getaddresses();

    };







#endif