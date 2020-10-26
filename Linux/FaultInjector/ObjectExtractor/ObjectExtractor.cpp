//L'objectExtractor sceglie autonomamente tutti gli indirizzi che possono essere selezionati dal programma come indirizzi per il fault
//Restituisce un'array di indirizzi


//Prende in input un ObjectDump e ritorna i nomi delle funzioni
#include <algorithm>
#include<fstream>
#include<iostream>
#include<vector>
#include<string>

#define FIND_SYMBOL_STRING "subprogram"

using namespace std;



class FunctionObject {
    
    string FunctionName;

    //uint_8 addresses[2];  //beginning, end
    string beginningaddr;
    string endaddr;
    vector<string> addresses;
    
    string linkageName;
    

    public:
    
    FunctionObject(string FunctionName, string linkageName){
        
        this->FunctionName = FunctionName;
        this->linkageName = linkageName;

        }
    
    string getname(){return this->FunctionName;}
    string getlinkagename(){return this->linkageName;}


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
                if(line!= ""){
                    
                    cout << line.substr(2,6) << endl;

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

                        cout << functionname << ": \n";

                        vector<string> addresses = ExtractAddresses(FunctionObject(functionname,linkagename));

                        cout << endl;
                }
                    else 
                        linkagename = "main";

                    FunctionObjects.emplace_back(FunctionObject(functionname,linkagename));

            }

            
        
    
        }


    ObjDumpFile.close();


    return FunctionObjects;


    }


int main(){
    
    fstream ObjDumpFile;

    ObjDumpFile.open("dump",ios::in);

    vector<FunctionObject> SymbolNames = ExtractFunctionNames(ObjDumpFile);



    for(auto a : SymbolNames)
        cout << a.getname() << " " << a.getlinkagename() << " " << endl;

    }



