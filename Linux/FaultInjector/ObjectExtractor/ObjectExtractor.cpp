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
    
    
    
    };







vector<string> ExtractFunctionNames(fstream& ObjDumpFile){

    vector<string> FunctionNames;

    string line;
    
        while(getline(ObjDumpFile, line) ) { 


           if (line.find(FIND_SYMBOL_STRING, 0) != string::npos) {
                    
                    getline(ObjDumpFile, line); 
                    getline(ObjDumpFile, line); 
                    

                    FunctionNames.emplace_back(line.substr(50));
    
            }
    
        }


    ObjDumpFile.close();


    return FunctionNames;


    }


int main(){
    
    fstream ObjDumpFile;

    ObjDumpFile.open("dump",ios::in);

    vector<string> SymbolNames = ExtractFunctionNames(ObjDumpFile);



    for(auto a : SymbolNames)
        cout << a << endl;

    }



