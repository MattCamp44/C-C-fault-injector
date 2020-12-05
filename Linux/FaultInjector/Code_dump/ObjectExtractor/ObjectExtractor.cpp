//L'objectExtractor sceglie autonomamente tutti gli indirizzi che possono essere selezionati dal programma come indirizzi per il fault
//Restituisce un'array di indirizzi


//Prende in input un Dwarfdump e un ObjectDump e ritorna i nomi delle funzioni e i loro indirizzi relativi
#include <algorithm>
#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define FIND_SYMBOL_STRING "subprogram"

using namespace std;



class FunctionObject {
    
    string FunctionName;

    //uint64_t addresses[2];  //beginning, end
    vector<string> addresses;
    
    string linkageName;
    

    public:
    
    FunctionObject(string FunctionName, string linkageName){
        
        this->FunctionName = FunctionName;
        this->linkageName = linkageName;

        }
    
    FunctionObject(string FunctionName, string linkageName, vector<string> addresses){
        
        this->FunctionName = FunctionName;
        this->linkageName = linkageName;
        this->addresses = addresses;

        }

    
    
    string getname(){return this->FunctionName;}
    string getlinkagename(){return this->linkageName;}
    vector<string> getaddresses(){return this->addresses;}

    };



vector<string> ExtractAddresses(FunctionObject functionobject){
    
    

    vector<string> addresses;


    fstream objdumpfile;
    objdumpfile.open("objdump",ios::in);


    string line;

    while(getline(objdumpfile, line) ) { 
        
       if (line.find(functionobject.getlinkagename(), 0) != string::npos) {

            while(line != ""){
                getline(objdumpfile, line) ;

                if(line!= "" ){
                    

                    addresses.emplace_back(line.substr(2,6));
                
                }
            }
            
        return addresses;

            }

        }
    
    
    
    }




vector<FunctionObject> ExtractFunctionNames(fstream& ObjDumpFile){

    vector<FunctionObject> FunctionObjects;

    string line;
    string linkagename;
    string functionname;

        while(getline(ObjDumpFile, line) ) { 
                


           if (line.find(FIND_SYMBOL_STRING, 0) != string::npos) {
                    getline(ObjDumpFile, line); 
                    getline(ObjDumpFile, line); 
                    
                    functionname = line.substr(50);

                    functionname.erase( remove( functionname.begin(), functionname.end(), '"') , functionname.end() );

                    if(functionname.find("main") == string::npos){

                        getline(ObjDumpFile, line); 
                        getline(ObjDumpFile, line); 
                        getline(ObjDumpFile, line); 

                        linkagename = line.substr(50); 
    
                        linkagename.erase( remove( linkagename.begin(), linkagename.end(), '"') , linkagename.end() );



                }
                    else 
   
                        linkagename = "<main>";

                    vector<string> addresses = ExtractAddresses(FunctionObject(functionname,linkagename));
                    FunctionObjects.emplace_back(FunctionObject(functionname,linkagename,addresses));

            }

            
        
    
        }


    ObjDumpFile.close();


    return FunctionObjects;


    }

string getBaseAddress(int pid){

    char curdir[500];
    getcwd(curdir,sizeof(curdir)); 

    string commandStr = "cat /proc/";
    string pid_s = to_string(pid);
    commandStr.append(pid_s);
    commandStr.append("/maps > ");
    commandStr.append(curdir);
    commandStr.append("/addrs.txt");
    cout << commandStr << endl ;
    char command[commandStr.size()+1];
    strcpy(command,commandStr.c_str());
    //system ((char *) &command);



}



int main(){
    
    fstream ObjDumpFile;

    ObjDumpFile.open("dump",ios::in);

    vector<FunctionObject> SymbolNames = ExtractFunctionNames(ObjDumpFile);

    getBaseAddress(2);
    

    //std::string commandStr = "cat proc/";
    //std::string pid_s = std::to_string(pid);

    
    for(auto a : SymbolNames){
        cout << a.getname() << " " << a.getlinkagename() << " "  << endl;
        for(auto b : a.getaddresses())
            cout << b << " stoull(b): " << stoull(b,nullptr,16) << endl;
        }
    }



