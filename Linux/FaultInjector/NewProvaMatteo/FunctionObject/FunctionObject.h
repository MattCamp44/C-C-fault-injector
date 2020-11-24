


#ifndef FUNCOBJ_H
#define FUNCOBJ_H

#include<vector>
#include<string>

using namespace std;

class FunctionObject {
    
    string FunctionName;

    //uint64_t addresses[2];  //beginning, end
    vector<string> addresses;
    
    string linkageName;
    

    public:
    
    FunctionObject(string FunctionName, string linkageName);
    
    FunctionObject(string FunctionName, string linkageName, vector<string> addresses);

    
    
    string getname();
    string getlinkagename();
    vector<string> getaddresses();

    };







#endif