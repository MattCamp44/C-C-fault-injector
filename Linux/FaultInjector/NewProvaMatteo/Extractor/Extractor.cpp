#include "Extractor.h"
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include<string.h>

using namespace std;

#define FIND_SYMBOL_STRING "subprogram"

vector<unsigned long> ExtractAddresses(FunctionObject functionobject, unsigned long int base){
    
    

    vector<string> addresses;
    vector<unsigned long> addresses_ui;

    fstream objdumpfile;
    objdumpfile.open("./Extractor/ObjectFiles/prova/objdump",ios::in);


    string line;

    while(getline(objdumpfile, line) ) { 
        
       if (line.find(functionobject.getlinkagename(), 0) != string::npos) {

            while(line != ""){
                getline(objdumpfile, line) ;

                if(line!= "" ){
                    

                    addresses.emplace_back(line.substr(2,6));
                
                }
            }
            
            for(auto s : addresses){
                //cout << s << endl;
                //addresses_ui.emplace_back(stoull(s,nullptr,16) + base );
                addresses_ui.emplace_back(stoull(s,nullptr,16) );
            }
            return addresses_ui;

            }

        }
    
    
    
    }





vector<FunctionObject> ExtractFunctionNames(fstream& ObjDumpFile, unsigned long int base){

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

                    vector<unsigned long> addresses = ExtractAddresses(FunctionObject(functionname,linkagename),base);
                    FunctionObjects.emplace_back(FunctionObject(functionname,linkagename,addresses));

            }

            
        
    
        }


    ObjDumpFile.close();


    return FunctionObjects;


    }


unsigned long int getBaseAddress(int pid){

    char curdir[5000];
    getcwd(curdir,sizeof(curdir)); 

    //string commandStr = "cat /proc/";
    string commandStr = "/proc/";

    string pid_s = to_string(pid);
    commandStr.append(pid_s);
    commandStr.append("/maps");
    fstream mapsfile;

    mapsfile.open(commandStr,ios::in);



    
    string a;
    string baseaddr;
    
    unsigned long base;

    getline(mapsfile,a);

    
    
    baseaddr = a.substr(0,8);

    

    base = stoull(baseaddr,nullptr,16);

    

    return base;

}





vector<FunctionObject> extractObjects(int pid,char * progname){

    fstream ObjDumpFile;

    string dumpPath = "./Extractor/Objectfiles/";
    //TODO -> take out path from progname
    //dumpPath.append(progname);
    dumpPath.append("prova");
    
    
    dumpPath.append("/dwarfdump");


    ObjDumpFile.open(dumpPath,ios::in);

    unsigned long int base = getBaseAddress(pid);

    vector<FunctionObject> FunctionObjectVector = ExtractFunctionNames(ObjDumpFile, base);

    char curdir[500];
    getcwd(curdir,sizeof(curdir)); 

    

    

    return FunctionObjectVector;


}








