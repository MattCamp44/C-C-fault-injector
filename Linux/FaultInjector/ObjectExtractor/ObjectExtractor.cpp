//L'objectExtractor sceglie autonomamente tutti gli indirizzi che possono essere selezionati dal programma come indirizzi per il fault
//Restituisce un'array di indirizzi


//Prende in input un ObjectDump e ritorna i nomi delle funzioni

#include<fstream>
#include<iostream>
#include<vector>
#include<string>

#define FIND_SYMBOL_STRING "subprogram"


using namespace std;

vector<string> ExtractFunctionNames(fstream& ObjDumpFile){

    vector<string> FunctionNames;

    unsigned int curLine = 0;
    string search = FIND_SYMBOL_STRING;
    string line;
    
        while(getline(ObjDumpFile, line) ) { // I changed this, see below

        //cout << "Tellg(): " << ObjDumpFile.tellg() << endl;

        curLine++;
        //cout << line << endl;
           if (line.find(search, 0) != string::npos) {
         //       cout << "found: " << search << "line: " << line << endl;
                    getline(ObjDumpFile, line); 
                    getline(ObjDumpFile, line); 
                    
                    

                    FunctionNames.emplace_back(line.substr(50));
    
            }
    
        }

    cout << "Before Close()" << endl;
    ObjDumpFile.close();

    cout << "After Close()" << endl;

    return FunctionNames;
    }





int main(){
    
    fstream ObjDumpFile;

    ObjDumpFile.open("dump",ios::in);

    vector<string> SymbolNames = ExtractFunctionNames(ObjDumpFile);


    cout << "Function ends" << endl;

    for(auto a : SymbolNames)
        cout << a << endl;

    }



