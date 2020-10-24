//L'objectExtractor sceglie autonomamente tutti gli indirizzi che possono essere selezionati dal programma come indirizzi per il fault
//Restituisce un'array di indirizzi


//Prende in input un ObjectDump e ritorna i nomi delle funzioni

#include<fstream>
#include<iostream>
#include<vector>
#include<string>

#define FIND_SYMBOL_STRING "subprogram"


using namespace std;

vector<string> ExtractFunctionNames(){

    fstream ObjDumpFile;

    ObjDumpFile.open("dump",ios::in);
    vector<string> FunctionNames;

    unsigned int curLine = 0;
    string search = FIND_SYMBOL_STRING;
    string line;
    
        while(getline(ObjDumpFile, line) ) { // I changed this, see below

        cout << "Tellg(): " << ObjDumpFile.tellg() << endl;

        curLine++;
        //cout << line << endl;
            if (line.find(search, 0) != string::npos) {
                cout << "found: " << search << "line: " << line << endl;
    
            }
    
        }

    cout << "Before Close()" << endl;
    ObjDumpFile.close();

    cout << "After Close()" << endl;
    }





int main(){
    

    vector<string> SymbolNames = ExtractFunctionNames();


    cout << "Function ends" << endl;


    }



