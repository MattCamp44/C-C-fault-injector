//L'objectExtractor sceglie autonomamente tutti gli indirizzi che possono essere selezionati dal programma come indirizzi per il fault
//Restituisce un'array di indirizzi


//Prende in input un ObjectDump e ritorna i nomi delle funzioni

#include<fstream>
#include<iostream>
#include<vector>
#include<string>

#define FIND_SYMBOL_STRING "subprogram"

using namespace std;



class FunctionObject {
    
    string FunctionName;

    //uint_8 addresses[2];  //beginning, end
    
    string LinkingName;
    

    public:
    
    FunctionObject(string FunctionName, string LinkingName){
        
        this->FunctionName = FunctionName;
        this->LinkingName = LinkingName;

        }
    
    string getname(){return this->FunctionName;}
    string getlinkingname(){return this->LinkingName;}


    };







vector<FunctionObject> ExtractFunctionNames(fstream& ObjDumpFile){

    vector<FunctionObject> FunctionObjects;

    string line;
    string linkingname;
    string functionname;

        while(getline(ObjDumpFile, line) ) { 


           if (line.find(FIND_SYMBOL_STRING, 0) != string::npos) {
                    
                    getline(ObjDumpFile, line); 
                    getline(ObjDumpFile, line); 
                    
                    functionname = line.substr(50);

                    //FunctionNames.emplace_back(line.substr(50));
                    
                    if(functionname.find("main") == string::npos){

                        getline(ObjDumpFile, line); 
                        getline(ObjDumpFile, line); 
                        getline(ObjDumpFile, line); 

                        linkingname = line.substr(50); 
    
                }
                    else linkingname = "main";
                    FunctionObjects.emplace_back(FunctionObject(functionname,linkingname));
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
        cout << a.getname() << a.getlinkingname() << endl;

    }



