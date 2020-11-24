#include<iostream>
#include<vector>
#include "./FunctionObject/FunctionObject.h"
#include <unistd.h> 
#include "./Extractor/Extractor.h"

using namespace std;





int main(int argc, char ** argv){


    if(argc != 2){

        std::cout << "I need the debugee (compiled with -g)\n";
        return 1;

    }

    int pid;

    if(pid = fork()){
        //parent
        //vector<FunctionObject> extractObjects(pid);
        vector<FunctionObject> functionObjects = extractObjects(pid,argv[1]);

    }
    else{
        //child

        

    }


    





}



