#include "./Extractor/Extractor.h"
#include "./FunctionObject/FunctionObject.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

int main(int argc,char ** argv){
    
    if(argc != 2){
        cout << "I need the binary file of the program (only)" << endl;
        return 1;  
    }

    // creo comando per creare file objdump
    char command[100];
    strcpy(command,"objdump ");
    strcpy(command,argv[1]);
    strcpy(command," >> objdump");

    system(command);

    vector<FunctionObject> functionObjects = extractObjects(argv[1]);

    for(vector<FunctionObject>::iterator it = functionObjects.begin(); it != functionObjects.end(); ++it){
        cout << (*it).getname() << endl;
    }

    //remove("objdump");
    return 0;
}